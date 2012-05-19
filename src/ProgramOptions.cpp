#include "ProgramOptions.h"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

using namespace std;

namespace ProgramOptions {

static OptionGroup *invalid_group = 0;
static Summary *options_summary = new Summary("");
class Private
{
public:
	friend class OptionGroup;
	list<OptionGroup*> groups; //parent = null
	map<string/*short name*/, Option*> short_options;
	map<string/*long name*/, Option*> long_options;
	
	void print() {
		options_summary->print();
		list<OptionGroup*>::const_iterator it; 
		for (it = groups.begin(); it != groups.end(); ++it) {
			(*it)->print();
			cout << endl;
		}
	}
};

static Private *priv = new Private();

class Summary::Impl
{
public:
	void print() {
		cout << text << endl << endl;
	}
	string text;
};

Summary::Summary(const char* text)
	:impl(new Impl)
{
	impl->text = text;
}

//create a root group
OptionGroup& Summary::operator [](const char* group_description)
{
	OptionGroup *g = new OptionGroup(group_description, 0);
	priv->groups.push_back(g);
	return *g;	
}

void Summary::setSummary(const char* text)
{
	impl->text = text;
}

void Summary::print()
{
	impl->print();
}

//TODO: printf like
static void parse_error(const char* fmt, ...)
{
	char msg[512];
        va_list args;
        va_start(args, fmt);
        vsprintf(msg, fmt, args);
        va_end(args);

	cout << "Invalid option: " << msg << endl;
	cout << "Usage:" << endl;
	help();
}





	
Summary& summary(const char* fmt, ...)
{
	char msg[512];
        va_list args;
        va_start(args, fmt);
        vsprintf(msg, fmt, args);
        va_end(args);
	options_summary->setSummary(msg);
	return *options_summary;
}

//create a root group
OptionGroup& add(const char* group_description)
{
	return (*options_summary)[group_description];
}

void parse(int argc, const char* const* argv)
{
	if (argc == 1)
		return;
	if (priv->groups.empty())
		return;

	vector<string> args;
	for (int i = 1; i < argc; ++i)
		args.push_back(argv[i]);
	vector<string>::iterator it = args.begin();
	Option *opt = 0;
	//TODO: check group
	while (it != args.end()) {
		if ((*it).substr(0,2) == "--") { //long option
			size_t eq = (*it).find('=');
			if (eq != string::npos) {
				string long_name = (*it).substr(2, eq - 2);
				if (priv->long_options.count(long_name) == 0)
					parse_error(long_name.c_str());
				opt = priv->long_options[long_name];
				if (opt->type() == NoToken) {
					opt->setValue(true);
				}
				//TODO: MultiToken
				else //if (opt->type() == Option::SingleToken)
					opt->setValue((*it).substr(eq + 1)); //store as string
			} else {
				string long_name = (*it).substr(2);
				if (priv->long_options.count(long_name) == 0)
					parse_error(long_name.c_str());
				opt = priv->long_options[long_name];
				if (opt->type() == NoToken) {
					opt->setValue(true);
				} else {
					it = args.erase(it);
					if ((*it)[0] == '-' || it == args.end())
						parse_error("Need an value for this option '--%s'", long_name.c_str());
					opt->setValue(*it);
				}
			}	
		} else if ((*it).substr(0, 1) == "-") {
			string short_name = (*it).substr(1);
			if (priv->short_options.count(short_name) == 0)
				parse_error(short_name.c_str());
			opt = priv->short_options[short_name];
			if (opt->type() == NoToken) {
				opt->setValue(true);
			} else {
				it = args.erase(it);
				if ((*it)[0] == '-' || it == args.end())
					parse_error("Need an value for this option  '-%s'", short_name.c_str());
				opt->setValue(*it);
			}
		}
		it = args.erase(it);
	}
}

AnyBasic get(const char* name)
{
	Option *opt = 0;
	if (priv->short_options.count(name))
		opt = priv->short_options[name];
	else if (priv->long_options.count(name))
		opt = priv->long_options[name];
	
	if (!opt)
		return AnyBasic();
	
	return opt->value();
}

void help()
{
	priv->print();
	exit(0);
}



//Impl
class Option::Impl
{
public:
	friend class OptionGroup;
	OptionGroup *group;
	Type type;
	AnyBasic value, default_value;
	std::string description;
	std::string name, short_name, long_name;

