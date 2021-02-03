#pragma once
#include<fstream>
#include<iostream>
#include<string>


using namespace std;


class parser {
public:
	virtual bool get_line() = 0;
	virtual int line_num() = 0;
	virtual string& get_string() = 0;
};

class parser_file : public parser {
public:

	parser_file(string file_name) {
		temp_f.open(file_name);
	}

	bool get_line() override {
		line++;
		if (getline(temp_f, cur_s)) {
			return true;
		}
		return false;
	}
	int line_num() override {
		return line;
	}
	string& get_string() override{
		return cur_s;
	}
private:
	int line = 0;
	string cur_s;
	ifstream temp_f;
};

class parser_cin : public parser {
public:
	bool get_line() override {
		line++;
		if (getline(cin, cur_s)) {
			if(cur_s != "EOF")
				return true;
		}
		return false;
	}
	int line_num() override {
		return line;
	}
	string& get_string() override {
		return cur_s;
	}
private:
	int line = 0;
	string cur_s;
};

parser* parser_choose(string temp) {
	if (temp == "console") {
		return new parser_cin();
	}
	else if (temp == "file"){
		cin >> temp;
		return new parser_file(temp);
	}
	else {
		return nullptr;
	}
}