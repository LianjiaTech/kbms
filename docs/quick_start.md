Quick Start
---

# Dependent open source code

1. libevent
2. glog
3. jsoncpp
4. curl
5. gtest (optional)

# How to compile it

### Install compiling tool chain

1. gcc (GCC) 4.8+
2. GNU Make
3. git
4. CMake

### Download the source code of evpp

	$ git clone project 
	$ cd ke-bear-microservice
	$ git submodule update --init --recursive

### Compile and run unit tests.
	
	$ mkdir -p build && cd build
	$ cmake  ..
	$ make -j
