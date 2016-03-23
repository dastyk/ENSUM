#ifndef _ENSUM_STRING_H_
#define _ENSUM_STRING_H_
#pragma once
#include <algorithm>
#include <string>

#ifdef ENSUM_EXPORTS
#	define ENSUM_EXPORT __declspec(dllexport)
#	define ENSUM_TEMPLATE
#else
#	define ENSUM_EXPORT __declspec(dllimport)
#	define ENSUM_TEMPLATE extern
#endif
namespace Ensum
{	/** A wrapper class for std::string.
	*
	*/
	class ENSUM_EXPORT string
	{
	public:
		string() : _string(nullptr) { _string = new std::string; }
		string(const char* str) : string() { (*_string) = str; }
		string(const string& other) : string() { (*(this->_string)) = (*(other._string)); }

		string(const long& other) : string() { (*_string) = std::to_string(other); }
		string(const double& other) : string() { (*_string) = std::to_string(other); }
		string(const bool& other) : string() { (*_string) = other ? "true" : "false"; }

		~string() { delete _string; }

		/** + operator between strings.
		* 
		*/
		string operator+(const string& str)const { return string(((*(this->_string)) + (*(str._string))).c_str()); }
		/** + operator string and char.
		*
		*/
		string operator+(const char* str)const { return string(((*(this->_string)) + str).c_str()); }
		/** convertion operator to char*.
		*
		*/
		operator const char*()const { return this->_string->c_str(); }
		/** convertion operator to void*.
		*
		*/
		operator const void*()const { return (void*)this->_string->c_str(); }
		/** = operator char.
		*
		*/
		string& operator=(const char* str) { (*(this->_string)) = str; return *this; }
		/** = operator string.
		*
		*/
		string& operator=(const string& other) { (*(this->_string)) = (*(other._string)); return *this; }

		/** == operator char.
		*
		*/
		const bool operator==(const char* str) { return (*(this->_string)) == str; }
		/** == operator string.
		*
		*/
		const bool operator==(const string& other) { return (*(this->_string)) == (*(other._string)); }

		/** returns const char*.
		*
		*/
		const char* c_str() const { return _string->c_str(); }
		/** transforms all letters in string to lowercase.
		*
		*/
		const void transform() { std::transform((*_string).begin(), (*_string).end(), (*_string).begin(), ::tolower); }
		/** clears the string.
		*
		*/
		const void clear() { _string->clear(); }
		/** Adds char to end of string.
		*
		*/
		const void push_back(const char str) { _string->push_back(str); }
		/** Returns the size of the string.
		*
		*/
		const uint32_t size() const { return static_cast<uint32_t>(_string->size()); }
		/** Resize the string.
		*
		*/
		const void Resize(uint32_t size) { _string->resize(size); }
		/** Returns a hash of the string.
		*
		*/
		const uint32_t GetHash() const
		{
			//uint32_t out = 0;
			//uint32_t size = static_cast<uint32_t>(_string->size());
			//for (auto& c : (*_string))
			//{
			//	out <<= 1;
			//	out |= (uint32_t)c;
			//}
			//return out;

			return static_cast<uint32_t>(hash(*(_string)));
		}
	private:
		std::string* _string;
		std::hash<std::string> hash;
	};
}
#endif