#ifndef CCONS_STRING_UTILS_H
#define CCONS_STRING_UTILS_H

//
// Defines useful string utility functions used by ccons.
//
// Part of ccons, the interactive console for the C programming language.
//
// Copyright (c) 2009 Alexei Svitkine. This file is distributed under the
// terms of MIT Open Source License. See file LICENSE for details.
//

#include <iostream>
#include <string>
#include <sstream>
#include <stdarg.h>

namespace ccons {

// Like vsprintf(), but to a std::string.
void vstring_printf(std::string *dst, const char *fmt, va_list ap);

// Like sprintf(), but to a std::string.
void string_printf(std::string *dst, const char *fmt, ...);

// Like printf(), but to a std::ostream.
void oprintf(std::ostream& out, const char *fmt, ...);

// Convert specified value to a std::string using a std::stringstream.
template<typename T>
inline std::string to_string(const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

} // namespace ccons

#endif // CCONS_STRING_UTILS_H
