CXX     = g++
CFLAGS  = -std=c++11 -Wall -pedantic -I./$(IDIR)
IDIR	= include
ODIR	= obj
SDIR	= src

DEPS	= $(IDIR)/dimension.h $(IDIR)/attribute.h $(IDIR)/lib.h 

all: brute brute2 exactSkipDim exactSkipDim2 appSplitDyn appSplitDyn2 clean

brute: $(ODIR)/brute.o $(ODIR)/dimension.o $(ODIR)/attribute.o $(ODIR)/lib.o
	$(CXX) -o $@ $^

$(ODIR)/%.o: $(SDIR)/%.cpp $(IDIR)/%.h $(DEPS)
	$(CXX) -c $(CFLAGS) $< -o $@

$(ODIR)/brute.o: $(DEPS)

brute2: $(ODIR)/brute2.o $(ODIR)/dimension.o $(ODIR)/attribute.o $(ODIR)/lib.o
	$(CXX) -o $@ $^

exactSkipDim: $(ODIR)/exactSkipDim.o $(ODIR)/dimension.o $(ODIR)/attribute.o $(ODIR)/lib.o
	$(CXX) -o $@ $^

exactSkipDim2: $(ODIR)/exactSkipDim2.o $(ODIR)/dimension.o $(ODIR)/attribute.o $(ODIR)/lib.o
	$(CXX) -o $@ $^

appSplitDyn: $(ODIR)/appSplitDyn.o $(ODIR)/dimension.o $(ODIR)/attribute.o $(ODIR)/lib.o
	$(CXX) -o $@ $^

appSplitDyn2: $(ODIR)/appSplitDyn2.o $(ODIR)/dimension.o $(ODIR)/attribute.o $(ODIR)/lib.o
	$(CXX) -o $@ $^

.PHONY: clean cleanest

clean:
	rm -f $(ODIR)/*.o

cleanest: clean
	rm -f brute
	rm -f brute2
	rm -f exactSkipDim
	rm -f exactSkipDim2
	rm -f appSplitDyn
