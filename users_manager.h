#ifndef USER_MANAGER_H
#define USER_MANAGER_H
#include "user.h"
#include <string>
#include <map>
#include <utility>
using std::string;
using std::map;
using std::pair;

class UsersManager {
private:
    static UsersManager users_manager;
    
    UsersManager() = default;
public:
    UsersManager(const UsersManager&) = delete;
    UsersManager& operator = (const UsersManager&) = delete;

    static const UsersManager& get_manager();

    int sign_up(map<int, User>& users, map<string, pair<string, int>>& members_login_data, int& users_id) const;

    int login(map<string, pair<string, int>>& members_login_data) const;

    void list_users(const map<int, User>& users) const;
};
#endif