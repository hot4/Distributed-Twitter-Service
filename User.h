#ifndef __USER_H_
#define __USER_H_

#include <string>
#include <vector>
#include <list>
#include <map>
#include "Tweet.h"
#include "Event.h"

class User {
public:
	/* Constructor */
	/**
	  * @effects Creates a default User object
	  * @returns A new User object
	  */
	User();

	/**
	  * @param userName: The name of the User
	  * @param index: Index value associated with this User in the matrix
	  * @effects Creates a new User object
	  * @modifies userName, index, and cI private fields
	  * @returns A new User object
	  */
	User(std::string userName, int index);

	/**
	  * @param u: User to compare to
	  * @returns true if both Users have the same userName
	  */
	bool operator== (const User &u) const;

	/**
	  * @param u: User to compare to
	  * @returns The alphabetic order of both Users based on userName
	  */
	bool operator< (const User &u) const;

	/**
	  * @param users: A list of Users for this User to follow
	  * @effects Adds all Users to this User's unblockedUsers list and adds a knowledge row within the matrix for this User
	  * @modies unblockedUsers and matrixT private fields
      */
	void follow(std::vector<User> users);

	/**
	  * @param user: User for this User to know about
	  * @effects Adds user to matrixT
	  * @modifies matrixT
	  */
	void addToMatrixT(User user, std::vector<int> knowledge);

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
	  * @param userName: The User who does not want this User to view their tweets
	  * @effects Sets this User's knowledge about being blocked to true based on the userName
	  * @modifies blockedStatus private field
	  */
	void blockView(std::string userName);

	/**
	  * @param userName: The User who does want this User to view their tweets
	  * @effects Sets this User's knowledge about being blocked to false based on the userName
	  * @modifies blockedStatus private field
	  */
	void unblockView(std::string userName);

	/**
	  * @param type: Catergorized to be one the following values {block | unblock| tweet}
	  * @param recipient: Location of where the event is received (i.e. which User received the event)
	  * @param message: The message the node wants to broadcast to other processes
	  * @effects Increments cI counter
      * @effects Updates matrixT direct knowledge of itself; i.e. Ti(i,i)
      * @effects Creates event eR and adds to Li
      * @effects If event type is Tweet, calls sendTweet()
      * @modifies cI, matrixT, and Li private fields
	  */
	void onEvent(int type, std::pair<std::string, int> recipient, std::string message);

	/**
	  * @param matrixT: This User's matrix of direct and indirect knowledge
	  * @param eR: The event that has occurred
	  * @param userName: The recipient of the message
	  * @effects Checks if this User knows that userName knows about event eR has occured
	  * @returns true if this User knows that the userName knows about the event and false otherwise
	  */
	bool hasRecv(std::map<User, std::vector<int> > matrixT, Event eR, std::string userName);

	/**
	  * @param sender: User who sent message
	  * @param tweet: A tweet this User has received from the sending User
	  * @param partialLog: A list of events this User is not aware about from the sending User
	  * @param matrixTk: The sending User's matrix of direct and indirect knowledge
	  * @effects Adds tweet to tweets
	  * @effects Adds all events in the partialLog into Li
	  * @effects Updates direct and indirect knowledge based on sending User's matrix
	  * @modifies tweets, Li, matrixT
	  */
	void onRecv(User sender, Tweet tweet, std::vector<Event> partialLog, std::map<User, std::vector<int> > matrixTk);

	/**
	  * @returns Private field
	  */
	std::string getUserName() const {return userName;}	

	/**
	  * @returns Private field index
	  */
	int getIndex() const {return index;}

	/**
	  * @returns Private field tweets
	  */
	std::vector<Tweet> getTweets() const {return tweets;}

	/**
	  * @returns Private field cI
	  */
	int getcI() const {return cI;}

	/**
	  * @returns Private field matrixT
	  */
	std::map<User, std::vector<int> > getMatrixT() const {return matrixT;}

	/**
	  * @returns Private field Li
	  */
	std::vector<Event> getLog() const {return Li;}

	/**
	  * @returns Private field blockedStatus
	  */
	std::map<std::string, std::pair<bool, bool> > getBlockedStatus() const {return blockedStatus;}

	/**
	  * @returns Private field partialLog
	  */
	std::map<std::string, std::list<Event> > getPartialLog() const {return partialLog;}

	/**
	  * @param userName: User to compare blocked status to
	  * @returns Relationship between this user and other User
	  */
	std::pair<bool, bool> getUserBlockedStatus(std::string userName);

	/* Private Fields */
private: 
	/* The name of the User */
	std::string userName;
	/* Associated with index within matrix*/
	int index;
	
	/* A list of Tweets this User has recevied */
	std::vector<Tweet> tweets;
	
	/* Amount of events that have causaully occurred on this User */
	int cI;
	/* An NxN matrix representing direct and indirect knowledge of other Users */
	/* Key: User */
	/* Value: Pair to hold index of User in matrix and an ordered list of integers that represent the causaully ordered events at that specific User */
	std::map<User, std::vector<int> > matrixT;
	/* Log: An ordered list of events */
	std::vector<Event> Li;
	/* Dictionary: A map of blocked status based on other Users */
	/* Key: User -> userName */
	/* Value: pair.first -> Represents if this User blocked the userName */
	/*        pair.second-> Represents if this User is blocked by the userName */
	std::map<std::string, std::pair<bool, bool> > blockedStatus;
	/* Partial Log: A map of Events some User does not know about */
	/* Key: User -> userName */
	/* Value: The events the current userName does not know about */
	std::map<std::string, std::list<Event> > partialLog;

};	
#endif