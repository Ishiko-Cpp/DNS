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

#include "QuestionMessageSection.h"

namespace Ishiko
{
namespace DNS
{

QuestionMessageSection::QuestionMessageSection()
{
}

QuestionMessageSection::QuestionMessageSection(const std::string& domainName)
{
    std::shared_ptr<QuestionMessageSectionEntry> newEntry =
        std::make_shared<QuestionMessageSectionEntry>(domainName, 
        QuestionMessageSectionEntry::QTYPE_A, QuestionMessageSectionEntry::QCLASS_IN);
    m_entries.push_back(newEntry);
}

Result QuestionMessageSection::initializeFromBuffer(uint16_t count,
                                                    const char* startPos,
                                                    const char* endPos,
                                                    const char** currentPos)
{
    Result result(Result::eSuccess);

    const char* localCurrentPos = *currentPos;

    for (size_t i = 0; i < count; ++i)
    {
        std::shared_ptr<QuestionMessageSectionEntry> newEntry = std::make_shared<QuestionMessageSectionEntry>();
        result.update(newEntry->initializeFromBuffer(startPos, endPos, &localCurrentPos));
        m_entries.push_back(newEntry);
    }

    if (result.succeeded())
    {
        *currentPos = localCurrentPos;
    }

    return result;
}

const std::vector<std::shared_ptr<QuestionMessageSectionEntry> > QuestionMessageSection::entries() const
{
    return m_entries;
}

void QuestionMessageSection::write(std::ostream& stream) const
{
    for (size_t i = 0; i < m_entries.size(); ++i)
    {
        m_entries[i]->write(stream);
    }
}

}
}
