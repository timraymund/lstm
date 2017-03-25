#ifndef NETDEFINE_
#define NETDEFINE_
/***************************
* defines a network
* including input edges, memory blocks and output edges
*
*/

#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <map>
#include <deque>
#include <iterator>
#include <string>
#include <cmath>

#include "gate.h"
#include "netContainer.h"

using namespace std;

/// make a network using the configuration file
/** make a network
 * parse the configuration file
 * create the nodes
 * create weighted connections
 */
class NetDefine
{
protected:
	string makeNetName; // class name
	ifstream configurationFile; // 
	
	NetContainer *netContainer;  // a network container pointer
	
	enum NodeName { INPUTEDGE, MEMORYBLOCK, OUTPUTEDGE, CONNECTION, INPUT, INPUTGATE, FORGETGATE, OUTPUTGATE };
	/**
	 * Three node names define the possible node types in
	 * a network.
	 * Input Edge: accepts input data and clocks it into the net
	 * Memory Block: a memory block as per Schmidhuber
	 * Output Edge: collates and writes out information
	 * 
	 * One node name, CONNECTION, is a state shift, not a node
	 * 
	 * The final four node names define the (input) gates on a memory block:
	 * input: accepts input data
	 * input gate: gates input to cell
	 * forget gate: clears cell
	 * output gate: gates output from cell
	 * 
	 */
	typedef map < string, NodeName > StringNodeMap;  // a type of map of enumerated node types by name
	
    NodeName nodeName;
    StringNodeMap networkNodeMap;  // map of names by enumerated node types
    StringNodeMap typeNodeMap; // map of string types by enumerated node type
    
    // need to add a map like StringNodeMap to serve as a node counter
    
    typedef map < string, int > CountNodeMap; // a type of map of node counts by name
    CountNodeMap countNodeMap; // a map of the count node map type
    CountNodeMap::iterator countNodeIterator;  // an iterator to the map of node counts by name
    
    enum MakeState { NODES, CONNECTIONS };
    MakeState makeState; // the maker state
    
    char *aNameStr; // need this to instantiate gates with name
    
    long netInputSize;	// the minimum number of input data records available to IE

public:

	// default constructor
	NetDefine()
	{
		// do nothing
		typeNodeMap["IE"] = INPUTEDGE;
		typeNodeMap["MB"] = MEMORYBLOCK;
		typeNodeMap["OE"] = OUTPUTEDGE;
		typeNodeMap["CONNECTIONS"] = CONNECTION; // this is not really a type
		typeNodeMap["IP"] = INPUT;
		typeNodeMap["IG"] = INPUTGATE;
		typeNodeMap["FG"] = FORGETGATE;
		typeNodeMap["OG"] = OUTPUTGATE;
	}
	
	// constructor with a name
	NetDefine(char* aName)
	{
		makeNetName += aName; // set class file name
		configurationFile.open(aName); // open the configuration file
		// other initialization
		typeNodeMap["IE"] = INPUTEDGE;
		typeNodeMap["MB"] = MEMORYBLOCK;
		typeNodeMap["OE"] = OUTPUTEDGE;
		typeNodeMap["CONNECTIONS"] = CONNECTION; // this is not really a type
		typeNodeMap["IP"] = INPUT;
		typeNodeMap["IG"] = INPUTGATE;
		typeNodeMap["FG"] = FORGETGATE;
		typeNodeMap["OG"] = OUTPUTGATE;
	}
	
	// destructor
	virtual ~NetDefine()
	{
		configurationFile.close(); // close the configuration file
	}
	
	// set the network container pointer
	void setNetContainer(NetContainer *aNetContainer);
	
	// parse a configuration file (this makes the net)
	void parseConfigurationFile();
	
	// check for network definition deficiences
	void checkNet();
	
	// list all the nodes and connections
	void listNet();
	
	// exposes the number of input records
	long size();

};

#endif /*NETDEFINE_*/
