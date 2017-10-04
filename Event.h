#ifndef __EVENT_H_
#define __EVENT_H_

#include <string>

class Event {
	/* Constructor */
public:
	/**
	  * @effects Creates a default Event object
	  * @returns A new Event object
	  */
	Event();

	/**
	  * @param type: Catergorized to be one the following values {block | unblock| tweet}
	  * @param node: Location of where the event occurred (i.e. which User caused the event)
	  * @param rawTimeStamp: Represents the time at which the User created the tweet with no timezone associated
	                         The value returned generally represents the number of seconds since 00:00 hours, 
	                         Jan 1, 1970 UTC (i.e., the current unix timestamp).
	  * @modifies type, node, and rawTimeStamp private fields
	  * @retunrs A new Event object
	  */
	Event(std::string type, std::string node, time_t rawTimeStamp);

	/**
	  * @param type: Catergorized to be one the following values {block | unblock| tweet}
	  * @param node: Location of where the event occurred (i.e. which User caused the event)
	  * @param recipient: Location of where the event is received (i.e. which User received the event)
	  * @param message: The message the node wants to broadcast to other processes
	  * @param rawTimeStamp: Represents the time at which the User created the tweet with no timezone associated
	                         The value returned generally represents the number of seconds since 00:00 hours, 
	                         Jan 1, 1970 UTC (i.e., the current unix timestamp).
	  * @modifies type, node, recipient, message, and rawTimeStamp private fields
	  * @retunrs A new Event object
	  */
	Event(std::string type, std::string node, std::string recipient, std::string message, time_t rawTimeStamp);

	/**
	  * @param e: Event to assign values from
	  * @returns this Event with t values
	  */
	Event& operator= (const Event &e);

	/**
      * @returns Private field type
	  */
	std::string getType() const {return type;}

	/**
	  * @returns Private field node
	  */
	std::string getNode() const {return node;}

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
	std::string node;
	std::string recipient;
	std::string message;
	time_t rawTimeStamp;
};
#endif