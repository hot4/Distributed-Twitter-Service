#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

#include "User.h"

/**
  * @param userName: The name of the User
  * @param index: Index value associated with this User in the matrix
  * @effects Creates a new User object
  * @modifies Private fields
  * @returns A new User object
  */
User::User(std::string userName, int index) : userName(userName), index(index), cI(0) {}

/**
  * @param u: User to compare to
  * @returns true if both Users have the same userName
  */
bool User::operator== (const User &u) const {
	return this->getUserName() == u.getUserName();
}

/**
  * @param u: User to compare to
  * @returns The alphabetic order of both Users based on userName
  */
bool User::operator< (const User &u) const {
	return (this->getUserName()).compare(u.getUserName());
}

/**
  * @param users: A list of Users for this User to follow
  * @effects Adds all Users to this User's unblockedUsers list and adds a knowledge row within the matrix for this User
  * @modifies unblockedUsers and matrixT private fields
  */
void User::follow(std::vector<User> users) {
	for (unsigned int i = 0; i < users.size(); i++) {
		User user = users[i];
		if (this->getIndex() == user.getIndex()) continue;
		this->unblock(user.getUserName());
		std::vector<int> knowledge(users.size(), 0);
		this->addToMatrixT(user, knowledge);
	}
}

/**
  * @param user: User for this User to know about
  * @effects Adds user to matrixT
  * @modifies matrixT
  */
void User::addToMatrixT(User user, std::vector<int> knowledge) {
	(this->matrixT).insert(std::pair<User, std::vector<int> >(user, knowledge));	
}

/**
  * @param tweet: A tweet the User will broadcast to other Users
  * @param Ti: User's matrix to help update recipient User to update direct and indirect knowledge
  * @effects Creates a partial Li of events recipient User does not know about
    @effects Sends tweet, Ti, and partial Li to all receipients who are not blocked by this User
  */
void User::sendTweet(Tweet tweet, std::map<User, std::vector<int> > matrixT) {
	/* Add tweet to tweets */
	(this->tweets).push_back(tweet);

	/* Temporoary placeholders */
	std::list<User> followers = this->getUnblockedUsers();
	std::vector<Event> currLog = this->getLog();

	/* Container for Events to send to recipient */
	std::vector<Event> partialLog;

	/* Iterator for unblockedUsers */
	std::list<User>::iterator itr = followers.begin();

	/* Loop through all unblockedUsers to send the tweet */
	while (itr != followers.end()) {
		/* Get current follower */
		User currUser = *itr;
		/* Clear partial Li */
		partialLog.clear();

		/* Loop through this User's Li and create a list of Events the current unblockedUser is not aware about */
		for (unsigned int j = 0; j < currLog.size(); j++) {
			Event event = currLog[j];
			/* Add event to partial Li */
			if (this->hasRecv(matrixT, event, currUser)) {
				partialLog.push_back(event);
			}
		}

		/* TODO: Broadcast message to current follower */
	}
}

/**
  * @effects Displays an ordered list of tweets from all User's this User is not blocked from seeing
  */
void User::view() {
	std::vector<Tweet> tweets = this->getTweets();
	int buffSize = 80;

	for (unsigned int i = 0; i < tweets.size(); i++) {
		Tweet tweet = tweets[i];
		/* Print userName */
		printf("User: %s\n", tweet.getUserName().c_str());
		/* Print message */
		printf("Message: %s\n", tweet.getMessage().c_str());
		/* Print local timestamp */
		char buffer [buffSize];
		strftime(buffer, buffSize, "%I:%M%p.\n\n", localtime(&(tweet.getRawTimeStamp())));
		puts(buffer);
	}
}

/**
  * @param userName: The User this User wishes to block
  * @effects Removes userName from unblockedUsers list and adds to blockedUsers list
  * @modifies unblockedUser and blockedUsers private fields
  */
void User::block(std::string userName) {
	/* Temporary placeholder */
	std::list<User> unblockedUsers = this->unblockedUsers;

	/* Loop through blocked list to find userName */
	for (std::list<User>::iterator itr = unblockedUsers.begin(); itr != unblockedUsers.end(); itr++) {
		/* Get current user */
		User user = *itr;
		/* Found User in list */
		if (user.getUserName() == userName) {
			/* Remove User from unblocked list */
			(this->unblockedUsers).remove(user);
			/* Add User to blocked list */
			(this->blockedUsers).push_back(user);
			return;
		}
	}
}

