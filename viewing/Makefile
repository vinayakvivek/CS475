CC = g++

GLEWLIB = -lGLEW
GLFWLIB = -lglfw
LIBS = $(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
CPPFLAGS = -std=c++11 -I/usr/local/include -I./

BIN = assign2
SRCS = main.cpp view.cpp model.cpp gl_framework.cpp shader_util.cpp
INCLUDES = view.hpp model.hpp gl_framework.hpp shader_util.hpp

OS := $(shell uname)
ifeq ($(OS), Darwin)
  # MacOS
  OPENGLLIB = -framework OpenGL
  LDFLAGS = -L/opt/local/lib
else
  # Linux
  OPENGLLIB = -lGL
  LDFLAGS = -L/usr/local/lib
endif

all: $(BIN)

$(BIN): $(SRCS) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS) -o $(BIN) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN)
