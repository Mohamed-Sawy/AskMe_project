#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <utility>

using std::cin;
using std::cout;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::string;
using std::map;
using std::pair;
//////////////////////////////////////////////////////////////////////////////////////

struct question {
    int parent_id, from_id, to_id;
    bool anonymous, parent, answered;
    string from_username, to_username, user_question, answer;
    map<int, question> thread;

    question() {
        parent_id = from_id = to_id = 0;
        anonymous = parent = true;
        answered = false;
    }
};

struct user {
    string username, email, password;
    bool allow_anonymous;
    map<int, question> received_questions, sent_questions;

    user() {
        allow_anonymous = true;
    }
};

namespace questions_manager {
    enum question_modes { received, sent, feed };

    void print_question(const int& id, const question& quest, const int& mode) {
        if (!quest.parent) {
            if (mode == sent || mode == feed)
                cout << "        Thread to question ("<<quest.parent_id<<"): ";
            else
                cout << "        Thread: ";
        }

        cout << "Question id (" << id<<") ";

        if (mode == feed)
            cout << "from " << quest.from_username << " to " << quest.to_username;
        else if (mode == received)
            cout << "from " << quest.from_username;
        else if (mode == sent)
            cout << "to " << quest.to_username;

        cout << ": [" << quest.user_question << "]\n";

        if (quest.answered)
            cout << "        Answer: " << quest.answer << "\n";
        else
            cout << "        -Not answered yet-\n";

        cout << "\n";
    }

    void print_received_questions(map<int, user>& users, const int& current_user_id) {
        cout << "\n";

        user& my_user = users[current_user_id];

        if (my_user.received_questions.empty()) {
            cout << "You haven't received any questions yet.\n";
            return;
        }

        for (const auto& [id, quest] : my_user.received_questions) {
            if (quest.parent)
                print_question(id, quest, received);

            for (const auto& [thread_id, thread_quest] : quest.thread)
                print_question(thread_id, thread_quest, received);
        }

        cout << "\n";
    }

    void print_sent_questions(map<int, user>& users, const int& current_user_id) {
        cout << "\n";

        user& my_user = users[current_user_id];

        if (my_user.sent_questions.empty()) {
            cout << "You haven't sent any questions yet.\n";
            return;
        }

        for (const auto& [id, quest] : my_user.sent_questions)
            print_question(id, quest, sent);

        cout << "\n";
    }

    void answer_question(map<int, user>& users, const int& current_user_id) {
        int quest_id{ 0 };
        bool valid{ false };
        user& my_user = users[current_user_id];

        if (my_user.received_questions.empty()) {
            cout << "\nYou haven't received any questions to answer yet.\n";
            return;
        }

        do {
            cout << "\nEnter Question id or -1 to cancel: ";
            cin >> quest_id;

            if (quest_id == -1)
                return;

            if (my_user.received_questions.find(quest_id) != my_user.received_questions.end() && !cin.fail())
                valid = true;
            else
                cout << "\nThere's no question with this id. Please try again.\n", cin.clear(), cin.ignore(1000, '\n');

        } while (!valid);

        cout << "\n";

        print_question(quest_id, my_user.received_questions[quest_id], received);

        if (my_user.received_questions[quest_id].answered)
            cout << "Warning: This question is already answered. Answer will be updated.\n";

        cout << "Enter your answer: ";
        cin.clear(), cin.ignore();
        getline(cin, my_user.received_questions[quest_id].answer);

        my_user.received_questions[quest_id].answered = true;

        users[my_user.received_questions[quest_id].from_id].sent_questions[quest_id] = my_user.received_questions[quest_id];

        if (!my_user.received_questions[quest_id].parent)
            my_user.received_questions[my_user.received_questions[quest_id].parent_id].thread[quest_id] = my_user.received_questions[quest_id];

        cout << "\n";
    }

    void delete_question(map<int, user>& users, const int& current_user_id) {
        int quest_id{ 0 };
        bool valid{ false };
        user& my_user = users[current_user_id];

        if (my_user.received_questions.empty()) {
            cout << "\nYou haven't received any questions that you can delete yet.\n";
            return;
        }

        do {
            cout << "\nEnter Question id or -1 to cancel: ";
            cin >> quest_id;

            if (quest_id == -1)
                return;

            if (my_user.received_questions.find(quest_id) != my_user.received_questions.end() && !cin.fail())
                valid = true;
            else
                cout << "\nThere's no question with this id for this user. Please try again.\n", cin.clear(), cin.ignore(1000, '\n');

        } while (!valid);

        if (my_user.received_questions[quest_id].parent)
            for (auto& [thread_id, thread_quest] : my_user.received_questions[quest_id].thread)
                users[thread_quest.from_id].sent_questions.erase(thread_id) , my_user.received_questions.erase(thread_id);
        else
            my_user.received_questions[my_user.received_questions[quest_id].parent_id].thread.erase(quest_id);

        users[my_user.received_questions[quest_id].from_id].sent_questions.erase(quest_id);
        my_user.received_questions.erase(quest_id);

        cout << "\nQuestion deleted successfully.\n";
    }

