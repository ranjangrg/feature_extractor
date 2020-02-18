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

EXECUTABLE_1=extract.out
EXECUTABLE_2=streamReader.out

SOURCES_1=extract.cpp
SOURCES_2=streamReader.cpp

OBJECTS=$(SOURCES:.cpp=.o)

.PHONY: extract
	
# list xxx: $(src1) $(src2)
# 'make' checks for changes in src1 and src2 file when 'make' command is run again
extract: $(SOURCES_EXT1) 
	g++ $(CFLAGS) $(INCLUDES) $(LDPATH) $(SOURCES_1) -g -o $(EXECUTABLE_1) $(LDFLAGS) # -g for debugging options in gdb
	
streamReader: $(SOURCES_EXT1)
	g++ $(CFLAGS) $(INCLUDES) $(LDPATH) $(SOURCES_2) -g -o $(EXECUTABLE_2) $(LDFLAGS) # -g for debugging options in gdb

clean:
	# quiet clean-up
	-@rm $(EXECUTABLE_1)  2> /dev/null || true;
	-@rm $(EXECUTABLE_2)  2> /dev/null || true;
