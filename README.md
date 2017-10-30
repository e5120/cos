# Docker C++ OS Project

### A Docker container for OS developing in C++.

The container can be build C++ source code.

### How to use this image?

#### Installation
1. Install Docker.
2. Cloning this repository.
3. Make ```shared``` directory in root of this repository.
4. Run ```docker-compose build``` and ```docker-compose create``` command. So you can make image and container.

#### Running and attach the container
1. Run ```docker-compose up -d``` command.
2. If you want to attach the container, please run ```docker exec -it rust-baremetal bash --login``` command.

#### Stop the container
1. Run ```docker-compose stop``` command.