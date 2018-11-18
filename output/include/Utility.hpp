#ifndef CA_POGANATZ_UTILITY_HPP
#define CA_POGANATZ_UTILITY_HPP

/**
	@file: Utility.hpp
	@author Alexander Poganatz
	@author a_poganatz@outlook.com
	@date 2018-11-18
	@brief Contains a bunch of utility functions that I use
*/

#include <string>
namespace ca_poganatz 
{
	namespace utility
	{
		/**
			@fn trimString
			@brief Trims the back and front of the string. std::locale is used to determine whitespace
			@param str [mutable] The string to trim
		*/
		template <class String = std::string>
		void trimString(String & str)
		{
			std::locale loc;
			//trim the back
			while (!str.empty() && std::isspace(str.back(), loc))
				str.pop_back();

			//Erase the beggining whitespace.
			size_t whiteSpaceCount = 0;
			while (whiteSpaceCount < str.size() && std::isspace(str[whiteSpaceCount], loc))
				++whiteSpaceCount;

			if (whiteSpaceCount > 0)
				str.erase(0, whiteSpaceCount);
		}
	}
}


#endif