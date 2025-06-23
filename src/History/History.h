#ifndef HISTORY_H
#define HISTORY_H

#include <string>
#include <vector>

#include "../DataManager/DataManager.h"
#include "../User/User.h"

using namespace std;

class History {
 private:
  DataManager dataManager;
  User user;
  vector<string> history;
  bool load();

 public:
  explicit History(DataManager &dataManager, User &user);
  User &getUser();

  // History methods
  bool set(vector<string> &history);
  vector<string> get();
  bool add(string &isbn);
  bool remove(string &isbn);
  bool clear();
  bool save();
};

#endif
