/*
	Copyright (c) 2010-2015 Xavier Leclercq

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

#include "QuestionMessageSectionEntry.h"
#include <boost/endian/conversion.hpp>

namespace Ishiko
{
namespace DNS
{

QuestionMessageSectionEntry::QuestionMessageSectionEntry(const std::string& domainName,
														 QTYPE qtype, 
														 QCLASS qclass)
	: m_QNAME(domainName), m_QTYPE(qtype), m_QCLASS(qclass)
{
}

QuestionMessageSectionEntry::QuestionMessageSectionEntry(std::istream& stream)
	: m_QNAME(stream)
{
	uint16_t tmp;
	stream.read((char*)&tmp, 2);
	m_QTYPE = boost::endian::big_to_native(tmp);
	stream.read((char*)&tmp, 2);
	m_QCLASS = boost::endian::big_to_native(tmp);
}

void QuestionMessageSectionEntry::write(std::ostream& stream) const
{
	m_QNAME.write(stream);
	uint16_t tmp = boost::endian::native_to_big(m_QTYPE);
	stream.write((char*)&tmp, 2);
	tmp = boost::endian::native_to_big(m_QCLASS);
	stream.write((char*)&tmp, 2);
}

}
}
