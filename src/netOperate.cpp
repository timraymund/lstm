/**
 * operates a network
 */

#include <iostream>
#include <cstdlib>
#include <map>
#include <deque>
#include <iterator>
#include <string>
#include <cmath>

#include "gate.h"
#include "netContainer.h"
#include "netOperate.h"

using namespace std;

/// operate a network
/** operate a network
 * net the network container pointer
 * walk through the net, operating the nodes
 */

/** set net work container
 * point to the collection of connected nodes defining the network
 */
void NetOperate::setNetContainer(NetContainer *aNetContainer)
{
	netContainer = aNetContainer;
}

void NetOperate::operate()
{
	// input edge list
	// for each node name, operate
	for (netContainer->nodeListIterator = netContainer->InputEdges.begin(); netContainer->nodeListIterator != netContainer->InputEdges.end(); netContainer->nodeListIterator++)
	{
		// operate
		inEdge *anInputEdgePointer;
		anInputEdgePointer = dynamic_cast<inEdge *>((*netContainer->nodeListIterator).second);
		anInputEdgePointer->updateValue();
	}

	// memory block list
	// for each node name, operate
	for (netContainer->nodeListIterator = netContainer->MemoryBlocks.begin(); netContainer->nodeListIterator != netContainer->MemoryBlocks.end(); netContainer->nodeListIterator++)
	{
		// operate
		memoryBlock *aBlockPointer; // temp memory block pointer
		aBlockPointer = dynamic_cast<memoryBlock *>((*netContainer->nodeListIterator).second);
		aBlockPointer->snapShot();
		aBlockPointer->updateValue();
	}

	// output edge list
	// for each node name, operate
	for (netContainer->nodeListIterator = netContainer->OutputEdges.begin(); netContainer->nodeListIterator != netContainer->OutputEdges.end(); netContainer->nodeListIterator++)
	{
		// operate
		outEdge *anOutputEdgePointer;
		anOutputEdgePointer = dynamic_cast<outEdge *>((*netContainer->nodeListIterator).second);
		anOutputEdgePointer->snapShot();
		anOutputEdgePointer->updateValue();
		anOutputEdgePointer->outputValue();
	}
}

