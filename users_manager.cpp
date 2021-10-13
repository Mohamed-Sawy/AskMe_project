#include "users_manager.h"
#include <iostream>

using std::cout;
using std::cin;

UsersManager UsersManager::users_manager;

const UsersManager& UsersManager::get_manager() {
    return users_manager;
}

int UsersManager::sign_up(map<int, User>& users, map<string, pair<string, int>>& members_login_data, int& users_id) const {
    User new_user;

    string username;
    bool valid{ false };
    do {
        cout << "\nEnter a username: ";
        cin.clear(), cin.ignore();
        getline(cin, username);

        if (members_login_data.find(username) != members_login_data.end())
            cout << "\nThis username is already taken. Please choose another one.";
        else
            valid = true;
    } while (!valid);

    string email;
    cout << "\nEnter your email: ";
    cin >> email;
    cin.ignore();

    string password;
    cout << "\nEnter your password: ";
    cin >> password;
    cin.ignore();

    bool anonymous{ true };
    valid = false;
    do {
        string choice;
        cout << "\nDo you want to allow anonymous questions? [Y/N]: ";
        cin >> choice;
        cin.ignore();

        if (choice != "Y" && choice != "N")
            cout << "\nThis is not a valid input. Please try again.";
        else {
            (choice == "Y") ? anonymous = true : anonymous = false;
            valid = true;
        }
    } while (!valid);

    new_user.set_username(username).set_email(email).set_password(password).set_anonymous(anonymous);

    users[users_id] = new_user;
    members_login_data[username] = { password, users_id };

    return users_id++;
}

int UsersManager::login(map<string, pair<string, int>>& members_login_data) const {
    string username;
    string password;
    
    bool valid{ false };
    do {
        cout << "\nEnter your username: ";
        cin.clear(), cin.ignore();
        getline(cin, username);

        cout << "\nEnter your password: ";
        cin >> password;
        cin.ignore();

        if (members_login_data.find(username) == members_login_data.end() || members_login_data[username].first != password)
            cout << "\nInvalid username or password. Please try again\n";
        else
            valid = true; 
    } while (!valid);

    return members_login_data[username].second;
}

void UsersManager::list_users(const map<int, User>& users) const {
    for (auto& [id, member] : users)
        cout << "\nID: " << id << "   Name: " << member.get_username();

    cout << "\n";
}