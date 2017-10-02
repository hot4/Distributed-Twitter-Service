#ifndef __User_h_
#define __User_h_

#include <string>
#include <vector>
#include <list>
#include <map>
#include "Tweet.h"
#include "Event.h"

class User {
	/* Constructor */
	/**
	  * @param userName: The name of the User
	  * @param index: Index value associated with this User in the matrix
	  * @effects Creates a new User object
	  * @modifies userName, index, and cI private fields
	  * @returns A new User object
	  */
	User(std::string userName, int index);

	/**
	  * @param users: A list of Users for this User to follow
      */
	void follow(std::vector<User> users);

	/**
	  * @param tweet: A tweet the User will broadcast to other Users
	  * @param Ti: User's matrix to help update recipient User to update direct and indirect knowledge
      * @effects Creates a partial Li of events recipient User does not know about
        @effects Sends tweet, Ti, and partial Li to all receipients who are not blocked by this User
	  */
	void sendTweet(Tweet tweet, std::map<User, std::vector<int> > matrixT);

	/**
	  * @effects Displays an ordered list of tweets from all User's this User is not blocked from seeing
	  */
	void view();

	/**
	  * @param userName: The User this User wishes to block
	  * @effects Removes userName from unblockedUsers list and adds to blockedUsers list
	  * @modifies unblockedUser and blockedUsers private fields
	  */
	void block(std::string userName);

	/**
	  * @param userName: The User, this User wishes to unblock
	  * @effects Removes userName from blockedUsers list and adds to unblockedUsers list
	  * @modifies blockedUsers and unblockedUsers private fields
	  */
	void unblock(std::string userName);

	/**
	  * @effects Increments cI counter
      * @effects Updates matrixT direct knowledge of itself; i.e. Ti(i,i)
      * @effects Creates event eR and adds to Li
      * @effects If event type is Tweet, calls sendTweet()
      * @modifies cI, matrixT, and Li private fields
	  */
	void onEvent();

	/**
	  * @param matrixT: This User's matrix of direct and indirect knowledge
	  * @param eR: The event that has occurred
	  * @param user: The recipient of the message
	  * @effects Checks if this User knows that userName knows about event eR has occured
	  * @returns true if this User knows that the userName knows about the event and false otherwise
	  */
	bool hasRecv(std::map<User, std::vector<int> > matrixT, Event eR, User user);

	/**
	  * @param tweet: A tweet this User has received from the sending User
	  * @param partialLog: A list of events this User is not aware about from the sending User
	  * @param matrixTk: The sending User's matrix of direct and indirect knowledge
	  * @effects Adds tweet to tweets
	  * @effects Adds all events in the partialLog into Li
	  * @effects Updates direct and indirect knowledge based on sending User's matrix
	  * @modifies tweets, Li, matrixT
	  */
	void onRecv(std::vector<Tweet> tweetRecv, std::vector<Event> partialLog, std::map<User, std::vector<int> > matrixTk);

	/**
	  * @returns Private field
	  */
	std::string getUserName() const {return userName;}	

	/**
	  * @returns Private field index
	  */
	int getIndex() const {return index;}

	/**
	  * @returns Private field blockedUsers
	  */
	std::list<User> getBlockedUsers() const {return blockedUsers;}

	/**
	  * @returns Private field unblockedUsers
	  */
	std::list<User> getUnblockedUsers() const {return unblockedUsers;}

	/**
	  * @returns Private field tweets
	  */
	std::vector<Tweet> getTweets() const {return tweets;}

	/**
	  * @returns Private field matrixT
	  */
	std::map<User, std::vector<int> > getMatrixT() const {return matrixT;}

	/**
	  * @returns Private field Li
	  */
	std::vector<Event> getLog() const {return Li;}

	/**
	  * @param user: User for this User to know about
	  * @effects Adds user to matrixT
	  * @modifies matrixT
	  */
	void addToMatrixT(User user);

	/* Private Fields*/
private: 
	/* The name of the User */
	std::string userName;
	/* Associated with index within matrix*/
	int index;
	/* ADD timeZone after figuring out how to get current machine's time zone */
	
	/* A list of Users this User has blocked */
	std::list<User> blockedUsers;
	/* A list of Users this User has not blocked */
	std::list<User> unblockedUsers;
	/* A list of Users this User cannot view tweets */
	std::list<User> blockedView;
	/* A list of Tweets this User has recevied */
	std::vector<Tweet> tweets;
	
	/* Amount of events that have causaully occurred on this User */
	int cI;
	/* An NxN matrix representing direct and indirect knowledge of other Users */
	/* Key: User */
	/* Value: Pair to hold index of User in matrix and an ordered list of integers that represent the causaully ordered events at that specific User */
	std::map<User, std::vector<int> > matrixT;
	/* An ordered list of events */
	std::vector<Event> Li;

};	

#endif