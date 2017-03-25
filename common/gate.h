// gate.h
// tdr 2007.11.21
// $Id: gate.h,v 1.5 2008/10/17 05:46:31 fatshark Exp $

#ifndef __gate_h__ // check for define to avoid double includes
#define __gate_h__ // define to avoid double includes

#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include <deque>
#include <vector>
#include <iterator>
#include <string>
#include <math.h>

using namespace std;


/// gate 
/** parent class for connections gate
 * holds a list of connectors and associated weights
 * indexes the list
 */
class gate 
{
protected:
	double sumResult; // squashed weighted summed value
	double sumDResult; // derivative squashed weighted summed value
	double sumError; // injection error
	double sumDelta; // backward pass delta
	double learningRate; // learning rate
	string gateName; // gate name
	deque<gate*> gateList; // list of attached gates
	deque<double> gateWeights; // list of weights associated with attached gates
	deque<double> oldValues; // inputs captured during snapshot prior to updating values
	deque<double> deltaValues; // a list of values pushed back
	deque<double> partialValues; // a list of partials
public:
	// Default constructor
	gate()
	{
		// do nothing
	}
	
	// constructor with a name
	gate(char* aName)
	{
		gateName += aName;  // set the gate instance name
		learningRate = 1.0; // 1.0E-6; // set the learning rate to default
		sumResult = 0;
		sumDResult = 0;
	}
	
	// constructor with a name and a learning rate
	gate(char* aName, double aValue)
	{
		gateName += aName;  // set the gate instance name
		learningRate = aValue; // set the learning rate
		sumResult = 0;
		sumDResult = 0;
	}
	
	// Copy constructor
	gate(const gate& aGate) :
		sumResult(aGate.sumResult)
		{}
	
	// Default destructor
	virtual ~gate()
	{
		// empty
	}
	
	// set gate instance name
	void setName(char* aName);
	
	// connect to a gate with a weight
	virtual void connectGate(gate* aGate, double weight);
	
	// return the number of connections
	virtual int numberConnections();
	
	// output a list of connectiosn
	virtual void listConnections();
	
	// return the squashed weighted sum
	virtual double getValue();
	
	// return the partial derivatives
	virtual void getDValues(deque<double>* partials, double gateValue, double inValue);
	
	// initialize
	virtual void setValue(double aResult);
	
	// capture input values
	virtual void snapShot();
	
	// sum up the weighted values from every connected gate and squash the result
	virtual void updateValue();
	
	// collect weighted delta pushed back
	virtual void listWeightedDelta(double weightedDelta);
	
	// set the injection error
	void setInjectionError(double injectionError);
	
	// update the delta
	virtual void updateDelta();
	
	// set the delta
	virtual void setDelta(double x);
	
	// update the weights
	virtual void updateWeight(vector<double> *weightChangeVector);
	
	// return the name of this gate instance
	string getName();
	
	// does nothing squashing function 
	virtual double squash(double x);
	
	// does nothing squashing function derivative
	virtual double dsquash(double x);
	
	// save the gate
	virtual void saveGate(ofstream *outFile);
	
	// restore the gate
	virtual void restoreGate(ifstream *inFile);
	
	// initialize the gate
	virtual void initializeGate();

};

class fGate: public gate
{
public:
	// constructor with a name
	fGate(char* aName) : gate(aName)
	{
		// do nothing
	}
	
	// destructor
	virtual ~fGate()
	{
		// do nothing
	}
	
	// set squash to f squash
	double squash(double x);
	
	// set squash derivative to f squash derivative
	double dsquash(double x);
	
	// update the weights
	virtual void updateWeight(vector<double> *weightChangeVector);
};

class gGate: public gate
{
public:	
	// constructor with a name
	gGate(char* aName) : gate(aName)
	{
		// do nothing
	}
	
	virtual ~gGate()
	{
		// empty
	}
	
	// set squash to g squash
	double squash(double x);
	
	// set squash derivative to g squash derivative
	double dsquash(double x);
	
	// update the weights
	virtual void updateWeight(vector<double> *weightChangeVector);
};

class hGate: public fGate
{
public:
	// constructor with a name and file
	hGate(char* aName) : fGate(aName)
	{
		// do nothing
	}
	
	// destructor
	virtual ~hGate()
	{
		// do nothing
	}
	
    // update the weights
	void updateWeight(vector<double> *weightChangeVector);

};

class memoryCell: public gate
{
private:
	// deque<gate*> inGates;
	gate* inGate;
	gate* forgetGate;
	double currentCellState;
	double previousCellState;	
	deque<double> cellPartials;
	deque<double> inPartials;
	deque<double> forgetPartials;
public:
	// constructor with a name
	memoryCell(char* aName) : gate(aName)
	{
		// do nothing
	}
	
	// destructor
	virtual ~memoryCell()
	{
		//empty
	}
	
	// connect an input gate
	void connectIn(gate* aGate);
	
	// connect a forget gate
	void connectForget(gate* aGate);
	
	// snapshot current value 
	void snapShot();
	
	// get input and multiply gate values
	void updateValue();
	
	// save the gate
	void saveGate(ofstream *outFile);
	
	// restore the gate
	void restoreGate(ifstream *inFile);
	
	// initialize the gate
	void initializeGate();

};

class memoryOutput: public gate
{
private:
	gate* outGate;
	deque<double> outPartials;
public:
	// constructor with a name
	memoryOutput(char* aName) : gate(aName)
	{
		// do nothing
	}
	
	// destructor
	virtual ~memoryOutput()
	{
		// empty
	}
	
	// connect an output gate
	void connectOut(gate* aGate);
	
	// get input and multiply gate value
	void updateValue();
	
