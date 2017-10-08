#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

#include "User.h"

/**
  * @effects Creates a default User object
  * @returns A new User object
  */
User::User(){}

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
  * @effects Sets blockedStatus from this User's perspective to other Users as false
  * @modifies blockedStatus and matrixT private fields
  */
void User::follow(std::vector<User> users) {
	/* Current User has no knowledge or triggered any events. Therefore, entire matrix should be initialized to 0 for the NxN matrix */
	std::vector<int> knowledge(users.size(), 0);
	/* No events has occurred. Therefore, no current User should have any missing knowledge about some Event that has occurred */
	/* Ignored parialLog for self */
	std::list<Event> partialLog(users.size()-1);

	for (unsigned int i = 0; i < users.size(); i++) {
		/* Get current user */
		User user = users[i];

		/* Insert relationship into dictionary to repsent that either direction, this User to other User or other User to this User, represents an unblocked relationship */
		(this->blockedStatus).insert(std::pair<std::string, std::pair<bool, bool> >(user.getUserName(), std::pair<bool, bool>(false, false)));

		/* Insert empty list of Events that current User does not know about */
		if (this->getUserName() != user.getUserName()) {
			(this->partialLog).insert(std::pair<std::string, std::list<Event> >(user.getUserName(), partialLog));
		}

		/* Insert knowledge of current User into matrixT */
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

	/* Temporary place holder */
	std::string currUserName;

	/* Iterator for blocked status for all Users */
	std::map<std::string, std::pair<bool, bool> > blockedStatus = this->getBlockedStatus();
	std::map<std::string, std::pair<bool, bool> >::iterator itr = blockedStatus.begin();

	while (itr != blockedStatus.end()) {
		/* Container for Events to send to recipient */
		std::vector<Event> NP;

		currUserName = itr->first;
		/* Given this is not this User and the current User is not blocked */
		if (((this->getUserName()) != currUserName) && !(this->getUserBlockedStatus(currUserName)).first) {
			std::list<Event>::iterator itr = this->partialLog[currUserName].begin();
			while (itr != this->partialLog[currUserName].end()) {
				/* Get current event that recipient supposedly does not know about this Event */
				Event event = *itr;
				/* Verify that recipient does not know about this Event */
				if (!(this->hasRecv(matrixT, event, currUserName))) {
					/* Add event to container of Events recipient should receive */
					NP.push_back(event);
					itr++;
				} else {
					/* Delete event for recipient from partialLog because recipient already knows about event */
					(this->partialLog)[currUserName].erase(itr);
				}
			}
		}
		/* TODO: Broadcast message to current follower */
		itr++;
	}
}

/**
  * @effects Displays an ordered list of tweets from all User's this User is not blocked from seeing
  */
void User::view() {
	/* Get list of this Users tweets */
	std::vector<Tweet> tweets = this->getTweets();

	/* Iterator for blocked status */
	std::map<std::string, std::pair<bool, bool> > blockedStatus = this->getBlockedStatus();
	std::map<std::string, std::pair<bool, bool> >::iterator itr;

	/* Fixed buffer size */
	int buffSize = 80;

	/* Loop over all tweets and print if this User is not blocked */
	for (unsigned int i = 0; i < tweets.size(); i++) {
		/* Get current tweet */
		Tweet tweet = tweets[i];

		/* Check if this User is blocked from seeing the creator of the current tweet's tweets*/
		itr = blockedStatus.find(tweet.getUserName());
		if (!(itr->second).second) {
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
}

/**
  * @param userName: The User this User wishes to block
  * @effects Sets blockedStatus from this User's perspective to userName as true
  * @modifies blockedStatus private field
  */
void User::block(std::string userName) {
	/* Update dictionary for this User to some other User's relationshp */
	(this->blockedStatus)[userName].first = true;
}

/**
  * @param userName: The User, this User wishes to unblock
  * @effects Sets blockedStatus from this User's perspective to userName as false
  * @modifies blockedStatus private field
  */
void User::unblock(std::string userName) {
	/* Update dictionary for this User to some other User's relationship */
	(this->blockedStatus)[userName].first = false;
}

/**
  * @param userName: The User who does not want this User to view their tweets
  * @effects Sets this User's knowledge about being blocked to true based on the userName
  * @modifies blockedStatus private field
  */
void User::blockView(std::string userName) {
	/* Update dictionary for some other User and this User's relationship */
	(this->blockedStatus)[userName].second = true;
}

/**
  * @param userName: The User who does want this User to view their tweets
  * @effects Sets this User's knowledge about being blocked to false based on the userName
  * @modifies blockedStatus private field
  */
void User::unblockView(std::string userName) {
	(this->blockedStatus)[userName].second = false;
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

	/* Temporary place holders */
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

	/* Add event to all partialLogs for Users */
	std::map<std::string, std::list<Event> > partialLog = this->getPartialLog();
	std::map<std::string, std::list<Event> >::iterator itr = partialLog.begin();
	while (itr != partialLog.end()) {
		(this->partialLog)[itr->first].push_back(event);
		itr++;
	}
}

/**
  * @param matrixT: This User's matrix of direct and indirect knowledge
  * @param eR: The event that has occurred
  * @param userName: The recipient of the message
  * @effects Checks if this User knows that userName knows about event eR has occured
  * @returns true if this User knows that the userName knows about the event and false otherwise
  */
bool User::hasRecv(std::map<User, std::vector<int> > matrixT, Event eR, std::string userName) {
	User user;
	std::map<User, std::vector<int> >::iterator itr = matrixT.begin();

	while (itr != matrixT.end()) {
		user = itr->first;
		if (user.getUserName() == userName) break;
		itr++;
	}

	/* Get this User's indirect knowledge of the recipient user */
	return matrixT[user][(eR.getNode()).second] >= eR.getcI();
}

/**
  * @param sender: User who sent message
  * @param tweet: A tweet this User has received from the sending User
  * @param NP: A list of events this User is not aware about from the sending User
  * @param matrixTk: The sending User's matrix of direct and indirect knowledge
  * @effects Adds tweet to tweets
  * @effects Adds all events in the NP into Li
  * @effects Updates direct and indirect knowledge based on sending User's matrix
  * @modifies tweets, Li, matrixT
  */
void User::onRecv(User sender, Tweet tweet, std::vector<Event> NP, std::map<User, std::vector<int> > matrixTk) {
	/* Add tweet sent from sending User to this User's tweets*/
	(this->tweets).push_back(tweet);
	
	/* Add all events sent from sending User to this User's Li */
	(this->Li).insert((this->Li).end(), NP.begin(), NP.end());

	/* Temporary place holder */
	std::string eventNodeName;
	std::string eventRecipientName;

	/* Update blocked status */
	for (unsigned int i = 0; i < NP.size(); i++) {
		/* Get current event and private fields associated */
		Event currEvent = NP[i];
		eventNodeName = currEvent.getNode().first;
		eventRecipientName = currEvent.getRecipient().first;

		/* Check if this User is the recipient of some event */
		if (eventRecipientName == this->getUserName()) {
			/* Switch on event type */
			switch (currEvent.getType()) {
				/* This User is blocked from viewing this node's tweets */
				case 2:
					 this->blockView(eventNodeName);
					break;
				/* This User is being unblocked from viewing this node's tweets */
				case 3:
					this->unblockView(eventNodeName);
					break;
			}
		}
	}

	/* Iterators for matrices */
	std::map<User, std::vector<int> > selfMatrix = this->matrixT;
	std::map<User, std::vector<int> >::iterator itrI = selfMatrix.begin();
	std::map<User, std::vector<int> >::iterator itrK = matrixTk.begin();

	/* Temporary place holders */
	int amtOfUsers = selfMatrix.size();

	/* Update direct knowledge */
	for (int j = 0; j < amtOfUsers; j++) {
		/* Set Ti(i,j) = max of Ti(i, j) and Tk(k, j) */
		((this->matrixT)[*this])[j] = std::max((selfMatrix[*this])[j], (matrixTk[sender])[j]);
	}

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

	/* Update partialLog */
	// std::map<std::string, std::list<Event> > partialLog = this->getPartialLog;

	// /* Update partialLog based on Events sending User had send this User */
	// /* This User's partialLog for sender */
	// std::list<Event> missingKnowledge = this->getPartialLog()[sender.getUserName()];
	// /* Iterate through missing knowledge of sender and remove any Events that sender is already aware about */
	// for (unsigned int i = 0; i < NP.size(); i++) {
	// 	/* Get current event */
	// 	Event event = NP[i];
	// 	/* Iterator for missingKnowledge */
	// 	std::list<Event>::iterator itr = missingKnowledge.begin();
	// 	 Loop through this User's partialLog for sender and see if any Events sender provided to this User can be removed from partialLog 
	// 	while (itr != missingKnowledge.end()) {
	// 		/* Check if sender already knows about an event in the partialLog */
	// 		if (event == *itr) {
	// 			/* Delete from partialLog and break because only one event can be equal another */
	// 			(this->partialLog)[sender.getUserName()].erase(itr);
	// 			break;
	// 		} else {
	// 			/* Increment iterator to check the rest of the partialLog */
	// 			itr++;
	// 		}
	// 	}
	// }
}

/**
  * @param userName: User to compare blocked status to
  * @returns Relationship between this user and other User
  */
std::pair<bool, bool> User::getUserBlockedStatus(std::string userName) {
	return this->getBlockedStatus()[userName];
}