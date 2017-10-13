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
	  * @param recipient: Location of where the event is received (i.e. which User received the event)
	  * @param cI: The Lamport TimeStamp of the User after the event executed
	  * @param rawTimeStamp: Represents the time at which the User created the tweet with no timezone associated
	                         The value returned generally represents the number of seconds since 00:00 hours, 
	                         Jan 1, 1970 UTC (i.e., the current unix timestamp).
	  * @modifies type, node, and rawTimeStamp private fields
	  * @retunrs A new Event object
	  */
	Event(int type, std::pair<std::string, int> node, std::pair<std::string, int> recipient, int cI, time_t rawTimeStamp);

	/**
	  * @param type: Catergorized to be one the following values {block | unblock| tweet}
	  * @param node: Location of where the event occurred (i.e. which User caused the event)
	  * @param recipient: Location of where the event is received (i.e. which User received the event)
	  * @param message: The message the node wants to broadcast to other processes
	  * @param cI: The Lamport TimeStamp of the User after the event executed
	  * @param rawTimeStamp: Represents the time at which the User created the tweet with no timezone associated
	                         The value returned generally represents the number of seconds since 00:00 hours, 
	                         Jan 1, 1970 UTC (i.e., the current unix timestamp).
	  * @modifies type, node, recipient, message, and rawTimeStamp private fields
	  * @retunrs A new Event object
	  */
	Event(int type, std::pair<std::string, int> node, std::pair<std::string, int> recipient, std::string message, int cI, time_t rawTimeStamp);

	/**
	  * @param e: Event to assign values from
	  * @returns this Event with t values
	  */
	Event& operator= (const Event &e);

	/**
      * @param e: Event to compare values to
      * @returns true if both events have the same private fields, false otherwise
	  */
	bool operator== (const Event &e);

	/**
	  * @param type: Integer repsentation of a particular type
	  * @returns Translation of Integer to appropriate string value
	  */
	std::string defineType(int type);

	/**
      * @returns Private field type
	  */
	int getType() const {return type;}
	void setType(int t) {type=t;}

	/**
	  * @returns Private field node
	  */
	std::pair<std::string, int> getNode() const {return node;}

	/**
	  * @returns Private field flag;
	  */
	bool isTweet() const {return flag;}
	bool setFlag(bool t) {flag=t;}
	/**
	  * @returns Private field recipient
	  */
	std::pair<std::string, int> getRecipient() const {return recipient;}

	/**
	  * @returns Private field tweet
	  */
	std::string getMessage() const {return message;}

	/**
	  * @returns Private field cI
	  */
	int getcI() const {return cI;}

	/**
	  * @returns Private field rawTimeStamp
	  */
	time_t getRawTimeStamp() const {return rawTimeStamp;}
	void setBorU(std::string a, int b, std::string c, int d, int pcI, int raw) {
		node.first=a;
		node.second=b;
		recipient.first=c;
		recipient.second=d;
		cI=pcI;
		rawTimeStamp=raw;
	}
	void setT(std::string a, int b, std::string m, int pcI, int raw) {
		node.first=a;
		node.second=b;
		message=m;
		cI=pcI;
		rawTimeStamp=raw;
	}
	/* Private Fields */
private:
	/* Operation */
	int type;
	/* Source of Event (userName, index) */
	std::pair<std::string, int> node;
	/* Indicates whether Event is Tweet or not */
	bool flag;
	/* Destination of Event (userName, index) */
	std::pair<std::string, int> recipient;
	/* Contents of Tweet */
	std::string message;
	/* Lamport TimeStamp of source */
	int cI;
	/* TimeStamp */
	time_t rawTimeStamp;
};
#endif