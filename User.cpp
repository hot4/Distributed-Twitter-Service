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