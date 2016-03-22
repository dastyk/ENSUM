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
{
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

		//string& operator+(const string& str) { *this + str; return *this; }
		string operator+(const char* str)const { return string(((*(this->_string)) + str).c_str()); }
		operator const char*() { return this->c_str(); }

		string& operator=(const char* str) { (*(this->_string)) = str; return *this; }
		string& operator=(const string& other) { (*(this->_string)) = (*(other._string)); return *this; }

		const bool operator==(const char* str) { return (*(this->_string)) == str; }
		const bool operator==(const string& other) { return (*(this->_string)) == (*(other._string)); }

		const char* c_str() { return _string->c_str(); }
		const void transform() { std::transform((*_string).begin(), (*_string).end(), (*_string).begin(), ::tolower); }
		const void clear() { _string->clear(); }
		const void push_back(const char str) { _string->push_back(str); }
		const uint32_t size() { return static_cast<uint32_t>(_string->size()); }
	private:
		std::string* _string;

	};
}
#endif