/*
    Copyright (c) 2010-2017 Xavier Leclercq

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#include "HeaderMessageSection.h"
#include <boost/endian/conversion.hpp>

namespace Ishiko
{
namespace DNS
{

HeaderMessageSection::HeaderMessageSection()
    : m_ID(0), m_bitField(0), m_QDCOUNT(0), m_ANCOUNT(0),
    m_NSCOUNT(0), m_ARCOUNT(0)
{
}

HeaderMessageSection::HeaderMessageSection(bool isResponse)
    : m_ID(0), m_bitField(0), m_QDCOUNT(0), m_ANCOUNT(0), 
    m_NSCOUNT(0), m_ARCOUNT(0)
{
    if (isResponse)
    {
        m_bitField |= 0x8000;
    }
}

Result HeaderMessageSection::initializeFromBuffer(const char* startPos,
                                                  const char* endPos,
                                                  const char** currentPos)
{
    Result result(Result::eSuccess);

    if ((*currentPos + 12) <= endPos)
    {
        m_ID = boost::endian::big_to_native(*(const uint16_t*)(*currentPos));
        *currentPos += 2;
        m_bitField = boost::endian::big_to_native(*(const uint16_t*)(*currentPos));
        *currentPos += 2;
        m_QDCOUNT = boost::endian::big_to_native(*(const uint16_t*)(*currentPos));
        *currentPos += 2;
        m_ANCOUNT = boost::endian::big_to_native(*(const uint16_t*)(*currentPos));
        *currentPos += 2;
        m_NSCOUNT = boost::endian::big_to_native(*(const uint16_t*)(*currentPos));
        *currentPos += 2;
        m_ARCOUNT = boost::endian::big_to_native(*(const uint16_t*)(*currentPos));
        *currentPos += 2;
    }
    else
    {
        result.update(Result::eError);
    }

    return result;
}

void HeaderMessageSection::write(std::ostream& stream) const
{
    uint16_t tmp = boost::endian::native_to_big(m_ID);
    stream.write((const char*)&tmp, 2);
    tmp = boost::endian::native_to_big(m_bitField);
    stream.write((const char*)&tmp, 2);
    tmp = boost::endian::native_to_big(m_QDCOUNT);
    stream.write((const char*)&tmp, 2);
    tmp = boost::endian::native_to_big(m_ANCOUNT);
    stream.write((const char*)&tmp, 2);
    tmp = boost::endian::native_to_big(m_NSCOUNT);
    stream.write((const char*)&tmp, 2);
    tmp = boost::endian::native_to_big(m_ARCOUNT);
    stream.write((const char*)&tmp, 2);
};

bool HeaderMessageSection::isResponse() const
{
    return (m_bitField & 0x8000);
}

uint16_t HeaderMessageSection::questionCount() const
{
    return m_QDCOUNT;
}

void HeaderMessageSection::setQuestionCount(uint16_t count)
{
    m_QDCOUNT = count;
}

uint16_t HeaderMessageSection::answerCount() const
{
    return m_ANCOUNT;
}

}
}
