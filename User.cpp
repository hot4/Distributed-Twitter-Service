#include <string>
#include <vector>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <utility>
#include "Tweet.h"
#include "Event.h"

/**
  * @param userName: The name of the User
  * @param index: Index value associated with this User in the matrix
  * @effects Creates a new User object
  * @modifies Private fields
  * @returns A new User object
  */
User::User(std::string userName int index) : userName_(userName), index_(index), cI_(0) {}

/**
  * @param users: A list of Users for this User to follow
  */
void User::follow(std::vector<User> users) {
	for (int i = 0; i < users.size(); i++) {
		User user = users[i];
		if (this.getIndex() == user.getInex() continue;
		this.addToUnblockedUser(user);
		this.addToMatrixT(user);
	}
}

/**
  * @param tweet: A tweet the User will broadcast to other Users
  * @param Ti: User's matrix to help update recipient User to update direct and indirect knowledge
  * @effects Creates a partial log of events recipient User does not know about
    @effects Sends tweet, Ti, and partial log to all receipients who are not blocked by this User
  */
void sendTweet(Tweet tweet, std::map<std::string, std::pair<int, std::vector<int> > > matrixT) {
	std::vector<User> followers = this.getUnblockedUsers();
	std::vector<Event> currLog = this.getLog();

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
			if (this.hasRecv(matrixT, event, currUser)) {
				partialLog.pushBack(event);
			}
		}

		/* TODO: Broadcast message to current follower */
	}
}

