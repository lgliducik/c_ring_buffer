all: build

build:
	clang++ -o ring_buffer ring_buffer.cpp

debug:
	clang++ -o ring_buffer -g ring_buffer.cpp
