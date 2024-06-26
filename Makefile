CXX=g++ -std=c++17
INCLUDES=-I includes/
CXXFLAGS=$(INCLUDES)

test_dns: bin/test_dns
bin/test_dns: ./src/*.cpp ./includes/*.hpp test.cpp
		$(CXX) $(CXXFLAGS) includes/*.hpp ./src/*.cpp test.cpp -O0 -o $@

server: bin/server
bin/server: ./src/*.cpp ./includes/*.hpp server_main.cpp 
		$(CXX) $(CXXFLAGS) includes/*.hpp ./src/*.cpp server_main.cpp -O0 -pthread -o $@

client: bin/client
bin/client: ./src/*.cpp ./includes/*.hpp client_main.cpp 
		$(CXX) $(CXXFLAGS) includes/*.hpp ./src/*.cpp client_main.cpp -O0 -pthread -o $@

debug: bin/debug
bin/debug: ./src/*.cpp ./includes/*.hpp test.cpp
		$(CXX) $(CXXFLAGS) includes/*.hpp ./src/*.cpp test.cpp -g -o $@

server_debug: bin/server_debug
bin/server_debug: ./src/*.cpp ./includes/*.hpp server_main.cpp 
		$(CXX) $(CXXFLAGS) includes/*.hpp ./src/*.cpp server_main.cpp -O0 -g -pthread -o $@
# 
.DEFAULT_GOAL := test_dns
# clean:
# 		rm -fr bin/*