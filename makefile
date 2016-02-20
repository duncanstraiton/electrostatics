CC := g++
SRCDIR := src
BUILDDIR := build
BINDIR := bin
TARGET := $(BINDIR)/electrostatics 
# Need to separate file with main so that it does not conflict with main from google test
MAINENTRYFILE = electrostatics.cpp
MAINENTRYOBJECT = build/electrostatics.o
TESTSRCDIR := test/src
TESTBUILDDIR := test/build
TESTBINDIR := test/bin
TESTTARGET := $(TESTBINDIR)/testAll
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f \( -iname *.$(SRCEXT) ! -iname $(MAINENTRYFILE) \))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
TESTSOURCES := $(shell find $(TESTSRCDIR) -type f -name *.$(SRCEXT))
TESTOBJECTS := $(patsubst $(TESTSRCDIR)/%,$(TESTBUILDDIR)/%,$(TESTSOURCES:.$(SRCEXT)=.o))
# NDEBUG flag avoids bounds checking for eigen vectors, uncomment once code is definitely stable
CFLAGS := -std=c++11 -g3 -Wall -fopenmp -O3 # -DNDEBUG
LIB := -fopenmp # -lOpenCL -L/usr/lib/x86_64-linux-gnu/libOpenCL.so
TESTLIB := -fopenmp -lgtest -lgtest_main -pthread
INC := -I include  -I /usr/include/eigen3 -I /usr/include/gtest -I $(HOME)/include # -I /usr/include/CL


###############################################################################
# Seperate targets for different vienna backends - default is openMP
###############################################################################
.PHONY: all default

# Make the main program with eigens sparseLU module by default.
all: $(TARGET)

# Compile with support for openMP multi-threading
openMP: CFLAGS += -DVIENNACL_WITH_OPENMP
openMP: $(TARGET)



###############################################################################
# Targets for main program.
###############################################################################
$(TARGET): $(OBJECTS) $(MAINENTRYOBJECT) | $(BINDIR)
	@echo " Linking..."
	@echo " $(CC) $^ $(MAINENTRY) -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(BINDIR):
	@mkdir -p $(BINDIR)



###############################################################################
# Targets for google test.
###############################################################################
testAll: $(TESTTARGET)
$(TESTTARGET):  $(OBJECTS) $(TESTOBJECTS) | $(TESTBINDIR)
	@echo " Linking Tests..."
	@echo " $(CC) $^ -o $(TESTTARGET) $(TESTLIB)"; $(CC) $^ -o $(TESTTARGET) $(TESTLIB)

$(TESTBUILDDIR)/%.o: $(TESTSRCDIR)/%.$(SRCEXT)
	@mkdir -p $(TESTBUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(TESTBINDIR):
	@mkdir -p $(TESTBINDIR)



###############################################################################
# Clean up build files - will also remove any generated data or plots
# (ie everything) from bin directory.
###############################################################################
clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(BINDIR) $(TESTBUILDDIR) $(TESTBINDIR)";
	$(RM) -r $(BUILDDIR) $(BINDIR) $(TESTBUILDDIR) $(TESTBINDIR)

cleanTest:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(TESTBUILDDIR) $(TESTBINDIR)";
	$(RM) -r $(TESTBUILDDIR) $(TESTBINDIR)

.PHONY: clean
.PHONY: cleanTest
