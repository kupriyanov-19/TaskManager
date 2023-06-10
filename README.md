# TaskManager
## Description
Client-server cross-platform application for scheduling tasks. The application was created using [Protobuf](https://developers.google.com/protocol-buffers) and [GRPC](https://grpc.io) libraries.
### Features
- Multiple client support
- Works in various operating systems
- Support for using the application locally, without a server
- Comfortable labeling system
- Support for a complex structure of tasks
## Installation
1. Install `CMake`, minimum version: **3.20**
2. Install libraries:
    - [Protobuf](https://github.com/protocolbuffers/protobuf)
    - [GRPC](https://github.com/grpc/grpc)
   
For example, if you have pacman, you can use:

`pacman -Syyu pkgconf make git cmake clang grpc protobuf`
   
3. Build from source:
```
git clone https://github.com/kupriyanov-19/TaskManager.git
cd TaskManager
git checkout <what you want to launch: server, client or local>
mkdir build
cd build
cmake ..
cmake -- build .
```
4. Get started:
- If you want to launch server:
    - `./server` at Linux/MacOS
    - `Debug/server.exe` at Windows
- If you want to launch client:
    - `./client` at Linux/MacOS
    - `Debug/client.exe` at Windows
- If you want to use the task manager locally:
    - `./TaskManager` at Linux/MacOS
    - `Debug/TaskManager.exe` at Windows

Enter `help` if you want to know about possible commands.