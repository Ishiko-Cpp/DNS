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

StartOfAuthorityRecord::StartOfAuthorityRecord(const std::string& domainName, 
                                               CLASS cl,
                                               uint32_t ttl,
                                               const std::string& authoritativeNameServer,
                                               const std::string& mailbox, 
                                               uint32_t serial,
                                               uint32_t refresh,
                                               uint32_t retry,
                                               uint32_t expire,
                                               uint32_t minimum)
    : ResourceRecord(domainName, TYPE_SOA, cl, ttl),
    m_MNAME(authoritativeNameServer), m_RNAME(mailbox), m_SERIAL(serial),
    m_REFRESH(refresh), m_RETRY(retry), m_EXPIRE(expire), m_MINIMUM(minimum)
{
}

const DomainName& StartOfAuthorityRecord::mname() const
{
    return m_MNAME;
}

void StartOfAuthorityRecord::writeBinary(std::ostream& stream) const
{
    writeBinaryBase(stream);
    uint16_t tmp = boost::endian::native_to_big((uint16_t)(m_MNAME.length() + m_RNAME.length() + 20));
    stream.write((const char*)&tmp, 2);
    m_MNAME.write(stream);
    m_RNAME.write(stream);
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
}

}
}
