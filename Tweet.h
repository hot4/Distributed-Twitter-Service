#ifndef __TWEET_H_
#define __TWEET_H_

#include <string>
#include <time.h>
#include "User.h"

class Tweet {
	/* Constructor */
	/**
	  * @param user: Creator of tweet
	  * @param message: The contents of the tweet 
	  * @param rawTimeStamp: Represents the time at which the user created the tweet with no timezone associated
	  * @modifies user, message, rawTimeStamp private fields
	  * @returns A new Tweet object
	  */
	Tweet(User user, std::string message, time_t rawTimeStamp);
	
	/**
	  * @returns Private field user
	  */
	std::string getUser() const {return user;}

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
	User user;
	std::string message;
	time_t rawTimeStamp;
};

#endif

/*
	std::vector<time_t> times;

	char buffer [80];

	time_t rawtime;
	struct tm * timeinfo;
	
	times.push_back(time (&rawtime));
	timeinfo = localtime (&rawtime);
	// std::cout << rawtime + "\n";

	// time_t utcraw;
	struct tm * ptm;

	strftime (buffer,80,"Now it's %I:%M%p %z %Z.",timeinfo);
	puts (buffer);

	Need to call after strftime
	// time (&utcraw);
	ptm = gmtime ( &rawtime );
	
	strftime (buffer,80,"Now it's %I:%M%p %z %Z.",ptm);
	puts (buffer);

	std::sort(times.begin(), times.end());
	for (unsigned int i = 0; i < times.size(); i++) {
		// strftime(buffer, 80, "Yay! It's %I:%M%p %z %Z.", localtime(times[i]));
		strftime (buffer,80,"Now it's %I:%M%p %z %Z.", localtime(&times[i]));
		puts(buffer);
		strftime (buffer,80,"Now it's %I:%M%p %z %Z.", gmtime(&times[i]));
		puts(buffer);
	}

	std::cout << "Hello World!";
*/