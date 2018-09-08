#include "Usefulstuff.hpp"

bool strict = false;
bool simple = false;
bool hidelinenumbers = false;
int main(int argc, char* args[])
{
	if (argc < 2) { //Check if there is exactly one start argument (file).
		log("Argument Error, expected Arguments: fileName (--strict) (--hidelinenumbers) (--simple) (--output:File)", Colors::RED, false, "Error");
		return 0;
	}
	string outputfile;
	regex outputvalid(R"rgx(^(--output:)[a-zA-Z-0-1._]+$)rgx");
	if (argc > 2) {
		for (int i = 2; argc > i; i++) {
			if ((string)args[i] == "--strict")
				strict = true;
			if ((string)args[i] == "--hidelinenumbers")
				hidelinenumbers = true;
			if ((string)args[i] == "--simple")
				simple = true;
			if (regex_match(string(args[i]), outputvalid))
				outputfile = split(string(args[i]), ":")[1];
		}
	}
	struct stat info; //Needed for detecting wether file exists or not.
	if (stat(args[1], &info) == 0) {
		ifstream in(args[1]);
		string buff;
		vector<string> input;
		while (getline(in, buff)) {
			input.push_back(buff);
		} //Read File line by line.

		if (!simple) {
			log("Read File: " + (string)args[1], Colors::GREEN, true, "Success");
			log("Invalid Instructions will be ignored, so you can freely comment your code!", Colors::YELLOW, true, "Info");
			log("Translating Code", Colors::GREEN, true, "Started");
			cout << endl;
		}

		vector<string> builtcode; //Array which will hold the code which we will later translate.
		//for (auto x : input) { // We will output the code here, so the user knows which lines we will use or not use.
		for (int i = 0; input.size() > i; i++) {
			auto x = input[i];
			if (isValid(x)) { //Check if Line is valid/allowed instruction.
				builtcode.push_back(x);
				if (!simple) cout << x << endl;
			}
			else
			{
				if (strncmp(x.c_str(), "//", 2) != 0) //If Line starts with // we wont spam the user with red error messages and just ignore it.
					if (!simple) log(x, Colors::RED, true, "Invalid");
				if (strict) {
					log("Invalid Instruction in line " + ::to_string(i), Colors::RED, true, "Fatal");
					log("Translation aborted due to error", Colors::RED, true, "Fatal");
					return 0;
				}
			}
		}
		cout << endl;
		vector<string> bonsaicode = translate(builtcode); //Translate the Code and output it.
		log("Translated", Colors::GREEN, true, "Success");
		cout << endl << join(bonsaicode, '\n') << endl;
		cout << endl;
		if (outputfile.size() > 0) {
			ofstream of(outputfile);
			for (auto x : bonsaicode) {
				of << x << endl;
			}
		}
	}
	else
	{
		log("Could not find file or directory: " + (string)args[1], Colors::RED, "Error");
	}
	return 0;
}

void format(vector<string> *in) {
	for (int i = 0; (*in).size() > i; i++) { //Loop through the code lines
		if (isMarked((*in)[i])) //If Line starts with "x: " then remove the beginning because this is no longer needed.
			(*in)[i] = (*in)[i].substr(3); // This skips "x: "
		if (!isFormatted(((*in)[i])) && !hidelinenumbers) //If Line is not formatted, format it... (Include Line Numbers)
			(*in)[i] = std::string(::to_string(i) + ": " + (*in)[i]); //This add the line number infront of the line
	}
}
void JmpOverwrite(vector<string> *in) {
	for (int i = 0; (*in).size() > i; i++) {
		if (isEasyJmp((*in)[i])) { //If Jmp goes something like this: jmp +2 then redirect it to the right line.
			if (strstr((*in)[i].c_str(), "-")) {  //Jmp back
				string afterminus = split(string((*in)[i]), "-")[1]; //Get the Amount of lines we want to skip
				int parsedskip = std::atoi(afterminus.c_str()); //Convert the Amount to an int.
				(*in)[i] = std::string("jmp " + ::to_string(i - parsedskip)); //Replace the instruction
			}
			if (strstr((*in)[i].c_str(), "+")) { //Jmp forward
				string afterplus = split(string((*in)[i]), "+")[1]; //Get the Amount of lines we want to skip
				int parsedskip = std::atoi(afterplus.c_str()); //Convert the Amount to an int.
				(*in)[i] = std::string("jmp " + ::to_string(i + parsedskip)); //Replace the instruction
			}
		}
	}
}
void FunctionSystem(vector<string> *in) {
	vector<pair<int, string>> markedFuncs;
	for (int i = 0; (*in).size() > i; i++) {
		if (isMarked((*in)[i])) { //If Instruction is marked as function beginning (x: (inst) (arg))
			string funcName = (*in)[i].substr(0, 1); //Get Name of Function.
			markedFuncs.push_back(make_pair(i, funcName)); //Add the found Function to the FuncList.
		}
	}
	for (pair<int, string> x : markedFuncs) {
		int funcLine = x.first;
		string funcName = x.second;
		for (int i = 0; (*in).size() > i; i++) {
			if (isFuncJmp((*in)[i])) { //If instruction refers func.
				string _fName = (*in)[i].substr((*in)[i].size() - 1); //Get referred function name.
				if (_fName == funcName) { //if function name equals current func Name
					(*in)[i] = string((*in)[i].substr(0, ((*in)[i].find(_fName.c_str()))) + ::to_string(funcLine)); //Replace line with refer to correct line.
				}
			}
		}
	}
}
void Allocation(vector<string> *in) {
	for (int i = 0; (*in).size() > i; i++) {
		if (isZeroAllocation((*in)[i])) {
			string first = split((*in)[i], "[")[0].substr(0, 1); //Gets the variable to write to
			vector<vector<string>> func = getZeroAllocation(i, (*in).size(), first); //Gets the new code
			(*in)[i] = func[0][0]; //Overwrites original instruction
			for (auto x : func[1]) //Adds the code.
				(*in).push_back(x);
		}
		if (isNormalAllocation((*in)[i])) {
			string first = split((*in)[i], "[")[0].substr(0, 1); //gets variable to write to
			string second = split((*in)[i], "[")[0].substr(2, 1); //gets variable to copy from
			string help = split((*in)[i], "[")[0].substr(4, 1); //gets helper variable
			vector<vector<string>> func = getNormalAllocation(i, (*in).size(), first, second, help); //Get new code
			(*in)[i] = func[0][0]; //Overwrites current instruction
			for (auto x : func[1])
				(*in).push_back(x);
		}
	}
}

vector<string> translate(vector<string> inp) {
	vector<string> t_code = inp; //Copy input just for ease

	FunctionSystem(&t_code); //Redirect Function References to correct lines.
	JmpOverwrite(&t_code); //Convert (5: JMP -2) to (5: JMP 3) and so on :)
	Allocation(&t_code); //Append Functions
	format(&t_code); //Get Code in right format.

	return t_code;
}