# TaskManager
## Description
Client-server application for scheduling tasks. The application was created using [Protobuf](https://developers.google.com/protocol-buffers), [GRPC](https://grpc.io) and [Boost](https://www.boost.org) libraries.
### Features
- Comfortable labeling system
- Multiple client support
- Support for using the application locally, without a server
## Installation
### Linux and MacOS using CMake
1. Install `CMake`, minimum version: **3.20**
2. Install libraries:
    - [Protobuf](https://github.com/protocolbuffers/protobuf)
    - [GRPC](https://github.com/grpc/grpc)
    - [Boost](https://www.boost.org)
    - [GoogleTest](https://github.com/google/googletest)
3. Build from source:
```
git clone https://github.com/MichaelKupriianov/TaskManager.git
cd TaskManager
mkdir build
cd build
cmake ..
make
```
4. Launch server (you can set the server port using `-p`):
    - `./server`
5. Launch client (you can set host using `-h` and port using `-p`):
    - `./client`
6. If you want to use the task manager locally:
    - `./TaskManager`

Enter `help` if you want to know about possible commands.