	void init(const char* n, const AnyBasic& def, Type t, const string& desc, OptionGroup* g) {
		default_value = def;
		type = t;
		description = desc;
		group = g;
		if (type == NoToken)
			default_value = false;
		value = default_value;
		initNames(n);
	}
	// --long_name  -short_name
	void initNames(const char* s) {
		name = s;
		//const char *c = strchr(s, ',');
		size_t c = name.find(',');
		if (c != string::npos) {
			if (s[0] == '-') {
				if (s[1] == '-') { // --longname,-short  short's length can larger than 1, e.g. -help
					long_name = name.substr(2, c - 2);
					int sp = c + 2; //"--help,-h
					if (s[c + 1] != '-')  // "--help,h"
						sp = c + 1;
					short_name = name.substr(sp);
					name = "-" + short_name + ",--" + long_name;
				} else {
					short_name = name.substr(1, c - 1);
					int lp = c + 3;
					if (s[c + 1] == '-') {// "-h,--help" "-h,help" "-h.-help"
						if (s[c + 2] != '-')
							lp = c + 2;
					} else {
						lp = c + 1;
					}
					long_name = name.substr(lp);
					name = "-" + short_name + ",--" + long_name; 
				}
			} else {
				if (c == 1) {
					short_name = name.substr(0, 1);
					int lp = 4; // "h,--help"
					if (s[2] == '-') {
						if (s[3] != '-') // "h,-help
							lp = 3;
						
					} else { //h,help
						lp = 2;
					}
					long_name = name.substr(lp);
					name = "-" + short_name + ",--" + long_name;

				} else {
					if (s[c + 1] == '-') {
						if (s[c + 2] == '-') { // "h,--help"
							short_name = name.substr(0, c);
							long_name = name.substr(c + 3);

						} else { //"help,-h"
							short_name = name.substr(c + 2);
							long_name = name.substr(0, c);
}
					} else {
						if (c < name.size() - c - 1) { // "h,help"
							short_name = name.substr(0, c);
							long_name = name.substr(c + 1);
						} else { // "help,h"
							short_name = name.substr(c + 1);
							long_name = name.substr(0, c);
						}
					}
					name = "-" + short_name + ",--" + long_name;
				}	
			}
			return;
		}
		
		//no ","
		if (s[0] == '-') {
			if (s[1] == '-') {
				long_name = name.substr(2);
			} else {
				short_name = name.substr(1);
			}
		} else {
			if (name.size() == 1) {
				short_name = name;
				name = "-" + name; //normalize
			} else {
				long_name = name;
				name = "--" + name;
			}
		}
	}
};

class OptionGroup::Impl
{
public:
	Impl():invalid(false),depth(-1) {}
	OptionGroup *parent;
	string description;
	list<OptionGroup*> childs;
	list<Option*> options;
	//std::string name, short_name, long_name;	
	bool invalid;
	int opt_name_w;
	int depth;
	void compute_depth(OptionGroup* p) {
		depth = 0;
		OptionGroup *g = p->impl->parent;
		while (g != 0) {
			depth++;
			g = g->impl->parent;
		}
	}
	int offset(OptionGroup *g) {
		return 2 * g->depth();
	}
		
	int option_name_width() {
		if (opt_name_w > 0) //calculate only once
			return opt_name_w;
		list<Option*>::const_iterator it = options.begin();
		while (it != options.end()) {
			opt_name_w = std::max(opt_name_w, (int)(*it)->impl->name.size());
			++it;
		}
//		assert(w <= 0);
		return opt_name_w;
	}

