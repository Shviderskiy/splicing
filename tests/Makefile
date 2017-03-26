TARGET        ?= tests
API_LIB       ?= libapi.a
CONFIGURATION ?= debug
ARCHITECTURE  ?=

COMPILER ?= g++

COMPILER_WARNINGS = -pedantic -Wall -Wextra -Weffc++

COMPILER_OPTIONS = -std=c++11 $(COMPILER_WARNINGS)


ifeq ($(CONFIGURATION), debug)
COMPILER_OPTIONS += -g -O0
else ifeq ($(CONFIGURATION), release)
COMPILER_OPTIONS += -s -O2
endif

ifeq ($(ARCHITECTURE), x86)
COMPILER_OPTIONS += -m32
else ifeq ($(ARCHITECTURE), amd64)
COMPILER_OPTIONS += -m64
endif

API_BUILD_DIR   = ../build
API_HEADERS_DIR = ../headers
API_SOURCES_DIR = ../sources

API_HEADERS_FILES = $(shell find $(API_HEADERS_DIR) -type f -regex ".*\.\(hpp\)")
API_SOURCES_FILES = $(shell find $(API_SOURCES_DIR) -type f -regex ".*\.\(cpp\)")

TESTS_SOURCE = main.cpp


build: $(TARGET)

clean:
	TARGET=$$(pwd)/$(API_LIB) \
	make --directory=$(API_BUILD_DIR) --no-print-directory clean
	rm -f $(TARGET)


$(TARGET): $(TESTS_SOURCE) $(API_LIB)
	@ \
	if [ ! -e $@ ]; \
	then \
		echo $(COMPILER) $(COMPILER_OPTIONS) -I$(API_HEADERS_DIR) -c $< -o $@; \
		$(COMPILER) $(COMPILER_OPTIONS) -I$(API_HEADERS_DIR) $^ -o $@; \
	else \
		LMF=$$( \
			$(COMPILER) $(COMPILER_OPTIONS) -I$(API_HEADERS_DIR) -MM $< | \
			python last_modified_file.py \
		); \
		if [ $$(stat --printf="%Y\n" $$LMF) -gt $$(stat --printf="%Y\n" $@) ]; \
		then \
			echo $(COMPILER) $(COMPILER_OPTIONS) -I$(API_HEADERS_DIR) -c $< -o $@; \
			$(COMPILER) $(COMPILER_OPTIONS) -I$(API_HEADERS_DIR) $^ -o $@; \
		fi \
	fi

$(API_LIB): $(API_HEADERS_FILES) $(API_SOURCES_FILES)
	CONFIGURATION=$(CONFIGURATION) \
	ARCHITECTURE=$(ARCHITECTURE) \
	COMPILER=$(COMPILER) \
	TARGET=$$(pwd)/$(API_LIB) \
	make --directory=$(API_BUILD_DIR) --no-print-directory --jobs $$(nproc --all)
