#ifndef ANYBASIC_H
#define ANYBASIC_H

#include <string>
#include <ostream>
#include "lexical_cast.h"


class AnyBasic
{
public:
	AnyBasic() {}
	template<typename T> AnyBasic(const T& value);
	AnyBasic(const AnyBasic& that);

	template<typename T> T to();
	template<typename T> T to() const;
	template<typename T> operator T();
	template<typename T> operator T() const;

	template<typename T> AnyBasic& operator =(const T& value);
	AnyBasic& operator =(const AnyBasic& that);

	std::string str() const {return value_str;}
	friend std::ostream& operator<<(std::ostream& out, const AnyBasic& any);
private:
	std::string value_str;	
};



template<typename T>
AnyBasic::AnyBasic(const T& value)
{
	value_str = lexical::cast_to<std::string>::from(value);
}

template<typename T> T AnyBasic::to()
{
	return lexical::cast_to<T>::from(value_str);
}

template<typename T> T AnyBasic::to() const
{
	return lexical::cast_to<T>::from(value_str);
}

template<typename T> AnyBasic::operator T()
{
	return to<T>();
}

template<typename T> AnyBasic::operator T() const
{
	return to<T>();
}


template<typename T>
AnyBasic& AnyBasic::operator =(const T& value)
{
	value_str = lexical::cast_to<std::string>::from(value);
	return *this;
}

#endif //ANYBASIC_H
