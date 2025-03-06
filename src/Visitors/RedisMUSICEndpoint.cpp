/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "RedisMUSICEndpoint.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages;
using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages;
using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages;
using namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages;
using namespace cpp_redis;
using namespace boost::json;

namespace MUSICLibrary::Visitors
{
	std::wstring RedisMUSICEndpoint::ConfigFilePath;

	std::shared_ptr<cpp_redis::subscriber> RedisMUSICEndpoint::getSubscriber()
	{
		return Subscriber;
	}

	void RedisMUSICEndpoint::setSubscriber(std::shared_ptr<cpp_redis::subscriber> value)
	{
		Subscriber = value;
	}

	std::shared_ptr<cpp_redis::client> RedisMUSICEndpoint::getPublisher()
	{
		return Publisher;
	}

	void RedisMUSICEndpoint::setPublisher(std::shared_ptr<cpp_redis::client> value)
	{
		Publisher = value;
	}

	std::unordered_map<std::wstring, std::wstring> RedisMUSICEndpoint::getChannels() const
	{
		return Channels;
	}

	void RedisMUSICEndpoint::setChannels(const std::unordered_map<std::wstring, std::wstring> &value)
	{
		Channels = value;
	}

	std::wstring RedisMUSICEndpoint::getConfigFilePath()
	{
		return ConfigFilePath;
	}

	void RedisMUSICEndpoint::setConfigFilePath(const std::wstring &value)
	{
		ConfigFilePath = value;
	}

