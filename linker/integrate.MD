# Linking the FeatureExtractor within another C-based project

## Steps
1. include `Wrapper.h` (which is already 'include guarded') in: `init.h`, `analyse.c` and/or `analyse.h`
1. Add new member to struct `globals_t`: namely `FeatureExtractot *fex`
1. Construct and destruct the FeatureExtract struct/object within `init.c`:`badcat_main(..)`
