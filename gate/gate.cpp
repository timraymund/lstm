// gate.cpp
// tdr 2007.11.21
// $Id: gate.cpp,v 1.5 2008/10/17 05:46:31 fatshark Exp $


#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include <deque>
#include <vector>
#include <iterator>
#include <string>
#include <math.h>

#include "gate.h"


using namespace std;


// name the gate instance
void gate::setName(char* aName)
{
	gateName += aName;
}
	
// connect to a gate with a weight
void gate::connectGate(gate* aGate, double weight)
{
	gateList.push_back(aGate);
	gateWeights.push_back(weight);
	oldValues.push_back(0.0);
}

// return the number of connections
int gate::numberConnections()
{
	int number;		
	
	number = gateList.size();
	return(number);
}

// list the connections
void gate::listConnections()
{
	unsigned int i;
	
	cout << gateName << " gate listConnections " << endl;
	cout << "gateList.size() = " << gateList.size() << endl;
	for(i=0; i<gateList.size(); ++i)
		cout << "Index = " << i << " Connection Name = " << gateList[i]->getName() << " Weight = " << gateWeights[i] << endl;
	cout << endl;
}

double gate::getValue()
{
	return(sumResult);
}

// compute partials for weight updates
void gate::getDValues(deque<double>* partials, double gateValue, double inValue)
{
	unsigned int i;
	double partialValue;
	
	cout << gateName << " gate getDValues " << endl;
	
	if ((*partials).empty())
		(*partials).assign(gateList.size(),0.0);
	
	for(i=0; i<gateList.size(); ++i)
	{
		partialValue = (*partials)[i]*gateValue + inValue * sumDResult * oldValues[i];
		(*partials)[i] = partialValue;
		cout << "(*partials)[" << i << "] = " << partialValue << endl;
	}
	
	cout << endl;
	
	partialValues = (*partials);
}

void gate::setValue(double aResult)
{
	sumResult = aResult;
}

// snapshot the current inputs
void gate::snapShot()
{
	unsigned int i;
	
	cout << gateName << " gate snapShot " << endl;
	cout << "gateList.size() = " << gateList.size() << endl;
	
	for(i=0; i<gateList.size(); ++i)
	{
		oldValues[i]=gateList[i]->getValue();
		cout << "oldValues[" << i << "] = " << oldValues[i] << endl;
	}
	cout << endl;
}

// sum up the weighted values from every gate and squash them
void gate::updateValue()
{
	unsigned int i;
	double result;
	
	result = 0;
	for(i=0; i<gateList.size(); ++i)
				result += gateWeights[i]*oldValues[i];
	sumResult = this->squash(result);
	sumDResult = this->dsquash(result);
	
	cout << gateName << " gate updateValue " << endl;
	cout << "result = " << result << endl;
	cout << "sumResult = " << sumResult << endl;
	cout << "sumDResult = " << sumDResult << endl;
	cout << endl;
	
}

// collect the delta pushed back
void gate::listWeightedDelta(double weightedDelta)
{
	
	cout << gateName << " gate listWeightedDelta " << endl;
	
	deltaValues.push_back(weightedDelta);
}

// return the gate instance name
string gate::getName()
{
	return(gateName);
}

double gate::squash(double x)
{
	// this is the no squash function
	double result;
	
	result = x;
	
	return(result);	
}

double gate::dsquash(double x)
{
	// this is the no squash function derivative
	double result;
	
	result = 0;
	
	return(result);
}

// set injection error
void gate::setInjectionError(double injectionError)
{
	sumError = injectionError;
	
	cout << gateName << " gate setInjectionError " << endl;
	cout <<	"injection error = " << sumError << endl;
	cout << endl;
}

// update the delta
void gate::updateDelta()
{
	sumDelta = sumDResult * sumError;
	
	cout << gateName << " gate updateDelta" << endl;
	cout << "sumDResult = " << sumDResult << endl;
	cout << "sumError = " << sumError << endl;
	cout << "sumDelta = " << sumDelta << endl;
	cout << endl;
}

// set the delta
void gate::setDelta(double x)
{
	sumDelta = x;
	
	cout << gateName << " gate setDelta" << endl;
	cout << "sumDelta = " << sumDelta << endl;
	cout << endl;
}

