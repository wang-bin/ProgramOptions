#ifndef PROGRAM_OPTIONS_H
#define PROGRAM_OPTIONS_H

#include "AnyBasic.h"
/*!
  TODO: move Option, OptionGroup to cpp

	ProgramOptions::add("help")
		("-V,--version", "version 1.0.0")
		("-h,--help", "print this help")  //call functor
		.add(...)
			(...)
		.paremt()
		(...)
		(...)
*/

namespace ProgramOptions {

enum Type {
	NoToken, SingleToken, MultiToken
};


class OptionGroup;

OptionGroup& add(const OptionGroup& group); //parent = null

void parse(int argc, const char* const* argv);

//const AnyBasic& get(const char* name); //const //get vaule
AnyBasic get(const char* name); //const //get vaule

void help();


class Option
{
public:
	//Add functor param
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
	const char* name() const;
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
	//OptionGroup(const char* name, const char* description, OptionGroup* parent = 0);
	OptionGroup(const char* description, OptionGroup* parent = 0); //OptionGroup == 0, use default values
	~OptionGroup();
	
	OptionGroup& parent();
	void print();
	
	//call constructor, parent = this. 1 more constructor. return the child group
	OptionGroup& add(const OptionGroup& group);
	//call constructor, group = this. 1 more constructor. return this group
	OptionGroup& operator ()(const Option& option);
	OptionGroup& operator ()(const char* name, const char* description = 0);//notoken
	OptionGroup& operator ()(const char* name, Type type, const char* description = 0);
	OptionGroup& operator ()(const char* name, const AnyBasic& defaultValue, const char* description = 0); //single token
	OptionGroup& operator ()(const char* name, const AnyBasic& defaultValue, Type type, const char* description = 0);
	const char* description() const;
	const char* name() const;  //-s,--long
	
	int depth();
private: 
	friend class Option;
	class Impl;
	Impl *impl;
};


}

#endif //PROGRAM_OPTIONS_H
