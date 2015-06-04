Target:=Delphes_Analysis
Mainfile:=Delphes_Analysis.cpp
#------------------------------------------------------------------------------
SrcSuf := cpp
IncSuf := h
ObjSuf := o
DepSuf := d
DllSuf := so

SrcDir := src
IncDir :=include
ObjDir := obj
DepDir := $(ObjDir)

INCDIRS = $(shell pwd)/include $(shell pwd)
EXTRAHDR = 
#------------------------------------------------------------------------------
# Architecture to compile for
ARCH          = linux
#ARCH          = solarisCC5

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

DELPHESCFLAGS := -I/home/teddy/workingspace/MC-Package/MG5_aMC_v2_2_1/Delphes -I/home/teddy/workingspace/MC-Package/MG5_aMC_v2_2_1/Delphes/external
DELPHESLIBS  := -L/home/teddy/workingspace/MC-Package/MG5_aMC_v2_2_1/Delphes -lDelphes
DELPHESSPECIAL := -Xlinker -rpath /home/teddy/workingspace/MC-Package/MG5_aMC_v2_2_1/Delphes/

#MT2CFLAGS := -I/home/teddy/workingspace/Useful-Package/MT2_Variable/include/oxbridgekinetics-1.0
#MT2LIBS := -L/home/teddy/workingspace/Useful-Package/MT2_Variable/lib/ -loxbridgekinetics-1.0
#MT2GLIBS := -L/home/teddy/workingspace/Useful-Package/MT2_Variable/lib/ -loxbridgekinetics-1.0
#MT2SPECIAL := -Xlinker -rpath /home/teddy/workingspace/Useful-Package/MT2_Variable/lib/


#INCDIRS	+= $(EXTRAHDR)
INCLUDES      = $(ROOTCFLAGS) $(MT2CFLAGS) $(addprefix -I, $(INCDIRS) ) 
CXX           = g++
LD            = g++
SOFLAGS       = -shared

ifdef DEBUG
  CXXFLAGS    = -g -O0 
  LDFLAGS     = -g -O0
  DEFINES     =
else
  CXXFLAGS    = -O3
  LDFLAGS     = -O3
  DEFINES     = 
#  DEFINES     = -DNDEBUG
endif

DEFINES      += -DLINUXVERS -DHAS_SSTREAM
ifdef VERBOSE
DEFINES      += -DVERBOSE
endif
ifdef TESTCODE
DEFINES      += -DTESTCODE
endif

CXXFLAGS     += $(DEFINES) $(INCLUDES) $(DELPHESCFLAGS)
LIBS         += $(ROOTLIBS) $(SYSLIBS) $(DELPHESLIBS) #$(MT2LIBS)
GLIBS        += $(ROOTGLIBS) $(SYSLIBS) #$(MT2GLIBS)

#------------------------------------------------------------------------------
SRC = $(wildcard $(SrcDir)/*.$(SrcSuf))
OBJ           = $(patsubst $(SrcDir)/%.$(SrcSuf),$(ObjDir)/%.$(ObjSuf),$(SRC))
DEP           = $(patsubst $(SrcDir)/%.$(SrcSuf),$(DepDir)/%.$(DepSuf),$(SRC))
OBJS          = $(OBJ) $(ObjDir)/EachEventDef.$(ObjSuf) $(ObjDir)/TreeReaderDef.$(ObjSuf)

#------------------------------------------------------------------------------
$(Target):	$(OBJS) $(Mainfile)
		@$(LD) $(LDFLAGS) $(CXXFLAGS) $(Mainfile) -o $@ -L./ $(OBJS) $(GLIBS) $(LIBS) $(DELPHESSPECIAL) #$(MT2SPECIAL)
		@echo "--->$@ done"

$(ObjDir)/%.$(ObjSuf):$(SrcDir)/%.$(SrcSuf)
	@$(CXX) $(CXXFLAGS) -o $@  $(GLIBS) $(LIBS) -c $<


$(SrcDir)/EachEventDef.$(SrcSuf):$(IncDir)/EachEvent.h $(IncDir)/EachEventLinkDef.h
	@echo "Generating dictionary $@..."
	@rootcint -f $@ -c $(INCLUDES) $(DEFINES) $(DELPHESCFLAGS) $^

$(SrcDir)/TreeReaderDef.$(SrcSuf):$(IncDir)/TreeReader.h $(IncDir)/TreeReaderLinkDef.h
	@echo "Generating dictionary $@..."
	@rootcint -f $@ -c $(INCLUDES) $(DEFINES) $(DELPHESCFLAGS) $^
#-------------------------------------------------------------------------------
clean:
	@rm -rf $(OBJS) $(Target) $(SrcDir)/EachEventDef.$(SrcSuf) $(SrcDir)/EachEventDef.$(IncSuf) $(SrcDir)/TreeReaderDef.$(SrcSuf) $(SrcDir)/TreeReaderDef.$(IncSuf)
	@echo "--->Clean Done"
