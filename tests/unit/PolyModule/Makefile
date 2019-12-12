CXX = g++

INCFOLD = ../../../inc ../../../tpls

MODFOLD = ../../../src/PolyModule

LIBFOLD = ../../../tpls/blaspp/lib
LIBSO = blaspp

CXXFLAGS := $(patsubst %,-I%,$(INCFOLD))
LINKFLAGS := $(patsubst %,-L%,$(LIBFOLD))
LINKFLAGS += $(patsubst %,-l%,$(LIBSO))
LINKFLAGS += -Wl,-rpath,$(LIBFOLD)

MODSRCS = $(wildcard $(MODFOLD)/*.cpp)
MODOBJ := $(patsubst $(MODFOLD)/%.cpp,obj/%.o,$(MODSRCS))

TESTSRC = $(wildcard *.cpp)
TESTOBJ = $(patsubst %.cpp, obj/%.o, $(TESTSRC))

RESTOBJ = obj/LinAlgEntities.o obj/ElementFactory.o obj/ElementComposite.o

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
