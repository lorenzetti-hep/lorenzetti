
# Set the shell.
SHELL=/usr/bin/env bash

# Include the configuration.
-include Makefile.inc

# Handle GZIP support.
GZIP_INC=
GZIP_FLAGS=
ifeq ($(GZIP_USE),true)
  GZIP_INC+= -DGZIPSUPPORT -I$(GZIP_INCLUDE)
  GZIP_FLAGS+= -L$(GZIP_LIB) -Wl,-rpath,$(GZIP_LIB) -lz
endif

CXX_COMMON:=-I$(PYTHIA8_INCLUDE) $(CXX_COMMON)
CXX_COMMON+= -L$(PYTHIA8_LIBRARY) -Wl,-rpath,$(PYTHIA8_LIBRARY) -lpythia8 -ldl -std=c++11


# Rules without physical targets (secondary expansion for specific rules).
.SECONDEXPANSION:
.PHONY: all clean

# All targets (no default behavior).
all:
	@echo "Usage: make 1"




# FASTJET3 and HEPMC2.
generator : generator.cpp $(PYTHIA8_LIBRARY)/libpythia8.a
	$(CXX) $< -o $@ -I$(FASTJET_INCLUDE) -I$(HEPMC_INCLUDE) -I$(ROOT_INCLUDE) -L$(ROOT_LIBRARY) $(CXX_COMMON)\
	 -L$(HEPMC_LIBRARY) -lHepMC -L$(FASTJET_LIBRARY) -lfastjet \
	  $(GZIP_INC) $(GZIP_FLAGS) -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript \
  -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -lpthread -stdlib=libc++ -lm -ldl








