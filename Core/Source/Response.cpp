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

#include "Response.h"

namespace Ishiko
{
namespace DNS
{

Response::Response()
    : m_headerSection(true)
{
}

Result Response::initializeFromBuffer(const char* startPos,
                                      const char* endPos)
{
    const char* currentPos = startPos;
    return initializeFromBuffer(startPos, endPos, &currentPos);
}

Result Response::initializeFromBuffer(const char* startPos,
                                      const char* endPos,
                                      const char** currentPos)
{
    Result result(Result::eSuccess);

    const char* localCurrentPos = *currentPos;

    if (result.update(m_headerSection.initializeFromBuffer(startPos, endPos, &localCurrentPos)).succeeded())
    {
        result.update(m_questionSection.initializeFromBuffer(m_headerSection.questionCount(), startPos, endPos, &localCurrentPos));
        result.update(m_answerSection.initializeFromBuffer(m_headerSection.answerCount(), startPos, endPos, &localCurrentPos));
    }

    return result;
}

const HeaderMessageSection& Response::headerSection() const
{
    return m_headerSection;
}

const QuestionMessageSection& Response::questionSection() const
{
    return m_questionSection;
}

const AnswerMessageSection& Response::answerSection() const
{
    return m_answerSection;
}

void Response::write(std::ostream& stream) const
{
    m_headerSection.write(stream);
    m_questionSection.write(stream);
    m_answerSection.write(stream);
}

}
}
