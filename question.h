#ifndef QUESTION_H
#define QUESTION_H
#include <string>
#include <map>

using std::string;
using std::map;

class Question {
private:
    int parent_id;
    int sender_id;
    int receiver_id;
    bool anonymous;
    bool parent;
    bool answered;
    string sender_username;
    string receiver_username;
    string user_question;
    string answer;
    map<int, Question> thread;

    bool exist(const int& id, const map<int, Question>& mp) const;
    void validate(const int& id) const;
public:
    Question();

    int get_parent_id() const;

    int get_sender_id() const;

    int get_receiver_id() const;

    bool is_anonymous() const;

    bool is_parent() const;

    bool is_answered() const;

    string get_sender_username() const;

    string get_receiver_username() const;

    string get_user_question() const;

    string get_answer() const;

    const map<int, Question>& get_thread() const;

    Question& set_parent_id(const int& parent_id);

    Question& set_sender_id(const int& sender_id);

    Question& set_receiver_id(const int& receiver_id);

    Question& set_anonymous(const bool& anonymous);

    Question& set_parent(const bool& parent);

    Question& set_answered(const bool& answered);

    Question& set_sender_username(const string& sender_username);

    Question& set_receiver_username(const string& receiver_username);

    Question& set_user_question(const string& user_question);

    Question& set_answer(const string& answer);

    Question& add_thread_question(const int& id, const Question& question);

    Question& edit_thread_question(const int& id, const Question& question);

    Question& remove_thread_question(const int& id);
};
#endif