	void addOption(Option* p) {
		if (p->isLongOption())
			priv->long_options.insert(pair<string, Option*>(string(p->longName()), p));
		if (p->isShortOption())
			priv->short_options.insert(pair<string, Option*>(string(p->shortName()), p));
		options.push_back(p);
	}
	void print(OptionGroup* g) {
		string space(g->impl->offset(g), '>');
		cout << space << g->description() << endl; //formated string
		list<Option*>::const_iterator i;
		for (i = g->impl->options.begin(); i != g->impl->options.end(); ++i) {
			(*i)->print();
		}

		list<OptionGroup*>::const_iterator it;
		for (it = g->impl->childs.begin(); it != g->impl->childs.end(); ++it) {
			print(*it);
		}
	}
};


//Option
Option::Option(const char* name, const char* description, OptionGroup* group)
	:impl(new Impl())
{
	impl->init(name, false, NoToken, description, group);
}

Option::Option(const char* name, const AnyBasic& defaultValue, const char* description, OptionGroup* group) //single token
	:impl(new Impl())
{
	impl->init(name, defaultValue, SingleToken, description, group);
}

Option::Option(const char* name, const AnyBasic& defaultValue, Type type, const char* description, OptionGroup* group)
	:impl(new Impl())
{
	impl->init(name, defaultValue, type, description, group);
}

Option::Option(const char* name, Type type, const char* description, OptionGroup* group)
	:impl(new Impl())
{
	impl->init(name, AnyBasic(), type, description, group);
}

Option::~Option()
{
	if (impl) {
		delete impl;
		impl = 0;
	}
}

OptionGroup* Option::group()
{
	return impl->group;
}

bool Option::in(OptionGroup* g)
{
	bool result = false;
	OptionGroup *p = g;
	//assume g is deeper
	while (!result && g) {
		result = g == impl->group;
		g = g->impl->parent;
	}
	if (result)
		return true;
	g = impl->group;
	//assume group() is deeper
	while (!result && g) {
		result = g == p;
		g = g->impl->parent;
	}
	return result;
}

bool Option::isLongOption() const
{
	return !impl->long_name.empty();
}
	
bool Option::isShortOption() const
{
	return !impl->short_name.empty();
}

const char* Option::longName() const
{
	return impl->long_name.c_str();
}

const char* Option::shortName() const
{
	return impl->short_name.c_str();
}

const char* Option::name() const
{
	return impl->name.c_str();
}

const char* Option::description() const
{
	return impl->description.c_str();
}

AnyBasic Option::value() const
{
	return impl->value;
}

AnyBasic Option::defaultValue() const
{
	return impl->default_value;
}

void Option::setValue(const AnyBasic& value)
{
	impl->value = value;
}

Type Option::type() const
{
	return impl->type;
}

void Option::setType(Type type)
{
	impl->type = type;
}
//calculate the space for all options
void Option::print()
{
	//TODO: compute in group once
	int max_name_len = group()->impl->option_name_width();
	int name_len = impl->name.size();
	string offset_space(group()->impl->offset(group()) + 2, ' ');
	string space_fill_name(max_name_len - name_len, ' ');
	cout << offset_space << impl->name << space_fill_name << "      " << impl->description;

	if (impl->type != NoToken) {
		cout << " (default=" << impl->default_value.to<string>() << ")";
	}
	cout << endl;
}	



/*!
  TODO: nest group option domain
*/
//OptionGroup

/*
OptionGroup::OptionGroup(const char* name, const char* description, OptionGroup* parent)
	:impl(new Impl())
{
	impl->parent = parent;
	impl->description = description;
}
*/
OptionGroup::OptionGroup(const char* description, OptionGroup* parent)
	:impl(new Impl())
{
	impl->parent = parent;
	impl->description = description;
}

OptionGroup::~OptionGroup()
{
	if (impl) {
		delete impl;
		impl = 0;
	}
}

OptionGroup& OptionGroup::parent()
{
	//printf("parent=%p\n", impl->parent);
	if (!impl->parent) {
		if (!invalid_group) {
			invalid_group = new OptionGroup("");
			invalid_group->impl->invalid = true;
		}
		return *invalid_group;
	}

	return *impl->parent;
}

void OptionGroup::print()
{
	impl->print(this);
}

OptionGroup& OptionGroup::operator [](const OptionGroup& g)
{	
	if (this == invalid_group)
		return add(g.description());
	OptionGroup *child = new OptionGroup(/*g.name(), */g.description(), this);
	impl->childs.push_back(child);
	return *child;
}

OptionGroup& OptionGroup::operator ()()
{
	return parent();
}

//FIXME: if invalid, add to previous root group. now it will do nothing
OptionGroup& OptionGroup::operator ()(const char* name, const char* description)
{
	if (this == invalid_group)
		return *invalid_group;

	Option *p = new Option(name, description, this);
	impl->addOption(p);
	return *this;
}

OptionGroup& OptionGroup::operator ()(const char* name, Type type, const char* description)
{
	if (this == invalid_group)
		return *invalid_group;

	Option *p = new Option(name, type, description, this);
	impl->addOption(p);
	return *this;
}

OptionGroup& OptionGroup::operator ()(const char* name, const AnyBasic& defaultValue, const char* description)
{
	if (this == invalid_group)
		return *invalid_group;

	Option *p = new Option(name, defaultValue, description, this);
	impl->addOption(p);
	return *this;
}

OptionGroup& OptionGroup::operator ()(const char* name, const AnyBasic& defaultValue, Type type, const char* description)
{
	if (this == invalid_group)
		return *invalid_group;

	Option *p = new Option(name, defaultValue, type, description, this);
	impl->addOption(p);
	return *this;
}

const char* OptionGroup::description() const
{
	return impl->description.c_str();
}

int OptionGroup::depth()
{
	if (impl->depth >= 0)
		return impl->depth;
	impl->compute_depth(this);
	return impl->depth;
}

} //namespace ProgramOptions
