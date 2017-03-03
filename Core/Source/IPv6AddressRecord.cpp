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

#include "IPv6AddressRecord.h"
#include <boost/endian/conversion.hpp>

namespace Ishiko
{
namespace DNS
{

IPv6AddressRecord::IPv6AddressRecord()
{
}

IPv6AddressRecord::IPv6AddressRecord(const std::string& domainName,
                                     uint32_t ttl,
                                     const std::string& address)
    : ResourceRecord(domainName, TYPE_AAAA, CLASS_IN, ttl)
{
    m_IPv6Address = boost::asio::ip::address_v6::from_string(address).to_bytes();
}

Result IPv6AddressRecord::initializeFromBuffer(const char* startPos,
                                               const char* endPos,
                                               const char** currentPos)
{
    Result result(Result::eSuccess);

    const char* localCurrentPos = *currentPos;

    if (result.update(initializeFromBufferBase(startPos, endPos, &localCurrentPos)).succeeded())
    {
        if ((localCurrentPos + 18) <= endPos)
        {
            uint16_t size = boost::endian::big_to_native(*(const uint16_t*)(localCurrentPos));
            localCurrentPos += 2;
            if (size == 16)
            {
                memcpy(m_IPv6Address.data(), localCurrentPos, 16);
                localCurrentPos += 16;
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

boost::asio::ip::address_v6 IPv6AddressRecord::IPAddress() const
{
    return boost::asio::ip::address_v6(m_IPv6Address);
}

void IPv6AddressRecord::writeBinary(std::ostream& stream) const
{
    writeBinaryBase(stream);
    stream.write("\x00\x10", 2);
    stream.write((const char*)m_IPv6Address.data(), m_IPv6Address.size());
}

void IPv6AddressRecord::writeText(std::ostream& stream) const
{
}

}
}
