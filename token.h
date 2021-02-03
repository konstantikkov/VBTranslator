#pragma once
#include <string>
#include <sstream>
using namespace std;

enum class token_var {
	Integer,
	Boolean,
	String,
	undefined
};

enum class token_word {
	Dim,
	If,
	While,
	ElseIf,
	EndIf,
	EndWhile,
	Writeline,
	undefined
};


enum class token_sign_lg {
	less,
	more,
	equal,
	unequal,
	undefined
};

enum class token_sign_ar {
	plus,
	minus,
	mult,
	div,
	undefined
};


enum class token_operand {
	or_op,
	and_op,
	undefined
};

token_operand tokenize_operand(string temp) {
	if (temp == "AND") {
		return token_operand::and_op;
	}
	else if (temp == "OR") {
		return token_operand::or_op;
	}
	else {
		return token_operand::undefined;
	}
}

token_sign_lg tokenize_sign_lg(string temp) {
	if (temp == "<") {
		return token_sign_lg::less;
	}
	else if (temp == ">") {
		return token_sign_lg::more;
	}
	else if (temp == "=") {
		return token_sign_lg::equal;
	}
	else if (temp == "!=") {
		return token_sign_lg::unequal;
	}
	else {
		return token_sign_lg::undefined;
	}
}

token_sign_ar tokenize_sign_ar(string temp) {
	if (temp == "+") {
		return token_sign_ar::plus;
	}
	else if (temp == "-") {
		return token_sign_ar::minus;
	}
	else if (temp == "*") {
		return token_sign_ar::mult;
	}
	else if (temp == "/") {
		return token_sign_ar::div;
	}
	else {
		return token_sign_ar::undefined;
	}
}

token_word tokenize_word(string temp) {
	if (temp == "Dim") {
		return token_word::Dim;
	}
	else if (temp == "If") {
		return token_word::If;
	}
	else if (temp == "ElseIf") {
		return token_word::ElseIf;
	}
	else if (temp == "While") {
		return token_word::While;
	}
	else if (temp == "EndWhile") {
		return token_word::EndWhile;
	}
	else if (temp == "EndIf") {
		return token_word::EndIf;
	}
	else if (temp.substr(0, 17) == "Console.WriteLine") {
		return token_word::Writeline;
	}
	else {
		return token_word::undefined;
	}
}

token_var tokenize_var(string temp) {
	if (temp == "Integer") {
		return token_var::Integer;
	}
	else if (temp == "Boolean") {
		return token_var::Boolean;
	}
	else if (temp == "String") {
		return token_var::String;
	}
	else {
		return token_var::undefined;
	}
}