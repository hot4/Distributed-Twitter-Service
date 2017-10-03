#ifndef __EVENT_H_
#define __EVENT_H_

#include <string>

class User;
class Tweet;

class Event {
	/* Constructor */
	/**
	  * @param type: Catergorized to be one the following values {block | unblock| tweet}
	  * @param operation: Location of where the event occurred (i.e. which User caused the event)
	  * @param recipient: Location of where the event is received (i.e. which User received the event)
	  * @param tweet: The message the operation wants to broadcast to other processes
	  * @param rawTimeStamp: Represents the time at which the User created the tweet with no timezone associated
	                         The value returned generally represents the number of seconds since 00:00 hours, 
	                         Jan 1, 1970 UTC (i.e., the current unix timestamp).
	  * @modifies type, operation, recipient, tweet, and rawTimeStamp private fields
	  * @retunrs A new Event object
	  */
	Event(std::string type, User &operation, User &recipient, Tweet &tweet, time_t rawTimeStamp);

	/**
      * @returns Private field type
	  */
	std::string getType();

	/**
	  * @returns Private field operation
	  */
	User& getOperation();

	/**
	  * @returns Private field recipient
	  */
	User& getRecipient();

	/**
	  * @returns Private field tweet
	  */
	Tweet& getTweet();

	/**
	  * @returns Private field rawTimeStamp
	  */
	time_t getRawTimeStamp();

	/* Private Fields */
private:
	std::string type;
	User &operation;
	User &recipient;
	Tweet &tweet;
	time_t rawTimeStamp;
};
#endif