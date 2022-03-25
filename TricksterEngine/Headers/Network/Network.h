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
	class Network
	{
	public:
		TRICKSTER_API Network();
		TRICKSTER_API ~Network();
		TRICKSTER_API const asio::io_context& Get();
		TRICKSTER_API void ReadAndPrintPage(std::string a_Host, std::string a_Path);
	private:
		//Connects current ssl socket to the host
		//Host example: "77.168.130.108" or "rickpijpers.com"
		TRICKSTER_API void ConnectSSL(std::string a_Host);
		//This requires a connection to an SSL host
		//Path example "index.php" or "TricksterEngine/index.php" or "TricksterEngine/info.txt"
		TRICKSTER_API void ReadSSLPageToBuffer(std::string a_Path, std::string a_Host);


        

		asio::io_context m_Context;
		std::string m_Buffer;
	};

}