// update the weights
void gate::updateWeight(vector<double> *weightChangeVector)
{
	unsigned int i;
	double weightChange;
	
	cout << gateName << " gate updateWeight " << endl;
	
	for (i=0; i<gateWeights.size(); ++i)
	{
		weightChange = learningRate * sumDelta * oldValues[i];
		weightChangeVector->push_back(weightChange);
		gateWeights[i] += weightChange;
		cout << "learningRate = " << learningRate << endl;
		cout << "sumDelta = " << sumDelta << endl;
		cout << "oldValues[" << i << "] = " << oldValues[i] << endl;
		cout << "gateWeights[" << i << "] = " << gateWeights[i] << endl;
		cout << "----" << endl;
	}
	cout << endl;

}

void gate::saveGate(ofstream *outFile)
{
	unsigned int i;  // loop index to dump the deques
	
	// save everything in order
	// gateWeights
	(*outFile) << gateWeights.size() << " ";
	for (i=0; i<gateWeights.size(); i++)
	{
		(*outFile) << gateWeights[i] << " ";
	}
	(*outFile) << endl;
	// sumResult
	(*outFile) << sumResult << endl ;
	// oldValues
	(*outFile) << oldValues.size() << " ";
	for (i=0; i<oldValues.size(); i++)
	{
		(*outFile) << oldValues[i] << " ";
	}
	(*outFile) << endl;
	// sumDResult
	(*outFile) << sumDResult << endl;
	// sumError
	(*outFile) << sumError << endl;
	// sumDelta
	(*outFile) << sumDelta << endl;
	// learningRate
	(*outFile) << learningRate << endl;
	// deltaValues
	(*outFile) << deltaValues.size() << " ";
	for (i=0; i<deltaValues.size(); i++)
	{
		(*outFile) << deltaValues[i] << " ";
	}
	(*outFile) << endl;
	// partialValues
	(*outFile) << partialValues.size() << " ";
	for (i=0; i<partialValues.size(); i++)
	{
		(*outFile) << partialValues[i] << " ";
	}
	(*outFile) << endl;
}


void gate::restoreGate(ifstream *inFile)
{
	int i;  // loop index to load the deques
	int dSize; // size of the deque
	double dValue; // temp to hold deque value
	
	// save everything in order
	// gateWeights
	(*inFile) >> dSize;
	gateWeights.clear();
	for (i=0; i<dSize; i++)
	{
		(*inFile) >> dValue;
		gateWeights.push_back(dValue);
	}
	// sumResult
	(*inFile) >> sumResult;
	// oldValues
	(*inFile) >> dSize;
	oldValues.clear();
	for (i=0; i<dSize; i++)
	{
		(*inFile) >> dValue;
		oldValues.push_back(dValue);
	}
	// sumDResult
	(*inFile) >> sumDResult;
	// sumError
	(*inFile) >> sumError;
	// sumDelta
	(*inFile) >> sumDelta;
	// learningRate
	(*inFile) >> learningRate;
	// deltaValues
	(*inFile) >> dSize;
	deltaValues.clear();
	for (i=0; i<dSize; i++)
	{
		(*inFile) >> dValue;
		deltaValues.push_back(dValue);
	}
	// partialValues
	(*inFile) >> dSize;
	partialValues.clear();
	for (i=0; i<dSize; i++)
	{
		(*inFile) >> dValue;
		partialValues.push_back(dValue);
	}
}

void gate::initializeGate()
{
	int i;  // loop index to load the deques
	int dSize;
	
	// initialize everything except the weights

	// sumResult
	sumResult = 0.0;
	// oldValues
	dSize = oldValues.size();
	oldValues.clear();
	for (i=0; i<dSize; i++)
	{
		oldValues.push_back(0.0);
	}
	// sumDResult
	sumDResult = 0.0;
	// sumError
	sumError = 0.0;
	// sumDelta
	sumDelta = 0.0;
	// learningRate
	learningRate = 1.0;
	// deltaValues
	dSize = deltaValues.size();
	deltaValues.clear();
	for (i=0; i<dSize; i++)
	{
		deltaValues.push_back(0.0);
	}
	// partialValues
	dSize = partialValues.size();
	partialValues.clear();
	for (i=0; i<dSize; i++)
	{
		partialValues.push_back(0.0);
	}
}

double fGate::squash(double x)
{
	// this is the f squash function
	double result;
	
	result = 1.0/(1.0+exp(-x));
	
	return(result);
}

