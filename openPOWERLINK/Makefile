#
# OMNeT++/OMNEST Makefile for openPOWERLINK
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -I/home/franz/Documents/MasterThesis/src/simulations/openPOWERLINK_omnetpp/openPOWERLINK_V2/sim -I/home/franz/Documents/MasterThesis/src/simulations/openPOWERLINK_omnetpp/openPOWERLINK_V2/stack/include -I/home/franz/Documents/MasterThesis/src/simulations/openPOWERLINK_omnetpp/openPOWERLINK_V2/stack/proj/linux/liboplkmn-sim
#

# Name of target to be created (-o option)
TARGET = openPOWERLINK$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I/home/franz/Documents/MasterThesis/src/simulations/openPOWERLINK_omnetpp/openPOWERLINK_V2/sim \
    -I/home/franz/Documents/MasterThesis/src/simulations/openPOWERLINK_omnetpp/openPOWERLINK_V2/stack/include \
    -I/home/franz/Documents/MasterThesis/src/simulations/openPOWERLINK_omnetpp/openPOWERLINK_V2/stack/proj/linux/liboplkmn-sim \
    -I. \
    -Isimulations \
    -Isrc \
    -Isrc/interface

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = $O/src/Timer.o $O/src/HresTimer.o $O/src/interface/SharedLibraryHelper.o $O/src/TimerMsg_m.o

# Message files
MSGFILES = \
    src/TimerMsg.msg

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f openPOWERLINK openPOWERLINK.exe libopenPOWERLINK.so libopenPOWERLINK.a libopenPOWERLINK.dll libopenPOWERLINK.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f simulations/*_m.cc simulations/*_m.h
	$(Q)-rm -f src/*_m.cc src/*_m.h
	$(Q)-rm -f src/interface/*_m.cc src/interface/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc simulations/*.cc src/*.cc src/interface/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/src/HresTimer.o: src/HresTimer.cc \
	src/HresTimer.h \
	src/MsgPtr.h \
	src/TimerBase.h \
	src/TimerMsg_m.h
$O/src/Timer.o: src/Timer.cc \
	src/MsgPtr.h \
	src/Timer.h \
	src/TimerBase.h \
	src/TimerMsg_m.h
$O/src/TimerMsg_m.o: src/TimerMsg_m.cc \
	src/TimerMsg_m.h
$O/src/interface/SharedLibraryHelper.o: src/interface/SharedLibraryHelper.cc \
	src/interface/SharedLibraryHelper.h

