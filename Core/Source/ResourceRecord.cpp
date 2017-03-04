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

#include "ResourceRecord.h"
#include "AddressRecord.h"
#include "NameServerRecord.h"
#include "CanonicalNameRecord.h"
#include "StartOfAuthorityRecord.h"
#include "MailExchangeRecord.h"
#include "TextRecord.h"
#include "IPv6AddressRecord.h"
#include <boost/endian/conversion.hpp>

namespace Ishiko
{
namespace DNS
{

Result ResourceRecord::createFromBuffer(const char* startPos,
                                        const char* endPos, 
                                        const char** currentPos,
                                        std::shared_ptr<ResourceRecord>& newRecord)
{
    Result result(Result::eSuccess);

    const char* localCurrentPos = *currentPos;

    const char* tempPos = localCurrentPos;
    DomainName domainName;
    result.update(domainName.initializeFromBuffer(startPos, endPos, &tempPos));
    if (result.succeeded())
    {
        uint16_t type = boost::endian::big_to_native(*(const uint16_t*)(tempPos));

        switch (type)
        {
            case ResourceRecord::TYPE_A:
                {
                    std::shared_ptr<AddressRecord> record = std::make_shared<AddressRecord>();
                    if (result.update(record->initializeFromBuffer(startPos, endPos, &localCurrentPos)).succeeded())
                    {
                        newRecord = record;
                    }
                }
                break;

            case ResourceRecord::TYPE_NS:
                {
                    std::shared_ptr<NameServerRecord> record = std::make_shared<NameServerRecord>();
                    if (result.update(record->initializeFromBuffer(startPos, endPos, &localCurrentPos)).succeeded())
                    {
                        newRecord = record;
                    }
                }
                break;

            case ResourceRecord::TYPE_CNAME:
                {
                    std::shared_ptr<CanonicalNameRecord> record = std::make_shared<CanonicalNameRecord>();
                    if (result.update(record->initializeFromBuffer(startPos, endPos, &localCurrentPos)).succeeded())
                    {
                        newRecord = record;
                    }
                }
                break;

            case ResourceRecord::TYPE_SOA:
                {
                    std::shared_ptr<StartOfAuthorityRecord> record = std::make_shared<StartOfAuthorityRecord>();
                    if (result.update(record->initializeFromBuffer(startPos, endPos, &localCurrentPos)).succeeded())
                    {
                        newRecord = record;
                    }
                }
                break;


            case ResourceRecord::TYPE_MX:
                {
                    std::shared_ptr<MailExchangeRecord> record = std::make_shared<MailExchangeRecord>();
                    if (result.update(record->initializeFromBuffer(startPos, endPos, &localCurrentPos)).succeeded())
                    {
                        newRecord = record;
                    }
                }
                break;

            case ResourceRecord::TYPE_TXT:
                {
                    std::shared_ptr<TextRecord> record = std::make_shared<TextRecord>();
                    if (result.update(record->initializeFromBuffer(startPos, endPos, &localCurrentPos)).succeeded())
                    {
                        newRecord = record;
                    }
                }
                break;

            case ResourceRecord::TYPE_AAAA:
                {
                    std::shared_ptr<IPv6AddressRecord> record = std::make_shared<IPv6AddressRecord>();
                    if (result.update(record->initializeFromBuffer(startPos, endPos, &localCurrentPos)).succeeded())
                    {
                        newRecord = record;
                    }
                }
                break;

            default:
                result.update(Result::eError);
                break;
        }
    }

    return result;
}

ResourceRecord::ResourceRecord()
    : m_TTL(0)
{
}

ResourceRecord::ResourceRecord(const std::string& domainName,
                               TYPE type,
                               CLASS cl,
                               uint32_t ttl)
    : m_NAME(domainName), m_TYPE(type), m_CLASS(cl), m_TTL(ttl)
{
}

ResourceRecord::~ResourceRecord()
{
}

const DomainName& ResourceRecord::name() const
{
    return m_NAME;
}

ResourceRecord::TYPE ResourceRecord::type() const
{
    return (ResourceRecord::TYPE)m_TYPE;
}

ResourceRecord::CLASS ResourceRecord::cl() const
{
    return (ResourceRecord::CLASS)m_CLASS;
}

const TTL& ResourceRecord::ttl() const
{
    return m_TTL;
}

Result ResourceRecord::initializeFromBufferBase(const char* startPos,
                                                const char* endPos,
                                                const char** currentPos)
{
    Result result(Result::eSuccess);

    const char* localCurrentPos = *currentPos;

    if (m_NAME.initializeFromBuffer(startPos, endPos, &localCurrentPos).succeeded() &&
        ((localCurrentPos + 4) <= endPos))
    {
        m_TYPE = boost::endian::big_to_native(*(const uint16_t*)(localCurrentPos));
        localCurrentPos += 2;
        m_CLASS = boost::endian::big_to_native(*(const uint16_t*)(localCurrentPos));
        localCurrentPos += 2;
        if (result.update(m_TTL.initializeFromBuffer(startPos, endPos, &localCurrentPos)).succeeded())
        {
            *currentPos = localCurrentPos;
        }
    }
    else
    {
        result.update(Result::eError);
    }

    return result;
}

void ResourceRecord::writeBinaryBase(std::ostream& stream) const
{
    m_NAME.writeBinary(stream);
    uint16_t tmp = boost::endian::native_to_big(m_TYPE);
    stream.write((char*)&tmp, 2);
    tmp = boost::endian::native_to_big(m_CLASS);
    stream.write((char*)&tmp, 2);
    m_TTL.writeBinary(stream);
}

void ResourceRecord::writeTextBase(std::ostream& stream) const
{
    stream << m_NAME.str().c_str() << " ";
    switch (m_CLASS)
    {
    case ResourceRecord::CLASS_IN:
        stream << "IN";
        break;

    default:
        // TODO
        break;
    }
    stream << " ";
    switch (m_TYPE)
    {
    case ResourceRecord::TYPE_A:
        stream << "A";
        break;

    case ResourceRecord::TYPE_NS:
        stream << "NS";
        break;

    case ResourceRecord::TYPE_SOA:
        stream << "SOA";
        break;

    default:
        // TODO
        break;
    }
}

}
}
