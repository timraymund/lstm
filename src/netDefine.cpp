/***************************
* defines a network
* including input edges, memory blocks and output edges
*
* during node creation checks for:
* valid syntax (line begins with node type or "CONNECTIONS", followed by node name)
* valid node type (IE, MB, or OE)
* unique node names
* 
* during node connections, checks for:
* valid syntax (line begins with a node type)
* valid node name for to and from nodes
* valid syntax (if the to node is a memory block, that the block gate has a valid type)
* valid weight
* 
* has nothing to do with a large moose or a small one either
*/

#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <deque>
#include <iterator>
#include <string>
#include <cmath>

#include <cerrno>

#include "gate.h"
#include "netContainer.h"
#include "netDefine.h"

using namespace std;

void NetDefine::setNetContainer(NetContainer *aNetContainer)
{
	netContainer = aNetContainer;
}

void NetDefine::parseConfigurationFile()
{
	// initialize state to expecting nodes ("IE", "MB" or "OE") or "CONNECTIONS"
	makeState = NODES;
	
	cout << "In parseConfigurations." << endl;
	
	// read from the configuration file
	
	// first thing must be a node
	string nodeString; // temporary string to id the node
	configurationFile >> nodeString;
	
	while (configurationFile.good())
	{

		cout << "nodeString = " << nodeString << endl;
		
		// if the state is NODES, then
		if (makeState == NODES)
		{
			// get the first two letters
			string nameString; // temporary string to name the node
			// gate *gatePointer; // temporary gate pointer

			
			// figure out if its a node or a new state
			// <member type of IE, MB or OE> <member name>
			// CONNECTIONS
			switch(typeNodeMap[nodeString])
			{
			case INPUTEDGE:
				// add an input edge
				// get the name
				configurationFile >> nameString;
				cout << "nameString = " << nameString << endl;
				
				// if the name already exists, crash
				if (networkNodeMap.count(nameString) > 0)
				{
					cerr << "Error while parsing name: " << nameString << endl;
					cerr << "A node with name " << nameString << " already exists." << endl;
					exit (1);
				}
				
				// add an input edge
				aNameStr = new char [nameString.size()+1];
				strcpy(aNameStr, nameString.c_str());
				netContainer->InputEdges[nameString] = new inEdge(aNameStr);
				delete[] aNameStr;
				
				// add this to the maps
				networkNodeMap[nameString] = INPUTEDGE;
				countNodeMap[nameString] = 0;
				break;
				
			case MEMORYBLOCK:
				// add a memory block
				// get the name
				configurationFile >> nameString;
				cout << "nameString = " << nameString << endl;
				
				// if the name already exists, crash
				if (networkNodeMap.count(nameString) > 0)
				{
					cerr << "Error while parsing name: " << nameString << endl;
					cerr << "A node with name " << nameString << " already exists." << endl;
					exit (1);
				}
				
				// add a memory block
				aNameStr = new char [nameString.size()+1];
				strcpy(aNameStr, nameString.c_str());
				netContainer->MemoryBlocks[nameString] = new memoryBlock(aNameStr);
				delete[] aNameStr;
				
				// add this to the maps
				networkNodeMap[nameString] = MEMORYBLOCK;
				countNodeMap[nameString] = 0;
				break;
				
			case OUTPUTEDGE:
				// add an output edge
				// get the name
				configurationFile >> nameString;
				cout << "nameString = " << nameString << endl;
				
				// if the name already exists, crash
				if (networkNodeMap.count(nameString) > 0)
				{
					cerr << "Error while parsing name: " << nameString << endl;
					cerr << "A node with name " << nameString << " already exists." << endl;
					exit (1);
				}
				
				// add an output edge
				aNameStr = new char [nameString.size()+1];
				strcpy(aNameStr, nameString.c_str());
				netContainer->OutputEdges[nameString] = new outEdge(aNameStr);
				delete[] aNameStr;
				
				// add this to the maps
				networkNodeMap[nameString] = OUTPUTEDGE;
				countNodeMap[nameString] = 0;
				break;
				
			case CONNECTION:
				// shift modes to CONNECTIONS
				makeState = CONNECTIONS;
				break;
				
			default:
				// should not be
				cerr << "Error while parsing node type: " << nodeString << endl;
				cerr << "In node creation mode.  Did not find expected node type or switch to connections." << endl;
				exit (1);
			}
		} else // makeState == CONNECTIONS
		{
			// make connections between nodes
			// get the first two letters
			string toNameString; // temporary string to name the to node
			gate *toGatePointer; // to gate pointer
			memoryBlock *toBlockPointer; // to memory block pointer
			string memoryBlockGate; // temporary string for memory block gates
			string fromNameString; // temporary string to name the from node
			gate *fromGatePointer; // from gate pointer
			NodeName aNode;
			string fromWeight; // temporary string for weight
			double fromDoubleWeight; // the weight as a double
			
			// make connections until end of file
			// <member type of IE, MB or OE> <member name> [gate if memory block of IP, IG, FG, or OG] <from name> <initial weight>

			switch(typeNodeMap[nodeString])
			{
			case INPUTEDGE:
				// connect to an input edge
				// get the to member name
				configurationFile >> toNameString;
				cout << "toNameString = " << toNameString << endl;
				
				// if the to name does not exist, crash
				if (networkNodeMap.count(toNameString) == 0)
				{
					cerr << "Error while parsing to name: " << toNameString << endl;
					cerr << "A node with (to) name " << toNameString << " does not exist." << endl;
					exit (1);
				}
				
				// get the pointer to the to gate
				toGatePointer = netContainer->InputEdges[toNameString];
				
				// now get the from name
				configurationFile >> fromNameString;
				cout << "fromNameString = " << fromNameString << endl;

				// if the from name does not exist, crash
				if (networkNodeMap.count(fromNameString) == 0)
				{
					cerr << "Error while parsing the from name: " << fromNameString << endl;
					cerr << "A node with (from) name " << fromNameString << " does not exist." << endl;
					exit (1);
				}
				
				// figure out which type this is from the networkNodeMap
				aNode = networkNodeMap[fromNameString];
				
				// get a fromGatePointer from the appropriate gate map
				if (aNode == INPUTEDGE)
				{
					fromGatePointer = netContainer->InputEdges[fromNameString];
				} else if (aNode == MEMORYBLOCK)
				{
					fromGatePointer = netContainer->MemoryBlocks[fromNameString];
				} else if (aNode == OUTPUTEDGE)
				{
					fromGatePointer = netContainer->OutputEdges[fromNameString];
				} else
				{
					cerr << "Error while parsing from name: " << fromNameString << endl;
					cerr << "IE from connection failed with invalid type." << endl;
					exit (1);
				}
				
				// get the weight and connect
				configurationFile >> fromWeight;
				cout << "fromWeight = " << fromWeight << endl;
				fromDoubleWeight = strtod(fromWeight.c_str(),NULL);
				
				// if the number did not parse, crash
				if (errno != 0)
				{
					cerr << "Error while parsing connection weight: " << fromWeight << endl;
					cerr << "The weight did not convert to a double." << endl;
					exit (1);
				}
				toGatePointer->connectGate(fromGatePointer,fromDoubleWeight);
				
				// count the connection
				countNodeMap[fromNameString] += 1;
				break;
				
			case MEMORYBLOCK:
				// connect to an memory block
				// get the to member name
				configurationFile >> toNameString;
				cout << "toNameString = " << toNameString << endl;
				
				// if the to name does not exist, crash
				if (networkNodeMap.count(toNameString) == 0)
				{
					cerr << "Error while parsing to name: " << toNameString << endl;
					cerr << "A node with (to) name " << toNameString << " does not exist." << endl;
					exit (1);
				}
				
				// get the pointer to the to gate
				toBlockPointer = dynamic_cast<memoryBlock *>(netContainer->MemoryBlocks[toNameString]);
				
				// now get the gate name (since this is a MB)
				configurationFile >> memoryBlockGate;
				cout << "memoryBlockGate = " << memoryBlockGate << endl;
				
				// now get the from name
				configurationFile >> fromNameString;
				cout << "fromNameString = " << fromNameString << endl;
				
				// if the from name does not exist, crash
				if (networkNodeMap.count(fromNameString) == 0)
				{
					cerr << "Error while parsing from name: " << fromNameString << endl;
					cerr << "A node with (from) name " << fromNameString << " does not exist." << endl;
					exit (1);
				}
				
				// figure out which type this is from the networkNodeMap
				aNode = networkNodeMap[fromNameString];
				
				// get a fromGatePointer from the appropriate gate map
				if (aNode == INPUTEDGE)
				{
					fromGatePointer = netContainer->InputEdges[fromNameString];
				} else if (aNode == MEMORYBLOCK)
				{
					fromGatePointer = netContainer->MemoryBlocks[fromNameString];
				} else if (aNode == OUTPUTEDGE)
				{
					fromGatePointer = netContainer->OutputEdges[fromNameString];
				} else
				{
					cerr << "Error while finding node type of from name: " << fromNameString << endl;
					cerr << "MB from connection failed." << endl;
					exit (1);
				}
				
				// get the weight
				configurationFile >> fromWeight;
				cout << "fromWeight = " << fromWeight << endl;
				fromDoubleWeight = strtod(fromWeight.c_str(),NULL);
				
				// if the number did not parse, crash
				if (errno != 0)
				{
					cerr << "Error while parsing connection weight: " << fromWeight << endl;
					cerr << "The weight did not convert to a double." << endl;
					exit (1);
				}
				
				// connect to the gate based on the memory block gate
				switch(typeNodeMap[memoryBlockGate])
				{
				case INPUT:
					toBlockPointer->connectGate(fromGatePointer,fromDoubleWeight);
					break;
				case INPUTGATE:
					toBlockPointer->connectInputGate(fromGatePointer,fromDoubleWeight);
					break;
				case FORGETGATE:
					toBlockPointer->connectForgetGate(fromGatePointer,fromDoubleWeight);
					break;
				case OUTPUTGATE:
					toBlockPointer->connectOutputGate(fromGatePointer,fromDoubleWeight);
					break;
				default:
					cerr << "Error while parsing gate type: " << memoryBlockGate << endl;
					cerr << "Did not find expected memory block gate." << endl;
					exit (1);
				}
				
				// count the connection
				countNodeMap[fromNameString] += 1;
				break;
				
			case OUTPUTEDGE:
				// connect to an output edge
				// get the to member name
				configurationFile >> toNameString;
				cout << "toNameString = " << toNameString << endl;
				
				// if the to name does not exist, crash
				if (networkNodeMap.count(toNameString) == 0)
				{
					cerr << "Error while parsing to name: " << toNameString << endl;
					cerr << "A node with (to) name " << toNameString << " does not exist." << endl;
					exit (1);
				}
				
				// get the pointer to the to gate
				toGatePointer = netContainer->OutputEdges[toNameString];
				
				// now get the from name
				configurationFile >> fromNameString;
				cout << "fromNameString = " << fromNameString << endl;
				
				// if the from name does not exist, crash
				if (networkNodeMap.count(fromNameString) == 0)
				{
					cerr << "Error while parsing from name: " << fromNameString << endl;
					cerr << "A node with (from) name " << fromNameString << " does not exist." << endl;
					exit (1);
				}
				
				// figure out which type this is from the networkNodeMap
				aNode = networkNodeMap[fromNameString];
				
				// get a fromGatePointer from the appropriate gate map
				if (aNode == INPUTEDGE)
				{
					fromGatePointer = netContainer->InputEdges[fromNameString];
					cout << "from is an IE " << endl;
				} else if (aNode == MEMORYBLOCK)
				{
					fromGatePointer = netContainer->MemoryBlocks[fromNameString];
					cout << "from is an MB " << endl;
				} else if (aNode == OUTPUTEDGE)
				{
					fromGatePointer = netContainer->OutputEdges[fromNameString];
					cout << "from is an OE " << endl;
				} else
				{
					cerr << "Error while finding node type for from name: " << fromNameString << endl;
					cerr << "OE from connection failed." << endl;
					exit (1);
				}
				
				// get the weight and connect
				configurationFile >> fromWeight;
				cout << "fromWeight = " << fromWeight << endl;
				fromDoubleWeight = strtod(fromWeight.c_str(),NULL);
				
				// if the number did not parse, crash
				if (errno != 0)
				{
					cerr << "Error while parsing connection weight: " << fromWeight << endl;
					cerr << "The weight did not convert to a double." << endl;
					exit (1);
				}
				toGatePointer->connectGate(fromGatePointer,fromDoubleWeight);
				
				// count the connection
				countNodeMap[fromNameString] += 1;
				break;
				
			default:
				// should not be
				cerr << "Error while parsing node type: " << nodeString << endl;
				cerr << "In connection mode.  Did not find expected node type." << endl;
				exit (1);
			}
		}
		
		// end of the while loop reading the input file
		if (configurationFile.good()) 
		{
			configurationFile >> nodeString;
		} else
		{
			cout << "The input file seems empty." << endl;
		}
	}
	
	cout << "Finished reading configuration file." << endl;


}

