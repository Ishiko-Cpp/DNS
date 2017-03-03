/*
    Copyright (c) 2017 Xavier Leclercq

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

#include "MailExchangeRecord.h"
#include <boost/endian/conversion.hpp>

namespace Ishiko
{
namespace DNS
{

MailExchangeRecord::MailExchangeRecord()
{
}

MailExchangeRecord::MailExchangeRecord(const std::string& domainName,
                                       uint32_t ttl,
                                       uint16_t preference,
                                       const std::string& exchangeDomainName)
    : ResourceRecord(domainName, TYPE_MX, CLASS_IN, ttl),
    m_PREFERENCE(preference), m_EXCHANGE(exchangeDomainName)
{
}

uint16_t MailExchangeRecord::preference() const
{
    return m_PREFERENCE;
}

const DomainName& MailExchangeRecord::exchange() const
{
    return m_EXCHANGE;
}

Result MailExchangeRecord::initializeFromBuffer(const char* startPos,
                                                const char* endPos,
                                                const char** currentPos)
{
    Result result(Result::eSuccess);

    const char* localCurrentPos = *currentPos;

    if (result.update(initializeFromBufferBase(startPos, endPos, &localCurrentPos)).succeeded())
    {
        if ((localCurrentPos + 4) <= endPos)
        {
            uint16_t size = boost::endian::big_to_native(*(const uint16_t*)(localCurrentPos));
            localCurrentPos += 2;
            uint16_t preference = boost::endian::big_to_native(*(const uint16_t*)(localCurrentPos));
            localCurrentPos += 2;
            DomainName tempDomainName;
            if (result.update(tempDomainName.initializeFromBuffer(startPos, endPos, &localCurrentPos)).succeeded() &&
                (size == (2 + tempDomainName.length())))
            {
                m_PREFERENCE = preference;
                m_EXCHANGE.swap(tempDomainName);
                *currentPos = localCurrentPos;
            }
            else
            {
                result.update(Result::eError);
            }
        }
        else
        {
            result.update(Result::eError);
        }
    }

    return result;
}

void MailExchangeRecord::writeBinary(std::ostream& stream) const
{
    writeBinaryBase(stream);
    uint16_t tmp = boost::endian::native_to_big((uint16_t)(m_EXCHANGE.length() + 2));
    stream.write((const char*)&tmp, 2);
    tmp = boost::endian::native_to_big(m_PREFERENCE);
    stream.write((const char*)&tmp, 2);
    m_EXCHANGE.writeBinary(stream);
}

void MailExchangeRecord::writeText(std::ostream& stream) const
{
}

}
}
