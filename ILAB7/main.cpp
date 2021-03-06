#include <iostream>
#include <fstream>
#include <cctype>

#include "LinkedList.h"
#include "Book.h"
#include "ResizableArray.h"
#include "String.h"
#include "Exception.h"
#include "Pair.h"


/* Returns reference to the book with most available copies in a resizable array */
Book& findBestAvailability(ResizableArray<Book>&);

/* Returns reference to the book with most available copies in an array			 */
Book& findBestAvailability(Book*, int);

/* Sorts a vector sent by a reference in non-descending order				     */
template<class T>
void sort(ResizableArray<T>&);

/* Sorts a linked list sent by a pointer in non-descending order                 */
template<class T>
void sort(LinkedList<T>&);

/* Sorts an array sent by a pointer in non-descending order					     */
template<class T>
void sort(T*, int);

/* Outputs a table to the stream &out from the books in vector &books			 */
void outputBooksTable(std::ostream& out, ResizableArray<Book>&);

/* Outputs the list of unique spheres to the stream &out from ResizableArray of Book @books */
void outputSpheresList(std::ostream& out, ResizableArray<Book>& books);

int main(int argc, char** argv) {

	std::fstream fin;
	char* fn = new char[255];
	while (!fin.is_open())
	{
		do {
			std::cout << "Enter text file name to read book database from (with .txt): ";
			std::cin.clear();
			std::cin >> fn;
		} while (std::cin.fail());

		fin.open(fn);
		if (!fin.is_open())
			std::cerr << "Can't open file " << fn << std::endl;
	}
	delete[] fn;

	char yn, delim = '\n';
	do {
		std::cout << "Use a delimiter (symbol separating books in input file? (y\n): ";
		std::cin.clear();
		std::cin >> yn;
	} while (std::cin.fail() || yn != 'y' && yn != 'n');
	if (yn == 'y') {
		do {
			std::cout << "Enter delimiting character (it shouldn't be in the actual text, every book will start with it, but it will be ignored when reading): ";
			std::cin.clear();
			std::cin >> delim;
		} while (std::cin.fail() || isalnum(delim));
	}

	ResizableArray<Book> books = ResizableArray<Book>();
	while (!fin.eof() && fin.good()) {

		if (delim != '\n') fin.ignore(INT_MAX, delim);
		else while (!fin.eof() && !std::isalnum(fin.peek()))
			fin.ignore();

		if (fin.eof()) break;

		Book book = Book();
		try {
			fin >> book;
			books.add(book);
		}

		catch (Exception& e) {
			std::cerr << e.getMsg() << ": " << e.getInfo() << std::endl;
			char yn;
			do {
				std::cout << "Continue reading file? (more exceptions may pop up if delimiter is not set) (y\n): ";
				std::cin.clear();
				std::cin >> yn;
			} while (std::cin.fail() || yn != 'y' && yn != 'n');
			if (yn == 'y') fin.clear();
		}

	}

	std::ofstream fout("bestAvailability.txt");
	if (!fout.is_open())
		std::cerr << "Can't create output file bestAvailability.txt" << std::endl;
	else {
		try {
			fout << "Book with most available copies is:\n";
			fout << findBestAvailability(books);
			fout << '\n';
		}
		catch (Exception& e) {
			fout << e.getMsg() << ": " << e.getInfo() << std::endl;
		}

		fout.close();
	}

	fout.open("booksTable.txt");
	if (!fout.is_open())
		std::cerr << "Can't create output file booksTable.txt" << std::endl;
	else {
		sort(books);
		outputBooksTable(fout, books);
		fout.close();
	}

	fout.open("spheresList.txt");
	if (!fout.is_open())
		std::cerr << "Can't create output file spheresList.txt" << std::endl;
	else {
		outputSpheresList(fout, books);
		fout.close();
	}

	std::cin.ignore(INT_MAX, '\n');
	String sphere;
	int c = books.getSize();
	bool isFound;
	do {
		std::cout << "Enter sphere name to output all books with such sphere into console (enter 0 to exit): ";
		std::cin.clear();
		getline(std::cin, sphere);
		isFound = false;
		for (int i = 0; i < c; ++i)
			for (int g = 0; g < books[i].getSpheresCount(); ++g)
				if (books[i].getSpheres()[g] == sphere) {
					std::cout << books[i];
					isFound = true;
				}
		if (!isFound)
			std::cout << "No books found" << std::endl;
	} while (std::cin.fail() || sphere != "0");

	return 0;
}

/* Overloaded comparison operator to properly sort spheres list */
bool operator>(const Pair<String, int>& p1, const Pair<String, int>& p2) {
	return p1.getFirst() > p2.getFirst();
}

