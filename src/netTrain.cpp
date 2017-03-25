/**
 * trains a network
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
#include "netMetric.h"
#include "netTrain.h"


using namespace std;


/** set net work container
 * point to the collection of connected nodes defining the network
 */
void NetTrain::setNetContainer(NetContainer *aNetContainer)
{
	netContainer = aNetContainer;
}

/** set training container
 * point to the collection of connected nodes defining the network
 */
void NetTrain::setTrainContainer(NetContainer *aTrainingContainer)
{
	trainContainer = aTrainingContainer;
}

/// train a network
/** train a network
 * walk through the output edges, training the nodes
 * walk through the memory blocks, training the nodes
 */
void NetTrain::train()
{
	double injectionError;
	
	// output edge list
	// for each node name, train
	trainContainer->nodeListIterator = trainContainer->InputEdges.begin(); 
	for (netContainer->nodeListIterator = netContainer->OutputEdges.begin(); netContainer->nodeListIterator != netContainer->OutputEdges.end(); netContainer->nodeListIterator++)
	{
		// train
		outEdge *anOutputEdgePointer;
		inEdge *aTrainingEdgePointer;
		anOutputEdgePointer = dynamic_cast<outEdge *>((*netContainer->nodeListIterator).second);
		aTrainingEdgePointer = dynamic_cast<inEdge *>((*trainContainer->nodeListIterator).second);
		
		aTrainingEdgePointer->updateValue();
		injectionError = aTrainingEdgePointer->getValue() - anOutputEdgePointer->getValue();
		anOutputEdgePointer->setInjectionError(injectionError);
		netMetric.injectionErrorVector->push_back(injectionError);
		
		anOutputEdgePointer->updateDelta();
		anOutputEdgePointer->pushDelta();
		anOutputEdgePointer->updateWeight(netMetric.weightChangeVector);
		
		trainContainer->nodeListIterator++;
	}
	
	// memory block list
	// for each node name, train
	for (netContainer->nodeListIterator = netContainer->MemoryBlocks.begin(); netContainer->nodeListIterator != netContainer->MemoryBlocks.end(); netContainer->nodeListIterator++)
	{
		// operate
		memoryBlock *aBlockPointer; // temp memory block pointer
		aBlockPointer = dynamic_cast<memoryBlock *>((*netContainer->nodeListIterator).second);
		aBlockPointer->updateDelta();
		aBlockPointer->updateWeight(netMetric.weightChangeVector);
	}

}

void NetTrain::metric()
{
	netMetric.writeClear();  // write and clear accumulator
}

/** reset network input
 * walk through the container
 * reset file for each input node
 */
void NetTrain::netInputReset()
{

	// input edge list
	// for each node name, reset the input file
	for (trainContainer->nodeListIterator = trainContainer->InputEdges.begin(); trainContainer->nodeListIterator != trainContainer->InputEdges.end(); trainContainer->nodeListIterator++)
	{
		// reset input file
		inEdge *anInEdge; // temp memory block pointer
		anInEdge = dynamic_cast<inEdge *>((*trainContainer->nodeListIterator).second);
		anInEdge->resetGateInput();
	}
}
