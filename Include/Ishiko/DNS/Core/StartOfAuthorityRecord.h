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

#ifndef _ISHIKO_DNS_CORE_STARTOFAUTHORITYRECORD_H_
#define _ISHIKO_DNS_CORE_STARTOFAUTHORITYRECORD_H_

#include "ResourceRecord.h"

namespace Ishiko
{
namespace DNS
{

// This class represents a SOA record.
class StartOfAuthorityRecord : public ResourceRecord
{
public:
    StartOfAuthorityRecord();
    StartOfAuthorityRecord(const std::string& domainName, CLASS cl,
        uint32_t ttl, const std::string& primaryNameServer,
        const std::string& mailbox, uint32_t serial, uint32_t refresh,
        uint32_t retry, uint32_t expire, uint32_t minimum);
    Result initializeFromBuffer(const char* startPos,
        const char* endPos, const char** currentPos);

    // Returns the primary nameserver.
    const DomainName& mname() const;
    // Returns the mailbox of the person responsible for this zone
    // coded as a domain name
    const DomainName& rname() const;
    uint32_t serial() const;
    uint32_t refresh() const;
    uint32_t retry() const;
    uint32_t expire() const;
    uint32_t minimum() const;

    void writeBinary(std::ostream& stream) const override;
    void writeText(std::ostream& stream) const override;

private:
    DomainName m_MNAME;
    DomainName m_RNAME;
    uint32_t m_SERIAL;
    uint32_t m_REFRESH;
    uint32_t m_RETRY;
    uint32_t m_EXPIRE;
    uint32_t m_MINIMUM;
};

}
}

#endif
