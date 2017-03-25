/**
 * trains a network
 */

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include "netMetric.h"

using namespace std;


/** compute injection error RMS
 * compute injection error RMS
 * walk through injection error vector
 * square each value, add it to the sum and count it
 * divide by the count
 * take the square root
 */
void NetMetric::injectionErrorRMS()
{
	unsigned int i;						// index
	double injectionErrorSum;   // sum of squared values
	double injectionErrorCount;  // number of values
	
	injectionErrorSum = 0.0;
	injectionErrorCount = 0.0;
	for(i=0; i<injectionErrorVector->size(); ++i)
	{
		injectionErrorSum += (*injectionErrorVector)[i] * (*injectionErrorVector)[i];
	}
	
	injectionErrorCount = (double)(injectionErrorVector->size());
	
	injectionErrorSum = injectionErrorSum / injectionErrorCount;
	
	IERMS = sqrt(injectionErrorSum);
}

/** compute weight change RMS
 * compute weight change RMS
 * walk through weight change vector
 * square each value, add it to the sum and count it
 * divide by the count
 * take the square root
 */
void NetMetric::weightChangeRMS()
{
	unsigned int i;						// index
	double weightChangeSum;		// sum of squared values
	double weightChangeCount;	// number of values
	
	weightChangeSum = 0.0;
	weightChangeCount = 0.0;
	for(i=0; i<weightChangeVector->size(); ++i)
	{
		weightChangeSum += (*weightChangeVector)[i] * (*weightChangeVector)[i];
	}
	
	weightChangeCount = (double)(weightChangeVector->size());
	
	weightChangeSum = weightChangeSum / weightChangeCount;
	
	WCRMS = sqrt(weightChangeSum);
}

/** write the results and clear the vectors
 * write the iteration count, IERMS and WCRMS to a file
 * clear the injection error vector
 * clear the weight change vector
 * increment the counter
 */
void NetMetric::writeClear()
{
	this->injectionErrorRMS();
	this->weightChangeRMS();
	
	netMetricFile << iterationCount << ", " << IERMS << ", " << WCRMS << endl;
	
	injectionErrorVector->clear();
	weightChangeVector->clear();
	
	++iterationCount;
}
