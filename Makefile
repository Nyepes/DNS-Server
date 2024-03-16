CXX=clang++ -std=c++20

exec: dns

dns: ./src/*.cpp ./src/*.hpp
	$(CXX) ./src/*.cpp
.DEFAULT_GOAL := exec
.PHONY: clean exec tests