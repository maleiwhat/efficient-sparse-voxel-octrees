/*
 *  Copyright 2009-2010 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
 
#include "base/String.hpp"

#include <stdio.h>

using namespace FW;

//------------------------------------------------------------------------

String& String::set(char chr)
{
    m_chars.reset(2);
    m_chars[0] = chr;
    m_chars[1] = '\0';
    return *this;
}

//------------------------------------------------------------------------

String& String::set(const char* chars)
{
    int len = strlen(chars);
    if (!len)
        return reset();

    m_chars.set(chars, len + 1);
    return *this;
}

//------------------------------------------------------------------------

String& String::setf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    setfv(fmt, args);
    va_end(args);
    return *this;
}

//------------------------------------------------------------------------

String& String::setfv(const char* fmt, va_list args)
{
    int len = _vscprintf(fmt, args);
    if (!len)
        return reset();

    m_chars.reset(len + 1);
    vsprintf_s(m_chars.getPtr(), len + 1, fmt, args);
    return *this;
}

//------------------------------------------------------------------------

String String::substring(int start, int end) const
{
    FW_ASSERT(end <= getLength());

    String res;
    res.m_chars.reset(end - start + 1);
    Array<char>::copy(res.m_chars.getPtr(), m_chars.getPtr(start), end - start);
    res.m_chars[end - start] = '\0';
    return res;
}

//------------------------------------------------------------------------

String& String::append(char chr)
{
    int len = getLength();
    m_chars.resize(len + 2);
    m_chars[len] = chr;
    m_chars[len + 1] = '\0';
    return *this;
}

//------------------------------------------------------------------------

String& String::append(const char* chars)
{
    int lenA = getLength();
    int lenB = strlen(chars);
    m_chars.resize(lenA + lenB + 1);
    Array<char>::copy(m_chars.getPtr(lenA), chars, lenB);
    m_chars[lenA + lenB] = '\0';
    return *this;
}

//------------------------------------------------------------------------

String& String::append(const String& other)
{
    if (&other != this)
        return append(other.getPtr());

    String tmp = other;
    return append(tmp.getPtr());
}

//------------------------------------------------------------------------

String& String::appendf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    appendfv(fmt, args);
    va_end(args);
    return *this;
}

//------------------------------------------------------------------------

String& String::appendfv(const char* fmt, va_list args)
{
    int lenA = getLength();
    int lenB = _vscprintf(fmt, args);
    m_chars.resize(lenA + lenB + 1);
    vsprintf_s(m_chars.getPtr(lenA), lenB + 1, fmt, args);
    return *this;
}

//------------------------------------------------------------------------

String String::toUpper(void) const
{
    String str;
    str.m_chars.reset(m_chars.getSize());
    for (int i = 0; i < m_chars.getSize(); i++)
    {
        char c = m_chars[i];
        if (c >= 'a' && c <= 'z')
            c += 'A' - 'a';
        str.m_chars[i] = c;
    }
    return str;
}

//------------------------------------------------------------------------

String String::toLower(void) const
{
    String str;
    str.m_chars.reset(m_chars.getSize());
    for (int i = 0; i < m_chars.getSize(); i++)
    {
        char c = m_chars[i];
        if (c >= 'A' && c <= 'Z')
            c += 'a' - 'A';
        str.m_chars[i] = c;
    }
    return str;
}

//------------------------------------------------------------------------

bool String::startsWith(const String& str) const
{
    const char* a = getPtr();
    const char* b = str.getPtr();
    for (int ofs = 0; b[ofs]; ofs++)
        if (a[ofs] != b[ofs])
            return false;
    return true;
}

//------------------------------------------------------------------------

bool String::endsWith(const String& str) const
{
    int a = getLength();
    int b = str.getLength();
    if (a < b)
        return false;
    return (strcmp(getPtr() + a - b, str.getPtr()) == 0);
}

//------------------------------------------------------------------------

String String::getFileName(void) const
{
    int idx = max(lastIndexOf('/'), lastIndexOf('\\'));
    return (idx == -1) ? *this : substring(idx + 1, getLength());
}

//------------------------------------------------------------------------

String String::getDirName(void) const
{
    int idx = max(lastIndexOf('/'), lastIndexOf('\\'));
    return (idx == -1) ? "." : substring(0, idx);
}

//------------------------------------------------------------------------

int String::strlen(const char* chars)
{
    if (!chars)
        return 0;

    int len = 0;
    while (chars[len])
        len++;
    return len;
}

//------------------------------------------------------------------------

int String::strcmp(const char* a, const char* b)
{
    int ofs = 0;
    while (a[ofs] && a[ofs] == b[ofs])
        ofs++;
    return a[ofs] - b[ofs];
}

//------------------------------------------------------------------------

bool FW::parseSpace(const char*& ptr)
{
    FW_ASSERT(ptr);
    while (*ptr == ' ')
        ptr++;
    return true;
}

//------------------------------------------------------------------------

bool FW::parseChar(const char*& ptr, char chr)
{
    FW_ASSERT(ptr);
    if (*ptr != chr)
        return false;
    ptr++;
    return true;
}

//------------------------------------------------------------------------

bool FW::parseLiteral(const char*& ptr, const char* str)
{
    FW_ASSERT(ptr && str);
    const char* tmp = ptr;

    while (*str && *tmp == *str)
    {
        tmp++;
        str++;
    }
    if (*str)
        return false;

    ptr = tmp;
    return true;
}

//------------------------------------------------------------------------

bool FW::parseInt(const char*& ptr, S32& value)
{
    const char* tmp = ptr;
    S32 v = 0;
    bool neg = (!parseChar(tmp, '+') && parseChar(tmp, '-'));
    if (*tmp < '0' || *tmp > '9')
        return false;
    while (*tmp >= '0' && *tmp <= '9')
        v = v * 10 + *tmp++ - '0';

    value = (neg) ? -v : v;
    ptr = tmp;
    return true;
}

//------------------------------------------------------------------------

bool FW::parseFloat(const char*& ptr, F32& value)
{
    const char* tmp = ptr;
    bool neg = (!parseChar(tmp, '+') && parseChar(tmp, '-'));

    F32 v = 0.0f;
    int numDigits = 0;
    while (*tmp >= '0' && *tmp <= '9')
    {
        v = v * 10.0f + (F32)(*tmp++ - '0');
        numDigits++;
    }
    if (parseChar(tmp, '.'))
    {
        F32 scale = 1.0f;
        while (*tmp >= '0' && *tmp <= '9')
        {
            scale *= 0.1f;
            v += scale * (F32)(*tmp++ - '0');
            numDigits++;
        }
    }
    if (!numDigits)
        return false;

    ptr = tmp;
    S32 e = 0;
    if ((parseChar(tmp, 'e') || parseChar(tmp, 'E')) && parseInt(tmp, e))
    {
        ptr = tmp;
        if (e)
            v *= pow(10.0f, (F32)e);
    }
    value = (neg) ? -v : v;
    return true;
}

//------------------------------------------------------------------------