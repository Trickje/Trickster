/*
================================================================================
		Copyright 2021 Rick Pijpers

		Licensed under the Apache License, Version 2.0 (the "License");
		you may not use this file except in compliance with the License.
		You may obtain a copy of the License at

			http://www.apache.org/licenses/LICENSE-2.0

		Unless required by applicable law or agreed to in writing, software
		distributed under the License is distributed on an "AS IS" BASIS,
		WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
		See the License for the specific language governing permissions and
		limitations under the License.
=================================================================================
 */
#include "pch.h"
#include "Network/Network.h"
#include "asio/ssl.hpp"

#include "Network/Listener.h"
using asio::ip::tcp;
Trickster::Network::Network()
{
}

Trickster::Network::~Network()
{
}

const asio::io_context& Trickster::Network::Get()
{
	return m_Context;
}

void Trickster::Network::ReadAndPrintPage(std::string a_Host, std::string a_Path)
{
	//TODO: add the readSSL function here
	ReadSSLPageToBuffer(a_Path, a_Host);
	return;
	//tcp::resolver resolver(m_Context);
	//tcp::resolver::results_type endpoints = resolver.resolve(a_URL, "443");
	asio::error_code ec;
	
	//tcp::endpoint endpoint(asio::ip::make_address(a_Host, ec), 443);
	tcp::socket socket(m_Context);
	//socket.connect(endpoint,ec);
	tcp::resolver resolver(m_Context);
	tcp::resolver::results_type endpoints = resolver.resolve(a_Host, "http");
	asio::connect(socket, endpoints,ec);
	if(socket.is_open())
	{
		LOG("[Network] Connected to " + a_Host);
		asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "GET " << a_Path << " HTTP/1.1\r\n";
		request_stream << "Host: " << a_Host << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";
		//std::string request = 
		//	"GET /index.php HTTP/1.1\r\n"
		//	"Host: "+ a_URL+ "\r\n"
		//	"Accept: */*\r\n"
		//"Connection: close\r\n\r\n";
		LOG(ec.message());
		socket.write_some(asio::buffer(request.data(), request.size()),ec);
		LOG(ec.message());
		socket.wait(socket.wait_read);
		size_t bytes = socket.available();
		if(bytes > 0)
		{
			std::vector<char> vBuffer(bytes);
			socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);

			for(auto c: vBuffer)
			{
				std::cout << c;
			}
		}
	}
}

void Trickster::Network::ReadSSLPageToBuffer(std::string a_Path, std::string a_Host)
{
	asio::ssl::context context(asio::ssl::context::sslv23);
	asio::ssl::stream<asio::ip::tcp::socket> socket(m_Context, context);
	socket.set_verify_mode(asio::ssl::verify_peer);
	tcp::resolver resolver(m_Context);
	tcp::resolver::results_type endpoints = resolver.resolve(a_Host + "/" + a_Path, "http");
	Listener listener(a_Host, a_Path, m_Context, context, endpoints);
	m_Context.run();
}
