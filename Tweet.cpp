#include "Tweet.h"

/**
  * @param user: Creator of tweet
  * @param message: The contents of the tweet 
  * @param rawTimeStamp: Represents the time at which the user created the tweet with no timezone associated
  * @modifies user, message, rawTimeStamp private fields
  * @returns A new Tweet object
  */
Tweet(User user, std::string message, time_t rawTimeStamp) : user(user), message(message), rawTimeStamp(rawTimeStamp) {}