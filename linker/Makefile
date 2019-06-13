CFLAGS+= -pipe -Wall -O2 -fPIC

#LDFLAGS+= -lfftw3	# for fft stuffs
#LDFLAGS+= -lrtfilter	# for filters
#LDFLAGS+= -lFLAC	# for readFLAC.c
#LDFLAGS+= -lxtract	# for extracting audio features
#LDFLAGS+= -lm		# has to be after -lxtract because libxtract.a uses Math.c ...
LDFLAGS+= -lessentia 
LDFLAGS+= -lfftw3 
LDFLAGS+= -lyaml 
LDFLAGS+= -lavcodec 
LDFLAGS+= -lavformat 
LDFLAGS+= -lavutil 
LDFLAGS+= -lsamplerate 
LDFLAGS+= -ltag 
LDFLAGS+= -lfftw3f 

INCLUDES= -I./extHeaders
INCLUDES+= -I/usr/local/include/essentia/ 
INCLUDES+= -I/usr/local/include/essentia/scheduler/ 
INCLUDES+= -I/usr/local/include/essentia/streaming/  
INCLUDES+= -I/usr/local/include/essentia/utils 
INCLUDES+= -I/usr/include/taglib 
INCLUDES+= -D__STDC_CONSTANT_MACROS

LDPATH+= -L/usr/local/lib

EXECUTABLE_MAIN=main.out

EXECUTABLE_1=extract.out
EXECUTABLE_2=analyseAudio.out

SOURCE_1=extract.cpp
SOURCE_2=analyseAudio.c
SOURCE_3=Wrapper.cc

OBJECT_1=extract.o
OBJECT_2=analyseAudio.o
OBJECT_3=Wrapper.o

# extract.o
$(OBJECT_1): $(SOURCE_1)
	g++ -c $(CFLAGS) $(INCLUDES) $(LDPATH) $(SOURCE_1) -g -o $@ $(LDFLAGS) # -g for debugging options in gdb

# analyseAudio.o
$(OBJECT_2): $(SOURCE_2)
	gcc -c $(CFLAGS) $(SOURCE_2) -g -o $@ # -g for debugging options in gdb

# wrapper.o
$(OBJECT_3): $(SOURCE_3)
	g++ -c $(SOURCE_3) -o $(OBJECT_3)

# main.out
$(EXECUTABLE_MAIN): $(OBJECT_1) $(OBJECT_2) $(OBJECT_3)
	g++ $(OBJECT_1) $(OBJECT_2) $(OBJECT_3) -g -o $@

.PHONY: clean
clean:
	# quiet clean-up
	-@rm $(EXECUTABLE_1)  2> /dev/null || true;
	-@rm $(OBJECT_1)  2> /dev/null || true;
	-@rm $(EXECUTABLE_2)  2> /dev/null || true;
	-@rm $(OBJECT_2)  2> /dev/null || true;
	-@rm $(EXECUTABLE_3)  2> /dev/null || true;
	-@rm $(OBJECT_3)  2> /dev/null || true;
	-@rm $(EXECUTABLE_MAIN) 2> /dev/null || true;
