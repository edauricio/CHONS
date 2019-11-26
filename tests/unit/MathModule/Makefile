CXX = g++

INCFOLD = ../../../inc ../../tpls

MODFOLD = ../../../src/MathModule

LIBFOLD = ../../../tpls/blaspp/lib

CXXFLAGS = -lblaspp 

SRCS = $(wildcard $(MODFOLD)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp,./%.o,$(SRCS))

test: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)
	rm *.o

./%.o: $(MODFOLD)/%.cpp
	$(CXX) -c -o $@ $< $(LIBFOLD) $(CXXFLAGS)
