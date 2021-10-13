#ifndef QUESTIONS_MANAGER_H
#define QUESTIONS_MANAGER_H
#include "question.h"
#include "user.h"
using std::string;
using std::map;

class QuestionsManager {
private:
    enum question_modes { RECEIVED, SENT, FEED };
    static QuestionsManager questions_manager;

    QuestionsManager() = default;
    
    void print_question(const int& question_id, const Question& question, const int& mode) const;
public:
    QuestionsManager(const QuestionsManager&) = delete;
    QuestionsManager& operator = (const QuestionsManager&) = delete;

    static const QuestionsManager& get_manager();

    void ask_question(map<int, User>& users, const int& current_user_id, int& questions_id) const;

    void answer_question(map<int, User>& users, const int& current_user_id) const;

    void delete_question(map<int, User>& users, const int& current_user_id) const;

    void print_received_questions(map<int, User>& users, const int& current_user_id) const;

    void print_sent_questions(map<int, User>& users, const int& current_user_id) const;

    void print_feed(const map<int, User>& users) const;
};
#endif