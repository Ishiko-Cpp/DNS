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

#include "TTL.h"
#include <boost/endian/conversion.hpp>
#include <ostream>

namespace Ishiko
{
namespace DNS
{

TTL::TTL(uint32_t ttl)
    : m_TTL(ttl)
{
}

TTL::TTL(const std::string& ttl)
    : m_TTL(0), m_TTLString(ttl)
{
}

void TTL::writeBinary(std::ostream& stream) const
{
    uint32_t tmp2 = boost::endian::native_to_big(m_TTL);
    stream.write((char*)&tmp2, 4);
}

void TTL::writeText(std::ostream& stream) const
{
}

}
}