    void ask_question(map<int, user>& users, int& current_user_id, int& questions_id) {
        question new_question;
        int user_id{ 0 };
        bool valid{ false };
        do {
            cout << "\nEnter a user id to ask or -1 to cancel: ";
            cin >> user_id;

            if (user_id == -1)
                return;

            if (user_id != current_user_id && users.find(user_id) != users.end() && !cin.fail())
                valid = true;
            else
                cout << "\nThere's no user with this id. Please try again.\n", cin.clear(), cin.ignore(1000, '\n');

        } while (!valid);

        bool anonymous{ false };
        if (users[user_id].allow_anonymous) {
            string choice;
            do {
                cout << "\nDo you want to ask this question anonymously? [Y/N]: ";
                cin >> choice;

                if (choice == "Y")
                    anonymous = true;
                else if (choice == "N")
                    anonymous = false;
                else
                    cout << "\nThis is not a valid input. Please try again.\n";
            } while (choice != "Y" && choice != "N");
        }
        else
            cout << "\nNote: anonymous questions are not allowed for this user\n";

        int quest_id{ 0 };
        valid = false;
        do {
            cout << "\nEnter the main question id for a thread question or -1 for a new question: ";
            cin >> quest_id;

            if (quest_id == -1)
                break;

            if (users[user_id].received_questions.find(quest_id) != users[user_id].received_questions.end() && users[user_id].received_questions[quest_id].parent && !cin.fail())
                valid = true;
            else
                cout << "\nThere's no main question with this id for that user. Please try again.\n", cin.clear(), cin.ignore(1000, '\n');

        } while (!valid);

        string quest;
        cout << "\nEnter your question: ";
        cin.clear(), cin.ignore();
        getline(cin, quest);

        new_question.user_question = quest;
        new_question.from_id = current_user_id;
        new_question.from_username = users[current_user_id].username;
        new_question.to_id = user_id;
        new_question.to_username = users[user_id].username;

        if (anonymous)
            new_question.from_username = "an anonymous user";
        else
            new_question.anonymous = false;


        if (quest_id != -1) {
            new_question.parent = false;
            new_question.parent_id = quest_id;

            users[user_id].received_questions[quest_id].thread[questions_id] = new_question;
        }

        users[current_user_id].sent_questions[questions_id] = users[user_id].received_questions[questions_id] = new_question;

        cout << questions_id << "\n";
        questions_id++;
    }

    void print_feed(const map<int, user>& users) {
        map<int, question, std::greater<int>> questions;
        for (auto& [user_id, member] : users)
            for (auto& [quest_id, quest] : member.received_questions)
                if (quest.parent)
                    questions[quest_id] = quest;

        cout << "\n";

        bool empty{ true };
        for (auto& [quest_id, quest] : questions) {
            if (quest.answered)
                print_question(quest_id, quest, feed) , empty = false;

            for (auto& [thread_id, thread_quest] : quest.thread)
                if (thread_quest.answered)
                    print_question(thread_id, thread_quest, feed) , empty = false;
        }

        if (empty)
            cout << "There's no questions at the moment.\n";
    }
}

namespace users_manager {
    int sign_up(map<int, user>& users, map<string, pair<string, int>>& members, int& users_id) {
        user& new_user = users[users_id];
        bool valid{ false };
        do {
            cout << "\nEnter a username: ";
            cin.clear(), cin.ignore();
            getline(cin, new_user.username);

            if (members.find(new_user.username) != members.end())
                cout << "\nThis username is already taken. Please choose another one.";
            else
                valid = true;
        } while (!valid);

        cout << "\nEnter your email: ";
        cin >> new_user.email;

        cout << "\nEnter your password: ";
        cin >> new_user.password;

        string choice;
        valid = false;
        do {
            cout << "\nDo you want to allow anonymous questions? [Y/N]: ";
            cin >> choice;

            if (choice == "Y" || choice == "N")
                valid = true;
            else
                cout << "\nThis is not a valid input. Please try again.";
        } while (!valid);

        if (choice == "N")
            new_user.allow_anonymous = false;

        members[new_user.username] = { new_user.password, users_id };

        return users_id++;
    }

    int login(map<string, pair<string, int>>& members) {
        string username, password;
        bool valid{ false };
        do {
            cout << "\nEnter your username: ";
            cin.clear(), cin.ignore();
            getline(cin, username);

            cout << "\nEnter your password: ";
            cin >> password;

            if (members.find(username) != members.end() && members[username].first == password)
                valid = true;
            else
                cout << "\nInvalid username or password. Please try again\n";
        } while (!valid);

        return members[username].second;
    }

    void list_users(const map<int, user>& users) {
        for (auto& [id, member] : users)
            cout << "\nID: " << id << "   Name: " << member.username;

        cout << "\n";
    }
}

struct AskMe_system {
    map<int, user> users;
    map<string, pair<string, int>> members;
    int current_user_id, questions_id, users_id;

    AskMe_system () {
        current_user_id = 0;
        questions_id = 1;
        users_id = 1;
    }

