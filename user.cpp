#include "user.h"
#include <stdexcept>

User::User() : allow_anonymous{ true } {
}

bool User::exist(const int& id, const map<int, Question>& mp) const {
	if (mp.find(id) != mp.end())
		return true;

	return false;
}

string User::get_username() const {
	return username;
}

string User::get_email() const {
	return email;
}

string User::get_password() const {
	return password;
}

bool User::is_anonymous_allowed() const {
	return allow_anonymous;
}

const map<int, Question>& User::get_received_questions() const {
	return received_questions;
}

const map<int, Question>& User::get_sent_questions() const {
	return sent_questions;
}

User& User::set_username(const string& username) {
	this->username = username;

	return *this;
}

User& User::set_email(const string& email) {
	this->email = email;

	return *this;
}

User& User::set_password(const string& password) {
	this->password = password;

	return *this;
}

User& User::set_anonymous(const bool& anonymous) {
	this->allow_anonymous = anonymous;

	return *this;
}

User& User::add_received_question(const int& id, const Question& question) {
	if (exist(id, received_questions))
		throw std::invalid_argument("\nThis id already exists. Please try again.\n");

	received_questions[id] = question;

	if (!question.is_parent())
		received_questions[question.get_parent_id()].add_thread_question(id, question);
	
	return *this;
}

User& User::add_sent_question(const int& id, const Question& question) {
	if (exist(id, sent_questions))
		throw std::invalid_argument("\nThis id already exists. Please try again.\n");

	sent_questions[id] = question;

	return *this;
}

User& User::edit_received_question(const int& id, const Question& question) {
	if (!exist(id, received_questions))
		throw std::invalid_argument("\ninvalid id. Please try again.\n");

	received_questions[id] = question;

	if (!question.is_parent())
		received_questions[question.get_parent_id()].edit_thread_question(id, question);

	return *this;
}

User& User::edit_sent_question(const int& id, const Question& question) {
	if (!exist(id, sent_questions))
		throw std::invalid_argument("\nThis id already exists. Please try again.\n");

	sent_questions[id] = question;

	return *this;
}

User& User::remove_received_question(const int& id){
	if (!exist(id, received_questions))
		throw std::invalid_argument("\ninvalid id. Please try again.\n");

	if (received_questions[id].is_parent()) {
		const map<int, Question>& thread {received_questions[id].get_thread()};
		for (auto& [thread_id, thread_question] : thread)
			received_questions.erase(thread_id);
	}
	else
		received_questions[received_questions[id].get_parent_id()].remove_thread_question(id);

	received_questions.erase(id);

	return *this;
}

User& User::remove_sent_question(const int& id){
	if (!exist(id, sent_questions))
		throw std::invalid_argument("\ninvalid id. Please try again.\n");

	sent_questions.erase(id);

	return *this;
}