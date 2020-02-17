# set non-optional compiler flags here
#CXXFLAGS += -std=c++11 -Wall 

# set non-optional preprocessor flags here
# eg. project specific include directories
CPPFLAGS += -I ./include

# find cpp files in subdirectories
SOURCES := $(shell find . -name '*.cpp' -not -path "./main/*")

# find headers
HEADERS := $(shell find . -name '*.h')

OUTPUT1 := build/LSH

OUTPUT2 := build/HC

# Everything depends on the output
all:	$(OUTPUT1) $(OUTPUT2)

# The output depends on sources and headers
$(OUTPUT1): $(SOURCES) main/mainLSH.cpp $(HEADERS)
	$(CXX) $(CPPFLAGS) -o $(OUTPUT1) $(SOURCES) main/mainLSH.cpp

$(OUTPUT2): $(SOURCES) main/mainHC.cpp $(HEADERS)
	$(CXX) $(CPPFLAGS) -o $(OUTPUT2) $(SOURCES) main/mainHC.cpp

.PHONY: clean

clean:
	rm -f $(OUTPUT1) $(OUTPUT2)