double fGate::dsquash(double x)
{
	// this is the f squash function derivative
	double result;
	
	result = exp(-x)/((1.0+exp(-x))*(1.0+exp(-x)));
	
	return(result);
}

// update the weights
void fGate::updateWeight(vector<double> *weightChangeVector)
{
	unsigned int i;
	double weightChange;
	
	cout << gateName << " fGate updateWeight " << endl;
	
	for (i=0; i<gateWeights.size(); ++i)
	{
		weightChange = learningRate * sumDelta * partialValues[i];
		weightChangeVector->push_back(weightChange);
		gateWeights[i] += weightChange;
		cout << "learningRate = " << learningRate << endl;
		cout << "sumDelta = " << sumDelta << endl;
		cout << "partialValues[" << i << "] = " << partialValues[i] << endl;
		cout << "gateWeights[" << i << "] = " << gateWeights[i] << endl;
		cout << "----" << endl;
	}
	cout << endl;

}


double gGate::squash(double x)
{
	// this is the g squash function
	double result;
	
	result = (4.0/(1.0+exp(-x)))-2.0;
	
	return(result);	
}

double gGate::dsquash(double x)
{
	// this is the g squash function derivative
	double result;
	
	result = (4.0*exp(-x))/((1.0+exp(-x))*(1.0+exp(-x)));
	
	return(result);
}

// update the weights
void gGate::updateWeight(vector<double> *weightChangeVector)
{
	unsigned int i;
	double weightChange;
	
	cout << gateName << " gGate updateWeight " << endl;
	
	for (i=0; i<gateWeights.size(); ++i)
	{
		weightChange = learningRate * sumDelta * partialValues[i];	
		weightChangeVector->push_back(weightChange);
		gateWeights[i] += weightChange;
		cout << "learningRate = " << learningRate << endl;
		cout << "sumDelta = " << sumDelta << endl;
		cout << "partialValues[" << i << "] = " << partialValues[i] << endl;
		cout << "gateWeights[" << i << "] = " << gateWeights[i] << endl;
		cout << "----" << endl;
	}
	cout << endl;

}


// update the weights
void hGate::updateWeight(vector<double> *weightChangeVector)
{
	unsigned int i;
	double weightChange;
	
	cout << gateName << " hGate updateWeight " << endl;
	
	for (i=0; i<gateWeights.size(); ++i)
	{
		weightChange = learningRate * sumDelta * oldValues[i];
		weightChangeVector->push_back(weightChange);
		gateWeights[i] += weightChange;
		cout << "learningRate = " << learningRate << endl;
		cout << "sumDelta = " << sumDelta << endl;
		cout << "oldValues[" << i << "] = " << oldValues[i] << endl;
		cout << "gateWeights[" << i << "] = " << gateWeights[i] << endl;
		cout << "----" << endl;
	}
	cout << endl;

}

// connect an input gate
void memoryCell::connectIn(gate* aGate)
{
	inGate = aGate;
}

// connect a forget gate
void memoryCell::connectForget(gate* aGate)
{
	forgetGate = aGate;
}

// snapshot the current inputs
void memoryCell::snapShot()
{
	previousCellState = currentCellState;
	
}

// sum up the weighted values from every gate and squash them
void memoryCell::updateValue()
{
	unsigned int i;
	double inCell;
	
	cout << gateName << " memoryCell updateValue " << endl;
	
	// collect from memory cell input
	inCell = 0;
	for(i=0; i<gateList.size(); ++i)
				inCell += gateList[i]->getValue();
	cout << "inCell = " << inCell << endl;
	
	// compute current state
	currentCellState = previousCellState*forgetGate->getValue() + inCell*inGate->getValue();
	cout << "previousCellState = " << previousCellState << endl;
	cout << "forgetGate->getValue() = " << forgetGate->getValue() << endl;
	cout << "inCell = " << inCell << endl;
	cout << "inGate->getValue() = " << inGate->getValue() << endl;
	cout << "currentCellState = " << currentCellState << endl;

	// expose current state through getValue
	sumResult = currentCellState;
	cout << "sumResult = " << sumResult << endl;
	
	// compute current cell input partials
	for(i=0; i<gateList.size(); ++i)
		gateList[i]->getDValues(&cellPartials, forgetGate->getValue(), inGate->getValue());
	
	// compute current input gate partials
	inGate->getDValues(&inPartials, forgetGate->getValue(), inCell);
	
	// compute current forget gate partials
	forgetGate->getDValues(&forgetPartials, forgetGate->getValue(), previousCellState);
	
	cout << endl;
	
}

