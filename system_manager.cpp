#include "system_manager.h"
#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::cin;
using std::ifstream;
using std::ofstream;
using std::istringstream;


SystemManager::SystemManager() : current_user_id{ -1 }, questions_id{ 1 }, users_id{ 1 },
								 users_manager{ UsersManager::get_manager() },
								 questions_manager{ QuestionsManager::get_manager() } {
}

SystemManager SystemManager::askMe_manager;

void SystemManager::update_database() {
    ofstream users_file("askfm_users");
    ofstream questions_file("askfm_questions");

    for (const auto& [user_id, user] : users) {
        users_file << user_id << " | " << user.get_username() << " | " << user.get_email() << " | " << user.get_password() << " | ";

        if (user.is_anonymous_allowed())
            users_file << "allowed\n";
        else
            users_file << "not_allowed\n";
    }

    users_file.close();

    for (const auto& [user_id, user] : users) {
        const map<int, Question>& received_questions{ user.get_received_questions() };
        for (const auto& [question_id, question] : received_questions) {
            questions_file << question_id << " | " << question.get_parent_id() << " | " << question.get_sender_id()
                           << " | " << question.get_receiver_id() << " | " << question.get_user_question() << " | " << question.get_answer() << " | ";

            if (question.is_anonymous())
                questions_file << "anonymous | ";
            else
                questions_file << "not_anonymous | ";

            if (question.is_answered())
                questions_file << "answered | ";
            else
                questions_file << "not_answered | ";

            if (question.is_parent())
                questions_file << "parent\n";
            else
                questions_file << "not_parent\n";
        }
    }

    questions_file.close();
}

bool SystemManager::load_database() {
    ifstream users_file("askfm_users");
    ifstream questions_file("askfm_questions");

    if (users_file.fail()) {
        cout << "Database error: Please restart the application.\n";
        return false;
    }

    string line;
    while (getline(users_file, line)) {
        istringstream iss(line);

        string check, seperator;
        int id{ 0 };
        iss >> id >> seperator;

        string username;
        while (check != "|") {
            if (!username.empty())
                username += " ";

            username += check;

            iss >> check;
        }

        string email;
        string password;
        iss >> email >> seperator >> password >> seperator;

        bool allow_anonymous{ true };
        iss >> check;
        if (check == "not_allowed")
            allow_anonymous = false;

        users[id].set_username(username).set_email(email).set_password(password).set_anonymous(allow_anonymous);
        members_login_data[username] = { password, id };
        users_id = std::max(users_id, id + 1);
    }

    users_file.close();

    if (questions_file.fail()) {
        cout << "Database error: Please restart the application.\n";
        return false;
    }

    while (getline(questions_file, line)) {
        istringstream iss(line);

        string check, seperator;
        int id{ 0 };
        iss >> id >> seperator;

        int parent_id {0};
        int sender_id {0};
        int receiver_id {0};
        iss >> parent_id >> seperator >> sender_id >> seperator >> receiver_id >> seperator;

        string user_question;
        while (check != "|") {
            if (!user_question.empty())
                user_question += " ";

            user_question += check;

            iss >> check;
        }

        string answer;
        check = "";
        while (check != "|") {
            if (answer.empty())
                answer += " ";

            answer += check;

            iss >> check;
        }

        bool anonymous{ true };
        iss >> check >> seperator;
        if (check == "not_anonymous")
            anonymous = false;

        bool answered {false};
        iss >> check >> seperator;
        if (check == "answered")
            answered = true;

        bool parent{ true };
        iss >> check;
        if (check == "not_parent") 
            parent = false;

        string sender_username{ users[sender_id].get_username() };
        string receiver_username{ users[receiver_id].get_username() };

        Question question;
        question.set_parent_id(parent_id).set_sender_id(sender_id).set_receiver_id(receiver_id);
        question.set_parent(parent).set_anonymous(anonymous).set_answered(answered);
        question.set_sender_username(sender_username).set_receiver_username(receiver_username);
        question.set_user_question(user_question).set_answer(answer);

        users[receiver_id].add_received_question(id, question);
        users[sender_id].add_sent_question(id, question);

        questions_id = std::max(questions_id, id + 1);
    }

    questions_file.close();

    return true;
}

int SystemManager::registration_menu() {
    cout << "registration menu: \n"
        << "         1. Sign up\n"
        << "         2. Login\n\n";

    bool valid{ false };
    int choice{ 0 };
    do {
        cout << "Enter 1 to sign up or 2 to login: ";
        cin >> choice;

        if ((choice != 1 && choice != 2) || cin.fail())
            cout << "\nInvalid input. Please try again.\n", cin.clear(), cin.ignore(1000, '\n');
        else
            valid = true;
    } while (!valid);

    if (choice == 1)
        return users_manager.sign_up(users, members_login_data, users_id);
    else
        return users_manager.login(members_login_data);
}

int SystemManager::menu() {
    int choice{ 0 };
    cout << "\nMenu: \n"
        << "     1:  Print received questions\n"
        << "     2:  Print sent questions\n"
        << "     3:  Answer question\n"
        << "     4:  Delete question\n"
        << "     5:  Ask question\n"
        << "     6:  List all users\n"
        << "     7:  Display the feed\n"
        << "     8:  Logout\n"
        << "     9:  Save changes\n"
        << "     10: End the program\n";

    bool valid{ false };
    do {
        cout << "\nEnter your choice [1 : 10]: ";
        cin >> choice;

        if (1 <= choice && choice <= 10 && !cin.fail())
            valid = true;
        else
            cout << "\nInvalid choice. Please try again\n", cin.clear(), cin.ignore(1000, '\n');
    } while (!valid);

    return choice;
}

void SystemManager::start() {
    if (!load_database())
        return;

    current_user_id = registration_menu();

    cout << "\n**Don't forget to save changes before closing the program or close it properly - from the menu - to prevent data loss**";
    while (true) {
        int choice = menu();

        if (choice == 1)
            questions_manager.print_received_questions(users, current_user_id);
        else if (choice == 2)
            questions_manager.print_sent_questions(users, current_user_id);
        else if (choice == 3)
            questions_manager.answer_question(users, current_user_id);
        else if (choice == 4)
            questions_manager.delete_question(users, current_user_id);
        else if (choice == 5)
            questions_manager.ask_question(users, current_user_id, questions_id);
        else if (choice == 6)
            users_manager.list_users(users);
        else if (choice == 7)
            questions_manager.print_feed(users);
        else if (choice == 8)
            current_user_id = registration_menu();
        else if (choice == 9)
            update_database();
        else
            return;
    }
}