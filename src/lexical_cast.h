#ifndef LEXICAL_CAST_H
#define LEXICAL_CAST_H

#include <exception>
#include <sstream>

namespace lexical {

class bad_cast : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "bad lexical cast: source type value could not be interpreted as target";
	}
};

template<typename Target>
struct  cast_to {
	template<typename Source>
	static Target from(const Source& in) {
		static std::stringstream s;
		s.clear();
		s.str("");
		Target result;
		if ((s << in).fail() || (s >> result).fail())
			throw bad_cast();
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
		if ((s << in).fail())
			throw bad_cast();
		return s.str();
	}
	static std::string from(const std::string& in) {
		return in;
	}
};

}

#endif //LEXICAL_CAST_H
