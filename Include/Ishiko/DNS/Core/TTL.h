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

#ifndef _ISHIKO_DNS_CORE_TTL_H_
#define _ISHIKO_DNS_CORE_TTL_H_

#include "Result.h"
#include <string>
#include <stdint.h>

namespace Ishiko
{
namespace DNS
{

// A class to represent a TTL. Many DNS constructs require a
// a TTL. A class is used because it is possible to specify a TTL
// using strings like 1d, 2w and we want to keep them in this form
// when used in text form like in a zone file for instance.
class TTL
{
public:
    TTL();
    TTL(uint32_t ttl);
    TTL(const std::string& ttl);
    Result initializeFromBuffer(const char* startPos,
        const char* endPos, const char** currentPos);

    uint32_t asUInt32() const;

    void writeBinary(std::ostream& stream) const;
    void writeText(std::ostream& stream) const;

private:
    // The TTL expressed in seconds
    uint32_t m_TTL;
    // If the TTL was constructed by specifiying the TTL using a string
    // like 1, or 2w, then the m_TTLString will be set to that value
    std::string m_TTLString;
};

}
}

#endif
