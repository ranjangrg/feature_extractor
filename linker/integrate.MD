# Linking the FeatureExtractor within another C-based project

## Steps
1. Include `Wrapper.h` (which is already 'include guarded') in: `init.h`, `analyse.c` and/or `analyse.h`
1. Add new member to struct `globals_t`: namely `FeatureExtractor *fex`
1. Construct and destruct the `FeatureExtractor` struct/object within `init.c`:`badcat_main(..)`

## Example MakeFile (only compile commands)
```bash
# badcat stuffs
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  downsampler.c -o downsampler.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  file.c -o file.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  linked_list.c -o linked_list.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  pid.c -o pid.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  timex.c -o timex.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  analyse_ac_filters.c -o analyse_ac_filters.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  bwbpf.c -o bwbpf.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  dumper.c -o dumper.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  loop.c -o loop.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  recorder.c -o recorder.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  transportdb.c -o transportdb.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  analyse_decimate.c -o analyse_decimate.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  bwhpf.c -o bwhpf.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  encode_mp3.c -o encode_mp3.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  init.c -o init.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  wavfile.c -o wavfile.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  analyse_sbrb_filters.c -o analyse_sbrb_filters.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  chkdir.c -o chkdir.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  event.c -o event.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  leaks.c -o leaks.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  percentile.c -o percentile.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  target.c -o target.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  encode_flac.c -o encode_flac.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  sbra_rails_bandpass.c -o sbra_rails_bandpass.o
gcc  -ggdb  -I/usr/local/include -pthread -c -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  badcat_main.c -o badcat_main.o

# FeatureExtractor
g++ -c -pipe -Wall -O2 -fPIC -I/usr/local/include/essentia/  -I/usr/local/include/essentia/scheduler/  -I/usr/local/include/essentia/streaming/   -I/usr/local/include/essentia/utils  -I/usr/include/taglib  -D__STDC_CONSTANT_MACROS extract.cpp -g -o extract.o # -g for debugging options in gdb

# Wrapper
g++ -c Wrapper.cc -o Wrapper.o
gcc -ggdb  -I/usr/local/include -pthread -c -pipe -O2 -fPIC -Wall -Winline -DSVN_REV="\"15392M\"" -pthread -c -Wall -Winline  analyse.c -o analyse.o

# main badcat
g++ -ggdb -L/usr/local/lib -lrt -lm -lpthread -lmp3lame -lconfig -lrtfilter -lfftw3 -lsqlite3 -lFLAC -logg -lessentia -lyaml  -lavcodec  -lavformat  -lavutil  -lsamplerate  -ltag  -lfftw3f analyse.o downsampler.o file.o linked_list.o pid.o timex.o analyse_ac_filters.o bwbpf.o dumper.o loop.o recorder.o transportdb.o analyse_decimate.o bwhpf.o encode_mp3.o init.o wavfile.o analyse_sbrb_filters.o chkdir.o event.o leaks.o percentile.o target.o encode_flac.o sbra_rails_bandpass.o badcat_main.o Wrapper.o extract.o -o badcat -lrt -lm -lpthread -lmp3lame -lconfig -lrtfilter -lfftw3 -lsqlite3 -lFLAC -logg -lessentia -lyaml  -lavcodec  -lavformat  -lavutil  -lsamplerate  -ltag  -lfftw3f
```
