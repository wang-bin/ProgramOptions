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


