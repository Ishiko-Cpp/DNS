/*
	Copyright (c) 2009-2015 Xavier Leclercq

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

#ifndef _ISHIKO_DNS_CORE_DOMAINNAME_H_
#define _ISHIKO_DNS_CORE_DOMAINNAME_H_

#include <istream>
#include <ostream>
#include <stdint.h>

namespace Ishiko
{
namespace DNS
{

class DomainName
{
public:
	DomainName(const std::string& domainName);
	DomainName(std::istream& stream);

	// The number of bytes that would be produced
	// by the write() method.
	uint16_t length() const;
	// Writes a domain name in the binary format
	// expected in resource records.
	// Use the str() method to get the domain name
	// in text format.
	void write(std::ostream& stream) const;

	const std::string& str() const;

	bool operator==(const DomainName& other) const;
	bool operator==(const std::string& other) const;
	bool operator!=(const DomainName& other) const;
	bool operator!=(const std::string& other) const;

private:
	std::string m_name;
};

}
}

#endif
