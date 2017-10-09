#include "Event.h"

/**
  * @effects Creates a default Event object
  * @returns A new Event object
  */
Event::Event() {}

/**
  * @param type: Catergorized to be one the following values {block | unblock| tweet}
  * @param node: Location of where the event occurred (i.e. which User caused the event)
  * @param recipient: Location of where the event is received (i.e. which User received the event)
  * @param cI: The Lamport TimeStamp of the User after the event executed
  * @param rawTimeStamp: Represents the time at which the User created the tweet with no timezone associated
                         The value returned generally represents the number of seconds since 00:00 hours, 
                         Jan 1, 1970 UTC (i.e., the current unix timestamp).
  * @effects Initializes private fields for unblock and block events
  * @modifies type, node, and rawTimeStamp private fields
  * @retunrs A new Event object
  */
Event::Event(int type, std::pair<std::string, int> node, std::pair<std::string, int> recipient, int cI, time_t rawTimeStamp) : type(type), node(node), flag(false), cI(cI), rawTimeStamp(rawTimeStamp) {}

/**
  * @param type: Catergorized to be one the following values {block | unblock| tweet}
  * @param node: Location of where the event occurred (i.e. which User caused the event)
  * @param recipient: Location of where the event is received (i.e. which User received the event)
  * @param message: The message the node wants to broadcast to other processes
  * @param cI: The Lamport TimeStamp of the User after the event executed
  * @param rawTimeStamp: Represents the time at which the User created the tweet with no timezone associated
                         The value returned generally represents the number of seconds since 00:00 hours, 
                         Jan 1, 1970 UTC (i.e., the current unix timestamp).
  * @effects Initializes private fields for tweet events
  * @modifies type, node, recipient, message, and rawTimeStamp private fields
  * @retunrs A new Event object
  */
Event::Event(int type, std::pair<std::string, int> node, std::pair<std::string, int> recipient, std::string message, int cI, time_t rawTimeStamp) : type(type), node(node), flag(true), recipient(recipient), message(message), cI(cI), rawTimeStamp(rawTimeStamp) {}

/**
  * @param e: Event to assign values from
  * @returns this Event with t values
  */
Event& Event::operator= (const Event &e) {
  this->type = e.getType();
  this->node = e.getNode();
  this->flag = e.isTweet();
  if (this->isTweet()) {
    this->recipient = e.getRecipient();
    this->message = e.getMessage();
  }
  this->rawTimeStamp = e.getRawTimeStamp();
  return *this;
}

/**
  * @param e: Event to compare values to
  * @returns true if both events have the same private fields, false otherwise
  */
bool Event::operator== (const Event &e) {
  return (this->getType() == e.getType()) && 
         (this->getNode().first == e.getNode().first) && (this->getNode().second == e.getNode().second) &&
         (this->isTweet() == e.isTweet()) && 
         (this->getRecipient().first == e.getRecipient().first) && (this->getRecipient().second == e.getRecipient().second) &&
         (this->getMessage() == e.getMessage()) && 
         (this->getcI() == e.getcI()) && 
         (this->getRawTimeStamp() == e.getRawTimeStamp());
}

/**
  * @param type: Integer repsentation of a particular type
  * @returns Translation of Integer to appropriate string value
  */
std::string Event::defineType(int type) {
  switch (1) {
    /* Map value of 1 with Tweet */
    case 1: 
      return "Tweet";
    /* Map value of 2 with Block */
    case 2: 
      return "Block";
    /* Map value of 3 with Unblock */
    case 3: 
      return "Unblock";
  }
}
