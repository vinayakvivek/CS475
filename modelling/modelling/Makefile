CC = g++

OPENGLLIB = -framework OpenGL
GLEWLIB = -lGLEW
GLFWLIB = -lglfw
LIBS = $(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS = -L/opt/local/lib 
CPPFLAGS =-I/usr/local/include -std=c++11

BIN = modelling
SRCS = model.cpp gl_framework.cpp shader_util.cpp 
INCLUDES = model.hpp gl_framework.hpp shader_util.hpp

all: $(BIN)

$(BIN): $(SRCS) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS) -o $(BIN) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN)