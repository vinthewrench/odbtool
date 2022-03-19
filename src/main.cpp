//
//  main.cpp
//  canrecord
//
//  Created by Vincent Moscaritolo on 3/12/22.
//

 

#include "CANbus.hpp"
#include "ODBIIsocket.hpp"

using namespace std;


class GMLAN : public CANbus {
	void rcvFrame(struct can_frame );
};

void GMLAN::rcvFrame(struct can_frame frame ){

	printf("%s\n",hexDump(frame).c_str());

}



int main(int argc, const char * argv[]) {
 
	GMLAN 	 gmlan;
	ODBIIsocket odb;

//	printf("\x1B[2J\x1B[HODB tool \n");
	
	//gmlan.begin("can1");

	odb.begin("can1");
	
	// Mode 1
	OBDIIResponse response = odb.performQuery(OBDIICommands.mode1SupportedPIDs_1_to_20);
	 
	
//	uint8_t odb1[8] = { 01, 11, 00, 00, 00, 00, 00, 00  };
//
//	can1.sendFrame(0x7DF, odb1, 8);
	
	while(true) {
		sleep(2);
	}
 
	return 0;
}
