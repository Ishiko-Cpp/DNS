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

#ifndef _ISHIKO_DNS_CORE_IPV6ADDRESSRECORD_H_
#define _ISHIKO_DNS_CORE_IPV6ADDRESSRECORD_H_

#include "ResourceRecord.h"
#include <boost/asio/ip/address_v6.hpp>

namespace Ishiko
{
namespace DNS
{

// This class represents an AAAA record.
class IPv6AddressRecord : public ResourceRecord
{
public:
    IPv6AddressRecord(const std::string& domainName, uint32_t ttl,
        const std::string& address);

    void writeBinary(std::ostream& stream) const override;
    void writeText(std::ostream& stream) const override;

private:
    std::array<unsigned char, 16> m_IPv6Address;
};

}
}

#endif
