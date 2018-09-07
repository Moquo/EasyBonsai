#include "Usefulstuff.hpp"

/*

Easy Bonsai Assembler

	Makes programming in (Bonsai)Assembler easier by translating a easier version of BonsaiAssembler to normal BonsaiAssembler.

	Libs used:
	https://archive.codeplex.com/?p=cppconlib -- Console Utility

*/
int main(int argc, char* args[])
{
	console_out_context ctxout;
	console_out conout(ctxout); //Some Console Utilities...

	if (argc < 2 || argc > 2) { //Check if there is exactly one start argument (file).
		log("Argument Error", console_text_colors::light_red, console_bg_colors::black, false, "Error");
		return 0;
	}
	if ((string)args[1] == "help") //Help 'n Stuff :)
	{
		cout
			<< "EasyBonsai is a easier version of the BonsaiAssembler which will be converted back to runnable (Bonsai)Assembler." << endl
			<< "Features of Easybonsai are:" << endl
			<< "- it ignores invalid lines, so you can comment your code." << endl
			<< "- it supports jmp with skipping, (so jmp +2 will jump 2 lines forward)" << endl;
		return 0;
	}
	struct stat info; //Needed for detecting wether file exists or not.
	if (stat(args[1], &info) == 0) {
		ifstream in(args[1]);
		string buff;
		vector<string> input;
		while (getline(in, buff)) {
			input.push_back(buff);
		} //Read File line by line.
		log("Read File: " + (string)args[1], console_text_colors::light_green, console_bg_colors::black, true, "Success");
		log("Invalid Instructions will be ignored, so you can freely comment your code!", console_text_colors::light_yellow, console_bg_colors::black, true, "Info");
		log("Translating Code", console_text_colors::light_white, console_bg_colors::black, true, "Started");
		cout << endl; //Tell user we read file.

		vector<string> builtcode; //Array which will hold the code which we will later translate.
		for (auto x : input) { // We will output the code here, so the user knows which lines we will use or not use.
			if (isValid(x)) { //Check if Line is valid/allowed instruction.
				builtcode.push_back(x);
				cout << setbgcolor(console_bg_colors::white) << settextcolor(console_text_colors::black) << x << endl;
				ctxout.restore(console_cleanup_options::restore_attibutes);
			}
			else
			{
				if (strncmp(x.c_str(), "//", 2) == 0) //If Line starts with // we wont spam the user with red error messages and just ignore it.
					cout << settextcolor(console_text_colors::light_white) << "// " << settextcolor(console_text_colors::white) << x.substr(2) << endl;
				else
					cout << settextcolor(console_text_colors::light_red) << "(INVALID) " << settextcolor(console_text_colors::white) << x << endl;

				ctxout.restore(console_cleanup_options::restore_attibutes); //This will restore colors.
			}
		}
		cout << endl;
		/*
		Basically this is fucking useless.
		log("Parsed Code: \n", console_text_colors::light_green, console_bg_colors::black, true, "Success");
		for (int i = 0; builtcode.size() > i; i++) {
			cout << setbgcolor(console_bg_colors::white) << settextcolor(console_text_colors::black) << i << ": " << builtcode[i] << endl;
		}
		ctxout.restore(console_cleanup_options::restore_attibutes);
		cout << endl;
		*/
		vector<string> bonsaicode = translate(builtcode); //Translate the Code and output it.
		log("Bonsai: ", console_text_colors::light_green, console_bg_colors::black, true, "Success");
		cout << endl << setbgcolor(console_bg_colors::white) << settextcolor(console_text_colors::black) << join(bonsaicode, '\n') << endl;
		ctxout.restore(console_cleanup_options::restore_attibutes);
		cout << endl;
	}
	else
	{
		log("Could not find file or directory: " + (string)args[1], console_text_colors::light_red, console_bg_colors::white);
	}
	return 0;
}

void format(vector<string> *in) {
	for (int i = 0; (*in).size() > i; i++) { //Loop through the code lines
		if (isMarked((*in)[i])) //If Line starts with "x: " then remove the beginning because this is no longer needed.
			(*in)[i] = (*in)[i].substr(3); // This skips "x: "
		if (!isFormatted(((*in)[i]))) //If Line is not formatted, format it... (Include Line Numbers)
			(*in)[i] = std::string(::to_string(i) + ": " + (*in)[i]);
	}
}
void JmpOverwrite(vector<string> *in) {
	for (int i = 0; (*in).size() > i; i++) {
		if (isEasyJmp((*in)[i])) { //If Jmp goes something like this: jmp +2 then redirect it to the right line.
			if (strstr((*in)[i].c_str(), "-")) {  //Jmp back
				string afterminus = split(string((*in)[i]), "-")[1]; //Get the Amount of lines we want to skip
				int parsedskip = std::atoi(afterminus.c_str()); //Convert the Amount to an int.
				(*in)[i] = std::string(::to_string(i) + ": jmp " + ::to_string(i - parsedskip)); //Replace the instruction
			}
			if (strstr((*in)[i].c_str(), "+")) { //Jmp forward
				string afterplus = split(string((*in)[i]), "+")[1]; //Get the Amount of lines we want to skip
				int parsedskip = std::atoi(afterplus.c_str()); //Convert the Amount to an int.
				(*in)[i] = std::string(::to_string(i) + ": jmp " + ::to_string(i + parsedskip)); //Replace the instruction
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
				string _fName = (*in)[i].substr((*in)[i].size() - 1); //Get refered function name.
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
			string first = split((*in)[i], "[")[0].substr(0, 1);
			vector<vector<string>> func = getZeroAllocation(i, (*in).size(), first);
			(*in)[i] = func[0][0];
			for (auto x : func[1])
				(*in).push_back(x);
		}
		if (isNormalAllocation((*in)[i])) {
			string first = split((*in)[i], "[")[0].substr(0, 1);
			string second = split((*in)[i], "[")[0].substr(2, 1);
			string help = split((*in)[i], "[")[0].substr(4, 1);
			vector<vector<string>> func = getNormalAllocation(i, (*in).size(), first, second, help);
			(*in)[i] = func[0][0];
			for (auto x : func[1])
				(*in).push_back(x);
		}
	}
}

vector<string> translate(vector<string> inp) {
	vector<string> t_code = inp;

	FunctionSystem(&t_code); //Redirect Function References to correct lines.
	JmpOverwrite(&t_code); //Convert (5: JMP -2) to (5: JMP 3) and so on :)
	Allocation(&t_code); //Append Functions
	format(&t_code); //Get Code in right format.

	return t_code;
}
