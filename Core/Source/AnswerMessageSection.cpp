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

    for (size_t i = 0; i < count; ++i)
    {
        DomainName domainName;
        domainName.initializeFromBuffer(startPos, endPos, &localCurrentPos);

        uint16_t type = boost::endian::big_to_native(*(const uint16_t*)(localCurrentPos));
        localCurrentPos += 2;

        switch (type)
        {
        case ResourceRecord::TYPE_A:
            {
                std::shared_ptr<AddressRecord> newRecord = std::make_shared<AddressRecord>();
                result.update(newRecord->initializeFromBuffer(startPos, endPos, &localCurrentPos));
                m_resourceRecords.push_back(newRecord);
            }
            break;

        case ResourceRecord::TYPE_NS:
            break;

        case ResourceRecord::TYPE_SOA:
            break;
        }    
    }

    return result;
}

const std::vector<std::shared_ptr<ResourceRecord> >& AnswerMessageSection::resourceRecords() const
{
    return m_resourceRecords;
}

void AnswerMessageSection::write(std::ostream& stream) const
{
}

}
}
