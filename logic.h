#pragma once
#include "variable.h"
#include "token.h"
#include <stack>
#include <string>

namespace logic {
	using namespace std;
	class expression {
	public:
		virtual bool evaluate() = 0;
	};

	static stack<expression*> trash;

	static void collect_trash() {
		while (!trash.empty()) {
			delete trash.top();
			trash.pop();
		}
	}

	class sign_ : public expression {
	public:
		sign_(token_sign_lg temp, Variable* lft, Variable* rht) {
			op = temp;
			left = lft;
			right = rht;
			trash.push(this);
		}

		sign_(token_sign_lg temp, Variable* lft, string rht) {
			op = temp;
			left = lft;
			rht_s = rht;
			trash.push(this);
		}

		sign_(token_sign_lg temp, string lft, string rht) {
			op = temp;
			lht_s = lft;
			rht_s = rht;
			trash.push(this);
		}

		bool evaluate() override {
			string temp_l, temp_r;
			(left != nullptr) ? temp_l = left->as_string() : temp_l = lht_s;
			(right != nullptr) ? temp_r = right->as_string() : temp_r = rht_s;
			if (op == token_sign_lg::less) {
				return temp_l < temp_r;
			}
			else if (op == token_sign_lg::more) {
				return temp_l > temp_r;
			}
			else if (op == token_sign_lg::equal) {
				return temp_l == temp_r;
			}
			else if (op == token_sign_lg::unequal) {
				return temp_l != temp_r;
			}
		}
	private:
		token_sign_lg op;
		Variable* left = nullptr;
		string rht_s;
		string lht_s;
		Variable* right = nullptr;
	};

	class Just_bool : public expression {
	public:
		Just_bool(bool temp) {
			result = temp;
			trash.push(this);
		}


		bool evaluate() override {
			return result;
		}
	private:
		bool result;
	};

	class operator_ : public expression {
	public:

		operator_(token_operand temp) {
			op = temp;
			if (temp == token_operand::and_op) {
				level = 2;
			}
			else {
				level = 1;
			}
			trash.push(this);
		}

		bool evaluate() override {
			if (op == token_operand::and_op) {
				return left->evaluate() && right->evaluate();
			}
			else if (op == token_operand::or_op) {
				return left->evaluate() != right->evaluate();
			}
		}

		int level = 0;
		void SetLeft(expression* expression) { left = expression; }
		void SetRight(expression* expression) { right = expression; }

	private:
		token_operand op;
		expression* left;
		expression* right;
	};


	expression* Parse_sign(stringstream& s, map<string, Variable*>& var_map, string temp) {
		try {
			string lft;
			lft = temp;
			map<string, Variable*>::iterator lft_it = var_map.find(lft);
			if (lft_it != var_map.end()) {
				string sym, rht;
				s >> sym;
				if (tokenize_operand(sym) != token_operand::undefined) {
					for (int i = 0; i <= sym.size(); i++) {
						s.unget();
					}
					return new Just_bool(bool(lft_it->second->as_string().c_str()));
				}
				s >> rht;
				if (tokenize_sign_lg(sym) != token_sign_lg::undefined) {
					map<string, Variable*>::iterator rht_it = var_map.find(rht);
					if (lft_it != var_map.end()) {
						if (rht_it != var_map.end()) {
							return new sign_(tokenize_sign_lg(sym), lft_it->second, rht_it->second);
						}
						else {
							if (atoi(rht.c_str()))
								return new sign_(tokenize_sign_lg(sym), lft_it->second, rht);
							else
								throw 3;
						}
					}
					else {
						if (atoi(lft.c_str()) && atoi(rht.c_str()))
							return new sign_(tokenize_sign_lg(sym), lft, rht);
						else
							throw 3;
					}
				}
				else {
					throw 3;
				}
			}
			else if(lft=="0" || lft=="1") {
				return new Just_bool(bool(lft.c_str()));
			}
			throw 3;
		}
		catch (...) {
			throw 3;
		}
	}


	bool solve(stringstream& s, map<string, Variable*>& var_map) {
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

					values.push(new Just_bool(op->evaluate()));
				}
			};

			string temp;
			while (s >> temp) {
				token_operand temp_token = tokenize_operand(temp);
				if (temp_token != token_operand::undefined) {
					s >> temp;
					if (temp_token == token_operand::and_op) {
						PopOps(2);
						ops.push(new operator_(token_operand::and_op));
					}
					else if(temp_token == token_operand::or_op) {
						PopOps(1);
						ops.push(new operator_(token_operand::or_op));
					}
					else {
						throw 3;
					}
					ops.top()->SetLeft(values.top());
					values.push(Parse_sign(s, var_map, temp));
					ops.top()->SetRight(values.top());
				}
				else {
					values.push(Parse_sign(s, var_map, temp));
				}
			}
			while (!ops.empty()) {
				PopOps(0);
			}
			bool result = values.top()->evaluate();
			collect_trash();
			return result;
		}
		catch (...) {
			throw 3;
		}
	}
}