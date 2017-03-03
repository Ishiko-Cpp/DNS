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

#ifndef _ISHIKO_DNS_CORE_MAILEXCHANGERECORD_H_
#define _ISHIKO_DNS_CORE_MAILEXCHANGERECORD_H_

#include "ResourceRecord.h"

namespace Ishiko
{
namespace DNS
{

// This class represents an MX record.
class MailExchangeRecord : public ResourceRecord
{
public:
    MailExchangeRecord();
    MailExchangeRecord(const std::string& domainName, uint32_t ttl,
        uint16_t preference, const std::string& exchangeDomainName);
    Result initializeFromBuffer(const char* startPos,
        const char* endPos, const char** currentPos);

    uint16_t preference() const;
    const DomainName& exchange() const;

    void writeBinary(std::ostream& stream) const override;
    void writeText(std::ostream& stream) const override;

private:
    uint16_t m_PREFERENCE;
    DomainName m_EXCHANGE;
};

}
}

#endif
