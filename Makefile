CXX     = g++
CFLAGS  = -std=c++11 -Wall -pedantic -I./$(IDIR)
IDIR	= include
ODIR	= obj
SDIR	= src

DEPS	= $(IDIR)/dimension.h $(IDIR)/attribute.h $(IDIR)/reader.h $(IDIR)/lib.h 

all: brute brute2 brute_chunks exactSkipDim exactSkipDim_chunks exactSkipDim2 appSplitDyn appSplitDyn2 appSplitDyn_chunks clean

$(ODIR)/%.o: $(SDIR)/%.cpp $(IDIR)/%.h $(DEPS)
	$(CXX) -c $(CFLAGS) $< -o $@

brute: $(ODIR)/brute.o $(ODIR)/dimension.o $(ODIR)/attribute.o $(ODIR)/reader.o $(ODIR)/lib.o 
	$(CXX) -o $@ $^

brute2: $(ODIR)/brute2.o $(ODIR)/dimension.o $(ODIR)/attribute.o $(ODIR)/reader.o $(ODIR)/lib.o 
	$(CXX) -o $@ $^

brute_chunks: $(ODIR)/brute_chunks.o $(ODIR)/dimension.o $(ODIR)/attribute.o $(ODIR)/reader.o $(ODIR)/lib.o 
	$(CXX) -o $@ $^

exactSkipDim: $(ODIR)/exactSkipDim.o $(ODIR)/dimension.o $(ODIR)/attribute.o $(ODIR)/reader.o $(ODIR)/lib.o 
	$(CXX) -o $@ $^

exactSkipDim_chunks: $(ODIR)/exactSkipDim_chunks.o $(ODIR)/dimension.o $(ODIR)/attribute.o $(ODIR)/reader.o $(ODIR)/lib.o 
	$(CXX) -o $@ $^

exactSkipDim2: $(ODIR)/exactSkipDim2.o $(ODIR)/dimension.o $(ODIR)/attribute.o $(ODIR)/reader.o $(ODIR)/lib.o 
	$(CXX) -o $@ $^

appSplitDyn: $(ODIR)/appSplitDyn.o $(ODIR)/dimension.o $(ODIR)/attribute.o $(ODIR)/reader.o $(ODIR)/lib.o 
	$(CXX) -o $@ $^

appSplitDyn2: $(ODIR)/appSplitDyn2.o $(ODIR)/dimension.o $(ODIR)/attribute.o $(ODIR)/reader.o $(ODIR)/lib.o 
	$(CXX) -o $@ $^

appSplitDyn_chunks: $(ODIR)/appSplitDyn_chunks.o $(ODIR)/dimension.o $(ODIR)/attribute.o $(ODIR)/reader.o $(ODIR)/lib.o 
	$(CXX) -o $@ $^

.PHONY: clean cleanest

clean:
	rm -f $(ODIR)/*.o

cleanest: clean
	rm -f brute
	rm -f brute2
	rm -f brute_chunks
	rm -f exactSkipDim
	rm -f exactSkipDim_chunks
	rm -f exactSkipDim2
	rm -f appSplitDyn_chunks
	rm -f appSplitDyn
