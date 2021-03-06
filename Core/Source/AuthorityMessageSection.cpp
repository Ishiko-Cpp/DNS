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

#include "AuthorityMessageSection.h"

namespace Ishiko
{
namespace DNS
{

AuthorityMessageSection::AuthorityMessageSection()
{
}

Result AuthorityMessageSection::initializeFromBuffer(uint16_t count,
                                                     const char* startPos,
                                                     const char* endPos, 
                                                     const char** currentPos)
{
    Result result(Result::eSuccess);

    const char* localCurrentPos = *currentPos;

    std::vector<std::shared_ptr<ResourceRecord> > newResourceRecords;
    for (size_t i = 0; (i < count) && result.succeeded(); ++i)
    {
        std::shared_ptr<ResourceRecord> newRecord;
        if (result.update(ResourceRecord::createFromBuffer(startPos, endPos, &localCurrentPos, newRecord)).succeeded())
        {
            newResourceRecords.push_back(newRecord);
        }
    }

    if (result.succeeded())
    {
        m_resourceRecords.swap(newResourceRecords);
        *currentPos = localCurrentPos;
    }

    return result;
}

const std::vector<std::shared_ptr<ResourceRecord> >& AuthorityMessageSection::resourceRecords() const
{
    return m_resourceRecords;
}

void AuthorityMessageSection::appendResourceRecord(std::shared_ptr<ResourceRecord> record)
{
    m_resourceRecords.push_back(record);
}

void AuthorityMessageSection::write(std::ostream& stream) const
{
    for (auto i : m_resourceRecords)
    {
        i->writeBinary(stream);
    }
}

}
}
