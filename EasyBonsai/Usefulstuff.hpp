#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "termcolor.hpp"
#include <sys/stat.h>
#include <fstream>
#include <sys/types.h>
#include <regex>

using namespace std;

enum Colors {
	RED, GREEN, YELLOW
};

bool isValid(string input);
bool isMarked(string input);
bool isEasyJmp(string input);
bool isFuncJmp(string input);
bool isFormatted(string input);
bool isZeroAllocation(string input);
bool isNormalAllocation(string input);

vector<string> translate(vector<string> inp);
vector<string> split(string str, string sep);
vector<vector<string>> getZeroAllocation(int back, int line, string first);
vector<vector<string>> getNormalAllocation(int back, int line/*(*in).size()*/, string first, string second, string help);

string join(const vector<string>& v, char c);

void format(vector<string> *in);
void JmpOverwrite(vector<string> *in);
void FunctionSystem(vector<string> *in);
void log(string message, Colors color, bool endline = true, string prefix = "");

//Basic Regex for instruction recognition.
regex validinst(R"rgx(^(.?:? ?)((jmp [+-]?\d+)|(jmp [a-z])|(tst \d)|(dec \d)|(inc \d)|(hlt)|(\[\d=zero\])|(\[\d=\d\#\d\]))$)rgx");
bool isValid(string input) {
	if (regex_match(input, validinst)) {
		return true;
	}
	return false;
}

regex easyjmp(R"rgx(^(jmp \+\d+)|(jmp \-\d+)$)rgx");
bool isEasyJmp(string input) {
	if (regex_match(input, easyjmp)) {
		return true;
	}
	return false;
}

regex _format(R"rgx(^(\d: [a-zA-Z0-9 ]*$)$)rgx");
bool isFormatted(string input) {
	if (regex_match(input, _format)) {
		return true;
	}
	return false;
}

regex funcmark(R"rgx(^(.: ).+$)rgx");
bool isMarked(string input) {
	if (regex_match(input, funcmark)) {
		return true;
	}
	return false;
}

regex jmpfunc(R"rgx(^(.+)?(jmp [a-z])$)rgx");
bool isFuncJmp(string input) {
	if (regex_match(input, jmpfunc)) {
		return true;
	}
	return false;
}

regex zallocation(R"rgx(^(\[\d=zero\])$)rgx");
bool isZeroAllocation(string input) {
	if (regex_match(input, zallocation)) {
		return true;
	}
	return false;
}

regex normalallocation(R"rgx(^(\[\d=\d\#\d\])$)rgx");
bool isNormalAllocation(string input) {
	if (regex_match(input, normalallocation)) {
		return true;
	}
	return false;
}

void log(string message, Colors color, bool endline, string prefix) {
	if(color == Colors::RED)
		cout << termcolor::red 
			<< (prefix != "" ? "[" + prefix + "] " : "")
			<< termcolor::reset
			<< message
			<< " " << ((endline == true) ? "\n" : "");
	if (color == Colors::GREEN)
		cout << termcolor::green
		<< (prefix != "" ? "[" + prefix + "] " : "")
		<< termcolor::reset
		<< message
		<< " " << ((endline == true) ? "\n" : "");
	if (color == Colors::YELLOW)
		cout << termcolor::yellow
		<< (prefix != "" ? "[" + prefix + "] " : "")
		<< termcolor::reset
		<< message
		<< " " << ((endline == true) ? "\n" : "");
}


vector<vector<string>> getZeroAllocation(int back, int line/*(*in).size()*/, string first) {
	vector<vector<string>> rtn;
	string sline = ::to_string(line);
	rtn.push_back({ string("jmp " + sline) });
	vector<string> func;
	func.push_back("tst " + first); //Line Number of this is sLine
	func.push_back("jmp " + ::to_string(line + 3)); //Jump to dec;
	func.push_back("jmp " + ::to_string(back + 1)); //Jump back to initial line + 1 cuz we done with that
	func.push_back("dec " + first);
	func.push_back("jmp " + sline);

	rtn.push_back(func);

	return rtn;
}
vector<vector<string>> getNormalAllocation(int back, int line/*(*in).size()*/, string first, string second, string help) {
	vector<vector<string>> rtn;
	string sline = ::to_string(line);
	rtn.push_back({ string("jmp " + sline) });
	vector<string> func;
	//Im not even going to try to commentate this shit properly...
	/*0*/func.push_back("tst " + second); //TST Y
	/*1*/func.push_back("jmp " + ::to_string(line + 3)); //JMP +2
	/*2*/func.push_back("jmp " + ::to_string(line + 5)); //JMP +3
	/*3*/func.push_back("dec " + second); //DEC Y
	/*4*/func.push_back("jmp " + sline); //JMP 0
	/*5*/func.push_back("tst " + help); //TST H
	/*6*/func.push_back("jmp " + ::to_string(line + 8)); //JMP +2
	/*7*/func.push_back("jmp " + ::to_string(line + 10)); //JMP +3
	/*8*/func.push_back("dec " + help); //DEC H
	/*9*/func.push_back("jmp " + ::to_string(line + 5)); //JMP -3
	/*10*/func.push_back("tst " + first); //TST X
	/*11*/func.push_back("jmp " + ::to_string(line + 13)); //JMP +2
	/*12*/func.push_back("jmp " + ::to_string(line + 17)); //JMP +5
	/*13*/func.push_back("dec " + first); //DEC X
	/*14*/func.push_back("inc " + second); // INC Y
	/*15*/func.push_back("inc " + help); // INC H
	/*16*/func.push_back("jmp " + ::to_string(line+10)); // JMP -6 (BEGINNING)
	/*17*/func.push_back("tst " + help); // TST H
	/*18*/func.push_back("jmp " + ::to_string(line + 20)); // JMP +2
	/*19*/func.push_back("jmp " + ::to_string(back + 1)); // JMP +4 (OUTSIDE OF FUNCTION)
	/*20*/func.push_back("dec " + help); // DEC H
	/*21*/func.push_back("inc " + first); // INC X
	/*22*/func.push_back("jmp " + ::to_string(line + 17)); // JMP -5
	rtn.push_back(func);
	return rtn;
}

//Stole those from Stackoverflow :^)
vector<string> split(string str, string sep) {
	char* cstr = const_cast<char*>(str.c_str());
	char* current;
	std::vector<std::string> arr;
	current = strtok(cstr, sep.c_str());
	while (current != NULL) {
		arr.push_back(current);
		current = strtok(NULL, sep.c_str());
	}
	return arr;
}
string join(const vector<string>& v, char c) {
	string s;
	s.clear();
	for (vector<string>::const_iterator p = v.begin();
		p != v.end(); ++p) {
		s += *p;
		if (p != v.end() - 1)
			s += c;
	}
	return s;
}