#pragma once
#include<map>
#include<string>
#include<vector>
#include<stack>
#include<sstream>
#include "variable.h"
#include "token.h"
#include "arithm.h"
#include "logic.h"

using namespace std;

namespace word {
	static map<string, Variable*> var_map;
	static stack<pair<bool, bool>> skip;
	static int vision;
	static int current_line;

	class word_structure {
	public:
		virtual vector<word_structure*>::iterator evaluate(vector<word_structure*>::iterator temp_it) = 0;
	};

	static vector<word_structure*> word_stack;
	static stack<pair<vector<word_structure*>::iterator, vector<word_structure*>::iterator>> while_it;

	class Equation : public word_structure {
	public:
		Equation(string new_left, string new_equation) {
			left = new_left;
			equation_s = new_equation;
		}
		vector<word_structure*>::iterator evaluate(vector<word_structure*>::iterator temp_it) override {
			try {
				if (skip.top().first) {
					return next(temp_it);
				}
				stringstream s;
				s << equation_s;
				auto lft = var_map.find(left)->second;
				if (dynamic_cast<Integer*>(lft) != nullptr) {
					dynamic_cast<Integer*>(lft)->value = arithm::solve(s, var_map);
				}
				else if (dynamic_cast<Boolean*>(lft) != nullptr) {
					dynamic_cast<Boolean*>(lft)->value = logic::solve(s, var_map);
				}
				else {
					throw 7;
				}
				return next(temp_it);
			}
			catch (int exception_code) {
				throw exception_code;
			}
		}

	private:
		string left;
		string equation_s;
	};

	class Dim : public word_structure {
	public:
		Dim(string s) {
			temp_s = s;
		}
		vector<word_structure*>::iterator evaluate(vector<word_structure*>::iterator temp_it) override {
			vision_ = vision;
			if (!skip.top().first) {
				stringstream s;
				s << temp_s;
				string name, temp;
				s >> name;
				s >> temp;
				new_variable(var_map, s, name, vision_);
				if (s >> temp) {
					string temp_s_e;
					getline(s, temp_s_e);
					Equation(name, temp_s_e).evaluate(temp_it);
				}
			}
			return next(temp_it);
		}
		int vision_;
	private:
		string temp_s;
	};


	class EndIf : public word_structure {
	public:
		EndIf() {
		}
		vector<word_structure*>::iterator evaluate(vector<word_structure*>::iterator temp_it) override {
			skip.pop();
			return next(temp_it);
		}
	};

	class EndWhile : public word_structure {
	public:
		EndWhile() {
		};
		vector<word_structure*>::iterator evaluate(vector<word_structure*>::iterator temp_it) override {
			
			if (skip.top().first) {
				skip.pop();
				while_it.pop();
				return next(temp_it);
			}
			skip.pop();
			while_it.top().second = temp_it;
			return while_it.top().first;
		}
	};

	class If : public word_structure {
	public:
		If(string new_cond, bool mode) {
			condition = new_cond;
			else_ = mode;
		}
		vector<word_structure*>::iterator evaluate(vector<word_structure*>::iterator temp_it) override {
			if (!else_) {
				if (skip.top().first) {
					skip.push({ true, true });
				}
				else {
					stringstream s;
					s << condition;
					bool equal_res = false;
					equal_res = logic::solve(s, var_map);
					skip.push({ !equal_res, equal_res });
				}
			}
			else {
				if (skip.top().second) {
					skip.top().first = true;
					return next(temp_it);
				}
				if (skip.top().first && !(skip.top().second)) {
					stringstream s;
					s << condition;
					bool equal_res = false;
					equal_res = logic::solve(s, var_map);
					skip.top() = { !equal_res, equal_res };
				}
			}
			return next(temp_it);
		}
		bool else_ = false;
	private:
		string condition;
	};

	class While : public word_structure {
	public:
		While(string new_cond){
			condition = new_cond;
		}
		vector<word_structure*>::iterator evaluate(vector<word_structure*>::iterator temp_it) override {
			if (skip.top().first)
			{
				while_it.push({ temp_it, word_stack.end() });
				skip.push({ true, true });
				return next(temp_it);
			}
			if (temp_it != while_it.top().first) {
				while_it.push({ temp_it, word_stack.end() });
			}
			stringstream s;
			s << condition;
			if (logic::solve(s, var_map)) {
				skip.push({ false, true });
				return next(temp_it);
			}
			else {
				skip.push({ true, true });
				return while_it.top().second;
			}
			return next(temp_it);
		}
	private:
		string condition;
	};

	class Write : public word_structure {
	public:
		Write(string temp_s) {
			s = temp_s;
		}
		vector<word_structure*>::iterator evaluate(vector<word_structure*>::iterator temp_it) override {
			if (!skip.top().first)
				cout << s.substr(19, s.size()-21) << '\n';
			return next(temp_it);
		}
	private:
		string s;
	};

}

// Удаление выделенной стековой памяти
void vision_clear() {
	for (auto i = word::var_map.begin(); i != word::var_map.end();) {
		if (i->second->get_vision() > word::vision) {
			auto temp_it = i;
			i = next(i);
			delete temp_it->second;
			word::var_map.erase(temp_it);
		}
		else {
			i = next(i);
		}
	}
}