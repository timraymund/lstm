#ifndef NETOPERATE_H_
#define NETOPERATE_H_
/***************************
* operates a network
* including input edges, memory blocks and output edges
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

using namespace std;

/// operate a network
/** operate a network
 * set the file name
 * walk through the net, operating the nodes
 * input edges, memory blocks, output edges
 */
class NetOperate
{
protected:
	string netOperateName; // instantiated class name
	NetContainer *netContainer;  // a network container ptr
	
public:
	
	// default constructor
	NetOperate()
	{
		// do nothing
	}
	
	// constructor with a name
	NetOperate(char* aName)
	{
		netOperateName += aName;
	}
	
	// default destructor
	~NetOperate()
	{
		// do nothing
	}
	
	// set the network container pointer
	void setNetContainer(NetContainer *aNetContainer);
	
	// operate the network
	void operate();
	
};
#endif /*NETOPERATE_H_*/

