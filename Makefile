CXX=g++
INCLUDES=-I includes/
CXXFLAGS=$(INCLUDES)

test_dns: bin/test_dns
bin/test_dns: ./src/*.cpp ./includes/*.hpp test.cpp
		$(CXX) $(CXXFLAGS) includes/*.hpp ./src/*.cpp test.cpp -O3 -o $@

debug: bin/debug
bin/debug: ./src/*.cpp ./includes/*.hpp test.cpp
		$(CXX) $(CXXFLAGS) includes/*.hpp ./src/*.cpp test.cpp -g -o $@
# 
.DEFAULT_GOAL := test_dns
# clean:
# 		rm -fr bin/*