void memoryCell::saveGate(ofstream *outFile)
{
	unsigned int i;  // loop index to dump the deques
	
	// save everything in order
	// gateWeights
	(*outFile) << gateWeights.size() << " ";
	for (i=0; i<gateWeights.size(); i++)
	{
		(*outFile) << gateWeights[i] << " ";
	}
	(*outFile) << endl;
	// sumResult
	(*outFile) << sumResult << endl ;
	// oldValues
	(*outFile) << oldValues.size() << " ";
	for (i=0; i<oldValues.size(); i++)
	{
		(*outFile) << oldValues[i] << " ";
	}
	(*outFile) << endl;
	// sumDResult
	(*outFile) << sumDResult << endl;
	// sumError
	(*outFile) << sumError << endl;
	// sumDelta
	(*outFile) << sumDelta << endl;
	// learningRate
	(*outFile) << learningRate << endl;
	// deltaValues
	(*outFile) << deltaValues.size() << " ";
	for (i=0; i<deltaValues.size(); i++)
	{
		(*outFile) << deltaValues[i] << " ";
	}
	(*outFile) << endl;
	// partialValues
	(*outFile) << partialValues.size() << " ";
	for (i=0; i<partialValues.size(); i++)
	{
		(*outFile) << partialValues[i] << " ";
	}
	// currentCellState
	(*outFile) << currentCellState << endl;
	// previousCellState
	(*outFile) << previousCellState << endl;
	(*outFile) << endl;
}

void memoryCell::restoreGate(ifstream *inFile)
{
	int i;  // loop index to load the deques
	int dSize; // size of the deque
	double dValue; // temp to hold deque value
	
	// save everything in order
	// gateWeights
	(*inFile) >> dSize;
	gateWeights.clear();
	for (i=0; i<dSize; i++)
	{
		(*inFile) >> dValue;
		gateWeights.push_back(dValue);
	}
	// sumResult
	(*inFile) >> sumResult;
	// oldValues
	(*inFile) >> dSize;
	oldValues.clear();
	for (i=0; i<dSize; i++)
	{
		(*inFile) >> dValue;
		oldValues.push_back(dValue);
	}
	// sumDResult
	(*inFile) >> sumDResult;
	// sumError
	(*inFile) >> sumError;
	// sumDelta
	(*inFile) >> sumDelta;
	// learningRate
	(*inFile) >> learningRate;
	// deltaValues
	(*inFile) >> dSize;
	deltaValues.clear();
	for (i=0; i<dSize; i++)
	{
		(*inFile) >> dValue;
		deltaValues.push_back(dValue);
	}
	// partialValues
	(*inFile) >> dSize;
	partialValues.clear();
	for (i=0; i<dSize; i++)
	{
		(*inFile) >> dValue;
		partialValues.push_back(dValue);
	}
	// currentCellState
	(*inFile) >> currentCellState;
	// previousCellState
	(*inFile) >> previousCellState;
}


void memoryCell::initializeGate()
{
	int i;  // loop index to load the deques
	int dSize; // size of the deque
	
	// initialize everything in order
	// except the gateWeights

	// sumResult
	sumResult = 0.0;
	// oldValues
	dSize = oldValues.size();
	oldValues.clear();
	for (i=0; i<dSize; i++)
	{
		oldValues.push_back(0.0);
	}
	// sumDResult
	sumDResult = 0.0;
	// sumError
	sumError = 0.0;
	// sumDelta
	sumDelta = 0.0;
	// learningRate
	learningRate = 1.0;
	// deltaValues
	dSize = deltaValues.size();
	deltaValues.clear();
	for (i=0; i<dSize; i++)
	{
		deltaValues.push_back(0.0);
	}
	// partialValues
	dSize = partialValues.size();
	partialValues.clear();
	for (i=0; i<dSize; i++)
	{
		partialValues.push_back(0.0);
	}
	// currentCellState
	currentCellState = 0.0;
	// previousCellState
	previousCellState = 0.0;
}

// connect a forget gate
void memoryOutput::connectOut(gate* aGate)
{
	outGate = aGate;
}

