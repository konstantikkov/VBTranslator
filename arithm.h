#pragma once
#include "token.h"
#include "variable.h"
#include <stack>
#include <map>
using namespace std;

namespace arithm {

	class expression {
	public:
		virtual int evaluate() = 0;
	};
	
	static stack<expression*> trash;

	static void collect_trash() {
		while (!trash.empty()) {
			delete trash.top();
			trash.pop();
		}
	}

	class value_ : public expression {
	public:
		value_(int val) : value(val) {
			trash.push(this);
		}

		int evaluate() {
			return value;
		}

	private:
		int value;
	};

	class variable_ : public expression {
	public:
		variable_(Variable* val) :value(val) {
			trash.push(this);
		}

		int evaluate() {
			return atoi(value->as_string().c_str());
		}

	private:
		Variable* value;
	};

	class operator_ : public expression {
	public:
		operator_(token_sign_ar temp_s) :sign(temp_s) {
			if (temp_s == token_sign_ar::mult || temp_s == token_sign_ar::div) {
				level = 2;
			}
			else {
				level = 1;
			}
			trash.push(this);
		}
		int evaluate() {
			switch (sign) {
			case token_sign_ar::plus:
				return left->evaluate() + right->evaluate();
				break;
			case token_sign_ar::minus:
				return left->evaluate() - right->evaluate();
				break;
			case token_sign_ar::mult:
				return left->evaluate() * right->evaluate();
				break;
			case token_sign_ar::div:
				return left->evaluate() / right->evaluate();
				break;
			}
		}



		void SetLeft(expression* node) { left = node; }
		void SetRight(expression* node) { right = node; }

		int level = 0;
	private:
		token_sign_ar sign;
		expression* left;
		expression* right;
	};


	int solve(stringstream& s, map<string, Variable*>& var_map) {
		try {
			stack<expression*> values;
			stack<operator_*> ops;

			auto PopOps = [&](int precedence) {
				while (!ops.empty() && ops.top()->level >= precedence) {
					auto value1 = values.top();
					values.pop();
					auto value2 = values.top();
					values.pop();
					auto op = ops.top();
					ops.pop();

					op->SetRight(value1);
					op->SetLeft(value2);

					values.push(op);
				}
			};

			string temp;
			while (s >> temp) {
				map<string, Variable*>::iterator temp_it = var_map.find(temp);
				if (temp_it != var_map.end()) {
					values.push(new variable_(temp_it->second));
				}
				else if (tokenize_sign_ar(temp) != token_sign_ar::undefined) {
					if (temp == "*" || temp == "/") {
						PopOps(2);
					}
					else {
						PopOps(1);
					}
					if (tokenize_sign_ar(temp) != token_sign_ar::undefined)
						ops.push(new operator_(tokenize_sign_ar(temp)));
					else
						throw 4;
				}
				else if(atoi(temp.c_str()) || temp=="0"){
					values.push(new value_(atoi(temp.c_str())));
				}
				else {
					throw 4;
				}
			}
			while (!ops.empty()) {
				PopOps(0);
			}

			int result = values.top()->evaluate();
			collect_trash();

			return result;
		}
		catch (...) {
			throw 4;
		}
	}
}