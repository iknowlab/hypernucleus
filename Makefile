# Makefie

.SUFFIXES: .O .C .S
.DEFAULT:

TARGET = LogLikelihood MCS_massrange Max_L_Est PDG_gen_ssd c1111 g4debug gen_g4ssd imputR likelihooood mc_loglikelihood mc_likelihood meas_q pid_config ref2ssd sample sample_g4 test_ssd thing chi2L likeli_convert
TARGETROOT = LLGraph LRgraph ssdhist ssdhist_mario trdraw trdraw-b trdraw-e
TARGETCOPY = G4referenceout.awk

INSTALL = install -c -m 775
INSTALL_DIR = -install -d -m 775

PREFIX?= ../
BINDIR?= $(PREFIX)/bin
ROOTBINDIR?= $(PREFIX)/rootfile

CXXFLAGS +=-std=c++0x -fopenmp 
ROOTFLAGS = `root-config --cflags`
ROOTLIBS = `root-config --libs`
CXX :=/opt/rh/devtoolset-2/root/usr/bin/g++
#CXX :=/opt/rh/devtoolset-3/root/usr/bin/g++

all:	bin root
bin:	$(TARGET)
root: 	$(TARGETROOT)

%: %.C
%.o:	%.C

%: %.O
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -o $@ $^ $(ROOTLIBS)

.cc.o:
	$(CXX) -c $(CXXFLAGS) $<

.cc.s:
	$(CXX) -S $(CXXFLAGS) $<

.o:
	$(CXX) $(CXXFLAGS) $< -o $@ $^

.C.O:
	$(CXX) -c $(CXXFLAGS) $(ROOTFLAGS) -o $@ $<

-include include.dep

install: $(TARGET) $(TARGETROOT) $(TARGETCOPY)
	$(INSTALL_DIR) $(BINDIR)/
	$(INSTALL_DIR) $(ROOTBINDIR)/
	$(INSTALL) $(TARGET) $(TARGETCOPY) $(BINDIR)/
	$(INSTALL) $(TARGETROOT) $(ROOTBINDIR)/

clean:
	$(RM) *.o *.O core.*

distclean:
	$(RM) *.o *.O $(TARGET) $(TARGETROOT) core.*
