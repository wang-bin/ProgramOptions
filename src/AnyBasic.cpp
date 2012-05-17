#include "AnyBasic.h"

AnyBasic::AnyBasic(const AnyBasic& that)
{
	value_str = that.value_str;
}

AnyBasic& AnyBasic::operator =(const AnyBasic& that)
{
	value_str = that.value_str;
	return *this;
}

std::ostream& operator<<(std::ostream& out, const AnyBasic& any)
{
	return out << any.str();
}


