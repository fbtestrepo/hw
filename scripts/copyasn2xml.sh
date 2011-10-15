#!/bin/sh
mkdir ~/Desktop/build
mkdir ~/Desktop/build/connect
cp ~/ncbi/asnlib/*.[ch] ~/Desktop/build
cp ~/ncbi/connect/*.[ch] ~/Desktop/build
cp ~/ncbi/corelib/*.[ch] ~/Desktop/build
cp ~/ncbi/corelib/ncbilcl.* ~/Desktop/build
cp ~/ncbi/connect/*.h ~/Desktop/build/connect
cp ~/Misc/ssc/ssclog/*.[ch] ~/Desktop/build
cp ~/Misc/ssc/include/*.h ~/Desktop/build
cp -r ~/Misc/ssc/tables ~/Desktop/build
cp ~/Misc/ssc/scripts/makeasn2xml ~/Desktop/build/makefile
cd ~/Desktop/build
