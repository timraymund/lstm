/**
 * saves or restores a network
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
#include "netSaveRestore.h"

using namespace std;

/// save or restore a network
/** save or restore a network
 * set the file name
 * walk through the net, saving the nodes
 * walk through the net, restoring the nodes
 */

/** set net work container
 * point to the collection of connected nodes defining the network
 */
void NetSaveRestore::setNetContainer(NetContainer *aNetContainer)
{
	netContainer = aNetContainer;
}

/** save a network
 * walk through the container
 * save each node
 */
void NetSaveRestore::netSave()
{
	// save the net
	netSaveFile.open(netSaveRestoreName.c_str()); // open the save file

	// input edge list
	// for each node name, save
	for (netContainer->nodeListIterator = netContainer->InputEdges.begin(); netContainer->nodeListIterator != netContainer->InputEdges.end(); netContainer->nodeListIterator++)
	{
		// save
		(*netContainer->nodeListIterator).second->saveGate(&netSaveFile);
	}

	// memory block list
	// for each node name, save
	for (netContainer->nodeListIterator = netContainer->MemoryBlocks.begin(); netContainer->nodeListIterator != netContainer->MemoryBlocks.end(); netContainer->nodeListIterator++)
	{
		// save
		memoryBlock *aBlockPointer; // temp memory block pointer
		aBlockPointer = dynamic_cast<memoryBlock *>((*netContainer->nodeListIterator).second);
		aBlockPointer->saveGate(&netSaveFile);
	}

	// output edge list
	// for each node name, save
	for (netContainer->nodeListIterator = netContainer->OutputEdges.begin(); netContainer->nodeListIterator != netContainer->OutputEdges.end(); netContainer->nodeListIterator++)
	{
		// save
		(*netContainer->nodeListIterator).second->saveGate(&netSaveFile);
	}
	
	netSaveFile.close(); // close the save file
}

/** restore a network
 * walk through the container
 * restore each node
 */
void NetSaveRestore::netRestore()
{
	// restore the net
	netRestoreFile.open(netSaveRestoreName.c_str()); // open the restore file

	// input edge list
	// for each node name, restore
	for (netContainer->nodeListIterator = netContainer->InputEdges.begin(); netContainer->nodeListIterator != netContainer->InputEdges.end(); netContainer->nodeListIterator++)
	{
		// save
		(*netContainer->nodeListIterator).second->restoreGate(&netRestoreFile);
	}

	// memory block list
	// for each node name, restore
	for (netContainer->nodeListIterator = netContainer->MemoryBlocks.begin(); netContainer->nodeListIterator != netContainer->MemoryBlocks.end(); netContainer->nodeListIterator++)
	{
		// save
		memoryBlock *aBlockPointer; // temp memory block pointer
		aBlockPointer = dynamic_cast<memoryBlock *>((*netContainer->nodeListIterator).second);
		aBlockPointer->restoreGate(&netRestoreFile);
	}

	// output edge list
	// for each node name, restore
	for (netContainer->nodeListIterator = netContainer->OutputEdges.begin(); netContainer->nodeListIterator != netContainer->OutputEdges.end(); netContainer->nodeListIterator++)
	{
		// save
		(*netContainer->nodeListIterator).second->restoreGate(&netRestoreFile);
	}
	
	netRestoreFile.close(); // close the restore file
}

/** reset network input
 * walk through the container
 * reset file for each input node
 */
void NetSaveRestore::netInputReset()
{

	// input edge list
	// for each node name, reset the input file
	for (netContainer->nodeListIterator = netContainer->InputEdges.begin(); netContainer->nodeListIterator != netContainer->InputEdges.end(); netContainer->nodeListIterator++)
	{
		// reset input file
		inEdge *anInEdge; // temp memory block pointer
		anInEdge = dynamic_cast<inEdge *>((*netContainer->nodeListIterator).second);
		anInEdge->resetGateInput();
	}
}

/** initialize a network state
 * walk through the container
 * initialize the variables for each node
 */
void NetSaveRestore::netInitializeState()
{

	// input edge list
	// for each node name
	for (netContainer->nodeListIterator = netContainer->InputEdges.begin(); netContainer->nodeListIterator != netContainer->InputEdges.end(); netContainer->nodeListIterator++)
	{
		// initialize
		inEdge *anInEdge; // temp in edge pointer
		anInEdge = dynamic_cast<inEdge *>((*netContainer->nodeListIterator).second);
		anInEdge->initializeGate();
	}

	// memory block list
	// for each node name
	for (netContainer->nodeListIterator = netContainer->MemoryBlocks.begin(); netContainer->nodeListIterator != netContainer->MemoryBlocks.end(); netContainer->nodeListIterator++)
	{
		// initialize
		memoryBlock *aBlockPointer; // temp memory block pointer
		aBlockPointer = dynamic_cast<memoryBlock *>((*netContainer->nodeListIterator).second);
		aBlockPointer->initializeGate();
	}

	// output edge list
	// for each node name
	for (netContainer->nodeListIterator = netContainer->OutputEdges.begin(); netContainer->nodeListIterator != netContainer->OutputEdges.end(); netContainer->nodeListIterator++)
	{
		// initialize
		outEdge *anOutEdge; // temp out edge pointer
		anOutEdge = dynamic_cast<outEdge *>((*netContainer->nodeListIterator).second);
		anOutEdge->initializeGate();
	}

}


