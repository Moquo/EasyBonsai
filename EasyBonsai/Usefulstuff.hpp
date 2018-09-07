#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "cppconlib.h"
#include <sys/stat.h>
#include <fstream>
#include <sys/types.h>
#include <regex>

using namespace std;
using namespace conmanip;

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
bool isValid(string input);
bool isEasyJmp(string input);
bool isFormatted(string input);
/////////////////////////////////////////////////////////
vector<string> translate(vector<string> inp);
vector<std::string> split(std::string str, std::string sep);
/////////////////////////////////////////////////////////
string join(const vector<string>& v, char c);
/////////////////////////////////////////////////////////
void format(vector<string> *in);
void JmpOverwrite(vector<string> *in);
void FunctionSystem(vector<string> *in);
void log(string message, console_text_colors color = console_text_colors::white, console_bg_colors bgcolor = console_bg_colors::black, bool endline = true,string prefix = "");
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

//regex validinst(R"rgx(^(jmp .?\d)|(tst .?\d)|(dec .\d)|(inc .?\d)|(hlt)$)rgx");
regex validinst(R"rgx(^(jmp .?\d)|(tst \d)|(dec \d)|(inc \d)|(hlt)$)rgx");
bool isValid(string input) {
	if (regex_match(input, validinst)) {
		return true;
	}
	return false;
}

regex easyjmp(R"rgx(^(jmp \+\d)|(jmp \-\d)$)rgx");
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


void log(string message, console_text_colors color, console_bg_colors bgcolor, bool endline, string prefix) {
	cout
		<< settextcolor(color)
		<< setbgcolor(bgcolor)
		<< (prefix != "" ? "["+prefix+"] " : "")
		<< settextcolor(console_text_colors::white)
		<< setbgcolor(console_bg_colors::black)
		<< message
		<< " " << ((endline == true) ? "\n" : "");
}

std::vector<std::string> split(std::string str, std::string sep) {
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
