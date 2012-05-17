#include "ProgramOptions.h"
#include "ezlog.h"
namespace po = ProgramOptions;
int main(int argc, char** argv)
{
	ezlog_init_default();
	po::add("Help for this program")
		("-h,help", po::NoToken, "show this message")
		("--version,v", "show the library version")
		.add("Author info")
			("-name", "Luke", "set the name")
		
	;
	po::parse(argc, argv);
	if (po::get("h"))
		po::help();
	if (po::get("v"))
		printf("Version 1.0.0\n");
	printf("name: %s\n", po::get("name").to<std::string>().c_str());
	return 0;
}
