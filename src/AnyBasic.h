/*
 * AnyBasic: a non-templete container class for basic types. This is a part of ProgramOptions.
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