// sum up the weighted values from every gate and squash them
void memoryOutput::updateValue()
{
	unsigned int i;
	double inCell;
	double result;
	
	cout << gateName << " memoryOutput updateValue" << endl;
	
	// collect from memory cell input
	inCell = 0;
	for(i=0; i<gateList.size(); ++i)
				inCell += gateList[i]->getValue();
	
	cout << "inCell = " << inCell << endl;
	
	// compute current state
	result = inCell*outGate->getValue();
	sumResult = result;
	
	cout << "outGate->getValue() = " << outGate->getValue() << endl;
	cout << "sumResult = " << sumResult << endl;
	cout << endl;
	
}

void memoryOutput::saveGate(ofstream *outFile)
{
	unsigned int i;  // loop index to dump the deques
	
	// save everything in order
	// gateWeights
	(*outFile) << gateWeights.size() << " ";
	for (i=0; i<gateWeights.size(); i++)
	{
		(*outFile) << gateWeights[i] << " ";
	}
	(*outFile) << endl;
	// sumResult
	(*outFile) << sumResult << endl ;
	// oldValues
	(*outFile) << oldValues.size() << " ";
	for (i=0; i<oldValues.size(); i++)
	{
		(*outFile) << oldValues[i] << " ";
	}
	(*outFile) << endl;
	// sumDResult
	(*outFile) << sumDResult << endl;
	// sumError
	(*outFile) << sumError << endl;
	// sumDelta
	(*outFile) << sumDelta << endl;
	// learningRate
	(*outFile) << learningRate << endl;
	// deltaValues
	(*outFile) << deltaValues.size() << " ";
	for (i=0; i<deltaValues.size(); i++)
	{
		(*outFile) << deltaValues[i] << " ";
	}
	(*outFile) << endl;
	// partialValues
	(*outFile) << partialValues.size() << " ";
	for (i=0; i<partialValues.size(); i++)
	{
		(*outFile) << partialValues[i] << " ";
	}
	// outPartials
		(*outFile) << outPartials.size() << " ";
		for (i=0; i<outPartials.size(); i++)
		{
			(*outFile) << outPartials[i] << " ";
		}
	(*outFile) << endl;
}

void memoryOutput::restoreGate(ifstream *inFile)
{
	int i;  // loop index to load the deques
	int dSize; // size of the deque
	double dValue; // temp to hold deque value
	
	// save everything in order
	// gateWeights
	(*inFile) >> dSize;
	gateWeights.clear();
	for (i=0; i<dSize; i++)
	{
		(*inFile) >> dValue;
		gateWeights.push_back(dValue);
	}
	// sumResult
	(*inFile) >> sumResult;
	// oldValues
	(*inFile) >> dSize;
	oldValues.clear();
	for (i=0; i<dSize; i++)
	{
		(*inFile) >> dValue;
		oldValues.push_back(dValue);
	}
	// sumDResult
	(*inFile) >> sumDResult;
	// sumError
	(*inFile) >> sumError;
	// sumDelta
	(*inFile) >> sumDelta;
	// learningRate
	(*inFile) >> learningRate;
	// deltaValues
	(*inFile) >> dSize;
	deltaValues.clear();
	for (i=0; i<dSize; i++)
	{
		(*inFile) >> dValue;
		deltaValues.push_back(dValue);
	}
	// partialValues
	(*inFile) >> dSize;
	partialValues.clear();
	for (i=0; i<dSize; i++)
	{
		(*inFile) >> dValue;
		partialValues.push_back(dValue);
	}
	// outPartials
	(*inFile) >> dSize;
	outPartials.clear();
	for (i=0; i<dSize; i++)
	{
		(*inFile) >> dValue;
		outPartials.push_back(dValue);
	}
}

