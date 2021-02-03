#include "token.h"
#include "word_structure.h"
#include "parser.h"
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

int main() {
	try {
	string temp;
	cin >> temp;
	parser* parser_ = parser_choose(temp);
	if (parser_ == nullptr)
		throw 6;
	word::skip.push({ false, false });
	word::vision = 0;
	word::word_stack;
	word::current_line = 0;
	word::vision = word::skip.size();
		while (parser_->get_line())
		{
			word::current_line++;
			stringstream str_s;
			str_s << parser_->get_string();

			if (str_s >> temp) {
				token_word c = tokenize_word(temp);
				switch (c) {
				case token_word::Dim:
				{
					string temp_s;
					getline(str_s, temp_s);
					word::word_stack.push_back(new word::Dim(temp_s));
				}
				break;
				case token_word::If:
				{
					string temp_s;
					getline(str_s, temp_s);
					word::word_stack.push_back(new word::If(temp_s.substr(0, temp_s.size() - 5), false));
				}
				break;

				case token_word::Writeline:
				{
					string temp_s_w;
					getline(str_s, temp_s_w);
					word::word_stack.push_back(new word::Write(temp + temp_s_w));
				}
				break;

				case token_word::EndIf:
					word::word_stack.push_back(new word::EndIf());
					break;

				case token_word::EndWhile:
					word::word_stack.push_back(new word::EndWhile());
					break;

				case token_word::While:
				{
					string temp_s;
					getline(str_s, temp_s);
					word::word_stack.push_back(new word::While(temp_s));
				}
				break;

				case token_word::ElseIf:
				{
					string temp_s;
					getline(str_s, temp_s);
					word::word_stack.push_back(new word::If(temp_s.substr(0, temp_s.size() - 5), true));
				}
				break;

				case token_word::undefined:
				{
					string temp_s;
					str_s >> temp_s;
					getline(str_s, temp_s);
					if (temp_s.find('=')) {
						word::word_stack.push_back(new word::Equation(temp, temp_s));
					}
					else {
						throw 1;
					}
				}
				break;
				}
			}
		}
		word::var_map;
		word::while_it.push({ word::word_stack.end(), word::word_stack.end() });
		word::current_line = 0;
		for (auto i = word::word_stack.begin(); i != word::word_stack.end();) {
			try {
				i = (*i)->evaluate(i);
			}
			catch (int exception_code) {
				throw exception_code;
			}
			catch (...) {
				throw 5;
			}
			word::vision = word::skip.size();
			word::current_line = i - word::word_stack.begin();
			vision_clear();
		}
		system("pause");
		return 0;
	}
	catch (int exception_code) {
		switch (exception_code) {
		case 1:
			cout << "Undefined token. ";
			break;
		case 2:
			cout << "Error: variable intialization ";
			break;
		case 3:
			cout << "Error: logic exprassion ";
			break;
		case 4:
			cout << "Error: arithmetic expression ";
			break;
		case 5:
			cout << "Error: evaluating ";
			break;
		case 6:
			cout << "Error of input stream\n";
			return 0;
		}
			cout << "line number: " << to_string(word::current_line) << '\n';
		system("pause");
	}
}