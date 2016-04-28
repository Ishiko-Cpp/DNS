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

#include "ResourceRecord.h"
#include <boost/endian/conversion.hpp>

namespace Ishiko
{
namespace DNS
{

ResourceRecord::ResourceRecord(const std::string& domainName,
							   TYPE type,
							   CLASS cl,
							   uint32_t ttl)
	: m_NAME(domainName), m_TYPE(type), m_CLASS(cl),m_TTL(ttl)
{
}

ResourceRecord::~ResourceRecord()
{
}

void ResourceRecord::writeBinary(std::ostream& stream) const
{
	m_NAME.write(stream);
	uint16_t tmp = boost::endian::native_to_big(m_TYPE);
	stream.write((char*)&tmp, 2);
	tmp = boost::endian::native_to_big(m_CLASS);
	stream.write((char*)&tmp, 2);
	m_TTL.writeBinary(stream);
	tmp = 0;
	if (m_RDATA)
	{
		tmp = boost::endian::native_to_big(m_RDATA->length());
	}
	stream.write((char*)&tmp, 2);
}

void ResourceRecord::writeText(std::ostream& stream) const
{
	stream << m_NAME.str().c_str() << " ";
	switch (m_CLASS)
	{
	case ResourceRecord::CLASS_IN:
		stream << "IN";
		break;

	default:
		// TODO
		break;
	}
	stream << " ";
	switch (m_TYPE)
	{
	case ResourceRecord::TYPE_A:
		stream << "A";
		break;

	case ResourceRecord::TYPE_NS:
		stream << "NS";
		break;

	case ResourceRecord::TYPE_SOA:
		stream << "SOA";
		break;

	default:
		// TODO
		break;
	}
}

const DomainName& ResourceRecord::name() const
{
	return m_NAME;
}

ResourceRecord::TYPE ResourceRecord::type() const
{
	return (ResourceRecord::TYPE)m_TYPE;
}

ResourceRecord::CLASS ResourceRecord::cl() const
{
	return (ResourceRecord::CLASS)m_CLASS;
}

}
}
