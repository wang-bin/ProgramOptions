/*
 * lexical cast: cast between types. This is a part of ProgramOptions.
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

#ifndef LEXICAL_CAST_H
#define LEXICAL_CAST_H

#define NO_EXCEPTIONS
#ifndef NO_EXCEPTIONS
#include <exception>
#else
#include <cstdlib>
#include <iostream>
#endif //QT_NO_EXCEPTIONS
#include <sstream>

namespace lexical {

#ifndef NO_EXCEPTIONS
class bad_cast : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "bad lexical cast: source type value could not be interpreted as target";
	}
};
#endif //NO_EXCEPTIONS
template<typename Target>
struct  cast_to {
	template<typename Source>
	static Target from(const Source& in) {
		static std::stringstream s;
		s.clear();
		s.str("");
		Target result;
		if ((s << in).fail() || (s >> result).fail()) {
#ifndef NO_EXCEPTIONS
			throw bad_cast();
#else
			std::cerr << "bad lexical cast: source type value could not be interpreted as target" << std::endl;
			exit(1);
#endif //NO_EXCEPTIONS
		}
		return result;
	}
	static Target from(const Target& in) {
		return in;
	}
};


template<>
struct cast_to<std::string> {
	template<typename Source>
	static std::string from(const Source& in) {
		static std::stringstream s;
		s.clear();
		s.str("");
		if ((s << in).fail()) {
#ifndef NO_EXCEPTIONS
			throw bad_cast();
#else
			std::cerr << "bad lexical cast: source type value could not be interpreted as target" << std::endl;
			exit(1);
#endif //NO_EXCEPTIONS
		}
		return s.str();
	}
	static std::string from(const std::string& in) {
		return in;
	}
};

}

#endif //LEXICAL_CAST_H
