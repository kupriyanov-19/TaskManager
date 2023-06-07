FROM archlinux:latest

RUN	pacman -Syyu --noconfirm pkgconf make git cmake clang grpc boost gtest protobuf


RUN git clone https://github.com/MichaelKupriianov/TaskManager.git
 
WORKDIR "/TaskManager"
  
RUN git checkout docker

RUN mkdir build
RUN cmake -S /TaskManager/ -B /TaskManager/build

RUN cmake --build build/ -- -j2

EXPOSE 1234
