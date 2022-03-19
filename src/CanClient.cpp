//
//  CanClient.cpp
//  canrecord
//
//  Created by Vincent Moscaritolo on 3/18/22.
//

#include "CanClient.hpp"


CANClient::CANClient(){
	_fd = -1;
	_isSetup = false;
	_cb = NULL;
	_running = false;
}

CANClient::~CANClient(){
	
	_fd = -1;
	_isSetup = false;
	_cb = NULL;

}

bool CANClient::begin(string port, CANClientProcessor* cb,  int * errorOut){
 
#if defined(__APPLE__)
	if(errorOut) *errorOut = ENOTSUP;
	return false;
#else
	
	struct ifreq ifr;
	struct sockaddr_can addr;
	
	// create a socket
	_fd = ::socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if(_fd == -1){
		if(errorOut) *errorOut = errno;
		return false;
	}
	
	// Get the index number of the network interface
	strcpy(ifr.ifr_name,  port.c_str() );
	if(::ioctl(_fd, SIOCGIFINDEX, &ifr) == -1){
		if(errorOut) *errorOut = errno;
		return false;
	}
	
	// fill out Interface request structure
	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	
	if (::bind(_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		if(errorOut) *errorOut = errno;
		return false;
	}
	
	_cb = cb;
	_isSetup = true;
	_running = true;

	_thread = std::thread(&CANClient::run, this);

	return true;
#endif

};


bool CANClient::setFilter(canid_t can_id, canid_t can_mask,int * errorOut) {
	
	if(!_isSetup) return false;

	 struct can_filter filter;
	 filter.can_id = can_id;
	 filter.can_mask = can_mask;

	if (::setsockopt(_fd, SOL_CAN_RAW, CAN_RAW_FILTER,
						  &filter, sizeof(struct can_filter)) < 0) {
		if(errorOut) *errorOut = errno;
		return false;
	}
	
	return true;
}

bool CANClient::sendFrame(canid_t frameID,  uint8_t* data, size_t dataLen) {
	if(!_isSetup) return false;

	if(dataLen > 8) return false;
	
	struct can_frame frame;
	memset(&frame, 0, sizeof(frame));

	frame.can_id = frameID;
	frame.can_dlc = dataLen;
	memcpy(frame.data, data, dataLen);
	
	if (write(_fd, &frame,
				 sizeof(struct can_frame)) != sizeof(struct can_frame)) {
		return false;
	}
	
	return true;
}

void CANClient::stop(){
	
	_running = false;
	 
 if (_thread.joinable())
		_thread.join();
	
	if(_fd > -1){
		close(_fd);
	}
	_fd = -1;
	_isSetup = false;
}

void CANClient::run() {

	while(_running){

		/* wait for something to happen on the socket */
		struct timeval selTimeout;
		selTimeout.tv_sec = 0;       /* timeout (secs.) */
		selTimeout.tv_usec = 100;            /* 100 microseconds */
		fd_set readSet;
		FD_ZERO(&readSet);
		FD_SET(_fd, &readSet);
 
		int numReady = select(_fd+1, &readSet, NULL, NULL, &selTimeout);
		if( numReady == -1 ) {
			perror("select");
			_running = false;
		}
		if (FD_ISSET(_fd, &readSet)){
			struct can_frame frame;

			size_t nbytes = read(_fd, &frame, sizeof(struct can_frame));

			if(nbytes > 0){
				if(_cb){
					_cb->rcvFrame(frame);
				}
			}
			if(nbytes == 0){ // shutdown
				_running = false;
			}
		}
	}
	 
	 
	
	// Close all sockets
	if(_fd > -1){
		close(_fd);
	}
	_fd = -1;

}
