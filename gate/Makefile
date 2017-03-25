# a makefile
#
# tdr 2007.12.12
# $Id: Makefile,v 1.3 2008/09/05 00:07:14 fatshark Exp $

LIBOBJ = gate
INCDIR	= ../common
HEADERS = ../common

CPP = g++
LD = g++
CFLAGS = -O0 -g3 -Wall -fmessage-length=0 

all: $(LIBOBJ).o

$(LIBOBJ).o: $(LIBOBJ).cpp $(HEADERS)/$(LIBOBJ).h
	$(CPP) $(CFLAGS) -I $(INCDIR) -c $(LIBOBJ).cpp

tester.o: tester.cpp $(HEADERS)/$(LIBOBJ).h
	$(CPP) $(CFLAGS) -I $(INCDIR) -c tester.cpp
	
tester: tester.o $(LIBOBJ).o
	$(LD) $(CFLAGS) -o tester tester.o $(LIBOBJ).o
	
clean:
	rm -f *.o *~ core tester tester.exe

	