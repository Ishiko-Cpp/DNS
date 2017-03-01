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

#ifndef _ISHIKO_DNS_CORE_RESPONSE_H_
#define _ISHIKO_DNS_CORE_RESPONSE_H_

#include "HeaderMessageSection.h"
#include "QuestionMessageSection.h"
#include "AnswerMessageSection.h"
#include "AuthorityMessageSection.h"
#include "AdditionalMessageSection.h"
#include "Result.h"

namespace Ishiko
{
namespace DNS
{

class Response
{
public:
    Response();
    Result initializeFromBuffer(const char* startPos,
        const char* endPos);
    Result initializeFromBuffer(const char* startPos,
        const char* endPos, const char** currentPos);

    const HeaderMessageSection& headerSection() const;
    const QuestionMessageSection& questionSection() const;
    const AnswerMessageSection& answerSection() const;

    void write(std::ostream& stream) const;

private:
    HeaderMessageSection m_headerSection;
    QuestionMessageSection m_questionSection;
    AnswerMessageSection m_answerSection;
    AuthorityMessageSection m_authoritySection;
    AdditionalMessageSection m_additionalSection;
};

}
}

#endif
