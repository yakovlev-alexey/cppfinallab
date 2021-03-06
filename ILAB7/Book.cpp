#include "Book.h"
#include "Util.h"
#include "Exception.h"

void copySpheres(String* dest, const String* source, const unsigned int size) {
	for (int i = 0; i < size; ++i) {
		if (!Book::isValidSphere(source[i]))
			throw Exception("Not a valid sphere name!", 8, "Book.cpp");
		dest[i] = source[i];
	}
}

#pragma region Constructors

/* Instantiates a Book with empty fields/default values */
Book::Book() {
	title = '\0';
	author = '\0';
	sphereCount = 0;
	spheres = nullptr;
	publicationYear = 1970;
	currentlyAvailable = 0;
}

/* Instantiates a Book with recieved arguments */
Book::Book(
	String author = '\0',
	String title = '\0',
	date_y publicationYear = 1970,
	unsigned int sphereCount = 0,
	String* spheres = nullptr,
	unsigned int currentlyAvailable = 0
) {
	if (!isValidName(author))
		throw Exception("Not a valid name!", 35, "Book.cpp");
	this->author = author;
	this->title = title;
	this->publicationYear = publicationYear;
	if (sphereCount > BOOK_MAX_SPHERE_COUNT || sphereCount == 0)
		throw Exception("Sphere count exceeds limit or must be at least 1!", 30, "Book.cpp");
	if (this->sphereCount < sphereCount) {
		delete[] spheres;
		spheres = new String[sphereCount];
	}
	copySpheres(this->spheres, spheres, sphereCount);
	this->sphereCount = sphereCount;
	this->currentlyAvailable = currentlyAvailable;
}

/* Instantiates a copy of the Book @book */
Book::Book(const Book& book) {
	if (this == &book)
		return;
	author = book.author;
	title = book.title;
	publicationYear = book.publicationYear;
	if (sphereCount < book.sphereCount) {
		delete[] spheres;
		spheres = new String[book.sphereCount];
	}
	copySpheres(spheres, book.spheres, book.sphereCount);
	sphereCount = book.sphereCount;
	currentlyAvailable = book.currentlyAvailable;
}

#pragma endregion

Book::~Book() {
	if (spheres != nullptr)
		delete[] spheres;
}

#pragma region Getters

/* Returns this Book's author as a String copy */
String Book::getAuthor() const {
	return author;
}

/* Returns this Book's title as a String copy */
String Book::getTitle() const {
	return title;
}

/* Returns this Book's publication year as an integer */
int Book::getPublicationYear() const {
	return publicationYear;
}

int Book::getSpheresCount() const {
	return sphereCount;
}

/* Returns this Book's sphere as a String copy */
const String* Book::getSpheres() const {
	return spheres;
}

/* Returns this Book's copy amount as an integer */
int Book::getCurrentAmount() const {
	return currentlyAvailable;
}

#pragma endregion

#pragma region Setters

/* Sets this Book's author */
void Book::setAuthor(const String& author) {
	if (!isValidName(author))
		throw Exception("Not a valid name!", 107, "Book.cpp");
	this->author = author;
}

/* Sets this Book's title */
void Book::setTitle(const String& title) {
	this->title = title;
}

/* Sets this Book's publication year */
void Book::setPublicationYear(const int year) {
	publicationYear = year;
}

/* Sets this Book's spehre*/
void Book::setSpheres(const String* spheres, const int sphereCount) {
	if (sphereCount > BOOK_MAX_SPHERE_COUNT || sphereCount <= 0)
		throw Exception("Sphere count exceeds limit!", 30, "Book.cpp");
	this->sphereCount = sphereCount;
	copySpheres(this->spheres, spheres, sphereCount);
}

/* Sets this Book's current copy amount */
void Book::setCurrentAmount(const int amount) {
	currentlyAvailable = amount;
}

#pragma endregion

/* Returns true if Books share the same author, title and publication year
   (not sphere though) */
bool operator==(const Book& b1, const Book& b2) {
	return b1.author == b2.author
		&& b1.title == b2.title
		&& b1.publicationYear == b2.publicationYear;
}

/* Returns true if Books have different authors or titles or publication years
   (not spheres though) */
bool operator!=(const Book& b1, const Book& b2) {
	return b1.author != b2.author
		|| b1.title != b2.title
		|| b1.publicationYear != b2.publicationYear;
}

/* Decrements the available amount of the first Book, if the Books are the same.
   Return the first Book otherwise */
Book operator-(const Book& b1, const Book& b2) {
	if (b1 != b2) return b1;
	return Book(b1.author, b1.title, b1.publicationYear, b1.sphereCount, b1.spheres, b1.currentlyAvailable - b2.currentlyAvailable);
}

/* Increments the available amount of the first Book, if the Books are the same.
   Return the first Book otherwise */
Book Book::operator+(const Book& book) const {
	if (*this != book) return *this;
	return Book(author, title, publicationYear, sphereCount, spheres, currentlyAvailable + book.currentlyAvailable);
}

/* Returns true if the first Book has less available copies
   (does not check for Book equality) */
bool operator<(const Book& b1, const Book& b2) {
	return b1.currentlyAvailable < b2.currentlyAvailable;
}

/* Return true if the first Book has more available copies
   (does not check for Book equality */
