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
	console_out conout(ctxout);

	conout.settitle("EasyBonsai ¯\_(ツ)_/¯");
	string filename;
	if (argc < 2) {
		log("Input Filename: ", console_text_colors::light_blue, console_bg_colors::white, false);
		cin >> settextcolor(console_text_colors::white)
			>> setbgcolor(console_bg_colors::black)
			>> filename;
	}
	else 
	{
		if ((string)args[1] == "help") 
		{
			cout
				<< "EasyBonsai is a easier version of the BonsaiAssembler which will be converted back to runnable (Bonsai)Assembler." << endl
				<< "Features of Easybonsai are:" << endl
				<< "- it ignores invalid lines, so you can comment your code." << endl
				<< "- it supports jmp with skipping, (so jmp +2 will jump 2 lines forward)" << endl;
			return 0;
		}
		filename = std::string(args[1]);
	}
	struct stat info;
	if (stat(filename.c_str(), &info) == 0) {

		//Todo: Add Error Message if krebs
		ifstream in(filename.c_str());
		string buff;
		vector<string> input;
		while (getline(in, buff)) {
			input.push_back(buff);
		}
		log("Read File: " + filename, console_text_colors::light_green, console_bg_colors::black, true, "Success");
		//Todo end.

		log("Invalid Instructions will be ignored, so you can freely comment your code!", console_text_colors::light_yellow, console_bg_colors::black, true, "Info");
		
		log("Translating Code", console_text_colors::light_white, console_bg_colors::black, true, "Started");
		cout << endl;

		vector<string> builtcode;

		for (auto x : input) {
			if (isValid(x)) {
				builtcode.push_back(x); 
				cout << setbgcolor(console_bg_colors::white) << settextcolor(console_text_colors::black) << x << endl;
				ctxout.restore(console_cleanup_options::restore_attibutes);
			}
			else 
			{
				if(strncmp(x.c_str(), "//", 2) == 0)
					cout << settextcolor(console_text_colors::light_white) << "// " << settextcolor(console_text_colors::white) << x.substr(2) << endl;
				else
					cout << settextcolor(console_text_colors::light_red) << "(INVALID) " << settextcolor(console_text_colors::white) << x << endl;

				ctxout.restore(console_cleanup_options::restore_attibutes);
			}
		}

		cout << endl;

		log("Parsed Code: \n", console_text_colors::light_green, console_bg_colors::black, true, "Success");

		for (int i = 0; builtcode.size() > i; i++) {
			cout << setbgcolor(console_bg_colors::white) << settextcolor(console_text_colors::black) << i << ": " << builtcode[i] << endl;
		}
		ctxout.restore(console_cleanup_options::restore_attibutes);

		cout << endl;

		vector<string> bonsaicode = translate(builtcode);

		log("Bonsai: ", console_text_colors::light_green, console_bg_colors::black, true, "Success");

		cout << endl << setbgcolor(console_bg_colors::white) << settextcolor(console_text_colors::black) << join(bonsaicode, '\n') << endl;
		ctxout.restore(console_cleanup_options::restore_attibutes);

		cout << endl;

	}
	else {
		log("Could not find file or directory: " + filename, console_text_colors::light_red, console_bg_colors::white);
	}

	system("pause");

	return 0;
}

void format(vector<string> *in) {
	for (int i = 0; (*in).size() > i; i++) {
		if(!isFormatted( string( ((*in)[i]))))
		(*in)[i] = std::string(::to_string(i) + ": " + (*in)[i]);
	}
}
void JmpOverwrite(vector<string> *in) {
	for (int i = 0; (*in).size() > i; i++) {
		if (isEasyJmp((*in)[i])) {
			string afterplus = split(string((*in)[i]), "+")[1];
			int parsedskip = std::atoi(afterplus.c_str());
			(*in)[i] = std::string(::to_string(i) + ": jmp " + ::to_string(i + parsedskip));
		}
	}
}

vector<string> translate(vector<string> inp) {
	
	vector<string> t_code = inp;

	//Create Pre-Function Instructions
	//Overwrite JMPS to be correct.
	JmpOverwrite(&t_code);
	//Append Functions
	//Format Code
	format(&t_code);

	return t_code;
}
