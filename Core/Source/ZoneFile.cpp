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

#include "ZoneFile.h"

namespace Ishiko
{
namespace DNS
{

ZoneFile::ZoneFile(const DomainName& origin,
				   const StartOfAuthorityRecord& soaRecord)
	: m_origin(origin), m_soaRecord(soaRecord)
{
}

void ZoneFile::write(std::ostream& stream) const
{
	stream << "$ORIGIN " << m_origin.str().c_str() << std::endl;
	stream << "$TTL 1d" << std::endl;

	if (m_soaRecord.name() == m_origin)
	{
		stream << "@";
	}
	else
	{
		stream << m_soaRecord.name().str().c_str();
	}
	stream << " ";
	switch (m_soaRecord.cl())
	{
	case ResourceRecord::CLASS_IN:
		stream << "IN";
		break;

	default:
		// TODO
		break;
	}
	stream << " ";
	switch (m_soaRecord.type())
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

    m_soaRecord.writeText(stream);

    stream << std::endl;
	
	for (size_t i = 0; i < m_records.size(); ++i)
	{
		m_records[i]->writeText(stream);
        stream << std::endl;
	}
}

void ZoneFile::append(std::shared_ptr<ResourceRecord> record)
{
	m_records.push_back(record);
}

}
}