void memoryOutput::initializeGate()
{
	int i;  // loop index to load the deques
	int dSize; // size of the deque
	
	// initialize everything in order
	// except the gateWeights
	dSize = gateWeights.size();
	gateWeights.clear();
	for (i=0; i<dSize; i++)
	{
		gateWeights.push_back(0.0);
	}
	// sumResult
	sumResult = 0.0;
	// oldValues
	dSize = oldValues.size();
	oldValues.clear();
	for (i=0; i<dSize; i++)
	{
		oldValues.push_back(0.0);
	}
	// sumDResult
	sumDResult = 0.0;
	// sumError
	sumError = 0.0;
	// sumDelta
	sumDelta = 0.0;
	// learningRate
	learningRate = 0.0;
	// deltaValues
	dSize = deltaValues.size();
	deltaValues.clear();
	for (i=0; i<dSize; i++)
	{
		deltaValues.push_back(0.0);
	}
	// partialValues
	dSize = partialValues.size();
	partialValues.clear();
	for (i=0; i<dSize; i++)
	{
		partialValues.push_back(0.0);
	}
	// outPartials
	dSize = outPartials.size();
	outPartials.clear();
	for (i=0; i<dSize; i++)
	{
		outPartials.push_back(0.0);
	}
}

void inEdge::updateValue()
{
	inputFile >> sumResult;		// read a value from the file into the gate state
	
	cout << gateName << " inEdge updateValue" << endl;
	cout << " sumResult = " << sumResult << endl;
	cout << endl;
}

// rewind the input file
void inEdge::resetGateInput()
{
	inputFile.close();
	inputFile.open(gateName.c_str());
	
	// expect that the number of lines will stay the same.
}

// expose the number of records available to the edge
long inEdge::size()
{
	return (inputEdgeSize);
}

// push delta back to output gate
void outEdge::pushDelta()
{
	double result;
	unsigned int i;
	
	cout << gateName << " outEdge pushDelta " << endl;
	cout << "gateList.size() = " << gateList.size() << endl;
	
	result = 0;
	for(i=0; i<gateList.size(); ++i)
	{
		result = gateWeights[i]*sumDelta;
		gateList[i]->listWeightedDelta(result);
		
		cout << "gateWeights[" << i << "] = " << gateWeights[i] << endl;
		cout << "sumDelta = " << sumDelta << endl;
		cout << "result = " << result << endl;
		cout << "----" << endl;
	}
	cout << endl;
}

// write output value
void outEdge::outputValue()
{
	outputFile << "output sumValue = " << sumResult << endl;
	
	cout << gateName << " outEdge outputValue " << endl;
	cout << " sumResult = " << sumResult << endl;
	cout << endl;
}

// update the weights
void outEdge::updateWeight(vector<double> *weightChangeVector)
{
	unsigned int i;
	double weightChange;
	
	cout << gateName << " outEdge updateWeight " << endl;
	
	for (i=0; i<gateWeights.size(); ++i)
	{
		weightChange = learningRate * sumDelta * oldValues[i];	
		weightChangeVector->push_back(weightChange);
		gateWeights[i] += weightChange;
		
		cout << "learningRate = " << learningRate << endl;
		cout << "sumDelta = " << sumDelta << endl;
		cout << "oldValues[" << i << "] = " << oldValues[i] << endl;
		cout << "gateWeights[" << i << "] = " << gateWeights[i] << endl;
		cout << "----" << endl;
	}
	cout << endl;

}




// connect to a gate with a weight
void memoryBlock::connectGate(gate* aGate, double weight)
{
	cellInput->connectGate(aGate, weight);
}

// return the number of connections
int memoryBlock::numberConnections()
{
	cout << gateName << " memoryBlock numberConnections " << endl;
	
	return(cellInput->numberConnections());
}

// output a list of connectiosn
void memoryBlock::listConnections()
{
	cout << gateName << " memoryBlock listConnections " << endl;
	
	cellInput->listConnections();
}

// special snapshot of the current inputs for memory blocks
void memoryBlock::snapShot()
{
	
	cout << gateName << " memoryBlock snapShot " << endl;
	
	cellInput->snapShot();
	inputGate->snapShot();
	forgetGate->snapShot();
	outputGate->snapShot();
	cellOne->snapShot();
}

// special update value for memory blocks
void memoryBlock::updateValue()
{
	/*
	gGate cellInput; // cell input
	fGate inputGate; // input gate
	fGate forgetGate; // forget gate
	fGate outputGate; // output gate
	memoryCell cellOne; // a single memory cell
	memoryOutput blockOutput; // the block output
	*/
	
	cout << gateName << " memoryBlock updateValue " << endl;
	
	cellInput->updateValue();
	inputGate->updateValue();
	forgetGate->updateValue();
	outputGate->updateValue();
	cellOne->updateValue();
	blockOutput->updateValue();
	
	sumResult = blockOutput->getValue();
}

