# a top level makefile
#
# tdr 2007.12.12
# $Id: Makefile,v 1.5 2008/10/01 03:15:10 fatshark Exp $

CPP = g++
LD = g++
CFLAGS = -O0 -g3 -Wall -fmessage-length=0 

all: src/tester

gate/gate.o: gate/gate.cpp common/gate.h
	cd gate; make all
	
src/netDefine.o: src/netDefine.cpp common/netDefine.h common/gate.h common/netContainer.h
	cd src; make all
	
src/netSaveRestore.o: src/netSaveRestore.cpp common/netSaveRestore.h common/gate.h common/netContainer.h
	cd src; make all
	
src/netOperate.o: src/netOperate.cpp common/netOperate.h common/gate.h common/netContainer.h
	cd src; make all

src/netTrain.o: src/netTrain.cpp src/netMetric.cpp common/netTrain.h common/gate.h common/netContainer.h common/netMetric.h
	cd src; make all

src/netMetric.o: src/netMetric.cpp common/netMetric.h
	cd src; make all
	
src/tester: src/tester.cpp gate/gate.o src/netDefine.o src/netSaveRestore.o src/netOperate.o src/netTrain.o src/netMetric.o
	cd src; make tester

clean:
	rm -f *.o *~ core tester tester.exe
	cd gate; make clean
	cd src; make clean

force_look : 
	true
	