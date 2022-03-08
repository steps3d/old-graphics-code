#
# Makefile to build OpenGL 3.3/4.1 examples for Linux
#
# Author: Alex V. Boreskoff, <steps3d@narod.ru>, <steps3d@gmail.com>
#

STEPS_PATH = /home/alex/Books/New-2/steps-framework/svn
GLUTPATH   = /usr/include/GL
CFLAGS     = -x c++ -g -I/usr/include/GL -Wall -I$(GLUTPATH)/include/GL -I. -I$(STEPS_PATH)/include
CONLYFLAGS = -g -I/usr/include/GL -Wall -I$(GLUTPATH)/include/GL
LIBS       = -L/usr/X11R6/lib -L$(GLUTPATH)/lib -lglut -lGL -lXt -lX11 -lm -ljpeg -lz -lpng -lGLEW
CC         = g++

ALL = pn phong

all: $(ALL)

pn: pn.o $(STEPS_PATH)/build/steps.a
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc

phong: phong.o $(STEPS_PATH)/build/steps.a
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS) -lc


clean:
	rm -r -f $(ALL) *.o 2> /dev/null

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $<



