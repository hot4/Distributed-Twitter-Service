#ifndef __EVENT_H_
#define __EVENT_H_

#include <string>

class Event {
	/* Constructor */
	/**
	  * @param type: Catergorized to be one the following values {block | unblock| tweet}
	  * @param operation: Location of where the event occurred (i.e. which User caused the event)
	  * @param rawTimeStamp: Represents the time at which the User created the tweet with no timezone associated
	                         The value returned generally represents the number of seconds since 00:00 hours, 
	                         Jan 1, 1970 UTC (i.e., the current unix timestamp).
	  * @modifies type, operation, and rawTimeStamp private fields
	  * @retunrs A new Event object
	  */
	Event(std::string type, std::string operation, time_t rawTimeStamp);

	/**
	  * @param type: Catergorized to be one the following values {block | unblock| tweet}
	  * @param operation: Location of where the event occurred (i.e. which User caused the event)
	  * @param recipient: Location of where the event is received (i.e. which User received the event)
	  * @param message: The message the operation wants to broadcast to other processes
	  * @param rawTimeStamp: Represents the time at which the User created the tweet with no timezone associated
	                         The value returned generally represents the number of seconds since 00:00 hours, 
	                         Jan 1, 1970 UTC (i.e., the current unix timestamp).
	  * @modifies type, operation, recipient, message, and rawTimeStamp private fields
	  * @retunrs A new Event object
	  */
	Event(std::string type, std::string operation, std::string recipient, std::string message, time_t rawTimeStamp);

	/**
      * @returns Private field type
	  */
	std::string getType() const {return type;}

	/**
	  * @returns Private field operation
	  */
	std::string getOperation() const {return operation;}

	/**
	  * @returns Private field recipient
	  */
	std::string getRecipient() const {return recipient;}

	/**
	  * @returns Private field tweet
	  */
	std::string getMessage() const {return message;}

	/**
	  * @returns Private field rawTimeStamp
	  */
	time_t getRawTimeStamp() const {return rawTimeStamp;}

	/* Private Fields */
private:
	std::string type;
	std::string operation;
	std::string recipient;
	std::string message;
	time_t rawTimeStamp;
};
#endif