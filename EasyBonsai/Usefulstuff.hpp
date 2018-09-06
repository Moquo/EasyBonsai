#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <vector>
#include <iostream>
#include "cppconlib.h"
#include <sys/stat.h>
#include <fstream>
#include <sys/types.h>
#include <regex>

using namespace std;
using namespace conmanip;

vector<string> translate(vector<string> inp) {

	vector<string> t_code;


	return t_code;
}

void log(string message, console_text_colors color = console_text_colors::white, console_bg_colors bgcolor = console_bg_colors::black, bool endline = true);
void log(string message, console_text_colors color, console_bg_colors bgcolor, bool endline) {

	cout
		<< settextcolor(color)
		<< setbgcolor(bgcolor)
		<< "[EasyBonsai] >> " << message
		<< settextcolor(console_text_colors::white)
		<< setbgcolor(console_bg_colors::black)
		<< " " << ((endline == true) ? "\n" : "");

}
std::vector<std::string> split(std::string str, std::string sep);
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

regex validinst( R"rgx(^(jmp .?\d)|(tst .?\d)|(dec .\d)|(inc .?\d)|(hlt)$)rgx" );
bool isValid(string input);
bool isValid(string input) {
	if (regex_match(input, validinst)) {
		return true;
	}
	return false;
}

string join(const vector<string>& v, char c);
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
