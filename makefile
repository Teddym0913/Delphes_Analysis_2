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

#INCDIRS	+= $(EXTRAHDR)
INCLUDES      = $(ROOTCFLAGS) $(addprefix -I, $(INCDIRS) ) 
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
LIBS         += $(ROOTLIBS) $(SYSLIBS) $(DELPHESLIBS)
GLIBS        += $(ROOTGLIBS) $(SYSLIBS)

#------------------------------------------------------------------------------
SRC = $(wildcard $(SrcDir)/*.$(SrcSuf))
OBJ           = $(patsubst $(SrcDir)/%.$(SrcSuf),$(ObjDir)/%.$(ObjSuf),$(SRC))
DEP           = $(patsubst $(SrcDir)/%.$(SrcSuf),$(DepDir)/%.$(DepSuf),$(SRC))
OBJS          = $(OBJ)

#------------------------------------------------------------------------------
$(Target):	$(OBJS) $(Mainfile)
		@$(LD) $(LDFLAGS) $(CXXFLAGS) $(Mainfile) -o $@ -L./ $(OBJS) $(GLIBS) $(LIBS)
		@echo "--->$@ done"

$(ObjDir)/%.$(ObjSuf):$(SrcDir)/%.$(SrcSuf)
	@$(CXX) $(CXXFLAGS) -o $@  $(GLIBS) $(LIBS) -c $<

#-------------------------------------------------------------------------------
clean:
	@rm -rf $(OBJS) $(Target)
	@echo "--->Clean Done"
