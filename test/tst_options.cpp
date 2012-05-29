/*
 * ProgramOptions test application. This is a part of ProgramOptions.
 * Copyright (C) 2012  Wang Bin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ProgramOptions.h"
#include <cstdio>
#include <iostream>
using namespace std;
namespace po = ProgramOptions;
int main(int argc, char** argv)
{
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
		("--version,ver", "show the library version")
		()
	;
	po::add("Do you like it?")
		("-y,--yes", "I like it")
		("n,no", "I don't like it")
	;
	po::parse(argc, argv);
	if (po::get("h"))
		po::help();
	if (po::get("ver"))
		printf("Version 1.0.0\n");
	printf("name: %s\n", po::get("name").to<std::string>().c_str());
	cout << "age: " << po::get("age") << endl; //AnyBasic::operator<<()
	float w = po::get("w");
	double H(po::get("H"));
	cout << "weight: " << w << endl;
	cout << "height: " << H << endl;
	cout << "girl: " << po::get("girl") << endl;
	bool like = po::get("y");
	if (like)
		cout << "You like it" << endl;
	else
		cout << "You don't like it" << endl;
	return 0;
}
