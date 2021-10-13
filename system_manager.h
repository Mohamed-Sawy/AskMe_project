#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H
#include "question.h"
#include "user.h"
#include "users_manager.h"
#include "questions_manager.h"

#include <map>
#include <string>
#include <utility>

using std::map;
using std::string;
using std::pair;

class SystemManager {
private:
    map<int, User> users;
    map<string, pair<string, int>> members_login_data;
    int current_user_id;
    int questions_id;
    int users_id;
    const UsersManager& users_manager;
    const QuestionsManager& questions_manager;
    static SystemManager askMe_manager;

    SystemManager();

    void update_database();

    bool load_database();

    int registration_menu();

    int menu();
public:
    SystemManager(const SystemManager&) = delete;
    SystemManager& operator = (const SystemManager&) = delete;

    ~SystemManager() {
        update_database();
    }
    
    static SystemManager& get_manager() {
        return askMe_manager;
    }

    void start();
};
#endif