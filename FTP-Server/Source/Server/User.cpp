#include "User.h"

using namespace std;

User::User(string _username_, string _password_, bool _isAdmin_, long _capacity_, string _path_) {
    this->username = _username_;
    this->password = _password_;
    this->isAdmin = _isAdmin_;
    this->capacity = _capacity_;
    this->path = _path_;
    this->stage = ENTER_USER;
    this->commandFD = -1;
    this->dataFD = -1;
}
