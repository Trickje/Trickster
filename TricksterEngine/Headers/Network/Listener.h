#pragma once
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

namespace TE {

	enum { max_length = 1024 };
	class Listener
	{
	public:
		Listener(std::string a_Host, std::string a_Path ,  asio::io_context& io_context,
			asio::ssl::context& context,
			asio::ip::tcp::resolver::results_type endpoints);
		TRICKSTER_API void HandleConnect(const asio::error_code& error);
		TRICKSTER_API void HandleHandshake(const asio::error_code& error);
		TRICKSTER_API void HandleWrite(const asio::error_code& error, size_t bytes_transferred);
		TRICKSTER_API void HandleRead(const asio::error_code& error, size_t bytes_transferred);
		TRICKSTER_API bool VerifyCertificate(bool preverified, asio::ssl::verify_context& ctx);
	private:

		//asio::io_context* io_context;

		asio::ssl::stream<asio::ip::tcp::socket> m_Socket;
		std::string m_Host;
		std::string m_Path;
		std::string m_Buffer;
	};


}