#ifndef Tweet_h_
#define Tweet_h_

#include "User.h"
#include <time.h> 


class Tweet {
public:
	Tweet(std::string userName, std::string message, time_t timeStamp);
	~Tweet() { 
  }
// ACCESSORS
	std::string getusername();
	std::string getmessage();
	time_t gettimestamp();
private: // REPRESENTATION (member variables)
	std::string userName;
	std::string message;
	time_t timeStamp;
};
#endif