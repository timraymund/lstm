/** connectionList test program 
 * 
 * tdr 2007.12.12
 * $Id: tester.cpp,v 1.2 2008/10/01 03:15:10 fatshark Exp $
 */

#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include <deque>
#include <vector>
#include <iterator>
#include <string>

#include "gate.h"

using namespace std;

int main(int argc, char* argv[])
{
	int i;
	vector<double> *weightChangeVector;
	
	weightChangeVector = new vector<double>;
	
	//declare a set of input edge gates, enough for one memory cell
	inEdge cellEdge("cellEdge.dat");
	inEdge inputEdge("inEdge.dat");
	inEdge forgetEdge("forgetEdge.dat");
	inEdge outputEdge("outputEdge.dat");
	
	// declare an output edge gate to capture the output
	outEdge blockEdge("blockEdge.dat");
	
	// declare a memory block
	memoryBlock oneBlock("oneBlock");
	
	// declare a training edge to match the output edge
	inEdge trainEdge("trainEdge.dat");
	
	// connect the block to the input edges
	oneBlock.connectGate(&cellEdge, 1.0);
	oneBlock.connectInputGate(&inputEdge, -1.0);
	oneBlock.connectForgetGate(&forgetEdge, 2.0);
	oneBlock.connectOutputGate(&outputEdge, -2.0);
	
	// connect the output edge to the memory block
	blockEdge.connectGate(&oneBlock, 1.0);
	
	// list the connections
	oneBlock.listConnections();
	oneBlock.listInputConnections();
	oneBlock.listForgetConnections();
	oneBlock.listOutputConnections();
	cout << endl;
	
	// list the output edge connections
	blockEdge.listConnections();
	cout << endl;
	
	// operate a two steps
	for (i=0; i<2; ++i)
	{
		// load the edge values for data files
		cellEdge.updateValue();
		inputEdge.updateValue();
		forgetEdge.updateValue();
		outputEdge.updateValue();
		cout << endl;
		
		// snapshot the memory block
		oneBlock.snapShot();
		// update the memory block
		oneBlock.updateValue();
		
		// regurgitate 
		cout << "Current Memory Block internal states " << endl;
		cout << "cell input state = " << oneBlock.getCellInputValue() << endl;
		cout << "input gate state = " << oneBlock.getInputValue() << endl;
		cout << "forget gate state = " << oneBlock.getForgetValue() << endl;
		cout << "cell state = " << oneBlock.getCellValue() << endl;
		cout << "output gate state = " << oneBlock.getOutputValue() << endl;
		cout << "output state = " << oneBlock.getValue() << endl << endl;
		
		// write the output edge
		blockEdge.snapShot();
		blockEdge.updateValue();
		blockEdge.outputValue();
		
		// regurgitate the output edge
		cout << "Current output edge state " << endl;
		cout << "cell state = " << blockEdge.getValue() << endl << endl;
		
		// for every output edge
			// compute the error and inject it
		trainEdge.updateValue();
		blockEdge.setInjectionError(trainEdge.getValue() - blockEdge.getValue());
			// compute the output edge delta
		blockEdge.updateDelta();
		blockEdge.pushDelta();
		
		// for every memory cell
			// compute delta of the output gate
			// the cell error
		oneBlock.updateDelta();
		
		// for every output edge
			// compute the weight updates
		blockEdge.updateWeight(weightChangeVector);
		
		// list the output edge connections and weights
		blockEdge.listConnections();
		cout << endl;
		
		// for every memory cell
			// update the output gate weights
			// update the input gate weights
			// update the forget gate weights
			// update the cell input weights
		oneBlock.updateWeight(weightChangeVector);
		
		// list the connections and weights
		oneBlock.listConnections();
		oneBlock.listInputConnections();
		oneBlock.listForgetConnections();
		oneBlock.listOutputConnections();
		cout << endl;
		

	}
	
	return 0;
}
