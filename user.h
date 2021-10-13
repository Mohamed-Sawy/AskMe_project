#ifndef USER_H
#define USER_H
#include "question.h"
#include <string>
#include <map>

using std::string;
using std::map;

class User{
private:
    string username;
    string email;
    string password;
    bool allow_anonymous;
    map<int, Question> received_questions;
    map<int, Question> sent_questions;

    bool exist(const int& id, const map<int, Question>& mp) const;
public:
    User();

    string get_username() const;

    string get_email() const;

    string get_password() const;

    bool is_anonymous_allowed() const;

    const map<int, Question>& get_received_questions() const;

    const map<int, Question>& get_sent_questions() const;

    User& set_username(const string& username);

    User& set_email(const string& email);

    User& set_password(const string& password);

    User& set_anonymous(const bool& anonymous);

    User& add_received_question(const int& id, const Question& question);

    User& add_sent_question(const int& id, const Question& question);

    User& edit_received_question(const int& id, const Question& question);

    User& edit_sent_question(const int& id, const Question& question);

    User& remove_received_question(const int& id);

    User& remove_sent_question(const int& id);
};
#endif