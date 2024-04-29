CXX=g++
CXXFLAGS=-Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Winit-self -Wshadow -Wlogical-op -Wcomment -Wctor-dtor-privacy -Wold-style-cast -D_GLIBCXX_DEBUG -fno-diagnostics-show-option -pthread

all: clean server client PI
		
server:
	$(CXX) $(CXXFLAGS) -o ./Server/Server.o ./Server/*.cc ./Networking/*.cc ./Encryption/[!main]*.cc

client:
	$(CXX) $(CXXFLAGS) -o ./Client/Client.o ./Client/*.cc ./Networking/*.cc ./Encryption/[!main]*.cc

PI: 
	$(CXX) $(CXXFLAGS) -o ./RasberryPI/PI.o ./RasberryPI/*.cc ./Networking/*.cc ./Encryption/[!main]*.cc

encrypt: 
	$(CXX) $(CXXFLAGS) -o Encrypt.o ./Encryption/*.cc

test:
	$(CXX) $(CXXFLAGS)  -o Test.o *.cc ./*/[!main]*.cc
clean:
	rm -f *.o