	// save the gate
	void saveGate(ofstream *outFile);
	
	// restore the gate
	void restoreGate(ifstream *inFile);
	
	// initialize the gate
	void initializeGate();
	
};

class inEdge: public gate
{
private:
	ifstream inputFile; // file handle for input data
	char* inputFileName; // file name for the handle
	long inputEdgeSize;		// number of records available to the edge gate
public:
	// constructor with a name and file
	inEdge(char* aName) : gate(aName)
	{
		// open the file
		inputFile.open(aName);
		
		// count newlines
		filebuf *pbuf;			// file buffer handle
		long inputFileSize;		// the size of the file
		long numNewLines;		// count of new lines = number of input data
		char *inputFileBuffer;	// a pointer for the file contents
		
		pbuf=inputFile.rdbuf();	// get the file buffer pointer
		
		inputFileSize = pbuf->pubseekoff (0,ios::end,ios::in);  // get the size (cheat)
		pbuf->pubseekpos (0,ios::in);							// reset pointer
		
		cout << "The file has a size of " << inputFileSize << endl;
		
		inputFileBuffer = new char[inputFileSize];		// allocate the file buffer
		
		pbuf->sgetn (inputFileBuffer, inputFileSize);	// get the file
		
		inputFile.close();		// close
		inputFile.open(aName);	// re-open
		
		int iBuffer;			// index into the buffer
		
		numNewLines = 0;
		for (iBuffer = 0; iBuffer < inputFileSize; ++iBuffer)
		{
			if (inputFileBuffer[iBuffer] == '\n')
			{
				++numNewLines;
			}
		}
		
		inputEdgeSize = numNewLines;
		delete[] inputFileBuffer;
	}
	
	// generic destructor
	virtual ~inEdge()
	{
		inputFile.close();
	}
	
	// get input and multiply gate value
	void updateValue();
	
	// reset gate input
	void resetGateInput();
	
	// return the size of the input
	long size();
};

class outEdge: public fGate
{
private:
	ofstream outputFile; // file handle for input data
	char* outputFileName; // file name for the handle
public:
	// constructor with a name and file
	outEdge(char* aName) : fGate(aName)
	{
		// open the file
		outputFile.open(aName);
	}
	
	// destructor
	virtual ~outEdge()
	{
		outputFile.close();
	}
	
	// push delta back to output gate
	void pushDelta();
	
	// write the output value
	void outputValue();
	
	// update the weights
	void updateWeight(vector<double> *weightChangeVector);

};

class memoryBlock: public gate
{
private:
	gGate* cellInput; // cell input
	fGate* inputGate; // input gate
	fGate* forgetGate; // forget gate
	hGate* outputGate; // output gate
	memoryCell* cellOne; // a single memory cell
	memoryOutput* blockOutput; // the block output
	double cellError; // a term for updates
public:
	memoryBlock(char* aName) : gate(aName)
	{
		// create the parts
		cellInput = new gGate("cellInput");
		inputGate = new fGate("inputGate");
		forgetGate = new fGate("forgetGate");
		outputGate = new hGate("outputGate");
		cellOne = new memoryCell("cellOne");
		blockOutput = new memoryOutput("blockOutput");
		
		// connect things together
		cellOne->connectGate(cellInput, 1.0);
		cellOne->connectIn(inputGate);
		cellOne->connectForget(forgetGate);
		inputGate->connectGate(cellOne, -1.0);
		forgetGate->connectGate(cellOne, 2.0);
		outputGate->connectGate(cellOne, -2.0);
		blockOutput->connectGate(cellOne, 1.0);
		blockOutput->connectOut(outputGate);
	}
	
	// destructor
	virtual ~memoryBlock()
	{
		//empty
	}
	
	// connect to a gate with a weight
	void connectGate(gate* aGate, double weight);
	
	// return the number of connections
	int numberConnections();
	
	// output a list of connectiosn
	void listConnections();	
	
	// snapshot current inputs
	void snapShot();

	// sum up the weighted values from every connected gate and squash the result
	void updateValue();
	
	// update the delta
	void updateDelta();
	
	// update the weights
	void updateWeight(vector<double> *weightChangeVector);
	
	// output the cell value
	double getCellValue();

	
	// connect cell input to a gate with a weight
	void connectCellInputGate(gate* aGate, double weight);
		
	// return the number of cell input connections
	int numberCellInputConnections();
		
	// output a list of cell input connectiosn
	void listCellInputConnections();
		
	// return the squashed weighted sum at the cell input
	double getCellInputValue();
	
	
	// connect input gate to a gate with a weight
	void connectInputGate(gate* aGate, double weight);
		
	// return the number of input gate connections
	int numberInputConnections();
		
	// output a list of input gate connectiosn
	void listInputConnections();
		
	// return the squashed weighted sum at the input gate
	double getInputValue();
	
	
	// connect forget gate to a gate with a weight
	void connectForgetGate(gate* aGate, double weight);
	
	// return the number of forget gate connections
	int numberForgetConnections();
	
	// output a list of forget gate connectiosn
	void listForgetConnections();
	
	// return the squashed weighted sum at the forget gate
	double getForgetValue();

	
	// connect output gate to a gate with a weight
	void connectOutputGate(gate* aGate, double weight);
		
	// return the number of output gate connections
	int numberOutputConnections();
		
	// output a list of output gate connectiosn
	void listOutputConnections();
		
	// return the squashed weighted sum at the output gate
	double getOutputValue();
	
	
	// save the gate
	void saveGate(ofstream *outFile);
	
	// restore the gate
	void restoreGate(ifstream *inFile);
	
	// initialize the gate
	void initializeGate();
	
};


#endif /* __gate_h__ */ // end check for double includes

