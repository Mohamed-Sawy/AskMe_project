#include "question.h"
#include <stdexcept>

Question::Question() : parent_id{ -1 }, sender_id{ 0 }, receiver_id{ 0 }, anonymous{ true },
				       parent{ true }, answered{ false }, sender_username{ "an anonymous user" }{
}

bool Question::exist(const int& id, const map<int, Question>& mp) const {
	if (mp.find(id) != mp.end())
		return true;

	return false;
}

void Question::validate(const int& id) const {
	if (id <= 0)
		throw std::invalid_argument("\ninvalid id. Please try again.\n");
}

int Question::get_parent_id() const {
	return parent_id;
}

int Question::get_sender_id() const {
	return sender_id;
}

int Question::get_receiver_id() const {
	return receiver_id;
}

bool Question::is_anonymous() const {
	return anonymous;
}

bool Question::is_parent() const {
	return parent;
}

bool Question::is_answered() const {
	return answered;
}

string Question::get_sender_username() const {
	return sender_username;
}

string Question::get_receiver_username() const {
	return receiver_username;
}

string Question::get_user_question() const {
	return user_question;
}

string Question::get_answer() const {
	return answer;
}

const map<int, Question>& Question::get_thread() const {
	return thread;
}

Question& Question::set_parent_id(const int& parent_id) {
	validate(parent_id);

	this->parent_id = parent_id;

	return *this;
}

Question& Question::set_sender_id(const int& sender_id) {
	validate(sender_id);

	this->sender_id = sender_id;

	return *this;
}

Question& Question::set_receiver_id(const int& receiver_id) {
	validate(receiver_id);

	this->receiver_id = receiver_id;

	return *this;
}

Question& Question::set_anonymous(const bool& anonymous) {
	this->anonymous = anonymous;

	return *this;
}

Question& Question::set_parent(const bool& parent) {
	this->parent = parent;

	return *this;
}

Question& Question::set_answered(const bool& answered) {
	this->answered = answered;

	return *this;
}

Question& Question::set_sender_username(const string& sender_username) {
	this->sender_username = sender_username;

	return *this;
}

Question& Question::set_receiver_username(const string& receiver_username) {
	this->receiver_username = receiver_username;
	
	return *this;
}

Question& Question::set_user_question(const string& user_question) {
	this->user_question = user_question;
	
	return *this;
}

Question& Question::set_answer(const string& answer) {
	this->answer = answer;

	return *this;
}

Question& Question::add_thread_question(const int& id, const Question& question) {
	if (exist(id, thread))
		throw std::invalid_argument("\nThis id already exists. Please try again.\n");

	thread[id] = question;
	
	return *this;
}

Question& Question::edit_thread_question(const int& id, const Question& question) {
	if (!exist(id, thread))
		throw std::invalid_argument("\nThis id doesn't exists. Please try again.\n");

	thread[id] = question;

	return *this;
}

Question& Question::remove_thread_question(const int& id) {
	if (!exist(id, thread))
		throw std::invalid_argument("\ninvalid id. Please try again.\n");

	thread.erase(id);

	return *this;
}