    ~AskMe_system() {
        update_database();
    }

    void update_database () {
        ofstream users_file("askfm_users"), questions_file("askfm_questions");

        for (auto& [user_id, member] : users) {
            users_file << user_id << " | " << member.username << " | " << member.email << " | " << member.password << " | ";

            if (member.allow_anonymous)
                users_file << "allowed\n";
            else
                users_file << "not_allowed\n";
        }

        users_file.close();

        for (auto& [user_id, member] : users)
            for (auto& [quest_id, quest] : member.received_questions) {
                questions_file << quest_id << " | " << quest.parent_id << " | " << quest.from_id << " | " << quest.to_id << " | " << quest.user_question << " | " << quest.answer << " | ";

                if (quest.anonymous)
                    questions_file << "anonymous | ";
                else
                    questions_file << "not_anonymous | ";

                if (quest.answered)
                    questions_file << "answered | ";
                else
                    questions_file << "not_answered | ";

                if (quest.parent)
                    questions_file << "parent\n";
                else
                    questions_file << "not_parent\n";
            }

        questions_file.close();
    }

    bool load_database () {
        ifstream users_file("askfm_users"), questions_file("askfm_questions");

        if (users_file.fail()) {
            cout << "Database error: Please restart the application.\n";
            return false;
        }

        string line;
        while (getline(users_file, line)) {
            istringstream iss(line);

            string check, symbol;
            int id{ 0 };
            iss >> id >> symbol;

            while (check != "|") {
                if (!users[id].username.empty())
                    users[id].username += " ";

                users[id].username += check;

                iss >> check;
            }

            iss >> users[id].email >> symbol >> users[id].password >> symbol;

            iss >> check;
            if (check == "not_allowed")
                users[id].allow_anonymous = false;

            members[users[id].username] = { users[id].password, id };
            users_id = id + 1;
        }

        users_file.close();

        if (questions_file.fail()) {
            cout << "Database error: Please restart the application.\n";
            return false;
        }

        while (getline(questions_file, line)) {
            istringstream iss(line);

            string check, symbol;
            int id{ 0 };
            iss >> id >> symbol;

            question quest;
            iss >> quest.parent_id >> symbol >> quest.from_id >> symbol >> quest.to_id >> symbol;

            quest.from_username = users[quest.from_id].username;
            quest.to_username = users[quest.to_id].username;

            while (check != "|") {
                if (!quest.user_question.empty())
                    quest.user_question += " ";

                quest.user_question += check;

                iss >> check;
            }

            check = "";
            while (check != "|") {
                if (!quest.answer.empty())
                    quest.answer += " ";

                quest.answer += check;

                iss >> check;
            }

            iss >> check >> symbol;
            if (check == "not_anonymous")
                quest.anonymous = false;

            iss >> check >> symbol;
            if (check == "answered")
                quest.answered = true;

            iss >> check;
            if (check == "not_parent") {
                quest.parent = false;

                users[quest.to_id].received_questions[quest.parent_id].thread[id] = quest;
            }

            users[quest.from_id].sent_questions[id] = quest;
            users[quest.to_id].received_questions[id] = quest;
            questions_id = std::max(questions_id, id + 1);
        }

        questions_file.close();

        return true;
    }

    int registration_menu() {
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
            return users_manager::sign_up(users, members, users_id);
        else
            return users_manager::login(members);
    }

    int menu() {
        int choice{ 0 };
        cout << "\nMenu: \n"
            << "     1: Print received questions\n"
            << "     2: Print sent questions\n"
            << "     3: Answer question\n"
            << "     4: Delete question\n"
            << "     5: Ask question\n"
            << "     6: List all users\n"
            << "     7: Display the feed\n"
            << "     8: Logout\n"
            << "     9: End the program\n";

        bool valid{ false };
        do {
            cout << "\nEnter your choice [1 : 9]: ";
            cin >> choice;

            if (1 <= choice && choice <= 9 && !cin.fail())
                valid = true;
            else
                cout << "\nInvalid choice. Please try again\n", cin.clear(), cin.ignore(1000, '\n');
        } while (!valid);

        return choice;
    }

    void start() {
        if (!load_database())
            return;

        current_user_id = registration_menu();

        cout<< "\n**don't forget to close the program properly - from the menu - to prevent data loss**";
        while (true) {
            int choice = menu();

            if (choice == 1)
                questions_manager::print_received_questions(users, current_user_id);
            else if (choice == 2)
                questions_manager::print_sent_questions(users, current_user_id);
            else if (choice == 3)
                questions_manager::answer_question(users, current_user_id);
            else if (choice == 4)
                questions_manager::delete_question(users, current_user_id);
            else if (choice == 5)
                questions_manager::ask_question(users, current_user_id, questions_id);
            else if (choice == 6)
                users_manager::list_users(users);
            else if (choice == 7)
                questions_manager::print_feed(users);
            else if (choice == 8)
                current_user_id = registration_menu();
            else
                return;
        }
    }
};

int main() {
    AskMe_system AskMe_manager;

    AskMe_manager.start();
    
    return 0;
}