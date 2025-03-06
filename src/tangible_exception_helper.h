#pragma once

#ifndef TANGIBLE_EXCEPTION_HELPER_H
#define TANGIBLE_EXCEPTION_HELPER_H

#include <string>
#include <exception>

class InvalidOperationException : public std::exception
{
private:
    std::string msg;

public:
    InvalidOperationException(const std::string &message = "") : msg(message)
    {
    }

    const char * what() const noexcept
    {
        return msg.c_str();
    }
};

class MissingMethodException : public std::exception
{
private:
    std::string msg;

public:
    MissingMethodException(const std::string &message = "") : msg(message)
    {
    }

    const char * what() const noexcept
    {
        return msg.c_str();
    }
};

class NotImplementedException : public std::exception
{
private:
    std::string msg;

public:
    NotImplementedException(const std::string &message = "") : msg(message)
    {
    }

    const char * what() const noexcept
    {
        return msg.c_str();
    }
};

class KeyNotFoundException : public std::exception
{
private:
    std::string msg;

public:
    KeyNotFoundException(const std::string &message = "") : msg(message)
    {
    }

    const char * what() const noexcept
    {
        return msg.c_str();
    }
};

class HandlerNotFoundException : public std::exception
{
private:
	std::string msg;

public:
	HandlerNotFoundException(const std::string &message = "") : msg(message){
	}

	const char * what() const noexcept
	{
		return msg.c_str();
	}
};

class EventNotFoundException : public std::exception
{
private:
	std::string msg;
public:
	EventNotFoundException(const std::string &message = "") : msg(message) {
	}

	const char * what () const throw()
	{
		return msg.c_str();
	}
};

class EndpointInitializationException : public std::exception
{
private:
	std::string msg;
public:
	EndpointInitializationException(const std::string &message = "") : msg(message) {
	}
	const char * what() const throw()
	{
		return msg.c_str();
	}
};

class SubscriptionException : public std::exception
{
private:
	std::string msg;
public:
	SubscriptionException(const std::string &message = "") : msg(message) {
	}
	const char * what() const throw()
	{
		return msg.c_str();
	}
};

class RequestStatusNotFoundException : public std::exception
{
private:
	std::string msg;
public:
	RequestStatusNotFoundException(const std::string &message = "") : msg(message) {
	}
	const char * what() const throw()
	{
		return msg.c_str();
	}
};

class TransactionAlreadyExistsForThreadException : public std::exception
{
private:
	std::string msg;

public:
	TransactionAlreadyExistsForThreadException(const std::string &message = "") : msg(message)
	{
	}

	const char * what() const noexcept
	{
		return msg.c_str();
	}
};

class EntityIDIsZeroException : public std::exception
{
private:
	std::string msg;

public:
	EntityIDIsZeroException(const std::string &message = "") : msg(message)
	{
	}

	const char * what() const noexcept
	{
		return msg.c_str();
	}
};

class ConstructNotFoundException : public std::exception
{
private:
	std::string msg;

public:
	ConstructNotFoundException(const std::string &message = "") : msg(message)
	{
	}

	const char * what() const noexcept
	{
		return msg.c_str();
	}
};

class TransactionCommitException : public std::exception
{
private:
	std::string msg;

public:
	TransactionCommitException(const std::string &message = "") : msg(message)
	{
	}

	const char * what() const noexcept
	{
		return msg.c_str();
	}
};

#endif // !TANGIBLE_EXCEPTION_HELPER_H