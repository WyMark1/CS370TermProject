CXX=g++
CXXFLAGS=-Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Winit-self -Wshadow -Wlogical-op -Wcomment -Wctor-dtor-privacy -Wold-style-cast -D_GLIBCXX_DEBUG -fno-diagnostics-show-option

all: clean
	$(CXX) $(CXXFLAGS) -o Test.o *.cc ./*/[!main]*.cc
		
server:
	$(CXX) $(CXXFLAGS) -o Server.o ./Server/*.cc

client:
	$(CXX) $(CXXFLAGS) -o Client.o ./Client/*.cc

PI: 
	$(CXX) $(CXXFLAGS) -o PI.o ./RasberryPI/*.cc

encrypt: 
	$(CXX) $(CXXFLAGS) -o Encrypt.o ./Encryption/*.cc

clean:
	rm -f *.o
