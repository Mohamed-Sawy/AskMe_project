#include "questions_manager.h"
#include <iostream>

using std::cout;
using std::cin;

QuestionsManager QuestionsManager::questions_manager;

void QuestionsManager::print_question(const int& question_id, const Question& question, const int& mode) const {
    if (!question.is_parent()) {
        if (mode == SENT || mode == FEED)
            cout << "        Thread to question (" << question.get_parent_id() << "):   ";
        else
            cout << "        Thread:   ";
    }

    cout << "Question id (" << question_id << ") ";

    if (mode == FEED)
        cout << "from " << question.get_sender_username() << " to " << question.get_receiver_username();
    else if (mode == RECEIVED)
        cout << "from " << question.get_sender_username();
    else if (mode == SENT)
        cout << "to " << question.get_receiver_username();

    cout << ": [" << question.get_user_question() << "]\n";

    if (question.is_answered())
        cout << "        Answer: " << question.get_answer() << "\n";
    else
        cout << "        -Not answered yet-\n";

    cout << "\n";
}

const QuestionsManager& QuestionsManager::get_manager() {
    return questions_manager;
}

void QuestionsManager::ask_question(map<int, User>& users, const int& sender_id, int& questions_id) const {
    Question new_question;

    int receiver_id{ 0 };
    bool valid{ false };
    do {
        cout << "\nEnter a user id to ask or -1 to cancel: ";
        cin >> receiver_id;

        if (receiver_id == -1)
            return;

        if (receiver_id == sender_id || users.find(receiver_id) == users.end() || cin.fail())
            cin.clear(), cin.ignore(1000, '\n'), cout << "\nInvalid id. Please try again.\n";
        else
            valid = true;
    } while (!valid);

    bool anonymous{ false };
    if (users[receiver_id].is_anonymous_allowed()) {
        string choice;
        valid = false;
        do {
            cout << "\nDo you want to ask this question anonymously? [Y/N]: ";
            cin >> choice;

            if (choice != "Y" && choice != "N")
                cout << "\nThis is not a valid input. Please try again.";
            else {
                (choice == "Y") ? anonymous = true : anonymous = false;
                valid = true;
            }
        } while (!valid);
    }
    else
        cout << "\nNote: anonymous questions are not allowed for this user\n";

    int parent_id{ 0 };
    valid = false;
    do {
        cout << "\nEnter the main question id for a thread question or -1 for a new question: ";
        cin >> parent_id;
        
        if (parent_id == -1)
            break;

        const map<int, Question>& receiver_questions = users[receiver_id].get_received_questions();
        if (receiver_questions.find(parent_id) == receiver_questions.end() || !receiver_questions.at(parent_id).is_parent() || cin.fail())
            cin.clear(), cin.ignore(1000, '\n'), cout << "\nThere's no main question with this id for that user. Please try again.\n";
        else
            valid = true;

    } while (!valid);

    string question;
    cout << "\nEnter your question: ";
    cin.clear(), cin.ignore();
    getline(cin, question);

    string receiver_username{ users[receiver_id].get_username() };

    new_question.set_user_question(question).set_sender_id(sender_id).set_receiver_id(receiver_id).set_receiver_username(receiver_username);

    if (!anonymous){
        string sender_username{ users[sender_id].get_username() };
        new_question.set_anonymous(false).set_sender_username(sender_username);
    }

    if (parent_id != -1)
        new_question.set_parent(false).set_parent_id(parent_id);

    users[sender_id].add_sent_question(questions_id, new_question);
    users[receiver_id].add_received_question(questions_id, new_question);

    questions_id++;
}

