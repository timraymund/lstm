#ifndef NETSAVERESTORE_H_
#define NETSAVERESTORE_H_
/***************************
* saves or restores a network
* including input edges, memory blocks and output edges
*
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

using namespace std;

/// save or restore a network
/** save or restore a network
 * set the file name
 * walk through the net, saving the nodes
 * walk through the net, restoring the nodes
 */
class NetSaveRestore
{
protected:
	string netSaveRestoreName; // instantiated class name	
	ofstream netSaveFile; // save file
	ifstream netRestoreFile; // restore file
	NetContainer *netContainer;  // a network container ptr

public:

	// default constructor
	NetSaveRestore()
	{
		// do nothing
	}
	
	// constructor with a name
	NetSaveRestore(char* aName)
	{
		netSaveRestoreName += aName; // set class file name	
	}
	
	// destructor
	virtual ~NetSaveRestore()
	{
		// do nothing
	}
	
	// set the network container pointer
	void setNetContainer(NetContainer *aNetContainer);
	
	// save a network
	void netSave();
	
	// restore a network
	void netRestore();
	
	// reset the input edge input files
	void netInputReset();
	
	// reset the state of the net
	void netInitializeState();

};
#endif /*NETSAVERESTORE_H_*/