void memoryBlock::updateDelta()
{
	unsigned int i;
	double result;
	double sumWeightedDeltas;
	
	cout << gateName << " memoryBlock updateDelta " << endl;
	
	// sum up the list of weighted deltas from output units
	result = 0;
	cout << "deltaValues.size() = " << deltaValues.size() << endl;
	for(i=0; i<deltaValues.size(); ++i)
	{
		result += deltaValues[i];
		cout << "deltaValues[" << i << "] = " << deltaValues[i] << endl;
	}
	
	// clear values for next step
	deltaValues.clear();

	sumWeightedDeltas = result;
	cout << "sumWeightedDeltas = " << sumWeightedDeltas << endl;
	
	// multiply that times the cell value
	result = sumWeightedDeltas * cellOne->getValue();
	cout << "cellOne->getValue() = " << cellOne->getValue() << endl;
	cout << "result = " << result << endl;
	
	// pass to output gate for delta computation
	outputGate->setInjectionError(result);
	
	// update the delta in the output gate
	outputGate->updateDelta();
	
	// update the cell error term
	cellError = sumWeightedDeltas * outputGate->getValue();
	cout << "sumWeightedDeltas = " << sumWeightedDeltas << endl;
	cout << "outputGate->getValue() = " << outputGate->getValue() << endl;
	cout << "cellError = " << cellError << endl;
	cout << endl;
	
	// push cell error to gates for weight update
	inputGate->setDelta(cellError);
	forgetGate->setDelta(cellError);
	cellInput->setDelta(cellError);
}

// update weights
void memoryBlock::updateWeight(vector<double> *weightChangeVector)
{
	cout << gateName << " memoryBlock updateWeight " << endl;
	
	outputGate->updateWeight(weightChangeVector);
	inputGate->updateWeight(weightChangeVector);
	forgetGate->updateWeight(weightChangeVector);
	cellInput->updateWeight(weightChangeVector);
}

// get cell value
double memoryBlock::getCellValue()
{
	return(cellOne->getValue());
}

// connect cell gate to a gate with a weight
void memoryBlock::connectCellInputGate(gate* aGate, double weight)
{
	cellInput->connectGate(aGate, weight);
}
	
// return the number of input gate connections
int memoryBlock::numberCellInputConnections()
{
	return(cellInput->numberConnections());
}
	
// output a list of input gate connectiosn
void memoryBlock::listCellInputConnections()
{
	cellInput->listConnections();
}
	
// return the squashed weighted sum at the input gate
double memoryBlock::getCellInputValue()
{
	return(cellInput->getValue());
}

// connect input gate to a gate with a weight
void memoryBlock::connectInputGate(gate* aGate, double weight)
{
	inputGate->connectGate(aGate, weight);
}
	
// return the number of input gate connections
int memoryBlock::numberInputConnections()
{
	return(inputGate->numberConnections());
}
	
// output a list of input gate connectiosn
void memoryBlock::listInputConnections()
{
	inputGate->listConnections();
}
	
// return the squashed weighted sum at the input gate
double memoryBlock::getInputValue()
{
	return(inputGate->getValue());
}

// connect forget gate to a gate with a weight
void memoryBlock::connectForgetGate(gate* aGate, double weight)
{
	forgetGate->connectGate(aGate, weight);
}

// return the number of forget connections
int memoryBlock::numberForgetConnections()
{
	return(forgetGate->numberConnections());
}

// output a list of forget connectiosn
void memoryBlock::listForgetConnections()
{
	forgetGate->listConnections();
}

// return the squashed weighted sum at the forget gate
double memoryBlock::getForgetValue()
{
	return(forgetGate->getValue());
}


// connect output gate to a gate with a weight
void memoryBlock::connectOutputGate(gate* aGate, double weight)
{
	outputGate->connectGate(aGate, weight);
}
	
// return the number of output connections
int memoryBlock::numberOutputConnections()
{
	return(outputGate->numberConnections());
}
	
// output a list of output connectiosn
void memoryBlock::listOutputConnections()
{
	outputGate->listConnections();
}
	
// return the squashed weighted sum at the output gate
double memoryBlock::getOutputValue()
{
	return(outputGate->getValue());
}


