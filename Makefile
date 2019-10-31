CXX=g++
IDIR=inc
ODIR=obj
SRCDIR=src/MeshModule
TESTDIR=tests/hard-coded
TESTSRC=integration


CXXFLAGS= -g -lprofiler
CPPFLAGS= -I$(IDIR)

_HEADERS = MeshModule/ElementComposite.h MeshModule/ElementFactory.h MeshModule/ElementNumInfo.h \
	  MeshModule/MeshReader.h MeshModule/GmshElements.h

HEADERS = $(patsubst %,$(IDIR)/%,$(_HEADERS))

SRCS = $(wildcard $(SRCDIR)/*.cpp)

OBJ = $(patsubst $(SRCDIR)/%.cpp,$(ODIR)/%.o,$(SRCS))

TESTOBJ = $(ODIR)/$(TESTSRC).o


CHONS: $(OBJ) $(TESTOBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)


$(ODIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(CPPFLAGS)

$(TESTOBJ): $(TESTDIR)/$(TESTSRC).cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(CPPFLAGS)



$(OBJ): | $(ODIR)

$(ODIR):
	@mkdir -p $(ODIR)

.PHONY: clean

clean:
	rm -rf $(ODIR)
