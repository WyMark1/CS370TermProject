CXX=g++
CXXFLAGS=-Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Winit-self -Wshadow -Wlogical-op -Wcomment -Wctor-dtor-privacy -Wold-style-cast -D_GLIBCXX_DEBUG -fno-diagnostics-show-option

all: clean
	$(CXX) $(CXXFLAGS) -o Test *.cc
		
clean:
	rm -f Test
	