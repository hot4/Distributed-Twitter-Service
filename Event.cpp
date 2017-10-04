#include "Event.h"
/**
  * @effects Creates a default Event object
  * @returns A new Event object
  */
Event::Event() {}

/**
  * @param type: Catergorized to be one the following values {block | unblock| tweet}
  * @param node: Location of where the event occurred (i.e. which User caused the event)
  * @param rawTimeStamp: Represents the time at which the User created the tweet with no timezone associated
                         The value returned generally represents the number of seconds since 00:00 hours, 
                         Jan 1, 1970 UTC (i.e., the current unix timestamp).
  * @effects Initializes private fields for unblock and block events
  * @modifies type, node, and rawTimeStamp private fields
  * @retunrs A new Event object
  */
Event::Event(std::string type, std::string node, time_t rawTimeStamp) : type(type), node(node), rawTimeStamp(rawTimeStamp) {}

/**
  * @param type: Catergorized to be one the following values {block | unblock| tweet}
  * @param node: Location of where the event occurred (i.e. which User caused the event)
  * @param recipient: Location of where the event is received (i.e. which User received the event)
  * @param message: The message the node wants to broadcast to other processes
  * @param rawTimeStamp: Represents the time at which the User created the tweet with no timezone associated
                         The value returned generally represents the number of seconds since 00:00 hours, 
                         Jan 1, 1970 UTC (i.e., the current unix timestamp).
  * @effects Initializes private fields for tweet events
  * @modifies type, node, recipient, message, and rawTimeStamp private fields
  * @retunrs A new Event object
  */
Event::Event(std::string type, std::string node, std::string recipient, std::string message, time_t rawTimeStamp) : type(type), node(node), recipient(recipient), message(message), rawTimeStamp(rawTimeStamp) {}

/**
  * @param e: Event to assign values from
  * @returns this Event with t values
  */
Event& Event::operator= (const Event &e) {
  this->type = e.getType();
  this->node = e.getNode();
  if (!e.getRecipient().empty()) this->recipient = e.getRecipient();
  if (!e.getMessage().empty()) this->message = e.getMessage();
  this->rawTimeStamp = e.getRawTimeStamp();
  return *this;
}