bool Book::operator>(const Book& b) const {
	return currentlyAvailable > b.currentlyAvailable;
}

/* Copies values from parameter to this Book */
Book& Book::operator=(const Book& book) {
	if (this == &book)
		return *this;
	author = book.author;
	title = book.title;
	publicationYear = book.publicationYear;
	if (sphereCount < book.sphereCount) {
		delete[] spheres;
		spheres = new String[book.sphereCount];
	}
	copySpheres(spheres, book.spheres, book.sphereCount);
	sphereCount = book.sphereCount;
	currentlyAvailable = book.currentlyAvailable;
	return *this;
}

/* Sets the amount of avaialable copies to the value on the right of equals */
Book& Book::operator=(const unsigned int amount) {
	currentlyAvailable = amount;
	return *this;
}

/* Incerements the amount of available copies of this Book by 1 (postfix version) */
const Book operator++(Book& b, int) {
	Book pb = b;
	b.currentlyAvailable++;
	return pb;
}

/* Incerements the amount of available copies of this Book by 1 (prefix version) */
const Book& Book::operator++() {
	currentlyAvailable++;
	return *this;
}

/* Decrements the amount of available copies of this Book by 1 (postfix version) */
const Book operator--(Book& b, int) {
	Book pb = b;
	b.currentlyAvailable--;
	return pb;
}

/* Incerements the amount of available copies of this Book by 1 (prefix version) */
const Book& Book::operator--() {
	currentlyAvailable--;
	return *this;
}

/* Outputs information about this Book into the stream &out as table row */
std::ostream& operator<<(std::ostream& out, const Book& b) {
	out <<
		std::setw(BOOK_AUTHOR_WIDTH) << Book::trimToSize(b.author, BOOK_AUTHOR_WIDTH - 1) <<
		std::setw(BOOK_TITLE_WIDTH) << Book::trimToSize(b.title, BOOK_TITLE_WIDTH - 1) << // -1 to leave space between columns
		std::setw(BOOK_YEAR_WIDTH) << b.publicationYear <<
		std::setw(BOOK_SPHERE_WIDTH) << Book::trimToSize(b.spheres[0], BOOK_SPHERE_WIDTH - 1) <<
		std::setw(BOOK_COUNT_WIDTH) << b.currentlyAvailable << std::endl;
	return out;
}

/* Reads Book object properties from input stream &in consecutively, each on a separate line
   Throws invalid_argument exception if input format is invalid.*/
std::istream& operator>>(std::istream& in, Book& b) {

	String line;

	try {
		getline(in, line);
	}
	catch (Exception& e) {
		e.setInfo("Wrong author name format");
		throw e;
	}
	if (!Book::isValidName(line))
		throw Exception("Not a valid name!", 250, "Book.cpp");
	b.author = line;
	Util::normalizeString(b.author);

	try {
		getline(in, b.title);
	}
	catch (Exception& e) {
		e.setInfo("Wrong title format");
		throw e;
	}
	Util::normalizeString(b.title);

	int num;
	in >> num;
	if (in.fail() || num < 0 || num > 2020)
		throw Exception("Wrong input stream format!", 258, "Book.cpp", "Wrong publication year format");
	b.publicationYear = num;
	in.ignore(INT_MAX, '\n');

	in >> num;
	if (in.fail() || num <= 0 || num > BOOK_MAX_SPHERE_COUNT)
		throw Exception("Wrong input stream format!", 258, "Book.cpp", "Wrong sphere count format");
	if (b.sphereCount < num) {
		delete[] b.spheres;
		b.spheres = new String[num];
	}
	b.sphereCount = num;
	in.ignore(INT_MAX, '\n');

	for (int i = 0; i < b.sphereCount; ++i) {
		try {
			getline(in, line);
		}
		catch (Exception& e) {
			e.setInfo("Wrong sphere format");
			throw e;
		}
		if (!Book::isValidSphere(line))
			throw Exception("Not a valid sphere name", 289, "Book.cpp");
		b.spheres[i] = line;
		Util::normalizeString(b.spheres[i]);
	}

	in >> b.currentlyAvailable;
	if (in.fail())
		throw Exception("Wrong input stream format!", 237, "Book.cpp", "Wrong book amount format");
	in.ignore(INT_MAX, '\n');

	return in;
}

bool Book::isValidName(const String& name) {
	for (int i = 0; i < name.getLength(); ++i)
		if (!(isalpha(name[i]) || name[i] == ' ' || name[i] == '-' || name[i] == '.'))
			return false;
	return true;
}

bool Book::isValidSphere(const String& sphere) {
	for (int i = 0; i < sphere.getLength(); ++i)
		if (!(isalpha(sphere[i]) || sphere[i] == ' '))
			return false;
	return true;
}

String Book::trimToSize(const String& string, const unsigned int size) {
	if (string.getLength() < size)
		return string;
	char* str = new char[size + 1];
	int i;
	for (i = 0; i < string.getLength() && i < size - 3; ++i)
		str[i] = string[i];
	if (i < string.getLength()) {
		str[i++] = '.';
		str[i++] = '.';
		str[i++] = '.';
	}
	str[i] = '\0';
	String ret = str;
	delete[] str;
	return ret;
}