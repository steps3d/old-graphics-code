#
# Makefile to build StepsFramework for Linux
#
# Author: Alex V. Boreskoff, <steps3d@narod.ru>, <steps3d@gmail.com>
#

GLUTPATH   = /usr/include/GL
CFLAGS     = -x c++ -g -I/usr/include/GL -Wall -Ilibs -Iinclude -Icontrib
CONLYFLAGS = -g -I/usr/include/GL -Wall -Ilibs -Iinclude
LIBS       = -L/usr/X11R6/lib -L$(GLUTPATH)/lib -lglut -lGL -lXt -lX11 -lm -ljpeg -lz -lpng -lGLEW

MATH = build/vec2.o build/vec3.o build/vec4.o build/mat2.o build/mat3.o build/mat4.o build/quat.o build/plane.o build/bbox.o build/randUtils.o build/eig3.o build/kdop.o
TEXT = build/Texture.o build/TexImage.o build/TgaEncoder.o build/TgaDecoder.o build/DdsDecoder.o build/BmpDecoder.o build/JpegDecoder.o build/PngDecoder.o build/Resources.o build/FileSystem.o build/ZipFileSystem.o build/TexFormat.o
WRAP = build/Data.o build/Program.o build/VertexBuffer.o build/Framebuffer.o build/stringUtils.o build/glUtilities.o build/UbAccessor.o build/Sampler.o build/half.o build/ScreenQuad.o build/Sync.o build/Query.o build/Pipeline.o build/BasicMesh.o build/GlutWindow.o

all: build steps.a

build:
	mkdir build
	
steps.a: $(MATH) $(TEXT) $(WRAP)
	ar -rcv $@ $^
	rm build/*.o
	mv steps.a build


clean:
	rm -r -f *.o 2> /dev/null
	rm -rf build 2> /dev/null

build/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

build/%.o: contrib/eig3x3/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@