void QuestionsManager::answer_question(map<int, User>&users, const int& current_user_id) const {
    const map<int, Question>& received_questions = users[current_user_id].get_received_questions();

    if (received_questions.empty()) {
        cout << "\nYou haven't received any questions to answer yet.\n";
        return;
    }

    int question_id{ 0 };
    bool valid{ false };
    do {
        cout << "\nEnter Question id or -1 to cancel: ";
        cin >> question_id;

        if (question_id == -1)
            return;

        if (received_questions.find(question_id) == received_questions.end() || cin.fail())
            cin.clear(), cin.ignore(1000, '\n'), cout << "\nThere's no question with this id. Please try again.\n";
        else
            valid = true;
    } while (!valid);

    cout << "\n";

    Question modified_question{ received_questions.at(question_id)};

    print_question(question_id, modified_question, RECEIVED);

    if (modified_question.is_answered())
        cout << "Warning: This question is already answered. Answer will be updated.\n";

    string answer;
    cout << "Enter your answer: ";
    cin.clear(), cin.ignore();
    getline(cin, answer);

    modified_question.set_answered(true).set_answer(answer);

    users[current_user_id].edit_received_question(question_id, modified_question);
    users[modified_question.get_sender_id()].edit_sent_question(question_id, modified_question);

    cout << "\n";
}

void QuestionsManager::delete_question(map<int, User>& users, const int& current_user_id) const {
    const map<int, Question>& received_questions{ users[current_user_id].get_received_questions() };

    if (received_questions.empty()) {
        cout << "\nYou haven't received any questions that you can delete yet.\n";
        return;
    }

    int question_id{ 0 };
    bool valid{ false };
    do {
        cout << "\nEnter Question id or -1 to cancel: ";
        cin >> question_id;

        if (question_id == -1)
            return;

        if (received_questions.find(question_id) == received_questions.end() || cin.fail())
            cin.clear(), cin.ignore(1000, '\n'), cout << "\nThere's no question with this id for this user. Please try again.\n";
        else
            valid = true;
    } while (!valid);

    if (received_questions.at(question_id).is_parent()) {
        const map<int, Question>& thread{ received_questions.at(question_id).get_thread() };
        for (auto& [thread_id, thread_question] : thread)
            users[thread_question.get_sender_id()].remove_sent_question(thread_id);
    }

    users[received_questions.at(question_id).get_sender_id()].remove_sent_question(question_id);
    users[current_user_id].remove_received_question(question_id);

    cout << "\nQuestion deleted successfully.\n";
}

void QuestionsManager::print_received_questions(map<int, User>& users, const int& current_user_id) const {
    cout << "\n";

    const map<int, Question>& received_questions{ users[current_user_id].get_received_questions() };
    if (received_questions.empty()) {
        cout << "You haven't received any questions yet.\n";
        return;
    }

    for (const auto& [question_id, question] : received_questions) {
        if (question.is_parent()) {
            print_question(question_id, question, RECEIVED);

            const map<int, Question>& thread = question.get_thread();
            for (const auto& [thread_id, thread_question] : thread)
                print_question(thread_id, thread_question, RECEIVED);
        }
    }

    cout << "\n";
}

void QuestionsManager::print_sent_questions(map<int, User>& users, const int& current_user_id) const {
    cout << "\n";

    const map<int, Question>& sent_questions = users[current_user_id].get_sent_questions();
    if (sent_questions.empty()) {
        cout << "You haven't sent any questions yet.\n";
        return;
    }

    for (const auto& [question_id, question] : sent_questions)
        print_question(question_id, question, SENT);

    cout << "\n";
}

void QuestionsManager::print_feed(const map<int, User>& users) const {
    map<int, Question, std::greater<int>> total_questions;
    for (const auto& [user_id, user] : users) {
        const map<int, Question>& received_questions{ user.get_received_questions() };
        for (const auto& [question_id, question] : received_questions)
            if (question.is_answered())
                total_questions[question_id] = question;
    }
    cout << "\n";


    if (total_questions.empty()) {
        cout << "There's no questions at the moment.\n";
        return;
    }

    for (const auto& [question_id, question] : total_questions)
        print_question(question_id, question, FEED);
}