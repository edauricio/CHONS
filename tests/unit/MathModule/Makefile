CXX = g++

INCFOLD = ../../../inc ../../../tpls

MODFOLD = ../../../src/MathModule

LIBFOLD = ../../../tpls/blaspp/lib
LIBSO = blaspp

CXXFLAGS := $(patsubst %,-I%,$(INCFOLD))
LINKFLAGS := $(patsubst %,-L%,$(LIBFOLD))
LINKFLAGS += $(patsubst %,-l%,$(LIBSO))
LINKFLAGS += -Wl,-rpath,$(LIBFOLD)

MODSRCS = $(wildcard $(MODFOLD)/*.cpp)
MODOBJ = $(patsubst $(MODFOLD)/%.cpp,obj/%.o,$(MODSRCS))

RESTOBJ = obj/ElementComposite.o obj/ElementFactory.o obj/Polynomials.o \
	  obj/BasisFactory.o obj/Basis.o obj/MeshReader.o obj/MeshInfoHolder.o \
	  obj/GraphInfoProcessor.o

TESTSRC = $(wildcard *.cpp)
TESTOBJ = $(patsubst %.cpp, obj/%.o, $(TESTSRC))

MAINDIR = ../
MAINTESTOBJ = obj/units-def-inc.o

test: $(MODOBJ) $(TESTOBJ) $(MAINTESTOBJ)
	$(CXX) -o $@ $^ $(RESTOBJ) $(CXXFLAGS) $(LINKFLAGS) -O0 -g

obj/%.o: $(MODFOLD)/%.cpp | mkfold
	$(CXX) -c -o $@ $< $(CXXFLAGS) -O0 -g

obj/%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) -O0 -g

obj/%.o: ../%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) -O0 -g

mkfold:
	@mkdir -p obj

clean:
	rm -rf obj/
	rm -rf *.o
	rm -rf test
