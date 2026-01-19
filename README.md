# MUSIC C++ DTO library

C++ Rewrite of the MUSIC DTO library

# Setup

### General
This library depends on the following packages:
- [open-dis](https://github.com/open-dis/open-dis-cpp.git)
- [asio](https://github.com/chriskohlhoff/asio.git)
- [nlohmann-json](https://github.com/nlohmann/json)
- [spdlog](https://github.com/gabime/spdlog)
- [cpputest](https://github.com/cpputest/cpputest) (*optional if not building unit tests)

>[!TIP]
> If using the VSCode dev container (see below), these will be automatically installed for you

### VSCode
1. If not already configured, install the [VSCode Dev Containers plugin](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) to your VSCode
2. Open this repository as the root directory of the VSCode workspace (e.g. by running `code .` in a terminal from the root of this repository)
3. VSCode should detect the dev container. Click 'Reopen in container'. The dev container will automatically set up the required depenencies on first build.

# Contributing
Please follow the below guidelines when contributing to this repository

## Code Style
This repository follows the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) for the project source.