/* Returns reference to the book with most available copies in a resizable array
   Throws Invalid Argument exception if recieved array is empty */
Book& findBestAvailability(ResizableArray<Book>& books) {
	if (books.isEmpty())
		throw Exception("Invalid argument exception!", 139, "main.cpp", "Books array must not be empty");
	Book* bestAvailable = &books[0];
	for (int i = 1; i < books.getSize(); ++i) {
		if (books[i] > *bestAvailable)
			bestAvailable = &books[i];
	}
	return *bestAvailable;
}

/* Returns reference to the book with most available copies in an array
   Throws Invalid Argument exception if recieved array is empty */
Book& findBestAvailability(Book* books, int n) {
	if (books == nullptr)
		throw Exception("Invalid argument exception!", 152, "main.cpp", "Books array must not be empty");
	Book* bestAvailable = nullptr;
	for (int i = 0; i < n; i++)
		if (bestAvailable == nullptr || books[i] > *bestAvailable)
			bestAvailable = &books[i];
	return *bestAvailable;
}

/* Sorts a resizable array sent by a reference in non-descending order*/
template<class T>
void sort(ResizableArray<T>& arr) {
	for (int i = 1; i < arr.getSize(); i++) {
		T key = arr[i];
		int j = i - 1;
		for (; j >= 0 && arr[j] > key; j--)
			arr[j + 1] = arr[j];
		arr[j + 1] = key;
	}
}

/* Sorts a linked list sent by a pointer in non-descending order*/
template<typename T>
void sort(LinkedList<T>& linkedList) {
	if (linkedList.getSize() != 0) {
		typename LinkedList<T>::LinkedListIterator begin = linkedList.begin();
		typename LinkedList<T>::LinkedListIterator end = linkedList.end();
		for (typename LinkedList<T>::LinkedListIterator itr = begin + 1; itr != end; ++itr) {
			T key = *itr;
			typename LinkedList<T>::LinkedListIterator j = itr - 1;
			for (; j != begin && *j > key; --j)
				(j + 1).setItem(*j);
			if (j != begin)
				++j;
			else (j + 1).setItem(*j);
			j.setItem(key);
		}
	}
}

/* Sorts an array sent by a pointer in non-descending order*/
template<class T>
void sort(T* arr, int n) {
	for (int i = 1; i < n; i++) {
		T key = arr[i];
		int j = i - 1;
		for (; j >= 0 && arr[j] > key; j--)
			arr[j + 1] = arr[j];
		arr[j + 1] = key;
	}
}

/* Outputs a table to the stream &out from the books in vector &books. Only first sphere(discipline) is being output */
void outputBooksTable(std::ostream& out, ResizableArray<Book>& books) {
	out <<
		std::setw(BOOK_AUTHOR_WIDTH) << "Author" <<
		std::setw(BOOK_TITLE_WIDTH) << "Title" <<
		std::setw(BOOK_YEAR_WIDTH) << "Year" <<
		std::setw(BOOK_SPHERE_WIDTH) << "Sphere" <<
		std::setw(BOOK_COUNT_WIDTH) << "Count" <<
		std::endl;
	for (int i = 0; i < books.getSize(); ++i)
		out << books[i];
}

/* Outputs the list of unique spheres to the stream &out from ResizableArray of Book @books */
void outputSpheresList(std::ostream& out, ResizableArray<Book>& books) {
	if (books.getSize() == 0)
		return;

	int c = books.getSize();
	LinkedList<String> spheres = LinkedList<String>();
	LinkedList<int> values = LinkedList<int>();
	for (int i = 0; i < c; ++i) {
		const String* currentSpheres = books[i].getSpheres();
		int sc = books[i].getSpheresCount();
		for (int g = 0; g < sc; ++g) {
			int ind = spheres.findIndex(currentSpheres[g]);
			if (ind == spheres.getSize()) {
				spheres.add(currentSpheres[g]);
				values.add(1);
			}
			else values[ind]++;
		}
	}

	LinkedList<Pair<String, int>> sortedSpheres;
	for (int i = 0; i < spheres.getSize(); ++i)
		sortedSpheres.add(makePair(spheres[i], values[i]));

	sort(sortedSpheres);

	out << std::setw(BOOK_SPHERE_WIDTH) << "Covered spheres" << std::setw(BOOK_COUNT_WIDTH) << "Count" << '\n';
	for (LinkedList<Pair<String, int>>::LinkedListIterator itr = sortedSpheres.begin(); itr != sortedSpheres.end(); ++itr)
		out << std::setw(BOOK_SPHERE_WIDTH) << (*itr).getFirst() << std::setw(BOOK_COUNT_WIDTH) << (*itr).getSecond() << '\n';
}