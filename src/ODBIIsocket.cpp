//
//  ODBIIsocket.cpp
//  odbtool
//
//  Created by Vincent Moscaritolo on 3/19/22.
//

#include "ODBIIsocket.hpp"

bool ODBIIsocket::begin(const char *ifname,  int * errorOut){

	if( CANbus::begin(ifname, errorOut)){
		
		setFilter(0x7e8, 0x7ff);
		
		return true;
	}
	else return false;;
};

void ODBIIsocket::rcvFrame(struct can_frame frame ){

	printf("%s\n",hexDump(frame).c_str());

}

OBDIIResponse ODBIIsocket::performQuery(OBDIICommand *command){
	
	OBDIIResponse response = { 0 };

	if(!_running) {
		return response;
	}
	
	sendFrame(0x7DF,command->payload, sizeof(command->payload));
	
	
	return response;
};



 
