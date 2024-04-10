CXX=g++
CXXFLAGS=-Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Winit-self -Wshadow -Wlogical-op -Wcomment -Wctor-dtor-privacy -Wold-style-cast -D_GLIBCXX_DEBUG -fno-diagnostics-show-option

all: clean
	$(CXX) $(CXXFLAGS) -o Test.o *.cc ./*/[!main]*
		
server:
	$(CXX) $(CXXFLAGS) -o Server.o ./Server/*

client:
	$(CXX) $(CXXFLAGS) -o Client.o ./Client/*

PI: 
	$(CXX) $(CXXFLAGS) -o PI.o ./RasberryPI/*

clean:
	rm -f *.o
