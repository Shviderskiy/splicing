TARGET        ?= libapi.a
CONFIGURATION ?= debug
ARCHITECTURE  ?=

COMPILER ?= g++
ARCHIVER = ar

COMPILER_WARNINGS = -pedantic -Wall -Wextra -Weffc++

COMPILER_OPTIONS = -std=c++11 $(COMPILER_WARNINGS)
ARCHIVER_OPTIONS = rc


ifeq ($(CONFIGURATION), debug)
COMPILER_OPTIONS += -g -O0
else ifeq ($(CONFIGURATION), release)
COMPILER_OPTIONS += -O2
endif

ifeq ($(ARCHITECTURE), x86)
COMPILER_OPTIONS += -m32
else ifeq ($(ARCHITECTURE), amd64)
COMPILER_OPTIONS += -m64
endif

HEADERS_DIR = ../headers
SOURCES_DIR = ../sources
OBJECTS_DIR = ../objects

HEADERS_FILES = $(shell find $(HEADERS_DIR) -type f -regex ".*\.\(hpp\)")
SOURCES_FILES = $(shell find $(SOURCES_DIR) -type f -regex ".*\.\(cpp\)")
OBJECTS_FILES = $(patsubst %.cpp, %.o, \
	$(addprefix $(OBJECTS_DIR)/, \
		$(SOURCES_FILES:$(SOURCES_DIR)/%=%)))


.PHONY: build clean

build: $(TARGET)

clean:
	rm -rf $(TARGET) $(OBJECTS_DIR)


$(TARGET): $(OBJECTS_FILES)
	$(ARCHIVER) $(ARCHIVER_OPTIONS) $(TARGET) $(OBJECTS_FILES)

$(OBJECTS_DIR)/%.o: $(SOURCES_DIR)/%.cpp $(HEADERS_FILES)
	@ \
	if [ ! -e $@ ]; \
	then \
		mkdir -p $$(dirname $@); \
		echo $(COMPILER) $(COMPILER_OPTIONS) -I$(HEADERS_DIR) -c $< -o $@; \
		$(COMPILER) $(COMPILER_OPTIONS) -I$(HEADERS_DIR) -c $< -o $@; \
	else \
		LMF=$$( \
			$(COMPILER) $(COMPILER_OPTIONS) -I$(HEADERS_DIR) -MM $< | \
			python last_modified_file.py \
		); \
		if [ $$(stat --printf="%Y\n" $$LMF) -gt $$(stat --printf="%Y\n" $@) ]; \
		then \
			echo $(COMPILER) $(COMPILER_OPTIONS) -I$(HEADERS_DIR) -c $< -o $@; \
			$(COMPILER) $(COMPILER_OPTIONS) -I$(HEADERS_DIR) -c $< -o $@; \
		fi \
	fi
