#include <string>
#include <vector>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <utility>
#include "User.h"
#include "Tweet.h"
#include "Event.h"

/**
  * @param userName: The name of the User
  * @param index: Index value associated with this User in the matrix
  * @effects Creates a new User object
  * @modifies Private fields
  * @returns A new User object
  */
User::User(std::string userName, int index) : userName(userName), index(index), cI(0) {}

/**
  * @param users: A list of Users for this User to follow
  */
void User::follow(std::vector<User> users) {
	for (unsigned int i = 0; i < users.size(); i++) {
		User user = users[i];
		if (this->getIndex() == user.getIndex()) continue;
		this->unblock(user.getUserName());
		this->addToMatrixT(user);
	}
}

/**
  * @param tweet: A tweet the User will broadcast to other Users
  * @param Ti: User's matrix to help update recipient User to update direct and indirect knowledge
  * @effects Creates a partial log of events recipient User does not know about
    @effects Sends tweet, Ti, and partial log to all receipients who are not blocked by this User
  */
void sendTweet(Tweet tweet, std::map<std::string, std::pair<int, std::vector<int> > > matrixT) {
	std::vector<User> followers = this->getUnblockedUsers();
	std::vector<Event> currLog = this->getLog();

	std::vector<Event> partialLog;

	/* Create a list of events the receipient User is not aware about to send */
	for (int i = 0; i < followers.size(); i++) {
		/* Get current follower */
		User currUser = followers[i];
		/* Clear partial log before adding events to this container that current follower is not aware about */
		partialLog.clear();

		for (int j = 0; j < currLog.size(); j++) {
			Event event = currLog[j];

			/* Add event to partial log */
			if (this->hasRecv(matrixT, event, currUser)) {
				partialLog.pushBack(event);
			}
		}

		/* TODO: Broadcast message to current follower */
	}
}

/**
  * @effects Displays an ordered list of tweets from all User's this User is not blocked from seeing
  */
void view() {
	std::vector<Tweet> tweets = this->getTweets();
	for (int i = 0; i< tweets.size(); i++) {
		/* TODO: Refer to Tweet class once implemented for accessor functions */
	}
}

/**
  * @param userName: The User this User wishes to block
  * @effects Removes userName from unblockedUsers list and adds to blockedUsers list
  * @modifies unblockedUser and blockedUsers private fields
  */
void block(std::string userName) {
	std::vector<User> blocked = this->blockedUsers;
	User user = NULL;

	for (int i = 0; i < (this->unblockedUsers).size(); i++) {
		user = (this->unblockedUsers)[i];
		if (userName == user.getUserName()) {
			(this->unblockedUsers).remove(user);
			break;
		}
	}

	(this->blockedUsers).add(user);
}