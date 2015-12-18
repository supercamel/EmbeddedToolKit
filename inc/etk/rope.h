/*
    Embedded Tool Kit
    Copyright (C) 2015 Samuel Cowen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ROPE_H_INCLUDED
#define ROPE_H_INCLUDED

#include <stdint.h>

namespace etk
{

/**
 * \class Rope
 *
 * \brief WHY ROPE? It's like a string, only more robust.
 *
 * Rope is a C-string manipulation class.
 *
 * Many C-string functions are prohibited by JSF-AV standard.
 * Rope is designed to replace standard C string functions
 * such as strncmp, strcpy and sprintf
 *
 * It uses pre-allocated buffers instead of dynamic memory like
 * std::string, so it's relatively safe to use in embedded systems
 * with tight memory constraints.
 *
 * http://www.camelsoftware.com/blog/2015/12/11/ditching-c-libraries-concatenation-of-c-strings-without-stdio/
 */

class Rope
{
public:
    Rope(char* buf, uint16_t maxlen, const char* c);
    Rope(char* c, uint16_t maxlen);
    Rope(Rope& r);
    Rope(const Rope& r);


    void append(char c);
    void append(const char* s, int len = 0);
    void append(int32_t j, uint32_t npad = 1);
    void append(uint32_t j, uint32_t npad = 1);
    void append(int64_t j, uint32_t npad = 1);
    void append(uint64_t j, uint32_t npad = 1);
    void append(float j, uint8_t precision = 2);
    void append(double d, uint8_t precision = 2);
    void append(Rope sb, uint16_t len = 0);

    uint32_t length();

    Rope& operator << (const char* s);
    Rope& operator << (int32_t i);
    Rope& operator << (uint32_t i);
    Rope& operator << (int64_t i);
    Rope& operator << (uint64_t i);
    Rope& operator << (float i);
    Rope& operator << (double d);
    Rope& operator << (char i);
    Rope& operator << (Rope& s);

    Rope& operator + (Rope& s);
    Rope& operator += (Rope& s);

    Rope& operator + (char* s);
    Rope& operator += (char* s);

    Rope& operator + (const char* s);
    Rope& operator += (const char* s);

    Rope& operator = (const char*);
    Rope operator = (Rope r);

    char& operator [](uint16_t p);

    bool operator == (Rope r);
    bool operator != (Rope r);

    bool compare(Rope r, uint32_t len = 0);
    bool compare(const char* c, uint32_t len = 0);
    bool compare(Rope r, uint32_t start_this, uint32_t start_that, uint32_t len);
    bool compare(const char* c, uint32_t start_this, uint32_t start_that, uint32_t len);

    void sub_string(char* buf, uint32_t start, uint32_t len);
    void sub_string(Rope& r, uint32_t start, uint32_t len);

    const char* c_str();

    void clear();

    int atoi();
    float atof();

    void set_cursor(uint16_t p) {
        pos = p;
    }
    uint16_t get_cursor() {
        return pos;
    }

    char* raw_buffer();
    void set_buffer(char* b) {
        str = b;
    }

    void copy(char* b, uint32_t len=0) const;

    static uint16_t c_strlen(const char* c, uint16_t maxlen);

private:
    void terminate(); //appends a null terminator
    uint16_t pos;
    uint16_t N;
    char* str;
};

}


#endif // ROPE_H_INCLUDED
