# TaskManager
## Description
Client-server application for scheduling tasks. The application was created using [Protobuf](https://developers.google.com/protocol-buffers) and [GRPC](https://grpc.io) libraries.
### Features
- Comfortable labeling system
- Multiple client support
- Support for using the application locally, without a server
## Installation
1. Install `CMake`, minimum version: **3.20**
2. Install libraries:
    - [Protobuf](https://github.com/protocolbuffers/protobuf)
    - [GRPC](https://github.com/grpc/grpc)
   
For example, if you have pacman, you can use:
```
pacman -Syyu pkgconf make git cmake clang grpc protobuf
```
   
3. Build from source:
```
git clone https://github.com/kupriyanov-19/TaskManager.git
cd TaskManager
git checkout <what you want to launch: server, client or local>
mkdir build
cd build
cmake ..
make
```
- If you want to launch server:

    - `./server`

- If you want to launch client:
    - `./client`
- If you want to use the task manager locally:
    - `./TaskManager`

Enter `help` if you want to know about possible commands.