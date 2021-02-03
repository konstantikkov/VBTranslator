#pragma once
#include "token.h"
#include <string>
#include <iostream>
#include <map>
using namespace std;


class Variable {
public:
	virtual std::string get_name() = 0;
	virtual void get_value(std::stringstream& s_str) = 0;
	virtual token_var get_type() = 0;
	virtual string as_string() = 0;
	virtual int get_vision() = 0;
};


class Integer : public Variable {
public:

	Integer(std::string new_name, int vis) {
		name = new_name;
		vision = vis;
	}

	Integer(std::string new_name, int vis, int value_) {
		name = new_name;
		vision = vis;
		value = value_;
	}

	friend std::istream& operator>> (std::istream& s, Integer& temp) {
		s >> temp.value;
		return s;
	}
	friend std::ostream& operator<< (std::ostream& s, Integer& temp) {
		s << temp.value;
		return s;
	}
	std::string get_name() override {
		return name;
	}
	void get_value(std::stringstream& s_str) override {
		s_str << value;
	}

	int get_vision() override {
		return vision;
	}

	inline token_var get_type() override { return token_var::Integer; };
	inline string as_string() override { return to_string(value); };
	int value = 0;
private:
	int vision = 0;
	std::string name;
};


class String : public Variable {
public:

	String(std::string new_name, int vis) {
		name = new_name;
		vision = vis;
	}

	friend std::istream& operator>> (std::istream& s, String& temp) {
		s >> temp.value;
		return s;
	}
	friend std::ostream& operator<< (std::ostream& s, String& temp) {
		s << temp.value;
		return s;
	}
	std::string get_name() override {
		return name;
	}

	void get_value(std::stringstream& s_str) override {
		s_str << value;
	}

	int get_vision() override {
		return vision;
	}

	inline token_var get_type() override { return token_var::String; };
	inline string as_string() override { return value; };
	std::string value;
private:
	int vision = 0;
	std::string name;
};


class Boolean : public Variable {
public:

	Boolean(std::string new_name, int vis) {
		name = new_name;
		vision = vis;
	}


	Boolean(std::string new_name, int vis, int value_) {
		name = new_name;
		vision = vis;
		value = value_;
	}

	friend std::istream& operator>> (std::istream& s, Boolean& temp) {
		s >> temp.value;
		return s;
	}
	friend std::ostream& operator<< (std::ostream& s, Boolean& temp) {
		s << temp.value;
		return s;
	}
	std::string get_name() override {
		return name;
	}

	void get_value(std::stringstream& s_str) override {
		s_str << value;
	}

	int get_vision() override {
		return vision;
	}

	inline token_var get_type() override { return token_var::Boolean; };
	inline string as_string() override { return to_string(value); };
	bool value = 0;
private:
	int vision = 0;
	std::string name;
};

void new_variable(map<string, Variable*>& variable_map, stringstream& s, string& name, int vision) {
	try {
		string temp;
		s >> temp;
		switch (tokenize_var(temp)) {
		case token_var::Integer:
			variable_map.insert({ name, new Integer(name, vision) });
			break;
		case token_var::Boolean:
			variable_map.insert({ name, new Boolean(name, vision) });
			break;
		case token_var::String:
			variable_map.insert({ name, new String(name, vision) });
			break;
		default:
			throw 2;
			break;
		}
	}
	catch (int exception_code) {
		throw exception_code;
	}
}
