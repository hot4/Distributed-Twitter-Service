#ifndef __TWEET_H_
#define __TWEET_H_

#include <string>
#include <time.h>

class Tweet {
public:
	/* Constructor */
	/**
	  * @effects Creates a default Tweet object
	  * @returns A new Tweet object
	  */
	Tweet();
	
	/**
	  * @param userName: Creator of tweet
	  * @param user: Creator of tweet
	  * @param message: The contents of the tweet 
	  * @param rawTimeStamp: Represents the time at which the User created the tweet with no timezone associated
	                         The value returned generally represents the number of seconds since 00:00 hours, 
	                         Jan 1, 1970 UTC (i.e., the current unix timestamp).
    * @effects Creates a new Tweet object	                         
	  * @modifies user, message, rawTimeStamp private fields
	  * @returns A new Tweet object
	  */
	Tweet(std::string userName, std::string message, time_t rawTimeStamp);

	/**
	  * @param t: Tweet to assign values from
	  * @returns this Tweet with t values
	  */
	Tweet& operator= (const Tweet &t);

	/**
	  * @returns Private field user
	  */
	std::string getUserName() const {return userName;}

	/**
	  * @returns Private field message
	  */
	std::string getMessage() const {return message;}

	/**
    * @returns Private field rawTimeStamp
	  */
	const time_t& getRawTimeStamp() const {return rawTimeStamp;}

	/* Private Fields */
private:
	std::string userName;
	std::string message;
	time_t rawTimeStamp;
};
#endif