void NetDefine::checkNet()
{
	// error check
	// at least one IE, OE, MB
	if (netContainer->InputEdges.empty())
	{
		cerr << "Error in the network construction: No input edges in the net." << endl;
		exit (1);
	}
	if (netContainer->MemoryBlocks.empty())
	{
		cerr << "Error in the network construction: No memory blocks in the net." << endl;
		exit (1);
	}
	if (netContainer->OutputEdges.empty())
	{
		cerr << "Error in the network construction: No output edges in the net." << endl;
		exit (1);
	}

	// output of each input edge and memory block node connected to something
	// go through the list
	cout << "List of IE and MB with output not connected to anything: " << endl;
	for (countNodeIterator = countNodeMap.begin(); countNodeIterator != countNodeMap.end(); countNodeIterator++)
	{
		// if the type is not OUTPUTEDGE
		if (networkNodeMap[(*countNodeIterator).first] != OUTPUTEDGE)
		{
			// if the count is zero, bark but do not crash
			if ((*countNodeIterator).second == 0)
			{
				cout << (*countNodeIterator).first << endl;
			}
		}
	}
	cout << "End of list." << endl;
	
	// input edge list
	// for each node name, expose the count
	inEdge *anInputEdgePointer;
	netContainer->nodeListIterator = netContainer->InputEdges.begin();
	anInputEdgePointer = dynamic_cast<inEdge *>((*netContainer->nodeListIterator).second);
	netInputSize = anInputEdgePointer->size();
	for (netContainer->nodeListIterator = netContainer->InputEdges.begin(); netContainer->nodeListIterator != netContainer->InputEdges.end(); netContainer->nodeListIterator++)
	{
		// operate
		anInputEdgePointer = dynamic_cast<inEdge *>((*netContainer->nodeListIterator).second);
		netInputSize = min(netInputSize, anInputEdgePointer->size());
	}
	cout << "Maximum number of operate steps = " << netInputSize << endl;
	
	netContainer->netInputSize = netInputSize;
}

