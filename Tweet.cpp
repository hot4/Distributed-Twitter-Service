#include "Tweet.h"

/**
  * @effects Creates a default Tweet object
  * @returns A new Tweet object
  */
Tweet::Tweet() {}

/**
  * @param userName: Creator of tweet
  * @param message: The contents of the tweet 
  * @param rawTimeStamp: Represents the time at which the user created the tweet with no timezone associated
  * @effects Creates a new Twet objects
  * @modifies user, message, rawTimeStamp private fields
  * @returns A new Tweet object
  */
Tweet::Tweet(std::string userName, std::string message, time_t rawTimeStamp) : userName(userName), message(message), rawTimeStamp(rawTimeStamp) {}

/**
  * @param t: Tweet to assign values from
  * @returns this Tweet with t values
  */
Tweet& Tweet::operator= (const Tweet &t) {
	this->userName = t.getUserName();
	this->message = t.getMessage();
	this->rawTimeStamp = t.getRawTimeStamp();
	return *this;
}