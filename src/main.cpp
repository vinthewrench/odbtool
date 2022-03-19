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

	printf("%s\n",hexDumpFrame(frame).c_str());

}



int main(int argc, const char * argv[]) {
 
	GMLAN 	 gmlan;
	ODBIIsocket odb;

//	printf("\x1B[2J\x1B[HODB tool \n");
	
	//gmlan.begin("can1");

	odb.begin("can1", 0x7df, 0x7e8);
	
	// Mode 1
//	OBDIIResponse response = odb.performQuery(OBDIICommands.mode1SupportedPIDs_1_to_20);
	OBDIICommandSet supportedCommands = odb.getSupportedCommands();

	
	for (int i = 0; i < supportedCommands.numCommands; ++i) {
		OBDIICommand *command = supportedCommands.commands[i];
		printf("%i: mode %02x, PID %02x: %s\n", i, OBDIICommandGetMode(command), OBDIICommandGetPID(command), command->name);
	}

	odb.stop();
 
	return 0;
}
