# a makefile
#
# tdr 2007.12.12
# $Id: Makefile,v 1.6 2008/10/01 03:15:10 fatshark Exp $

INCDIR	= ../common
HEADERS = ../common

CPP = g++
LD = g++
CFLAGS = -O0 -g3 -Wall -fmessage-length=0 

all: netDefine.o netSaveRestore.o netOperate.o netTrain.o

netDefine.o: netDefine.cpp $(HEADERS)/netDefine.h $(HEADERS)/netContainer.h $(HEADERS)/gate.h
	$(CPP) $(CFLAGS) -I $(INCDIR) -c netDefine.cpp

netSaveRestore.o: netSaveRestore.cpp $(HEADERS)/netSaveRestore.h $(HEADERS)/netContainer.h $(HEADERS)/gate.h
	$(CPP) $(CFLAGS) -I $(INCDIR) -c netSaveRestore.cpp

netOperate.o: netOperate.cpp $(HEADERS)/netOperate.h $(HEADERS)/netContainer.h $(HEADERS)/gate.h
	$(CPP) $(CFLAGS) -I $(INCDIR) -c netOperate.cpp
	
netTrain.o: netTrain.cpp $(HEADERS)/netTrain.h $(HEADERS)/netContainer.h $(HEADERS)/gate.h $(HEADERS)/netMetric.h
	$(CPP) $(CFLAGS) -I $(INCDIR) -c netTrain.cpp

netMetric.o: netMetric.cpp $(HEADERS)/netMetric.h
	$(CPP) $(CFLAGS) -I $(INCDIR) -c netMetric.cpp

tester.o: tester.cpp $(HEADERS)/netDefine.h $(HEADERS)/netSaveRestore.h $(HEADERS)/netOperate.h $(HEADERS)/netTrain.h
	$(CPP) $(CFLAGS) -I $(INCDIR) -c tester.cpp
	
tester: tester.o netDefine.o netSaveRestore.o netOperate.o netTrain.o netMetric.o ../gate/gate.o
	$(LD) $(CFLAGS) -o tester tester.o netDefine.o netSaveRestore.o netOperate.o netTrain.o netMetric.o ../gate/gate.o
	
clean:
	rm -f *.o *~ core tester tester.exe

	