/**
  * @param userName: The User, this User wishes to unblock
  * @effects Removes userName from blockedUsers list and adds to unblockedUsers list
  * @modifies blockedUsers and unblockedUsers private fields
  */
void User::unblock(std::string userName) {
	/* Temporary placeholder */
	std::list<User> blockedUsers = this->blockedUsers;

	/* Loop through blocked list to find userName */
	for (std::list<User>::iterator itr = blockedUsers.begin(); itr != blockedUsers.end(); itr++) {
		/* Get current user */
		User user = *itr;
		/* Found User in list */
		if (user.getUserName() == userName) {
			/* Remove User from blocked list */
			(this->blockedUsers).remove(user);
			/* Add User to unblockedList */
			(this->unblockedUsers).push_back(user);
			return;
		}
	}
}

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
void User::onEvent(int type, std::pair<std::string, int> recipient, std::string message) {
	/* Get current time */
	time_t rawTimeStamp;
	time(&rawTimeStamp);

	/* Increment this User's counter */
	(this->cI)++;

	/* Get value based on key and then access index to increment this User's Ti(i,i) */
	((this->matrixT)[*this])[this->getIndex()]++;

	/* Temporary placeholders */
	Event event;
	Tweet tweet;
	std::string userName = this->getUserName();
	std::pair<std::string, int> node(userName, this->getIndex());

	/* Call specific action for event type */
	switch (type) {
		/* Create tweet object and tweet event and trigger event */
		case 1: 
			tweet = Tweet(userName, message, rawTimeStamp);
			event = Event(type, node, recipient, message, cI, rawTimeStamp);
			this->sendTweet(tweet, (this->getMatrixT()));
			break;
		/* Create block event and trigger event */
		case 2:
			event = Event(type, node, recipient, cI, rawTimeStamp);
			this->block(recipient.first);
			break;
		/* Create unblock event and trigger event */
		case 3:
			event = Event(type, node, recipient, cI, rawTimeStamp);
			this->unblock(recipient.first);
			break;
	}
	
	/* Add event to Li */
	(this->Li).push_back(event);
}

/**
  * @param matrixT: This User's matrix of direct and indirect knowledge
  * @param eR: The event that has occurred
  * @param user: The recipient of the message
  * @effects Checks if this User knows that userName knows about event eR has occured
  * @returns true if this User knows that the userName knows about the event and false otherwise
  */
bool User::hasRecv(std::map<User, std::vector<int> > matrixT, Event eR, User user) {
	/* Get this User's indirect knowledge of the recipient user */
	return matrixT[user][(eR.getNode()).second] >= eR.getcI();
}

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
void User::onRecv(User sender, Tweet tweet, std::vector<Event> partialLog, std::map<User, std::vector<int> > matrixTk) {
	/* Add tweet sent from sending User to this User's tweets*/
	(this->tweets).push_back(tweet);
	/* Add all events sent from sending User to this User's Li */
	(this->Li).insert((this->Li).end(), partialLog.begin(), partialLog.end());

	/* Temporary placeholders */
	std::map<User, std::vector<int> > selfMatrix = this->matrixT;
	int amtOfUsers = selfMatrix.size();

	/* Update direct knowledge */
	for (int j = 0; j < amtOfUsers; j++) {
		/* Set Ti(i,j) = max of Ti(i, j) and Tk(k, j) */
		((this->matrixT)[*this])[j] = std::max((selfMatrix[*this])[j], (matrixTk[sender])[j]);
	}

	/* Iterators for matrices */
	std::map<User, std::vector<int> >::iterator itrI = selfMatrix.begin();
	std::map<User, std::vector<int> >::iterator itrK = matrixTk.begin();

	/* Update indirect knowledge */
	while (itrI != selfMatrix.end()) {
		/* Skip over direct knowledge */
		if (this -> getIndex() == (itrI->first).getIndex()) {
			itrI++;
			itrK++;
			continue;
		}
		/* Set Ti(j,k) = max of Ti(j, l) and Tk (j, l) where i != j (Direct knowledge) */
		for (int j = 0; j < amtOfUsers; j++) {
			((this->matrixT)[itrI->first][j]) = std::max((itrI->second)[j], (itrK->second)[j]);
		}

		itrI++;
		itrK++;
	}
}