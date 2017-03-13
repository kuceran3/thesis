CXX     = g++
CFLAGS  = -std=c++11 -Wall -pedantic -I./$(IDIR)
IDIR	= include
ODIR	= obj
SDIR	= src

DEPS	= $(IDIR)/dimension.h $(IDIR)/attribute.h $(IDIR)/lib.h 

all: brute clean

brute: $(ODIR)/brute.o $(ODIR)/dimension.o $(ODIR)/attribute.o $(ODIR)/lib.o
	$(CXX) -o $@ $^

$(ODIR)/%.o: $(SDIR)/%.cpp $(IDIR)/%.h
	$(CXX) -c $(CFLAGS) $< -o $@

$(ODIR)/brute.o: $(DEPS)

.PHONY: clean cleanest

clean:
	rm -f $(ODIR)/*.o

cleanest: clean
	rm -f brute