void NetDefine::listNet()
{
	// regurgitate the net and its connections

	// input edge list
	cout << "List of Input Edges and their incoming connections: " << endl;
	// for each node name, list the node name and all incoming connections
	for (netContainer->nodeListIterator = netContainer->InputEdges.begin(); netContainer->nodeListIterator != netContainer->InputEdges.end(); netContainer->nodeListIterator++)
	{
		// list each node name
		cout << (*netContainer->nodeListIterator).first << endl;
		// list all incoming connections
		(*netContainer->nodeListIterator).second->listConnections();
		cout << endl;
	}

	// memory block list
	cout << "List of Memory Blocks and their incoming connections: " << endl;
	// for each node name, list the node name and all incoming connections
	for (netContainer->nodeListIterator = netContainer->MemoryBlocks.begin(); netContainer->nodeListIterator != netContainer->MemoryBlocks.end(); netContainer->nodeListIterator++)
	{
		// list each node name
		cout << (*netContainer->nodeListIterator).first << endl;
		// list all incoming connections
		memoryBlock *aBlockPointer; // temp memory block pointer
		aBlockPointer = dynamic_cast<memoryBlock *>((*netContainer->nodeListIterator).second);
		aBlockPointer->listConnections();
		aBlockPointer->listInputConnections();
		aBlockPointer->listForgetConnections();
		aBlockPointer->listOutputConnections();
		cout << endl;
	}

	// input edge list
	cout << "List of Output Edges and their incoming connections: " << endl;
	// for each node name, list the node name and all incoming connections
	for (netContainer->nodeListIterator = netContainer->OutputEdges.begin(); netContainer->nodeListIterator != netContainer->OutputEdges.end(); netContainer->nodeListIterator++)
	{
		// list each node name
		cout << (*netContainer->nodeListIterator).first << endl;
		// list all incoming connections
		(*netContainer->nodeListIterator).second->listConnections();
		cout << endl;
	}
}

long NetDefine::size()
{
	return (netInputSize);
}