void memoryBlock::saveGate(ofstream *outFile)
{
	unsigned int i;  // loop index to dump the deques
	
	// save everything in order
	// gateWeights
	(*outFile) << gateWeights.size() << " ";
	for (i=0; i<gateWeights.size(); i++)
	{
		(*outFile) << gateWeights[i] << " ";
	}
	(*outFile) << endl;
	// sumResult
	(*outFile) << sumResult << endl ;
	// oldValues
	(*outFile) << oldValues.size() << " ";
	for (i=0; i<oldValues.size(); i++)
	{
		(*outFile) << oldValues[i] << " ";
	}
	(*outFile) << endl;
	// sumDResult
	(*outFile) << sumDResult << endl;
	// sumError
	(*outFile) << sumError << endl;
	// sumDelta
	(*outFile) << sumDelta << endl;
	// learningRate
	(*outFile) << learningRate << endl;
	// deltaValues
	(*outFile) << deltaValues.size() << " ";
	for (i=0; i<deltaValues.size(); i++)
	{
		(*outFile) << deltaValues[i] << " ";
	}
	(*outFile) << endl;
	// partialValues
	(*outFile) << partialValues.size() << " ";
	for (i=0; i<partialValues.size(); i++)
	{
		(*outFile) << partialValues[i] << " ";
	}
	// cellError
	(*outFile) << cellError << endl;
	(*outFile) << endl;
	
	cellInput->saveGate(outFile);
	inputGate->saveGate(outFile);
	forgetGate->saveGate(outFile);
	outputGate->saveGate(outFile);
	cellOne->saveGate(outFile);
	blockOutput->saveGate(outFile);
	
}


void memoryBlock::restoreGate(ifstream *inFile)
{
	int i;  // loop index to load the deques
	int dSize; // size of the deque
	double dValue; // temp to hold deque value
	
	// save everything in order
	// gateWeights
	(*inFile) >> dSize;
	gateWeights.clear();
	for (i=0; i<dSize; i++)
	{
		(*inFile) >> dValue;
		gateWeights.push_back(dValue);
	}
	// sumResult
	(*inFile) >> sumResult;
	// oldValues
	(*inFile) >> dSize;
	oldValues.clear();
	for (i=0; i<dSize; i++)
	{
		(*inFile) >> dValue;
		oldValues.push_back(dValue);
	}
	// sumDResult
	(*inFile) >> sumDResult;
	// sumError
	(*inFile) >> sumError;
	// sumDelta
	(*inFile) >> sumDelta;
	// learningRate
	(*inFile) >> learningRate;
	// deltaValues
	(*inFile) >> dSize;
	deltaValues.clear();
	for (i=0; i<dSize; i++)
	{
		(*inFile) >> dValue;
		deltaValues.push_back(dValue);
	}
	// partialValues
	(*inFile) >> dSize;
	partialValues.clear();
	for (i=0; i<dSize; i++)
	{
		(*inFile) >> dValue;
		partialValues.push_back(dValue);
	}
	(*inFile) >> cellError;
	
	
	cellInput->restoreGate(inFile);
	inputGate->restoreGate(inFile);
	forgetGate->restoreGate(inFile);
	outputGate->restoreGate(inFile);
	cellOne->restoreGate(inFile);
	blockOutput->restoreGate(inFile);
	
}

void memoryBlock::initializeGate()
{
	int i;  // loop index to load the deques
	int dSize; // size of the deque
	
	// initialize everything in order
	// except the gateWeights

	// sumResult
	sumResult = 0.0;
	// oldValues
	dSize = oldValues.size();
	oldValues.clear();
	for (i=0; i<dSize; i++)
	{
		oldValues.push_back(0.0);
	}
	// sumDResult
	sumDResult = 0.0;
	// sumError
	sumError = 0.0;
	// sumDelta
	sumDelta = 0.0;
	// learningRate
	learningRate = 1.0;
	// deltaValues
	dSize = deltaValues.size();
	deltaValues.clear();
	for (i=0; i<dSize; i++)
	{
		deltaValues.push_back(0.0);
	}
	// partialValues
	dSize = partialValues.size();
	partialValues.clear();
	for (i=0; i<dSize; i++)
	{
		partialValues.push_back(0.0);
	}
	cellError = 0.0;
	
	
	cellInput->initializeGate();
	inputGate->initializeGate();
	forgetGate->initializeGate();
	outputGate->initializeGate();
	cellOne->initializeGate();
	blockOutput->initializeGate();
	
}




