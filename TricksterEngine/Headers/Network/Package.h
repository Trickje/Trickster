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
#include <cstdint>
#include <vector>
#include <string>
namespace TE {
	enum MessageTypes
	{
		
	};
	//id = MessageTypes
	//Message header contains the type of message and the data size
	struct MessageHeader
	{
		int id{};
		uint32_t size = 0;
	};

	struct Message
	{
		MessageHeader header{};
		//Vector of bytes
		std::vector<uint8_t> body;

		size_t size() const
		{
			return sizeof(MessageHeader) + body.size();
		}

		Message& operator+=(std::string data)
		{
			const size_t size = data.size();
			const size_t start = body.size();
			body.resize(body.size() + size);
			
			for (int i = 0; i < size; i++) {
				body[start + i] = data[i];
			}
			header.size = static_cast<uint32_t>(body.size());
			return *this;
		}
		template<size_t n>
		Message& operator+=(const char (&a_Byte)[n])
		{
			//The end character has to be removed that's why the - 1
			const int size = sizeof(char) * n - 1;
			const size_t start = body.size();
			body.resize(body.size() + size);

			for (int i = 0; i < size; i++) {
				body[start + i] = a_Byte[i];
			}
			header.size = static_cast<uint32_t>(body.size());
			return *this;
		}
		template<typename DataType>
		Message& operator+=(const DataType& singleDataObject)
		{
			const int size = sizeof(DataType);
			size_t start = this->body.size();
			body.resize(body.size() + size);

			std::memcpy(body.data() + start, &singleDataObject, size);
			header.size = static_cast<uint32_t>(body.size());
			return *this;
		}

		template<typename T>
		T GetValueAt(int index = 0)
		{
			return *(T*)&body[index * sizeof(T)];
		}

		std::string to_string()
		{
			std::string str;
			str.resize(body.size());
			std::memcpy(&str[0],&body[0], body.size());
			return str;
		}
	};
	struct Package
	{

	};

}