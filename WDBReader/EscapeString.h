/* Copyright (c) 2000, 2019, Oracle and/or its affiliates. All rights reserved.
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.
   This program is also distributed with certain software (including
   but not limited to OpenSSL) that is licensed under separate terms,
   as designated in a particular file or component or in included license
   documentation.  The authors of MySQL hereby grant you an additional
   permission to link the program and your derivative works with the
   separately licensed software that they have included with MySQL.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License, version 2.0, for more details.
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
*/

#ifndef H_ESCAPE_STRING
#define H_ESCAPE_STRING

#include "Defines.h"
#include <string>
#include <cstring>

size_t escape_string_for_mysql(char *to,
    size_t to_length, const char *from,
    size_t length) {
    const char *to_start = to;
    const char *end,
        *to_end = to_start + (to_length ? to_length - 1 : 2 * length);
    bool overflow = false;
    bool use_mb_flag = false;
    for (end = from + length; from < end; from++) {
        char escape = 0;
        int tmp_length;
        if (use_mb_flag && (tmp_length = 0)) {
            if (to + tmp_length > to_end) {
                overflow = true;
                break;
            }
            while (tmp_length--) *to++ = *from++;
            from--;
            continue;
        }
        /*
        If the next character appears to begin a multi-byte character, we
        escape that first byte of that apparent multi-byte character. (The
        character just looks like a multi-byte character -- if it were actually
        a multi-byte character, it would have been passed through in the test
        above.)
        Without this check, we can create a problem by converting an invalid
        multi-byte character into a valid one. For example, 0xbf27 is not
        a valid GBK character, but 0xbf5c is. (0x27 = ', 0x5c = \)
        */
        tmp_length = use_mb_flag ? 1 : 0;
        if (tmp_length > 1)
            escape = *from;
        else
            switch (*from) {
            case 0: /* Must be escaped for 'mysql' */
                escape = '0';
                break;
            case '\n': /* Must be escaped for logs */
                escape = 'n';
                break;
            case '\r':
                escape = 'r';
                break;
            case '\\':
                escape = '\\';
                break;
            case '\'':
                escape = '\'';
                break;
            case '"': /* Better safe than sorry */
                escape = '"';
                break;
            case '\032': /* This gives problems on Win32 */
                escape = 'Z';
                break;
            }
        if (escape) {
            if (to + 2 > to_end) {
                overflow = true;
                break;
            }
            *to++ = '\\';
            *to++ = escape;
        }
        else {
            if (to + 1 > to_end) {
                overflow = true;
                break;
            }
            *to++ = *from;
        }
    }
    *to = 0;
    return overflow ? (size_t)-1 : (size_t)(to - to_start);
}

uint32 mysql_escape_string(char *to, const char *from, uint32 length) {
    return (uint32)escape_string_for_mysql(to, 0, from,
        length);
}

std::string EscapeString(char* unescapedString)
{
    char* escapedString = new char[strlen(unescapedString) * 2 + 1];
    mysql_escape_string(escapedString, unescapedString, strlen(unescapedString));
    std::string returnString = escapedString;
    delete[] escapedString;
    return returnString;
}

std::string EscapeString(std::string unescapedString)
{
    char* escapedString = new char[unescapedString.length() * 2 + 1];
    mysql_escape_string(escapedString, unescapedString.c_str(), unescapedString.length());
    std::string returnString = escapedString;
    delete[] escapedString;
    return returnString;
}

#endif
