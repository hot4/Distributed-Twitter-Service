#include "Event.h"

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
Event::Event(std::string type, User &operation, User &recipient, Tweet &tweet, time_t rawTimeStamp) : type(type), operation(operation), recipient(recipient), tweet(tweet), rawTimeStamp(rawTimeStamp) {}