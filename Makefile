CXX=g++
CXXFLAGS=-Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Winit-self -Wshadow -Wlogical-op -Wcomment -Wctor-dtor-privacy -Wold-style-cast -D_GLIBCXX_DEBUG -fno-diagnostics-show-option

all: clean server client PI
		
server:
	$(CXX) $(CXXFLAGS) -o ./Server/Server.o ./Server/*.cc ./Networking/*.cc

client:
	$(CXX) $(CXXFLAGS) -o ./Client/Client.o ./Client/*.cc ./Networking/*.cc

PI: 
	$(CXX) $(CXXFLAGS) -o ./RasberryPI/PI.o ./RasberryPI/*.cc ./Networking/*.cc

encrypt: 
	$(CXX) $(CXXFLAGS) -o Encrypt.o ./Encryption/*.cc

test:
	$(CXX) $(CXXFLAGS)  -o Test.o *.cc ./*/[!main]*.cc
clean:
	rm -f *.o
