#
# Makefile to build OpenGL 3.3/4.1 examples for Linux
#
# Author: Alex V. Boreskoff, <steps3d@narod.ru>, <steps3d@gmail.com>
#

GLUTPATH   = /usr/include/GL
CFLAGS     = -x c++ -g -I/usr/include/GL -Wall -I$(GLUTPATH)/include/GL -I.
CONLYFLAGS = -g -I/usr/include/GL -Wall -I$(GLUTPATH)/include/GL
LIBS       = -L/usr/X11R6/lib -L$(GLUTPATH)/lib -lglut -lGL -lXt -lX11 -lm -ljpeg -lz -lpng -lGLEW

MATH = vec2.o vec3.o vec4.o mat2.o mat3.o mat4.o quat.o plane.o bbox.o randUtils.o eig3.o
WRAP = Data.o Program.o VertexBuffer.o stringUtils.o glUtilities.o Sampler.o ScreenQuad.o Sync.o Query.o

ALL = tf3

all: $(ALL)

tf3: tf3.o  $(MATH) $(WRAP)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

clean:
	rm -r -f $(ALL) *.o 2> /dev/null

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $<


%.o: eig3x3/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $<


