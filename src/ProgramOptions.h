/*
 * ProgramOptions: friendly and easy to use program option infterfaces. This is a part of ProgramOptions.
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
#ifndef PROGRAM_OPTIONS_H
#define PROGRAM_OPTIONS_H

#include <vector>
#include "AnyBasic.h"
#include "programoptions_global.h"
/*!
  TODO: move Option, OptionGroup to cpp

	ProgramOptions::summary("Summary for this library")
		["Usage:"]
		("-V,--version", "version 1.0.0")
		("-h,--help", "print this help")
			["sub group"]
			(...)
			() //parent group
		(...)
		(...)
*/

namespace ProgramOptions {

/*
 * type = (Positional + PositionalCount(N)), N == 0 means the count of option is not limited. Otherwise the option must appears exactly N
 * times.
 * Sticky: -t123 equals -t 123
 */
enum Type {
	NoToken = 0, SingleToken = 1, MultiToken = 2, Sticky = 4
	, Positional = 0xff//put it at the last
};

template<int N> struct BinaryLength { enum { value = BinaryLength<(N >> 1)>::value + 1}; };
template<> struct BinaryLength<0> { enum { value = 0}; };

#define PositionalShift ProgramOptions::BinaryLength<ProgramOptions::Positional>::value
#define PositionalCount(N) (N << PositionalShift)
#define PositionalCountOfType(type) (type >> PositionalShift)

class OptionGroup;
class Summary;

Q_EXPORT Summary& summary(const char* fmt, ...);

Q_EXPORT OptionGroup& add(const char* group_description); //parent = null

Q_EXPORT void parse(int argc, const char* const* argv);

Q_EXPORT int count(const char* name); //name appears in command line
//const AnyBasic& get(const char* name); //const //get vaule
Q_EXPORT AnyBasic get(const char* name); //const //get vaule
//template<typename T> T getAs(const char* name);
Q_EXPORT void help();


class Q_EXPORT Summary
{
public:
	Summary(const char* text = 0);
	OptionGroup& operator [](const char* group_description);
	void setSummary(const char* text);
	void print();
private:
	class Impl;
	Impl *impl;
};

class Q_EXPORT Option
{
public:
	//TODO: Add functor param
	Option(const char* name, const char* description, OptionGroup* group = 0);//notoken
	Option(const char* name, Type type, const char* description, OptionGroup* group = 0);
	Option(const char* name, const AnyBasic& defaultValue, const char* description, OptionGroup* group = 0); //single token
	Option(const char* name, const AnyBasic& defaultValue, Type type, const char* description, OptionGroup* group = 0);
	~Option();
	
	OptionGroup* group();  //
	bool in(OptionGroup* g);
	bool isLongOption() const;
	bool isShortOption() const;
	const char* longName() const;
	const char* shortName() const;
	const char* name() const;  //-short_name,--long_name
	const char* description() const;
	int valuesCount() const; //how many values setted by command line
	std::vector<AnyBasic> values() const; //-Idir1 -Idir2 -llib1 -llib2
	AnyBasic value() const;
	AnyBasic defaultValue() const;
	//setValues, addValue
	void setValue(const AnyBasic& value);
	Type type() const;
	void setType(Type type);
	
	void print();
private:
	friend class OptionGroup;
	class Impl;
	Impl *impl;
};

class Q_EXPORT OptionGroup
{
public:
	OptionGroup(const char* description, OptionGroup* parent = 0); //OptionGroup == 0, use default values
	~OptionGroup();
	
	OptionGroup& parent();
	void print();
	
	//return the child group
	OptionGroup& operator [](const OptionGroup& group);
	/*!
	 * return parent group. if parent is null, return an invalid group. you can call it any times. The options after an invalid
	 * group will be ignored. You must add a option group first immediatly after the invalid group
	*/
	OptionGroup& operator ()();
	//return this group or invalid group. options added after an invalid group will be ignored
	OptionGroup& operator ()(const char* name, const char* description = 0);//notoken
	OptionGroup& operator ()(const char* name, Type type, const char* description = 0);
	OptionGroup& operator ()(const char* name, const AnyBasic& defaultValue, const char* description = 0); //single token
	OptionGroup& operator ()(const char* name, const AnyBasic& defaultValue, Type type, const char* description = 0);
	const char* description() const;
	//const char* name() const;  //-s,--long. not supported now
	
	int depth();
private: 
	friend class Option;
	class Impl;
	Impl *impl;
};

}

#endif //PROGRAM_OPTIONS_H
