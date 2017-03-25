#ifndef NETCONTAINER_H_
#define NETCONTAINER_H_
/*** net container
 * netContainer
 * Defines a container class for network
 * 
 */

#include <map>
#include <string>
#include "gate.h"

using namespace std;

class NetContainer
{
public:
	
	long netInputSize;
	
	// class contains these maps
	typedef map < string, gate* > GateNameList; // a type of map of gate pointers (nodes) by name
	
	GateNameList InputEdges; // map of input edge gates by name
	GateNameList MemoryBlocks; // map of memory blocks by name
	GateNameList OutputEdges; // map of output edges by name
	
	GateNameList::iterator nodeListIterator;
	
	// default constructor
	NetContainer()
	{
		// do nothing
	}
	
	// default destructor
	~NetContainer()
	{
		// do nothing
	}
};
#endif /*NETCONTAINER_H_*/

