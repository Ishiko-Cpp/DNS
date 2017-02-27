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

#include "AddressRecord.h"
#include <boost/endian/conversion.hpp>

namespace Ishiko
{
namespace DNS
{

AddressRecord::AddressRecord()
{
}

AddressRecord::AddressRecord(const std::string& domainName,
                             uint32_t ttl,
                             const std::string& dottedDecimalIP)
    : ResourceRecord(domainName, TYPE_A, CLASS_IN, ttl)
{
    m_IPv4Address = boost::asio::ip::address_v4::from_string(dottedDecimalIP).to_ulong();
}

Result AddressRecord::initializeFromBuffer(const char* startPos,
                                           const char* endPos,
                                           const char** currentPos)
{
    Result result(Result::eSuccess);

    const char* localCurrentPos = *currentPos;

    if (result.update(initializeFromBufferBase(startPos, endPos, &localCurrentPos)).succeeded())
    {
        if ((localCurrentPos + 6) <= endPos)
        {
            uint16_t size = boost::endian::big_to_native(*(const uint16_t*)(localCurrentPos));
            localCurrentPos += 2;
            if (size == 4)
            {
                m_IPv4Address = boost::endian::big_to_native(*(const uint32_t*)(localCurrentPos));
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

void AddressRecord::writeBinary(std::ostream& stream) const
{
    writeBinaryBase(stream);
    stream.write("\x00\x04", 2);
    uint32_t tmp = boost::endian::native_to_big(m_IPv4Address);
    stream.write((const char*)&tmp, 4);
}

void AddressRecord::writeText(std::ostream& stream) const
{
    writeTextBase(stream);
    stream << " ";
    boost::asio::ip::address_v4 address(m_IPv4Address);
    stream << address.to_string();
}

}
}
