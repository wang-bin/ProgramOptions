ProgramOptions
==============

An easy to use program options library. something like boost.program_options



An example:

tst_options.cpp


#include "ProgramOptions.h"
#include <iostream>
using namespace std;
namespace po = ProgramOptions;
int main(int argc, char** argv)
{
	ezlog_init_default();
	po::summary("It's a demo for ProgramOptions")
		["Help for this program"]
		("-h,help", po::NoToken, "show this message")
		["Author info"]
			("-name", "Luke", "the name of author")
			("--age", 24, "the age of author")
			("-w,weight", 66.6, "the weight of author")
			("--height,-H", 175.6, "the height of author")
			["Other infomation"]
				("girl", "DLF", "first girl friend")
				()
			()()()
		("--bye", "can you see me?")
		["Version information"]
		("--version,v", "show the library version")
		()
	;
	po::add("Do you like it?")
		("-y,--yes", "I like it")
		("n,no", "I don't like it")
	;
	po::parse(argc, argv);
	if (po::get("h"))
		po::help();
	if (po::get("v"))
		printf("Version 1.0.0\n");
	printf("name: %s\n", po::get("name").to<std::string>().c_str());
	cout << "age: " << po::get("age") << endl; //AnyBasic::operator<<()
	float w = po::get("w");
	double H(po::get("H"));
	cout << "weight: " << w << endl;
	cout << "height: " << H << endl;
	cout << "girl: " << po::get("girl") << endl;
	return 0;
}
