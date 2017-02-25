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

#ifndef _ISHIKO_DNS_CORE_ANSWERMESSAGESECTION_H_
#define _ISHIKO_DNS_CORE_ANSWERMESSAGESECTION_H_

#include "ResourceRecord.h"
#include "Result.h"
#include <ostream>
#include <vector>
#include <memory>

namespace Ishiko
{
namespace DNS
{

class AnswerMessageSection
{
public:
    AnswerMessageSection();

    Result initializeFromBuffer(uint16_t count, const char* startPos,
        const char* endPos, const char** currentPos);

    const std::vector<std::shared_ptr<ResourceRecord> >& resourceRecords() const;

	void write(std::ostream& stream) const;

private:
	std::vector<std::shared_ptr<ResourceRecord> > m_resourceRecords;
};

}
}

#endif
