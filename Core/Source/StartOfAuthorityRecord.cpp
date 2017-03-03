/*
    Copyright (c) 2015-2017 Xavier Leclercq

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

#include "StartOfAuthorityRecord.h"
#include <boost/endian/conversion.hpp>

namespace Ishiko
{
namespace DNS
{
    
StartOfAuthorityRecord::StartOfAuthorityRecord()
{
}

StartOfAuthorityRecord::StartOfAuthorityRecord(const std::string& domainName, 
                                               CLASS cl,
                                               uint32_t ttl,
                                               const std::string& primaryNameServer,
                                               const std::string& mailbox, 
                                               uint32_t serial,
                                               uint32_t refresh,
                                               uint32_t retry,
                                               uint32_t expire,
                                               uint32_t minimum)
    : ResourceRecord(domainName, TYPE_SOA, cl, ttl),
    m_MNAME(primaryNameServer), m_RNAME(mailbox), m_SERIAL(serial),
    m_REFRESH(refresh), m_RETRY(retry), m_EXPIRE(expire), m_MINIMUM(minimum)
{
}

Result StartOfAuthorityRecord::initializeFromBuffer(const char* startPos,
                                                    const char* endPos,
                                                    const char** currentPos)
{
    Result result(Result::eSuccess);

    const char* localCurrentPos = *currentPos;

    if (result.update(initializeFromBufferBase(startPos, endPos, &localCurrentPos)).succeeded())
    {
        if ((localCurrentPos + 2) <= endPos)
        {
            uint16_t size = boost::endian::big_to_native(*(const uint16_t*)(localCurrentPos));
            localCurrentPos += 2;
            DomainName localMNAME;
            DomainName localRNAME;
            if (result.update(localMNAME.initializeFromBuffer(startPos, endPos, &localCurrentPos)).succeeded() && 
                result.update(localRNAME.initializeFromBuffer(startPos, endPos, &localCurrentPos)).succeeded() &&
                ((localCurrentPos + 20) <= endPos) && 
                (size == (localMNAME.length() + localRNAME.length() + 20)))
            {
                m_MNAME.swap(localMNAME);
                m_RNAME.swap(localRNAME);

                m_SERIAL = boost::endian::big_to_native(*(const uint32_t*)(localCurrentPos));
                localCurrentPos += 4;
                m_REFRESH = boost::endian::big_to_native(*(const uint32_t*)(localCurrentPos));
                localCurrentPos += 4;
                m_RETRY = boost::endian::big_to_native(*(const uint32_t*)(localCurrentPos));
                localCurrentPos += 4;
                m_EXPIRE = boost::endian::big_to_native(*(const uint32_t*)(localCurrentPos));
                localCurrentPos += 4;
                m_MINIMUM = boost::endian::big_to_native(*(const uint32_t*)(localCurrentPos));
                localCurrentPos += 4;

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

const DomainName& StartOfAuthorityRecord::mname() const
{
    return m_MNAME;
}

const DomainName& StartOfAuthorityRecord::rname() const
{
    return m_RNAME;
}

uint32_t StartOfAuthorityRecord::serial() const
{
    return m_SERIAL;
}

uint32_t StartOfAuthorityRecord::refresh() const
{
    return m_REFRESH;
}

uint32_t StartOfAuthorityRecord::retry() const
{
    return m_RETRY;
}

uint32_t StartOfAuthorityRecord::expire() const
{
    return m_EXPIRE;
}

uint32_t StartOfAuthorityRecord::minimum() const
{
    return m_MINIMUM;
}

void StartOfAuthorityRecord::writeBinary(std::ostream& stream) const
{
    writeBinaryBase(stream);
    uint16_t tmp = boost::endian::native_to_big((uint16_t)(m_MNAME.length() + m_RNAME.length() + 20));
    stream.write((const char*)&tmp, 2);
    m_MNAME.writeBinary(stream);
    m_RNAME.writeBinary(stream);
    uint32_t tmp2 = boost::endian::native_to_big(m_SERIAL);
    stream.write((const char*)&tmp2, 4);
    tmp2 = boost::endian::native_to_big(m_REFRESH);
    stream.write((const char*)&tmp2, 4);
    tmp2 = boost::endian::native_to_big(m_RETRY);
    stream.write((const char*)&tmp2, 4);
    tmp2 = boost::endian::native_to_big(m_EXPIRE);
    stream.write((const char*)&tmp2, 4);
    tmp2 = boost::endian::native_to_big(m_MINIMUM);
    stream.write((const char*)&tmp2, 4);
}

void StartOfAuthorityRecord::writeText(std::ostream& stream) const
{
    stream << " ";
    m_MNAME.writeText(stream);
    stream << " ";
    m_RNAME.writeText(stream);
    stream << " (" << m_SERIAL << " " << m_REFRESH << " " << m_RETRY
        << " " << m_EXPIRE << " " << m_MINIMUM << ")";
}

}
}
