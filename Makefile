# Project Name
TARGET = Brizachord

# Sources
CPP_SOURCES = main.cpp DrumSynth.cpp Sequencer.cpp ChordSynth.cpp StrumSynth.cpp Brizachord.cpp


# Library Locations
LIBDAISY_DIR = ../../libDaisy/
DAISYSP_DIR = ../../DaisySP/

# Core location, and generic Makefile.
C_INCLUDES += -I$(DAISYSP_DIR)/Source
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
