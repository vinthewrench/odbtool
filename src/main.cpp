//
//  main.cpp
//  canrecord
//
//  Created by Vincent Moscaritolo on 3/12/22.
//

 

#include "CanClient.hpp"

using namespace std;


class GMLANlient : public CANClientProcessor {
	void rcvFrame(struct can_frame );
};

void GMLANlient::rcvFrame(struct can_frame frame ){

	printf("%s\n",hexDump(frame).c_str());

}



int main(int argc, const char * argv[]) {
 
	GMLANlient  gmlan;
	CANClient 	can1;

//	printf("\x1B[2J\x1B[HODB tool \n");
	
	can1.begin("can1", &gmlan);

	uint8_t odb1[8] = { 01, 11, 00, 00, 00, 00, 00, 00  };
	
	can1.sendFrame(0x7DF, odb1, 8);
	
	while(true) {
		sleep(2);
	}
 
	return 0;
}
