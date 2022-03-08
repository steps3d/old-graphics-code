#
# Makefile to build ARB_separate_shader_objects for Linux
#
# Author: Alex V. Boreskoff, <steps3d@narod.ru>, <steps3d@gmail.com>
#

GLUTPATH   = /usr/include/GL
CONLYFLAGS = -g -I/usr/include/GL -Wall -I$(GLUTPATH)/include/GL
LIBS       = -L/usr/X11R6/lib -L$(GLUTPATH)/lib -lglut -lGL -lXt -lX11 -lm -ljpeg -lz -lpng -lGLEW
STEPS_PATH = ..
CFLAGS     = -x c++ -g -I/usr/include/GL -Wall -I$(GLUTPATH)/include/GL -I. -I$(STEPS_PATH)/include

ALL = test-pipeline test-pipeline-2

all: $(ALL)

test-pipeline: test-pipeline.o
	$(CC) $(LDFLAGS) $^ $(STEPS_PATH)/build/steps.a -o $@ $(LIBS) -lc

test-pipeline-2: test-pipeline-2.o 
	$(CC) $(LDFLAGS) $^ $(STEPS_PATH)/build/steps.a -o $@ $(LIBS) -lc


clean:
	rm -r -f $(ALL) *.o 2> /dev/null

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $<



