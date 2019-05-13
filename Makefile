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

EXECUTABLE_1=extract

SOURCES_1=extract.cpp

OBJECTS=$(SOURCES:.c=.o)

.PHONY: extract
	
extract: $(SOURCES_EXT1)
	g++ $(CFLAGS) $(INCLUDES) $(LDPATH) $(SOURCES_1) -g -o $(EXECUTABLE_1) $(LDFLAGS) # -g for debugging options in gdb

clean:
	# quiet clean-up
	-@rm $(EXECUTABLE_1)  2> /dev/null || true;
