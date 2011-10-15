#!/bin/sh
mkdir ~/Desktop/build
mkdir ~/Desktop/build/connect
cp ~/ncbi/asnlib/*.[ch] ~/Desktop/build
cp ~/ncbi/connect/*.[ch] ~/Desktop/build
cp ~/ncbi/corelib/*.[ch] ~/Desktop/build
cp ~/ncbi/corelib/ncbilcl.* ~/Desktop/build
cp ~/ncbi/vibrant/*.[ch] ~/Desktop/build
cp ~/ncbi/connect/*.h ~/Desktop/build/connect
cp ~/ssc/ssclog/*.[ch] ~/Desktop/build
cp ~/ssc/include/*.h ~/Desktop/build
cp -r ~/ssc/tables ~/Desktop/build
cp ~/ssc/scripts/ssclogo.ico ~/Desktop/build
cp ~/ssc/scripts/ssclogo.rc ~/Desktop/build
cp ~/ssc/scripts/makemingwssc ~/Desktop/build/makefile
cd ~/Desktop/build
/usr/local/i386-mingw32-4.3.0/bin/i386-mingw32-windres -i ssclogo.rc -o ssclogo.o
