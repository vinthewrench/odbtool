//
//  ODBIIsocket.hpp
//  odbtool
//
//  Created by Vincent Moscaritolo on 3/19/22.
//

#ifndef ODBIIsocket_hpp
#define ODBIIsocket_hpp

#include "CANbus.hpp"
#include "OBDII.h"

using namespace std;


class ODBIIsocket : public CANbus {
	
public:
	bool begin(const char *ifname,  int * errorOut = NULL);

	OBDIIResponse performQuery(OBDIICommand *command);
	
private:
	void rcvFrame(struct can_frame );
	
	
};


#endif /* ODBIIsocket_hpp */
