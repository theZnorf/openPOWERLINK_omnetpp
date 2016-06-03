#! /bin/sh

export NEDPATH="../../../src/:$NEDPATH"

MAKEMAKE_OPTS="-f --deep -O out -I../../../openPOWERLINK_V2/stack/include -I../../../openPOWERLINK_V2/stack/proj/linux/liboplkmn-sim -I../../../openPOWERLINK_V2/sim/include"

if [ ! -d work ];  then mkdir work; fi
rm -fR work/*

opp_test gen -v *.test || exit 1

# obsolete build process due to no generated C++ parts or new code to be compiled
#(cd work; opp_makemake $MAKEMAKE_OPTS; make) || exit 1


opp_test run -v -a "-l ../../../openPOWERLINK" *.test 
