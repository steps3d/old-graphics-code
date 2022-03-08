#
# Makefile to build OpenGL 3.3/4.1 examples for Linux
#
# Author: Alex V. Boreskoff, <steps3d@narod.ru>, <steps3d@gmail.com>
#

GLUTPATH   = /usr/include/GL
CFLAGS     = -x c++ -g -I/usr/include/GL -Wall -I$(GLUTPATH)/include/GL
CONLYFLAGS = -g -I/usr/include/GL -Wall -I$(GLUTPATH)/include/GL
LIBS       = -L/usr/X11R6/lib -L$(GLUTPATH)/lib -lglut -lGL -lXt -lX11 -lm -ljpeg -lz -lpng -lGLEW

ALL = light-ub

all: $(ALL)

light-ub: light-ub.o vec2.o vec3.o vec4.o mat3.o mat4.o Data.o Program.o checks.o VertexBuffer.o stringUtils.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

clean:
	rm -r -f $(ALL) *.o 2> /dev/null

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $<


