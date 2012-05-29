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

#include "AnyBasic.h"
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

enum Type {
	NoToken, SingleToken, MultiToken
};


class OptionGroup;
class Summary;

Summary& summary(const char* fmt, ...);

OptionGroup& add(const char* group_description); //parent = null

void parse(int argc, const char* const* argv);

bool has(const char* name); //name appears in input option
//const AnyBasic& get(const char* name); //const //get vaule
AnyBasic get(const char* name); //const //get vaule
//template<typename T> T getAs(const char* name);
void help();


class Summary
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

class Option
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
	AnyBasic value() const;
	AnyBasic defaultValue() const;
	void setValue(const AnyBasic& value);
	Type type() const;
	void setType(Type type);
	
	void print();
private:
	friend class OptionGroup;
	class Impl;
	Impl *impl;
};

class OptionGroup
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
