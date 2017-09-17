CC = g++

OPENGLLIB = -framework OpenGL
GLEWLIB = -lGLEW
GLFWLIB = -lglfw
LIBS = $(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS = -L/opt/local/lib
CPPFLAGS = -std=c++11 -I/usr/local/include -I./

BIN = assign2
SRCS= main.cpp gl_framework.cpp shader_util.cpp
INCLUDES = gl_framework.hpp shader_util.hpp

all: $(BIN)

$(BIN): $(SRCS) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS) -o $(BIN) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN)
