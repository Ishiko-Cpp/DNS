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

#include "AnswerMessageSection.h"
#include "AddressRecord.h"
#include "NameServerRecord.h"
#include "StartOfAuthorityRecord.h"
#include <boost/endian/conversion.hpp>

namespace Ishiko
{
namespace DNS
{

AnswerMessageSection::AnswerMessageSection()
{
}

Result AnswerMessageSection::initializeFromBuffer(uint16_t count,
                                                  const char* startPos,
                                                  const char* endPos, 
                                                  const char** currentPos)
{
    Result result(Result::eSuccess);

    const char* localCurrentPos = *currentPos;

    std::vector<std::shared_ptr<ResourceRecord> > newResourceRecords;

    for (size_t i = 0; (i < count) && result.succeeded(); ++i)
    {
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
                    std::shared_ptr<AddressRecord> newRecord = std::make_shared<AddressRecord>();
                    result.update(newRecord->initializeFromBuffer(startPos, endPos, &localCurrentPos));
                    newResourceRecords.push_back(newRecord);
                }
                break;

            case ResourceRecord::TYPE_NS:
                {
                    std::shared_ptr<NameServerRecord> newRecord = std::make_shared<NameServerRecord>();
                    result.update(newRecord->initializeFromBuffer(startPos, endPos, &localCurrentPos));
                    newResourceRecords.push_back(newRecord);
                }
                break;

            case ResourceRecord::TYPE_SOA:
                {
                    std::shared_ptr<StartOfAuthorityRecord> newRecord = std::make_shared<StartOfAuthorityRecord>();
                    result.update(newRecord->initializeFromBuffer(startPos, endPos, &localCurrentPos));
                    newResourceRecords.push_back(newRecord);
                }
                break;

            default:
                result.update(Result::eError);
                break;
            }
        }
    }

    if (result.succeeded())
    {
        m_resourceRecords.swap(newResourceRecords);
        *currentPos = localCurrentPos;
    }

    return result;
}

const std::vector<std::shared_ptr<ResourceRecord> >& AnswerMessageSection::resourceRecords() const
{
    return m_resourceRecords;
}

void AnswerMessageSection::appendResourceRecord(std::shared_ptr<ResourceRecord> record)
{
    m_resourceRecords.push_back(record);
}

void AnswerMessageSection::write(std::ostream& stream) const
{
    for (auto i : m_resourceRecords)
    {
        i->writeBinary(stream);
    }
}

}
}
