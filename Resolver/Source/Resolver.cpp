/*
	Copyright (c) 2015 Xavier Leclercq

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

#include "Resolver.h"
#include "Ishiko/DNS/DNSCore.h"
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/streambuf.hpp>

namespace Ishiko
{
namespace DNS
{

void Resolver::getResourceRecords(const std::string& domainName)
{
	boost::asio::io_service service;
	boost::asio::ip::udp::socket socket(service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0));
	boost::asio::ip::address serverAddress = boost::asio::ip::address::from_string("8.8.8.8");
	boost::asio::ip::udp::endpoint endpoint(serverAddress, 53);
	
	Query query(domainName);
	boost::asio::streambuf requestBuffer;
	std::ostream requestStream(&requestBuffer);
	query.write(requestStream);

	socket.send_to(requestBuffer.data(), endpoint);

	boost::asio::streambuf responseBuffer;
	socket.receive_from(responseBuffer.prepare(5000), endpoint);

	responseBuffer.commit(5000);
	std::istream responseStream(&responseBuffer);
	Response response(responseStream);

	/*
	char reply[max_length];
	udp::endpoint sender_endpoint;
	size_t reply_length = s.receive_from(
		boost::asio::buffer(reply, max_length), sender_endpoint);*/

	socket.close();
}

}
}
