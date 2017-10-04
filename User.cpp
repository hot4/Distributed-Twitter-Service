#include <iostream>
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
bool operator== (const User &u) const {
	return this->getUserName() == u.getUserName();
}

/**
  * @param u: User to compare to
  * @returns The alphabetic order of both Users based on userName
  */
bool operator< (const User &u) const {
	return (this->getUserName()).compare(u.getUserName());
}

/**
  * @param users: A list of Users for this User to follow
  */
void User::follow(std::vector<User> users) {
	for (unsigned int i = 0; i < users.size(); i++) {
		User user = users[i];
		if (this->getIndex() == user.getIndex()) continue;
		this->unblock(user.getUserName());
		// this->addToMatrixT(user);
	}
}

/**
  * @param tweet: A tweet the User will broadcast to other Users
  * @param Ti: User's matrix to help update recipient User to update direct and indirect knowledge
  * @effects Creates a partial Li of events recipient User does not know about
    @effects Sends tweet, Ti, and partial Li to all receipients who are not blocked by this User
  */
void User::sendTweet(Tweet &tweet, std::map<User, std::vector<int> > matrixT) {
	std::list<User> followers = this->getUnblockedUsers();
	std::vector<Event> currLog = this->getLog();

	std::vector<Event> partialLog;

	std::list<User>::iterator itr = followers.begin();

	/* Create a list of events the receipient User is not aware about to send */
	while (itr != followers.end()) {
		/* Get current follower */
		User currUser = *itr;
		/* Clear partial Li before adding events to this container that current follower is not aware about */
		partialLog.clear();

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
	char buffer [buffSize];
	for (unsigned int i = 0; i < tweets.size(); i++) {
		/* TODO: Refer to Tweet class once implemented for accessor functions */
		strftime(buffer, buffSize, "%I:%M%p.", localtime(&(tweets[i].getRawTimeStamp())));

		/*
			  	time_t rawtime;
				struct tm * timeinfo;
				char buffer [80];

				time (&rawtime);
				timeinfo = localtime (&rawtime);

				strftime (buffer,80,"Now it's %I:%M%p.",timeinfo);
				puts (buffer);
		*/
	}
}

/**
  * @param userName: The User this User wishes to block
  * @effects Removes userName from unblockedUsers list and adds to blockedUsers list
  * @modifies unblockedUser and blockedUsers private fields
  */
void User::block(std::string userName) {
	std::list<User> unblockedUsers = this->unblockedUsers;

	/* Loop through blocked list to find userName */
	for (std::list<User>::iterator itr = unblockedUsers.begin(); itr != unblockedUsers.end(); itr++) {
		User user = *itr;

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
	std::list<User> blockedUsers = this->blockedUsers;

	/* Loop through blocked list to find userName */
	for (std::list<User>::iterator itr = blockedUsers.begin(); itr != blockedUsers.end(); itr++) {
		User user = *itr;

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
  * @effects Increments cI counter
  * @effects Updates matrixT direct knowledge of itself; i.e. Ti(i,i)
  * @effects Creates event eR and adds to Li
  * @effects If event type is Tweet, calls sendTweet()
  * @modifies cI, matrixT, and Li private fields
  */
void User::onEvent() {
	/* Increment this User's counter */
	(this->cI)++;

	/* Get value based on key and then access index to increment this User's Ti(i,i) */
	((this->matrixT)[*this])[this->getIndex()]++;

	/* TODO: Create Event based on Event class constructor */
	/* TODO: Add event to Li */
	/* TODO: Type check Event and call sendTweet() if appropriate */
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
	std::vector<int> indirectKnowledge = matrixT[user];
	/* TODO: Use Event accessor functions to get node (location of event) and time (amount of causually preceding events */
	/* return indirectKnowledge[eR.getNode()] >= eR.getTime() */
	return false;
}

/**
  * @param tweet: A tweet this User has received from the sending User
  * @param partialLog: A list of events this User is not aware about from the sending User
  * @param matrixTk: The sending User's matrix of direct and indirect knowledge
  * @effects Adds tweet to tweets
  * @effects Adds all events in the partialLog into Li
  * @effects Updates direct and indirect knowledge based on sending User's matrix
  * @modifies tweets, Li, matrixT
  */
void User::onRecv(std::vector<Tweet> tweetsRecv, std::vector<Event> partialLog, std::map<User, std::vector<int> > matrixTk) {
	/* Add all tweets sent from sending User to this User's tweets*/
	(this->tweets).insert((this->tweets).end(), tweetsRecv.begin(), tweetsRecv.end());
	/* Add all events sent from sending User to this User's Li */
	// (this->Li).insert((this->Li).end(), partialLog.begin(), partialLog.end());

	std::map<User, std::vector<int> > selfMatrix = this->matrixT;
	int amtOfUsers = selfMatrix.size();

	/* Update direct knowledge */
	/* TODO: Use Tweet accessor function to get User of tweet */
	for (int j = 0; j < amtOfUsers; j++) {
		/* Set Ti(i,j) = max of Ti(i, j) and Tk(k, j) */
		// ((this->matrixT)[this])[j] = max((selfMatrix[this])[j], (matrixTk[tweet.getUser()])[j]);
	}

	/* Update indirect knowledge */
	std::map<User, std::vector<int> >::iterator itrI = selfMatrix.begin();
	std::map<User, std::vector<int> >::iterator itrK = matrixTk.begin();

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