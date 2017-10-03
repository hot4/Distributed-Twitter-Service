#ifndef __TWEET_H_
#define __TWEET_H_

#include <string>
#include <time.h>

class User;

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
	Tweet(User &user, std::string message, time_t rawTimeStamp);
	
	/**
	  * @returns Private field user
	  */
	User& getUser() const {return user;}

	/**
	  * @returns Private field message
	  */
	std::string getMessage() const {return message;}

	/**
      * @returns Private field rawTimeStamp
	  */
	time_t getRawTimeStamp() const {return rawTimeStamp;}

	/* Private Fields */
private:
	User &user;
	std::string message;
	time_t rawTimeStamp;
};
#endif