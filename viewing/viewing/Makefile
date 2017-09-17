CC = g++

OPENGLLIB = -framework OpenGL
GLEWLIB = -lGLEW
GLFWLIB = -lglfw
LIBS = $(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS = -L/opt/local/lib
CPPFLAGS = -std=c++11 -I/usr/local/include -I./

BIN = assign2
SRCS = main.cpp view.cpp model.cpp gl_framework.cpp shader_util.cpp
INCLUDES = view.hpp model.hpp gl_framework.hpp shader_util.hpp

all: $(BIN)

$(BIN): $(SRCS) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS) -o $(BIN) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN)


# src = $(wildcard *.cpp)
# obj = $(src:.c=.o)
# dep = $(obj:.o=.d)  # one dependency file for each source

# OPENGLLIB = -framework OpenGL
# GLEWLIB = -lGLEW
# GLFWLIB = -lglfw
# LIBS = $(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
# LDFLAGS = -L/opt/local/lib $(LIBS)
# CPPFLAGS = -std=c++11 -I/usr/local/include -I./

# mygame: $(obj)
# 	g++ $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

# -include $(dep)   # include all dep files in the makefile

# # rule to generate a dep file by using the C preprocessor
# # (see man cpp for details on the -MM and -MT options)
# %.d: %.c
# 	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

# .PHONY: clean
# clean:
# 	rm -f $(obj) mygame

# .PHONY: cleandep
# cleandep:
# 	rm -f $(dep)
