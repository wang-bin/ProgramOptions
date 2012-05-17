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
		("--version,v", "show the library version")
		.add("Author info")
			("-name", "Luke", "the name of author")
			("--age", 24, "the age of author")
			("-w,weight", 66.6, "the weight of author")
			("--height,-H", 175.6, "the height of author")
	;
	po::parse(argc, argv);
	if (po::get("h"))
		po::help();
	if (po::get("v"))
		printf("Version 1.0.0\n");
	printf("name: %s\nage: %d\n", po::get("name").to<std::string>().c_str(), po::get("age").to<int>());
	float w = po::get("w");
	double H(po::get("H"));
	cout << "weight: " << w << endl;
	cout << "height: " << H << endl;
	return 0;
}
