#ifndef NETMETRIC_H_
#define NETMETRIC_H_
/***************************
* metrics a network
* expect to be used in net train
*
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

/// metric a network 
/** metric a network
 * provide pointer to accumulation of injection errors
 * provide pointer to accumulation of weight changes
 * compute root of the summed squares of the accumulated weight
 * save the results to an indexed file (gnuplot-able)
 * train the network
 */
class NetMetric
{
protected:
	string netMetricName;		// class name
	int iterationCount;			// index for output metrics
	ofstream netMetricFile;		// a file for the results
	double IERMS;				// injection error RMS value
	double WCRMS;				// weight change RMS value


public:
	vector<double> *weightChangeVector;  // vector pointer for the weight changes
	vector<double> *injectionErrorVector;  // vector pointer for the injection errors
	
	// default constructor with a default name
	NetMetric()
	{
		// do nothing
		weightChangeVector = new vector<double>;  // put a vector behind the pointer
		injectionErrorVector = new vector<double>;  // put a vector behind the pointer
		iterationCount = 1;  // initialize the metric index to 1
		
		netMetricName += "metric.dat";   // set the default name
		netMetricFile.open("metric.dat");   // open the output file
	}
	
	// default constructor with a name
	NetMetric(char* aName)
	{
		// do nothing
		weightChangeVector = new vector<double>;  // put a vector behind the pointer
		injectionErrorVector = new vector<double>;  // put a vector behind the pointer
		iterationCount = 1;  // initialize the metric index to 1
		
		netMetricName += aName;    // set the name
		netMetricFile.open(aName);  // open the output file
	}
	
	// defaut destructor
	virtual ~NetMetric()
	{
		// do nothing
		netMetricFile.close();
	}

	// compute the root mean squared weight change
	void weightChangeRMS();
	
	// compute the root mean squared injection error
	void injectionErrorRMS();
	
	// write out current metrics and clear vectors
	void writeClear();
	
};

#endif /*NETMETRIC_H_*/
