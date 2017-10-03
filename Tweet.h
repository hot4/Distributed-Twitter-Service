#ifndef Tweet_h_
#define Tweet_h_

#include "User.h"
#include <time.h> 

class Tweet {
public:
	/* Constructor */
	/**
	  * @param user: Creator of tweet
	  * @param message: The contents of the tweet 
	  * @param rawTimeStamp: Represents the time at which the user created the tweet with no timezone associated
	  * @modifies user, message, rawTimeStamp private fields
	  * @returns A new Tweet object
	  */
	Tweet(User user, std::string message, time_t rawTimeStamp);
	
	/**
	  * @returns Private field user
	  */
	std::string getUser();

	/**
	  * @returns Private field message
	  */
	std::string getMessage();

	/**
      * @returns Private field rawTimeStamp
	  */
	time_t getRawTimeStamp();

	/* Private Fields */
private:
	User user;
	std::string message;
	time_t rawTimeStamp;
};
#endif