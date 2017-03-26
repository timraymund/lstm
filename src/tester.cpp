/** netDefine test program 
 * 
 * tdr 2007.12.12
 * $Id: tester.cpp,v 1.13 2008/10/17 05:51:37 fatshark Exp $
 */

#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include <deque>
#include <vector>
#include <iterator>
#include <string>

#include "netContainer.h"
#include "netDefine.h"
#include "netSaveRestore.h"
#include "netOperate.h"
#include "netTrain.h"

using namespace std;

int main(int argc, char* argv[])
{
	NetContainer netContainer;
	NetContainer trainContainer;
	NetSaveRestore netSaveRestore("here.dat");
	NetOperate netOperate;
	NetTrain netTrain;
	
	cout << "Starting the tester." << endl;
	
	cout << "Declaring netDefine." << endl;
	NetDefine netDefine("simple.cfg");
	
	cout << "Passing the network pointer." << endl;
	netDefine.setNetContainer(&netContainer);
		
	cout << "About to parse the configuration file." << endl;
	netDefine.parseConfigurationFile();
	
	cout << "About to show the net." << endl;
	netDefine.listNet();
	
	cout << "About to check the net." << endl;
	netDefine.checkNet();
	
	cout << "Declaring trainDefine." << endl;
	NetDefine trainDefine("train.cfg");
	
	cout << "Passing the training network pointer." << endl;
	trainDefine.setNetContainer(&trainContainer);
	
	cout << "About to parse the training configuration file." << endl;
	trainDefine.parseConfigurationFile();
	
	cout << "About to set operate network pointer." << endl;
	netOperate.setNetContainer(&netContainer);
	
	cout << "About to set training network pointers." << endl;
	netTrain.setNetContainer(&netContainer);
	netTrain.setTrainContainer(&trainContainer);
	
	cout << "About to do four steps of operate and train with metrics." << endl;
	netOperate.operate();
	netTrain.train();
	netTrain.metric();
	
	netOperate.operate();
	netTrain.train();
	netTrain.metric();
	
	netOperate.operate();
	netTrain.train();
	netTrain.metric();
	
	netOperate.operate();
	netTrain.train();
	netTrain.metric();
	
	cout << "About to pass the network pointer to save restore." << endl;
	netSaveRestore.setNetContainer(&netContainer);
	
	cout << "About to save the network." << endl;
	netSaveRestore.netSave();
	
	cout << "About to restore the network." << endl;
	netSaveRestore.netRestore();
	
	cout << "About to reset the input edges." << endl;
	netSaveRestore.netInputReset();
	
	cout << "About to reset the training sequence." << endl;
	netTrain.netInputReset();
	
	cout << "About to re-initialize the network." << endl;
	netSaveRestore.netInitializeState();
	
	cout << "About to do many steps of operate and train with metrics." << endl;
	long i, j;

	for (i = 0; i < 100; ++i)
	{
		for (j = 0; j < netContainer.netInputSize; ++j)
		{
			netOperate.operate();
			netTrain.train();
			netTrain.metric();
		}
		netSaveRestore.netInputReset();			// goto beginning of input files
		netSaveRestore.netInitializeState();	// initialize states except weights
		
		netTrain.netInputReset();				// reset training sequence
	}

	cout << "About to save the network. " << endl;
	netSaveRestore.netSave();

	return 0;
}
