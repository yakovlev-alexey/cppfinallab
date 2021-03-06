#include "String.h"
#include "Util.h"
#include "Exception.h"

#include <iostream>

/* Unparameterized constructor instantiates empty C-string */
String::String() {
	length = 0;
	str = new char[length + 1];
	str[length] = '\0';
}

/* Parameterized constructor copies argumenent C-string */
String::String(const char* newstr) {
	if (newstr == str)
		return;
	length = newstr == nullptr ? 0 : Util::strlen(newstr);
	str = new char[length + 1];
	Util::strcpy(str, newstr);
}

/* Copy constructor copies another String */
String::String(const String& string) {
	if (this == &string)
		return;
	length = string.length;
	str = new char[length + 1];
	Util::strcpy(str, string.str);
}

/* Destructor return allocated memory */
String::~String() {
	if (str != nullptr)
		delete[] str;
}

/* Returns String length without terminator */
int String::getLength() const {
	return length;
}

/* Returns C-style string (immutable) */
const char* String::get() const {
	return str;
}

/* Copies C-style string recieved as a parameter */
void String::set(const char* newstr) {
	if (str == newstr)
		return;
	length = newstr == nullptr ? 0 : Util::strlen(newstr);
	if (str != nullptr)
		delete[] str;
	str = new char[length + 1];
	Util::strcpy(str, newstr);
}

/* Copies C-style string recieved as a parameter */
String& String::operator=(const char* newstr) {
	set(newstr);
	return *this;
}

/* Copies String recieved as a parameter */
String& String::operator=(const String& string) {
	set(string.str);
	return *this;
}

/* Concats two Strings */
String operator+(const String& str1, const String& str2) {
	char* newstr = new char[str1.length + str2.length + 1];
	Util::strcpy(newstr, str1.str);
	Util::strcat(newstr, str2.str);
	return String(newstr);
}

/* Appends String with a single character */
String operator+(const String& string, const char ch) {
	char* newstr = new char[string.length + 2];
	Util::strcpy(newstr, string.str);
	newstr[string.length] = ch;
	newstr[string.length + 1] = '\0';
	return String(newstr);
}

/* Appends this String with another String */
String& String::operator+=(const String& string) {
	char* newstr = new char[length + string.length + 1];
	Util::strcpy(newstr, str);
	Util::strcat(newstr, string.str);
	if (str != nullptr)
		delete[] str;
	length += string.length;
	str = newstr;
	return *this;
}

/* Appends this String with a single character */
String& String::operator+=(const char ch) {
	char* newstr = new char[length + 2];
	Util::strcpy(newstr, str);
	newstr[length] = ch;
	newstr[length + 1] = '\0';
	if (str != nullptr)
		delete[] str;
	length++;
	str = newstr;
	return *this;
}

/* Returns true if two Strings are of the same length and consist of exactly
   the same characters */
bool operator==(const String& str1, const String& str2) {
	if (str1.length != str2.length)
		return false;
	for (int i = 0; i < str1.length; ++i)
		if (str1.str[i] != str2.str[i])
			return false;
	return true;
}

/* Returns true if two Strings are of the different length or consist of different characters */
bool operator!=(const String& str1, const String& str2) {
	return !(str1 == str2);
}

/* Returns character at index */
char& String::operator[](const int index) {
	if (index < 0 || index > length)
		throw Exception("Index out of range in string!", 126, "String.cpp");
	return str[index];
}

/* Immutable version */
const char& String::operator[](const int index) const {
	if (index < 0 || index > length)
		throw Exception("Index out of range in string!", 126, "String.cpp");
	return str[index];
}

/* Outputs String into stream &out */
std::ostream& operator<<(std::ostream& out, const String& string) {
	out << string.str;
	return out;
}

/* Returns true if @str1 is lexicographically less than @str2 */
bool operator<(const String& str1, const String& str2) {
	if (str1.length == 0)
		return true;
	int lim = str1.length < str2.length ? str1.length : str2.length;
	for (int i = 0; i < lim; ++i) {
		if (tolower(str1.str[i]) < tolower(str2.str[i]))
			return true;
		else if (tolower(str1.str[i]) > tolower(str2.str[i]))
			return false;
	}
	return false;
}

/* Returns true if @str1 is lexicographically more than @str2 */
bool operator>(const String& str1, const String& str2) {
	if (str2.length == 0)
		return true;
	int lim = str1.length < str2.length ? str1.length : str2.length;
	for (int i = 0; i < lim; ++i) {
		if (tolower(str1.str[i]) > tolower(str2.str[i]))
			return true;
		else if (tolower(str1.str[i]) < tolower(str2.str[i]))
			return false;
	}
	return false;
}

/* Reads a String until a delimiting character is met */
void getline(
	std::istream& in,
	String& string,
	const unsigned int len,
	const char delim
) {
	char* buf = new char[len];
	in.getline(buf, len, delim);
	if (in.fail())
		throw Exception("Wrong input stream format!", 174, "String.cpp");
	string.set(buf);
	delete[] buf;
}