#include "Usefulstuff.hpp"

/*

Easy Bonsai Assembler

	Makes programming in (Bonsai)Assembler easier by translating a easier version of BonsaiAssembler to normal BonsaiAssembler.

	Libs used:
	https://archive.codeplex.com/?p=cppconlib -- Console Utility

*/
int main(int argc, char* args[]) {

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

		if (args[1] == "help") {

			cout
				<< "EasyBonsai is a easier version of the BonsaiAssembler which will be converted back to runnable (Bonsai)Assembler." << endl
				<< "Features of Easybonsai are:" << endl
				<< "- it ignores invalid lines, so you can comment your code." << endl
				<< "- it supports jmp with skipping, (so jmp +2 will jump 2 lines forward)" << endl
				;

			return 0;
		}

		filename = std::string(args[1]);
	}

	struct stat info;

	if (stat(filename.c_str(), &info) == 0) {
		log("Reading File: " + filename, console_text_colors::light_green);

		ifstream in(filename.c_str());
		string buff;
		vector<string> input;
		while (getline(in, buff)) {
			input.push_back(buff);
		}
		log("Invalid Instructions will be ignored, so you can freely comment your code!", console_text_colors::light_yellow);
		log("Reading File... Translating Code: ", console_text_colors::light_cyan);
		cout << endl;

		vector<string> builtcode;

		for (auto x : input) {
			if (isValid(x)) {
				builtcode.push_back(x); 
				cout << setbgcolor(console_bg_colors::white) << settextcolor(console_text_colors::magenta) << x << endl;
				ctxout.restore(console_cleanup_options::restore_attibutes);
			}
			else {
				cout << "(INVALID)" << settextcolor(console_text_colors::light_red) << x << endl;
				ctxout.restore(console_cleanup_options::restore_attibutes);
			}
		}

		cout << endl << endl;

		log("Parsed Code: \n", console_text_colors::light_green);

		for (int i = 0; builtcode.size() > i; i++) {
			cout << setbgcolor(console_bg_colors::white) << settextcolor(console_text_colors::magenta) << i << ": " << builtcode[i] << endl;
		}
		ctxout.restore(console_cleanup_options::restore_attibutes);

		log("Translating code...", console_text_colors::light_yellow);


	}
	else {
		log("Could not find file or directory: " + filename, console_text_colors::light_red, console_bg_colors::white);
	}

	system("pause");

	return 0;
}
