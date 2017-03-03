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

#include "QuestionMessageSectionEntry.h"
#include <boost/endian/conversion.hpp>

namespace Ishiko
{
namespace DNS
{

QuestionMessageSectionEntry::QuestionMessageSectionEntry()
{
}

QuestionMessageSectionEntry::QuestionMessageSectionEntry(const std::string& domainName,
                                                         QTYPE qtype, 
                                                         QCLASS qclass)
    : m_QNAME(domainName), m_QTYPE(qtype), m_QCLASS(qclass)
{
}

Result QuestionMessageSectionEntry::initializeFromBuffer(const char* startPos,
                                                         const char* endPos,
                                                         const char** currentPos)
{
    Result result(Result::eSuccess);

    const char* localCurrentPos = *currentPos;

    DomainName tempDomainName;
    if (tempDomainName.initializeFromBuffer(startPos, endPos, &localCurrentPos).succeeded() &&
        ((localCurrentPos + 4) <= endPos))
    {
        m_QNAME.swap(tempDomainName);
        m_QTYPE = boost::endian::big_to_native(*(const uint16_t*)(localCurrentPos));
        localCurrentPos += 2;
        m_QCLASS = boost::endian::big_to_native(*(const uint16_t*)(localCurrentPos));
        localCurrentPos += 2;

        *currentPos = localCurrentPos;
    }
    else
    {
        result.update(Result::eError);
    }

    return result;
}

const DomainName& QuestionMessageSectionEntry::qname() const
{
    return m_QNAME;
}

uint16_t QuestionMessageSectionEntry::qtype() const
{
    return m_QTYPE;
}

uint16_t QuestionMessageSectionEntry::qclass() const
{
    return m_QCLASS;
}

void QuestionMessageSectionEntry::write(std::ostream& stream) const
{
    m_QNAME.writeBinary(stream);
    uint16_t tmp = boost::endian::native_to_big(m_QTYPE);
    stream.write((char*)&tmp, 2);
    tmp = boost::endian::native_to_big(m_QCLASS);
    stream.write((char*)&tmp, 2);
}

}
}
