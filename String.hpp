/* 
* String.hpp - a java-style wraper of std::string
*/

/*									MIT License
*
*							  Copyright (c) 2025 mLazy
*
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files (the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*	copies of the Software, and to permit persons to whom the Software is
*	furnished to do so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included in all
*	copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*	SOFTWARE.
*/

#ifndef _STRING_HPP_
#define _STRING_HPP_
#include <iostream>
#include <regex>
#define self (*this)
namespace lazy {
	class String {
		std::string value;
		int hash = 0;
	public:
		String() {}
		String(const std::string& value) :value(value) {}
		operator const std::string& () const {
			return value;
		}
		// not recommended
		String(const char* value) :value(value) {}

		int hashCode() {
			if (hash == 0 && value.size() > 0) {
				for (int i = 0; i < value.size(); i++) {
					hash = 31 * hash + value[i];
				}
			}
			return hash;
		}

		/* convert */

		std::string toString() const {
			return value;
		}

		/* string methods */

		template<typename T>
		static String valueOf(T data) {
			return std::to_string(data);
		}

		bool equals(const String& anotherString) const {
			return value == anotherString.value;
		}
		bool equalsIgnoreCase(const String& anotherString) const {
			return self.toLowerCase().equals(anotherString.toLowerCase());
		}

		bool isEmpty() const {
			return value.empty();
		}
		bool isBlank() const {
			for (unsigned char c : value) {
				if (!std::isspace(c)) {
					return false;
				}
			}
			return true;
		}

		bool startWith(const String& prefix) const {
			if (prefix.length() > self.length())return false;
			for (size_t i = 0; i < prefix.length(); i++) {
				if (self.value[i] != prefix.value[i])return false;
			}
			return true;
		}
		bool endWith(const String& suffix) const {
			if (suffix.length() > self.length())return false;
			for (size_t i = 1; i <= suffix.length(); i++) {
				if (self.value[self.length() - i] != suffix.value[suffix.length() - i])return false;
			}
			return true;
		}

		char charAt(size_t index) const {
			return value[index];
		}

		size_t length() const {
			return value.size();
		}

		size_t indexOf(char ch) const {
			return value.find_first_of(ch);
		}
		size_t indexOf(const String& str) const {
			return value.find(str);
		}
		size_t lastIndexOf(char ch) const {
			return value.find_last_of(ch);
		}
		size_t lastIndexOf(const String& str) const {
			return value.rfind(str);
		}

		String toUpperCase() const {
			std::string res = value;
			for (auto i = res.begin(); i != res.end(); i++) {
				*i = toupper(*i);
			}
			return res;
		}
		String toLowerCase() const {
			std::string res = value;
			for (auto i = res.begin(); i != res.end(); i++) {
				*i = tolower(*i);
			}
			return res;
		}

		String substring(size_t beginIndex) const {
			return value.substr(beginIndex);
		}
		String substring(size_t beginIndex, size_t endIndex) const {
			return value.substr(beginIndex, endIndex - beginIndex + 1);
		}

		String replace(char oldChar, char newChar) const {
			std::string res = value;
			for (auto i = res.begin(); i != res.end(); i++) {
				if (*i == oldChar)*i = newChar;
			}
			return res;
		}
		String replace(const String& target, const String& replacement) const {
			return std::regex_replace(
				value,
				std::regex(target.toString()),
				replacement.toString());
		}

		String operator+(const String& another) const {
			return String(self.value + another.value);
		}
	};
} // namespace lazy
#undef self
#endif // _STRING_HPP_