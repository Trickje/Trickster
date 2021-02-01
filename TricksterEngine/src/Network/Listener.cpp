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
#include "asio.hpp"
#include "asio/ssl.hpp"
#include "Network/Listener.h"

Trickster::Listener::Listener(std::string a_Host, std::string a_Path, asio::io_context& io_context, asio::ssl::context& context, asio::ip::tcp::resolver::results_type endpoints)
	: m_Socket(io_context, context), m_Host(a_Host), m_Path(a_Path)
{
	m_Socket.set_verify_mode(asio::ssl::verify_peer);
	m_Socket.set_verify_callback(
		std::bind(&Listener::VerifyCertificate, this,
			std::placeholders::_1, std::placeholders::_2));

	asio::async_connect(m_Socket.lowest_layer(), endpoints,
		std::bind(&Listener::HandleConnect, this, std::placeholders::_1));
}

TRICKSTER_API void Trickster::Listener::HandleConnect(const asio::error_code& error)
{
	if (!error)
	{
		m_Socket.async_handshake(asio::ssl::stream_base::client,
			std::bind(&Listener::HandleHandshake, this,
				std::placeholders::_1));
	}
	else
	{
		std::cout << "Connect failed: " << error.message() << "\n";
	}
}

TRICKSTER_API void Trickster::Listener::HandleHandshake(const asio::error_code& error)
{
	if (!error)
	{
		asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "GET /" << m_Path << " HTTP/1.1\r\n";
		request_stream << "Host: " << m_Host << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";
		asio::async_write(m_Socket,
			asio::buffer(request.data(), request.size()),
			std::bind(&Listener::HandleWrite, this,
				std::placeholders::_1,
				std::placeholders::_2));
	}
	else
	{
		std::cout << "Handshake failed: " << error.message() << "\n";
	}
}

TRICKSTER_API void Trickster::Listener::HandleWrite(const asio::error_code& error, size_t bytes_transferred)
{
	if (!error)
	{
		asio::async_read(m_Socket,
			asio::buffer(m_Buffer, bytes_transferred),
			std::bind(&Listener::HandleRead, this,
				std::placeholders::_1,
				std::placeholders::_2));
	}
	else
	{
		std::cout << "Write failed: " << error.message() << "\n";
	}
}

TRICKSTER_API void Trickster::Listener::HandleRead(const asio::error_code& error, size_t bytes_transferred)
{
	if (!error)
	{
		std::cout.write(m_Buffer.c_str(), bytes_transferred);
	}
	else
	{
		std::cout << "Read failed: " << error.message() << "\n";
	}
}

TRICKSTER_API bool Trickster::Listener::VerifyCertificate(bool preverified, asio::ssl::verify_context& ctx)
{
	// The verify callback can be used to check whether the certificate that is
   // being presented is valid for the peer. For example, RFC 2818 describes
   // the steps involved in doing this for HTTPS. Consult the OpenSSL
   // documentation for more details. Note that the callback is called once
   // for each certificate in the certificate chain, starting from the root
   // certificate authority.

   // In this example we will simply print the certificate's subject name.
	//char subject_name[256];
	//X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
	//X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
	//std::cout << "Verifying " << subject_name << "\n";

	return preverified;
}