	RedisMUSICEndpoint::RedisMUSICEndpoint(std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> siteAndAppID, unsigned int exerciseID, 
		std::shared_ptr<MUSICLibrary::Interfaces::IMessageFilter> messageFilter, 
		std::shared_ptr<IInternalMessageVisitor> internalVisitor, 
		std::shared_ptr<IMUSICMessageVisitor> externalVisitor) : MUSICEndpoint(siteAndAppID, exerciseID, messageFilter, internalVisitor, externalVisitor)
	{
		// If the file path is not set, make it the default
		if (getConfigFilePath() == L"")
		{
			// Set to the ProgramData folder
			TCHAR szPath[MAX_PATH];
			if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath)))
			{
				// Because of type weirdness between TCHAR the folder path is in and the wstring the setter requires, 
				// use a string stream to combine the folder and file path strings
				std::wstringstream stream;
				stream << szPath;
				stream << "\\DiscoveryMachine\\dm\\RedisEndpointConfig.json";
				setConfigFilePath(stream.str());
			}
		}

		// If the file doesn't exist, create one
		if (!FileSystem::fileExists(getConfigFilePath()))
		{
			// C++ does not have an explicit file create function
			// By opening an output stream to the file path, the file is created
			std::ofstream configFileStream = std::ofstream(getConfigFilePath());
			configFileStream.close();
		}

		try
		{
			// Default server values to use
			std::string ip = defaultIP;
			unsigned int port = defaultPort;
			std::string password = defaultPassword;

			// Open the file and put all its contents into a buffer
			std::ifstream streamReader = std::ifstream(getConfigFilePath());
			std::stringstream buffer = std::stringstream();
			buffer << streamReader.rdbuf();
			streamReader.close();

			std::string configString = buffer.str();

			// If the config file was empty, write default server info into config file
			if (configString.empty())
			{
				writeDefaultValuesToConfigFile();
			}
			// Otherwise, parse the buffer's content as a JSON object
			else
			{				
				error_code ec = error_code();
				object configJson = parse(configString, ec).as_object();

				// If there was an error parsing, overwrite the file with the default DMI Redis server info
				if (ec)
				{
					std::cout << "There was an error parsing the Redis config file.\n";
					std::cout << "Overwriting file with working default DMI Redis server details and";
					std::cout << "connecting the the default DMI Redis server." << std::endl;

					writeDefaultValuesToConfigFile();
				}
				// Otherwise, set the server based on the parsed information
				else
				{
					ip = std::string(configJson["Ip-Address"].as_string());
					port = value_to<unsigned int>(configJson["Port"]);
					password = std::string(configJson["Password"].as_string());
				}
			}

			// Set up the Redis subscriber and publisher
			// First the subscriber
			std::shared_ptr<cpp_redis::subscriber> sub = std::make_shared<cpp_redis::subscriber>();
			// connect to the Redis server
			sub->connect(ip, port, [](const std::string& host, std::size_t port, cpp_redis::subscriber::connect_state status)
				{
					if (status == cpp_redis::subscriber::connect_state::dropped)
					{
						std::cout << "Redis subscriber disconnected from " << host << ":" << port << std::endl;
					}
				});
			// Authorize with the password
			sub->auth(password, [](const cpp_redis::reply& reply)
				{
					if (reply.is_error())
					{
						std::cerr << "Redis Subscriber Authentication failed: " << reply.as_string() << std::endl;
					}
					else
					{
						std::cout << "successful Redis subscriber authentication" << std::endl;
					}
				});
			// Commit the connect/auth request we just pipelined, we'll handle subscribing in the SubscribeToMUSIC function later.
			// Make this subscriber object the member we keep track of
			sub->commit();
			setSubscriber(sub);

			// Now set up the publisher (called client in this library. They only have subscriber and a client which can get/send but not subscribe so
			// I'm using a subscriber for gets but making a client just for publishing/sending).
			std::shared_ptr<cpp_redis::client> pub = std::make_shared<cpp_redis::client>();
			// connect to the Redis server
			pub->connect(ip, port, [](const std::string& host, std::size_t port, cpp_redis::client::connect_state status) {
				if (status == cpp_redis::client::connect_state::dropped) 
				{
					std::cout << "Redis client disconnected from " << host << ":" << port << std::endl;
				}
				});
			// Authorize with the password
			pub->auth(password, [](const cpp_redis::reply& reply)
				{
					if (reply.is_error())
					{
						std::cerr << "Publisher Authentication failed: " << reply.as_string() << std::endl;
					}
					else
					{
						std::cout << "successful publisher authentication" << std::endl;
					}
				});
			// Commit the connect/auth request we just pipelined, we'll handle publishing in the MUSIC message Visit functions later.
			// Make this publisher object the member we keep track of
			pub->commit();
			setPublisher(pub);
		}
		catch (const std::runtime_error &e)
		{
			std::wcout << e.what() << std::endl;
			throw e;
		}

		subscribedChannelID = L"Ex:" + std::to_wstring(exerciseID) + L":Site:*:App:*";

		setChannels(std::unordered_map<std::wstring, std::wstring>
		{
			{ L"ConstructData", L"ConstructData:" + subscribedChannelID },
			{ L"StateFieldData", L"StateFieldData:" + subscribedChannelID },
			{ L"PerceptionData", L"PerceptionData:" + subscribedChannelID },
			{ L"WaypointData", L"WaypointData:" + subscribedChannelID },
			{ L"EventData", L"EventData:" + subscribedChannelID },
			{ L"Command", L"Command:" + subscribedChannelID },
			{ L"InteractionRequest", L"InteractionRequest:" + subscribedChannelID },
			{ L"InteractionResponse", L"InteractionResponse:" + subscribedChannelID },
			{ L"ControlRequest", L"ControlRequest:" + subscribedChannelID },
			{ L"ControlTransferRequest", L"ControlTransferRequest:" + subscribedChannelID },
			{ L"ControlLost", L"ControlLost:" + subscribedChannelID },
			{ L"ControlReclamation", L"ControlReclamation:" + subscribedChannelID },
			{ L"ControlRegained", L"ControlRegained:" + subscribedChannelID },
			{ L"ControlTransferResponse", L"ControlTransferResponse:" + subscribedChannelID },
			{ L"ControlResponse", L"ControlResponse:" + subscribedChannelID },
			{ L"ControlRelinquished", L"ControlRelinquished:" + subscribedChannelID },
			{ L"PrimaryControlRelinquished", L"PrimaryControlRelinquished:" + subscribedChannelID },
			{ L"SetCurrentController", L"SetCurrentController:" + subscribedChannelID },
			{ L"PrimaryControlResponse", L"PrimaryControlResponse:" + subscribedChannelID },
			{ L"PrimaryControlRequest", L"PrimaryControlRequest:" + subscribedChannelID },
			{ L"ControlGained", L"ControlGained:" + subscribedChannelID },
			{ L"PrimaryControlGranted", L"PrimaryControlGranted:" + subscribedChannelID },
			{ L"ConstructControlGranted", L"ConstructControlGranted:" + subscribedChannelID },
			{ L"ControlTransferGranted", L"ControlTransferGranted:" + subscribedChannelID }
		});
	}

	void RedisMUSICEndpoint::writeDefaultValuesToConfigFile()
	{
		object defaultConfig = object();
		defaultConfig["Ip-Address"] = defaultIP;
		defaultConfig["Port"] = defaultPort;
		defaultConfig["Password"] = defaultPassword;

		// Write the default config to the config file. The << operator on object handles serialization
		std::ofstream streamWriter = std::ofstream(getConfigFilePath());
		streamWriter << defaultConfig << std::endl;
		streamWriter.close();
	}

	void RedisMUSICEndpoint::SubscribeToMUSIC()
	{
		if (isSubscribed)
		{
			throw SubscriptionException("Cannot subscribe to MUSIC while already subscribed");
		}

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"ConstructData"]), [&](const std::string& channel, const std::string& message)
		{
			_logger->debug("RECEIVE ConstructData on channel {} received\n\t{}", channel, message);
			Receive(std::make_shared<ConstructDataMessage>(std::make_shared<object>(parse(message).as_object())));
			_logger->debug("RECEIVE COMPLETE ConstructData");
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"StateFieldData"]), [&](const std::string& channel, const std::string& message)
		{
				_logger->debug("StateFieldData on channel {} received\n\t{}", channel, message);
				try
				{
					Receive(std::make_shared<StateFieldMessage>(std::make_shared<object>(parse(message).as_object())));
				}
				catch (KeyNotFoundException e)
				{
					_logger->error("KeyNotFound for StateFieldData\t\n{}", e.what());
				}
				_logger->debug("RECEIVE COMPLETE StateFieldData");
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"InteractionRequest"]), [&](const std::string& channel, const std::string& message)
		{
				_logger->debug("InteractionRequest on channel {} received\n\t{}", channel, message);
				try
				{
					Receive(std::make_shared<InteractionRequestMessage>(std::make_shared<object>(parse(message).as_object())));
				}
			catch (KeyNotFoundException e)
			{
				_logger->error("KeyNotFound for InteractionRequest\t\n{}", e.what());
			}
			_logger->debug("RECEIVE COMPLETE InteractionRequest");
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"InteractionResponse"]), [&](const std::string& channel, const std::string& message)
		{
			_logger->debug("InteractionResponse on channel {} received\n\t{}", channel, message);
			try
			{
				Receive(std::make_shared<InteractionResponseMessage>(std::make_shared<object>(parse(message).as_object())));
			}
			catch (KeyNotFoundException e)
			{
				_logger->error("KeyNotFound for InteractionResponse\t\n{}", e.what());
			}
			_logger->debug("RECEIVE COMPLETE InteractionResponse");
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlRequest"]), [&](const std::string& channel, const std::string& message)
		{
			Receive(std::make_shared<ControlRequestMessage>(std::make_shared<object>(parse(message).as_object())));
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlTransferRequest"]), [&](const std::string& channel, const std::string& message)
		{
			Receive(std::make_shared<ControlTransferRequestMessage>(std::make_shared<object>(parse(message).as_object())));
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlLost"]), [&](const std::string& channel, const std::string& message)
		{
			Receive(std::make_shared<ControlLostMessage>(std::make_shared<object>(parse(message).as_object())));
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"PerceptionData"]), [&](const std::string& channel, const std::string& message)
		{
				_logger->debug("PerceptionData on channel {} received\n\t{}", channel, message);
				try
				{
			Receive(std::make_shared<PerceptionDataMessage>(std::make_shared<object>(parse(message).as_object())));
				}
				catch (KeyNotFoundException e)
				{
					_logger->error("KeyNotFound for PerceptionData\t\n{}", e.what());
				}
				_logger->debug("RECEIVE COMPLETE PerceptionData");
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"WaypointData"]), [&](const std::string& channel, const std::string& message)
		{
				_logger->debug("WaypointData on channel {} received\n\t{}", channel, message);
				try
				{
			Receive(std::make_shared<WaypointDataMessage>(std::make_shared<object>(parse(message).as_object())));
				}
				catch (KeyNotFoundException e)
				{
					_logger->error("KeyNotFound for WaypointData\t\n{}", e.what());
				}
				_logger->debug("RECEIVE COMPLETE WaypointData");
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"EventData"]), [&](const std::string& channel, const std::string& message)
		{
				_logger->debug("EventData on channel {} received\n\t{}", channel, message);
			Receive(std::make_shared<MUSICEventMessage>(std::make_shared<object>(parse(message).as_object())));
			_logger->debug("RECEIVE COMPLETE EventData");
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"Command"]), [&](const std::string& channel, const std::string& message)
		{
			_logger->debug("Command on channel {} received\n\t{}", channel, message);
			
			std::shared_ptr<object> commandJSON = std::make_shared<object>(parse(message).as_object());

			try 
			{
				unsigned int commandValue = 1;

				commandValue = commandJSON->find("commandIdentifier")->value().as_int64();

				// If we find we need to access these command identifiers elsewhere, we should make an enumeration in a location everyone can access it.
				if (commandValue == 454000002)
				{
					Receive(std::make_shared<TransferConstructIDMessage>(commandJSON));
				}
				else if (commandValue == 454004005)
				{
					Receive(std::make_shared<DisplayMessagesMessage>(commandJSON));
				}
				else if (commandValue == 454007001)
				{
					Receive(std::make_shared<StopConstructMessage>(commandJSON));
				}
				else if (commandValue == 454007002)
				{
					Receive(std::make_shared<RemoveConstructMessage>(commandJSON));
				}
				else if (commandValue == 454009000)
				{
					Receive(std::make_shared<RequestSimulationTimeMessage>(commandJSON));
				}
				else if (commandValue == 454009001)
				{
					Receive(std::make_shared<SetSimulationTimeMessage>(commandJSON));
				}
				else if (commandValue == 454009999)
				{
					Receive(std::make_shared<SimulationTimeMessage>(commandJSON));
				}
				else if (commandValue == 454013000)
				{
					Receive(std::make_shared<CreateEnvironmentRequestMessage>(commandJSON));
				}
				else if (commandValue == 454013001)
				{
					Receive(std::make_shared<CreateEnvironmentResponseMessage>(commandJSON));
				}
				else if (commandValue == 454013002)
				{
					Receive(std::make_shared<CreateConstructRequestMessage>(commandJSON));
				}
				else if (commandValue == 454013003)
				{
					Receive(std::make_shared<CreateConstructResponseMessage>(commandJSON));
				}
				else if (commandValue == 454013004)
				{
					Receive(std::make_shared<ParameterizeConstructRequestMessage>(commandJSON));
				}
				else if (commandValue == 454013005)
				{
					Receive(std::make_shared<ParameterizeConstructResponseMessage>(commandJSON));
				}
				else if (commandValue == 454013006)
				{
					Receive(std::make_shared<FinalizeScenarioRequestMessage>(commandJSON));
				}
				else if (commandValue == 454013007)
				{
					Receive(std::make_shared<FinalizeScenarioResponseMessage>(commandJSON));
				}
				else if (commandValue == 454013008)
				{
					Receive(std::make_shared<ScenarioStartMessage>(commandJSON));
				}
				else if (commandValue == 454999001)
				{
					Receive(std::make_shared<ControlInitiatedMessage>(commandJSON));
				}
				else if (commandValue == 454999002)
				{
					Receive(std::make_shared<ControlReleasedMessage>(commandJSON));
				}
			}
			catch(const KeyNotFoundException& e1)
			{
				_logger->error("KeyNotFound for Command\t\n{}", e1.what());
			}
			catch (const std::invalid_argument& e2)
			{
				_logger->error("Invalid Argument for Command\t\n{}", e2.what());
			}

			_logger->debug("RECEIVE COMPLETE Command");
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlReclamation"]), [&](const std::string& channel, const std::string& message)
		{
			Receive(std::make_shared<ControlReclamationMessage>(std::make_shared<object>(parse(message).as_object())));
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlRegained"]), [&](const std::string& channel, const std::string& message)
		{
			Receive(std::make_shared<ControlRegainedMessage>(std::make_shared<object>(parse(message).as_object())));
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlTransferResponse"]), [&](const std::string& channel, const std::string& message)
		{
			Receive(std::make_shared<ControlTransferResponseMessage>(std::make_shared<object>(parse(message).as_object())));
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlResponse"]), [&](const std::string& channel, const std::string& message)
		{
			Receive(std::make_shared<ControlResponseMessage>(std::make_shared<object>(parse(message).as_object())));
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlRelinquished"]), [&](const std::string& channel, const std::string& message)
		{
			Receive(std::make_shared<ControlRelinquishedMessage>(std::make_shared<object>(parse(message).as_object())));
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"PrimaryControlRelinquished"]), [&](const std::string& channel, const std::string& message)
		{
			Receive(std::make_shared<PrimaryControlRelinquishedMessage>(std::make_shared<object>(parse(message).as_object())));
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"SetCurrentController"]), [&](const std::string& channel, const std::string& message)
		{
			Receive(std::make_shared<SetCurrentControllerMessage>(std::make_shared<object>(parse(message).as_object())));
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"PrimaryControlResponse"]), [&](const std::string& channel, const std::string& message)
		{
			Receive(std::make_shared<PrimaryControlResponseMessage>(std::make_shared<object>(parse(message).as_object())));
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"PrimaryControlRequest"]), [&](const std::string& channel, const std::string& message)
		{
			Receive(std::make_shared<PrimaryControlRequestMessage>(std::make_shared<object>(parse(message).as_object())));
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlGained"]), [&](const std::string& channel, const std::string& message)
		{
			Receive(std::make_shared<ControlGainedMessage>(std::make_shared<object>(parse(message).as_object())));
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"PrimaryControlGranted"]), [&](const std::string& channel, const std::string& message)
		{
			Receive(std::make_shared<PrimaryControlGrantedMessage>(std::make_shared<object>(parse(message).as_object())));
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"ConstructControlGranted"]), [&](const std::string& channel, const std::string& message)
		{
			Receive(std::make_shared<ControlGrantedMessage>(std::make_shared<object>(parse(message).as_object())));
		});

		getSubscriber()->psubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlTransferGranted"]), [&](const std::string& channel, const std::string& message)
		{
			Receive(std::make_shared<ControlTransferGrantedMessage>(std::make_shared<object>(parse(message).as_object())));
		});

		// In this redis library, you have to commit pipelined transactions, which include all of what we've subscribed to above
		getSubscriber()->commit();
		isSubscribed = true;
	}

	void RedisMUSICEndpoint::UnsubscribeFromMUSIC()
	{
		_logger->debug("Unsubscribe from MUSIC Data");
		// No unsubscribe from all function in this library so gotta type it all out here.
		// No need to save a list of what we've subscribed to because we always subscribe to
		// all MUSIC channels so we need to always unsubscribe from every MUSIC channel.
		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"ConstructData"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"StateFieldData"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"InteractionRequest"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"InteractionResponse"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlRequest"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlTransferRequest"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlLost"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"PerceptionData"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"WaypointData"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"EventData"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"Command"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlReclamation"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlRegained"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlTransferResponse"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlResponse"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlRelinquished"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"PrimaryControlRelinquished"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"SetCurrentController"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"PrimaryControlResponse"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"PrimaryControlRequest"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlGained"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"PrimaryControlGranted"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"ConstructControlGranted"]));

		getSubscriber()->punsubscribe(StringHelper::wstring_to_string(getChannels()[L"ControlTransferGranted"]));

		// In this redis library, you have to commit pipelined transactions, which include all of what we've unsubscribed from above
		getSubscriber()->commit();
		isSubscribed = false;
	}

	void RedisMUSICEndpoint::VisitStateField(std::shared_ptr<StateFieldMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"StateFieldData"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {
			// If we want to print something on reply from this publish we'd do it like this.
			//std::cout << "set *channel* *message*: " << reply << std::endl;
			}).commit();
	}

	void RedisMUSICEndpoint::VisitConstructData(std::shared_ptr<ConstructDataMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"ConstructData"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitMUSICEvent(std::shared_ptr<MUSICEventMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"EventData"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitInteractionRequest(std::shared_ptr<InteractionRequestMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"InteractionRequest"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitPrimaryControlGranted(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::PrimaryControlGrantedMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"PrimaryControlGranted"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitControlGranted(std::shared_ptr<ControlGrantedMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"ConstructControlGranted"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitControlTransferGranted(std::shared_ptr<ControlTransferGrantedMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"ControlTransferGranted"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitInteractionResponse(std::shared_ptr<InteractionResponseMessage> message)
	{
		std::string json = serialize(*message->ToJsonObject());
		std::string channel = StringHelper::wstring_to_string(getChannels()[L"InteractionResponse"]);
		getPublisher()->publish(channel, json, [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitControlRequest(std::shared_ptr<ControlRequestMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"ControlRequest"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitControlTransferRequest(std::shared_ptr<ControlTransferRequestMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"ControlTransferRequest"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitCreateConstructRequest(std::shared_ptr<CreateConstructRequestMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitCreateEnvironmentRequest(std::shared_ptr<CreateEnvironmentRequestMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitFinalizeScenarioRequest(std::shared_ptr<FinalizeScenarioRequestMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitParameterizeConstructRequest(std::shared_ptr<ParameterizeConstructRequestMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitPrimaryControlRequest(std::shared_ptr<PrimaryControlRequestMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"PrimaryControlRequest"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitControlResponse(std::shared_ptr<ControlResponseMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"ControlResponse"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitControlTransferResponse(std::shared_ptr<ControlTransferResponseMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"ControlTransferResponse"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitCreateConstructResponse(std::shared_ptr<CreateConstructResponseMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitCreateEnvironmentResponse(std::shared_ptr<CreateEnvironmentResponseMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitFinalizeScenarioResponse(std::shared_ptr<FinalizeScenarioResponseMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitParameterizeConstructResponse(std::shared_ptr<ParameterizeConstructResponseMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitPrimaryControlResponse(std::shared_ptr<PrimaryControlResponseMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"PrimaryControlResponse"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitControlInitiated(std::shared_ptr<ControlInitiatedMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitControlLost(std::shared_ptr<ControlLostMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"ControlLost"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitControlReclamation(std::shared_ptr<ControlReclamationMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"ControlReclamation"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitControlRegained(std::shared_ptr<ControlRegainedMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"ControlRegained"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitControlReleased(std::shared_ptr<ControlReleasedMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitControlRelinquished(std::shared_ptr<ControlRelinquishedMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"ControlRelinquished"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitDisplayMessages(std::shared_ptr<DisplayMessagesMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitPrimaryControlRelinquished(std::shared_ptr<PrimaryControlRelinquishedMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"PrimaryControlRelinquished"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitRemoveConstruct(std::shared_ptr<RemoveConstructMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitRequestSimulationTime(std::shared_ptr<RequestSimulationTimeMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitScenarioStart(std::shared_ptr<ScenarioStartMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitSetCurrentController(std::shared_ptr<SetCurrentControllerMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"SetCurrentController"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitSetSimulationTime(std::shared_ptr<SetSimulationTimeMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitSimulationTime(std::shared_ptr<SimulationTimeMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitStopConstruct(std::shared_ptr<StopConstructMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitTransferConstructID(std::shared_ptr<TransferConstructIDMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"Command"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitControlGained(std::shared_ptr<ControlGainedMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"ControlGained"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitWaypointData(std::shared_ptr<WaypointDataMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"WaypointData"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}

	void RedisMUSICEndpoint::VisitPerceptionData(std::shared_ptr<PerceptionDataMessage> message)
	{
		getPublisher()->publish(StringHelper::wstring_to_string(getChannels()[L"PerceptionData"]), serialize(*message->ToJsonObject()), [](cpp_redis::reply& reply) {}).commit();
	}
}
