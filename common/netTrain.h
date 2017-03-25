#ifndef NETTRAIN_H_
#define NETTRAIN_H_
/***************************
* trains a network
* expect to be used in conjunction with net operate 
* (do that first, then this, then that, then this...)
*
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

using namespace std;

/// train a network 
/** train a network
 * set pointers to the network and the training data (network)
 * train the network
 */
class NetTrain
{
protected:
	string netTrainName; // class name
	NetContainer *netContainer;  // a network container ptr
	NetContainer *trainContainer; // the training network ptr
	NetMetric netMetric; // a class to accumulate changes
	
public:
	// default constructor
	NetTrain()
	{
		// do nothing
	}
	
	// constructor with a name
	NetTrain(char* aName)
	{
		netTrainName += aName;
	}
	
	// default destructor
	~NetTrain()
	{
		// do nothing
	}
	
	// set the net container pointer
	void setNetContainer(NetContainer *aNetContainer);
	
	// set the training container pointer
	void setTrainContainer(NetContainer *aTrainingContainer);
	
	// train the network
	void train();
	
	// burp the metrics
	void metric();
	
	// reset the inputs (start training sequence again)
	void netInputReset();
};

#endif /*NETTRAIN_H_*/
