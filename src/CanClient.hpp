//
//  CanClient.hpp
//  canrecord
//
//  Created by Vincent Moscaritolo on 3/18/22.
//

#ifndef CanClient_hpp
#define CanClient_hpp


#if defined(__APPLE__)
// used for cross compile on osx
#include "can.h"

#else
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#endif

#include <unistd.h>
#include <sys/time.h>

#include <iostream>
#include <thread>			//Needed for std::thread
#include <cstring>			//Needed for memset and string functions
#include <list>
#include <set>


using namespace std;
 

inline  std::string hexDump(struct can_frame frame) {
	
	char      	 lineBuf[80];
	char       	 *p = lineBuf;

	p += sprintf(p, "%03X [%d] ",  frame.can_id, frame.can_dlc);
	
	for (int i = 0; i < frame.can_dlc; i++) p += sprintf(p,"%02X ",frame.data[i]);
	for (int i = 7; i >=  frame.can_dlc ; i--) p += sprintf(p,"   ");
	p += sprintf(p,"  ");
	for (int i = 0; i < frame.can_dlc; i++)  {
		
		uint8_t c = frame.data[i] & 0xFF;
		if (c > ' ' && c < '~')
			*p++ = c ;
		else {
			*p++ = '.';
		}
	}
	*p++ = 0;
	return string(lineBuf);
}


class  CANClientProcessor {
 
public:
	virtual ~CANClientProcessor() {};
	virtual void rcvFrame(struct can_frame ) = 0;
};

class CANClient {

public:
 
	CANClient();
	~CANClient();
	
	bool begin(string port, CANClientProcessor* cb,  int *error = NULL);
	bool setFilter(canid_t can_id, canid_t can_mask, int *error = NULL);
	
	bool sendFrame(canid_t frameID,  uint8_t* data, size_t dataLen);
 
	void stop();
	
private:
	
	void run();

	bool 						_running;	 //Flag for starting and terminating the main loop
	std::thread 			_thread;		 //Internal thread, this is in order to start and stop the thread from
	
	
	CANClientProcessor*  _cb;
	int						_fd;
	fd_set 					_read_fds;

	bool						_isSetup;

};

#endif /* CanClient_hpp */
