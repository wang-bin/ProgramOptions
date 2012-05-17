#include "ProgramOptions.h"
#include "ezlog.h"
#include <iostream>
using namespace std;
namespace po = ProgramOptions;
int main(int argc, char** argv)
{
	ezlog_init_default();
	po::add("Help for this program")
		("-h,help", po::NoToken, "show this message")
		.add("Author info")
			("-name", "Luke", "the name of author")
			("--age", 24, "the age of author")
			("-w,weight", 66.6, "the weight of author")
			("--height,-H", 175.6, "the height of author")
			.add("Other infomation")
				("girl", "DLF", "first girl friend")
			.parent()
		.parent()
		
		("--version,v", "show the library version")
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
