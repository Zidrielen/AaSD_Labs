#include<math.h>
#include<iostream>
#include <windows.h>
#include<conio.h>
#include <complex>
#include<math.h>


template<typename T>
struct Points { T x, y; };

//Menu
void menu() 
{
	std::cout << "1. Create a polyline\n";
	std::cout << "2. Get all vertices of a particular polyline\n";
	std::cout << "3. Get the length of a specific polyline\n";
	std::cout << "4. Add a vertex to the beginning of a specific polyline\n";
	std::cout << "5. Add a vertex to the end of a certain polyline\n";
	std::cout << "6. Create a polyline that is the 'sum' of two polylines\n";
	std::cout << "7. Create a polyline in the form of an isosceles triangle\n";
	std::cout << "8. Exit the program\n";
	std::cout << "\nEnter the number: ";
}


//----------------------------------------------------------------

//in this block methods are similar to methods for std::complex (see below)
template<typename T>
class Broken
{
private:
	Points<T>* data;
	int cap, n;
	void memory();

public:
	Broken();
	Broken(const int cap);
	~Broken();
	Broken(const Broken<T>&);
	double len_broken();
	Broken operator +(const Broken<T>&);
	void operator <<(const Points<T>&);
	void operator >>(const Points<T>&);
	Points<T>& operator [](int);
	int get_cap() const;
	friend std::ostream& operator<<(std::ostream&, const Points<T>&);
	friend bool operator ==(const Points<T>&, const Points<T>&);
	friend bool operator !=(const Points<T>&, const Points<T>&);
	int get_n() const;
	void operator =(const Broken<T>&);
	void set_n();
};

template<typename T>
Broken<T>::Broken() :cap(0), n(0), data(NULL) {}

template<typename T>
Broken<T>::Broken(const int cap) : cap(cap), n(0) 
{ 
	data = new Points<T>[cap];
}

template<typename T>
Broken<T>::~Broken() { delete[] data; }

template<typename T>
Broken<T>::Broken(const Broken<T>& obj) : cap(obj.cap), n(obj.n) 
{
	data = new Points<T>[cap];

	for (int i = 0; i < n; i++) {
		if (i < cap) data[i] = obj.data[i];
		else throw "Error! Access outside of allocated memory.\n\n";
	}
}

template<typename T>
void Broken<T>::memory() 
{
	cap += 4;
	Points<T>* tmp_data = new Points<T>[cap];

	for (int i = 0; i < n; i++) {
		if (i < cap) tmp_data[i] = data[i];
		else throw "Error! Access outside of allocated memory.\n\n";
	}

	delete[] data;
	data = tmp_data;
}

template<typename T>
double Broken<T>::len_broken()
{
	double sum = 0;

	for (int i = 0; i < n - 1; i++) {
		if (i >= 1 && i < n - 1 && data[i - 1] == data[i + 1]) continue;

		double x = 0, y = 0;

		x = pow(data[i].x - data[i + 1].x, 2);
		y = pow(data[i].y - data[i + 1].y, 2);

		sum += sqrt(x + y);
	}

	return sum;
}

template<typename T>
bool operator==(const Points<T> p1, const Points<T>& p2)
{
	if (fabs(p1.x - p2.x) > 0.000001) return false;
	if (fabs(p1.y - p2.y) > 0.000001) return false;
	return true;
}

template<typename T>
bool operator!=(const Points<T> p1, const Points<T>& p2)
{
	if (fabs(p1.x - p2.x) < 0.000001) return false;
	if (fabs(p1.y - p2.y) < 0.000001) return false;
	return true;
}

template<typename T>
std::ostream& operator <<(std::ostream& os, const Points<T>& p)
{
	return os << "(" << p.x << ", " << p.y << ") ";
}

template<typename T>
Broken<T> Broken<T>:: operator +(const Broken<T>& obj) 
{
	Broken<T> tmp_obj(cap + obj.cap);
	int tmp_cap = cap + obj.cap;
	tmp_obj.n = n + obj.n;

	for (int i = 0; i < n; i++) {
		if (i < tmp_cap) tmp_obj.data[i] = data[i];
		else throw "Error! Access outside of allocated memory.\n\n";
	}

	for (int i = n; i < tmp_obj.n; i++) {
		if (i < tmp_cap) tmp_obj.data[i] = obj.data[i - n];
		else throw "Error! Access outside of allocated memory.\n\n";
	}

	return tmp_obj;
}

template<typename T>
void Broken<T>:: operator <<(const Points<T>& str)
{
	if (cap <= n) memory();
	data[n++] = str;
}

template<typename T>
void Broken<T>:: operator >>(const Points<T>& str)
{
	if (cap <= n) memory();

	Points<T>* tmp_data = new Points<T>[cap];

	for (int i = 0; i < n; i++) {
		if (i < cap) tmp_data[i + 1] = data[i];
		else throw "Error! Access outside of allocated memory.\n\n";
	}

	tmp_data[0] = str;

	delete[] data;
	data = tmp_data;
	n++;
}

template<typename T>
void Broken<T>:: operator =(const Broken<T>& obj) 
{
	data = new Points<T>[obj.cap];
	n = obj.n;
	cap = obj.cap;
	for (int i = 0; i < n; i++) {
		if (i < cap) data[i] = obj.data[i];
		else throw "Error! Access outside of allocated memory.\n\n";
	}
}

template<typename T>
Points<T>& Broken<T>:: operator [](int i)
{
	if (i < n) return data[i];
	else throw "Error! Attempt to access by case index.\n\n";
}

template<typename T>
int Broken<T>::get_cap() const { return cap; }

template<typename T>
int Broken<T>::get_n() const { return n; }

template<typename T>
void Broken<T>::set_n() { n++; }

//----------------------------------------------------------

//Class declaration (for std::complex)
template<typename S>
class Broken<std::complex<S>>
{
private:
	std::complex<S>* data;
	int cap, n;
	void memory();

public:
	Broken();
	Broken(const int cap);
	~Broken();
	Broken(const Broken<std::complex<S>>&);
	double len_broken();
	Broken<std::complex<S>> operator +(const Broken<std::complex<S>>&);
	void operator <<(const std::complex<S>&);
	void operator >>(const std::complex<S>&);
	std::complex<S>& operator [](int);
	int get_cap() const;
	friend std::ostream& operator<<(std::ostream&, const std::complex<S>&);
	friend bool operator ==(std::complex<S>&, std::complex<S>&);
	friend bool operator !=(std::complex<S>&, std::complex<S>&);
	int get_n() const;
	void operator =(const Broken<std::complex<S>>&);
	void set_n();
};

//Constructor without parameters (for std::complex)
template<typename S>
Broken<std::complex<S>>::Broken() :cap(0), n(0), data(NULL) {}

//Constructor (for std::complex)
template<typename S>
Broken<std::complex<S>>::Broken(const int cap) : cap(cap), n(0) 
{ 
	data = new std::complex<S>[cap];
}

//Destructor (for std::complex)
template<typename S>
Broken<std::complex<S>>::~Broken() { delete[] data; }

//Copy constructor (for std::complex)
template<typename S>
Broken<std::complex<S>>::Broken(const Broken<std::complex<S>>& obj) : cap(obj.cap), n(obj.n)
{
	data = new std::complex<S>[cap];

	for (int i = 0; i < n; i++) {
		if (i < cap) data[i] = obj.data[i];
		else throw "Error! Access outside of allocated memory.\n\n";
	}
}

//Allocating memory for an array of vertices (for std::complex)
template<typename S>
void Broken<std::complex<S>>::memory()
{
	cap += 4;
	std::complex<S>* tmp_data = new std::complex<S>[cap];

	for (int i = 0; i < n; i++) {
		if (i < cap) tmp_data[i] = data[i];
		else throw "Error! Access outside of allocated memory.\n\n";
	}

	delete[] data;
	data = tmp_data;
}

//Returns the length of the polyline (for std::complex)
template<typename S>
double Broken<std::complex<S>>::len_broken()
{
	double sum = 0;

	for (int i = 0; i < n - 1; i++) {
		if (i >= 1 && i < n - 1 && data[i - 1] == data[i + 1]) continue;

		double x = 0, y = 0;

		x = pow(data[i].real() - data[i + 1].real(), 2);
		y = pow(data[i].imag() - data[i + 1].imag(), 2);

		sum += sqrt(x + y);
	}

	return sum;
}

//Returns a new polyline obtained by joining two other polylines (for std::complex)
template<typename S>
Broken<std::complex<S>> Broken<std::complex<S>>:: operator +(const Broken<std::complex<S>>& obj)
{
	Broken<std::complex<S>> tmp_obj(cap + obj.cap);
	int tmp_cap = cap + obj.cap;
	tmp_obj.n = n + obj.n;

	for (int i = 0; i < n; i++) {
		if (i < tmp_cap) tmp_obj.data[i] = data[i];
		else throw "Error! Access outside of allocated memory.\n\n";
	}

	for (int i = n; i < tmp_obj.n; i++) {
		if (i < tmp_cap) tmp_obj.data[i] = obj.data[i - n];
		else throw "Error! Access outside of allocated memory.\n\n";
	}

	return tmp_obj;
}

//Adding a vertex to the end of a polyline (for std::complex)
template<typename S>
void Broken<std::complex<S>>:: operator <<(const std::complex<S>& str)
{
	if (cap <= n) memory();
	data[n++] = str;
}

//Overloaded Output Operator (for std::complex)
template<typename S>
std::ostream& operator<<(std::ostream& os, const std::complex<S> c)
{
	return os << "(" << c.real() << ", " << c.imag() << ") ";
}

//Comparing vertices for equality
template<typename S>
bool operator==(std::complex<S>& c1, std::complex<S>& c2)
{
	if (fabs(c1.real() - c2.real()) > 0.000001) return false;
	if (fabs(c1.imag() - c2.imag()) > 0.000001) return false;
	return true;
}

//Comparing vertices for inequality
template<typename S>
bool operator!=(std::complex<S>& c1, std::complex<S>& c2)
{
	if (fabs(c1.real() - c2.real()) < 0.000001) return false;
	if (fabs(c1.imag() - c2.imag()) < 0.000001) return false;
	return true;
}

//Adding a vertex to the beginning of a polyline (for std::complex)
template<typename S>
void Broken<std::complex<S>>:: operator >>(const std::complex<S>& str)
{
	if (cap <= n) memory();

	std::complex<S>* tmp_data = new std::complex<S>[cap];

	for (int i = 0; i < n; i++) {
		if (i < cap) tmp_data[i + 1] = data[i];
		else throw "Error! Access outside of allocated memory.\n\n";
	}

	tmp_data[0] = str;

	delete[] data;
	data = tmp_data;
	n++;
}

//Assigns an already existing object the value of another object (for std::complex)
template<typename S>
void Broken<std::complex<S>>:: operator =(const Broken<std::complex<S>>& obj)
{
	data = new std::complex<S>[obj.cap];
	n = obj.n;
	cap = obj.cap;
	for (int i = 0; i < n; i++) {
		if (i < cap) data[i] = obj.data[i];
		else throw "Error! Access outside of allocated memory.\n\n";
	}
}

//Returns a reference to the top (for std::complex)
template<typename S>
std::complex<S>& Broken<std::complex<S>>:: operator [](int i) 
{ 
	if (i < n) return data[i];
	else throw "Error! Attempt to access by case index.\n\n";
}

//Returns the capacity of the array of vertices (for std::complex)
template<typename S>
int Broken<std::complex<S>>::get_cap() const { return cap; }

//Returns the number of stored vertices (for std::complex)
template<typename S>
int Broken<std::complex<S>>::get_n() const { return n; }

//Increases the number of stored vertices (for std::complex)
template<typename S>
void Broken<std::complex<S>>::set_n() { n++; }

//----------------------------------------------------------

//Create a polyline
template<typename T>
void create_polyline(Broken<T>* mas_obj, int* count)
{
	int n = 0;
	Points<T> p = { 0 , 0 };

	std::cout << "\n";
	do
	{
		std::cout << "The maximum number of vertices in a polyline: ";
		std::cin >> n;
	} while (n <= 0);

	Broken<T> tmp(n);

	do
	{
		std::cout << "How many vertices do you want to add now?: ";
		std::cin >> n;
	} while (n <= 0 || n > tmp.get_cap());
	

	for (int i = 0; i < n; i++)
	{
		tmp.set_n();
		std::cout << "\nEnter x: ";
		std::cin >> p.x;
		std::cout << "Enter y: ";
		std::cin >> p.y;
		tmp[i] = p;
	}

	mas_obj[*count] = tmp;
	*count += 1;
}

//Displaying polyline vertices
template<typename T>
void output_vertices(Broken<T>* mas_obj, int count)
{
	int n = 0;

	std::cout << "\n";
	do
	{
		std::cout << "Which polyline vertices do you want to get?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);
	

	std::cout << "\nVertices: ";
	for (int i = 0; i < mas_obj[n].get_n(); i++)
	{
		std::cout << mas_obj[n][i];
		if (i < mas_obj[n].get_n() - 1) std::cout << "-> ";
	}

	std::cout << "\n\n";
	system("pause");
}

//Getting the length of a polyline
template<typename T>
void len_poly(Broken<T>* mas_obj, int count)
{
	int n = 0;

	std::cout << "\n";
	do
	{
		std::cout << "Which polyline length do you want to calculate?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	std::cout << "The length of the polyline is " << mas_obj[n].len_broken();
	std::cout << "\n\n";
	system("pause");
}

//Adding a vertex to the beginning of a polyline
template<typename T>
void ver_beg(Broken<T>* mas_obj, int count)
{
	int n = 0;
	Points<T> p = { 0, 0 };

	std::cout << "\n";
	do
	{
		std::cout << "In which polyline do I need to add a vertex to the beginning?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	std::cout << "\nEnter x: ";
	std::cin >> p.x;
	std::cout << "Enter y: ";
	std::cin >> p.y;

	mas_obj[n] >> p;
}

//Adding a vertex to the end of a polyline
template<typename T>
void ver_back(Broken<T>* mas_obj, int count)
{
	int n = 0;
	Points<T> p = { 0, 0 };

	std::cout << "\n";
	do
	{
		std::cout << "In which polyline do I need to add a vertex to the beginning?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	std::cout << "\nEnter x: ";
	std::cin >> p.x;
	std::cout << "Enter y: ";
	std::cin >> p.y;

	mas_obj[n] << p;
}

//Creating a new polyline obtained by joining two other polylines
template<typename T>
void sum_ver(Broken<T>* mas_obj, int* count)
{
	int n = 0, m = 0;
	
	std::cout << "\n";
	do
	{
		std::cout << "Select the first vertex (counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= *count);

	do
	{
		std::cout << "Select the second vertex (counting from zero): ";
		std::cin >> m;
	} while (m < 0 || m >= *count);

	mas_obj[*count] = mas_obj[n] + mas_obj[m];
	*count += 1;
}

//Additional task to create a triangle
template<typename T>
void add_task(Broken<T>* mas_obj, int* count)
{
	Points<T> p = { 0, 0 };
	double phi = 0, a = 0, pi = 3.1415;

	std::cout << "\n";
	do
	{
		std::cout << "Set the length of the side: ";
		std::cin >> a;
	} while (a <= 0);

	do
	{
		std::cout << "Set the angle at the top: ";
		std::cin >> phi;
	} while (phi < 0 || phi > 180);

	mas_obj[*count] << p;

	phi = (phi * pi) / 180;
	p.x = p.x + a * cos(phi);
	p.y = p.y + a * sin(phi);
	mas_obj[*count] << p;

	p.x = 2 * p.x;
	p.y = 0;
	mas_obj[*count] << p;

	p.x = 0;
	mas_obj[*count] << p;

	*count += 1;
}

//---------------------------------------------------------

//Create a polyline (only std::complex)
template<typename S>
void create_polyline(Broken<std::complex<S>>* mas_obj, int* count)
{
	int n = 0;
	std::complex<S> p;
	S num = 0;

	std::cout << "\n";
	do
	{
		std::cout << "The maximum number of vertices in a polyline: ";
		std::cin >> n;
	} while (n <= 0);

	Broken<std::complex<S>> tmp(n);

	do
	{
		std::cout << "How many vertices do you want to add now?: ";
		std::cin >> n;
	} while (n <= 0 || n > tmp.get_cap());


	for (int i = 0; i < n; i++)
	{
		tmp.set_n();
		std::cout << "\nEnter x: ";
		std::cin >> num;
		p.real(num);
		std::cout << "Enter y: ";
		std::cin >> num;
		p.imag(num);
		tmp[i] = p;
	}

	mas_obj[*count] = tmp;
	*count += 1;
}

//Adding a vertex to the beginning of a polyline (only std::complex)
template<typename S>
void ver_beg(Broken<std::complex<S>>* mas_obj, int count)
{
	int n = 0;
	std::complex<S> p;
	S num = 0;

	std::cout << "\n";
	do
	{
		std::cout << "In which polyline do I need to add a vertex to the beginning?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	std::cout << "\nEnter x: ";
	std::cin >> num;
	p.real(num);
	std::cout << "Enter y: ";
	std::cin >> num;
	p.imag(num);

	mas_obj[n] >> p;
}

//Adding a vertex to the end of a polyline (only std::complex)
template<typename S>
void ver_back(Broken<std::complex<S>>* mas_obj, int count)
{
	int n = 0;
	std::complex<S> p;
	S num = 0;

	std::cout << "\n";
	do
	{
		std::cout << "In which polyline do I need to add a vertex to the beginning?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	std::cout << "\nEnter x: ";
	std::cin >> num;
	p.real(num);
	std::cout << "Enter y: ";
	std::cin >> num;
	p.imag(num);

	mas_obj[n] << p;
}

//Additional task to create a triangle (only std::complex)
template<typename S>
void add_task(Broken<std::complex<S>>* mas_obj, int* count)
{
	std::complex<S> p(0, 0);
	double phi = 0, a = 0, pi = 3.1415;

	std::cout << "\n";
	do
	{
		std::cout << "Set the length of the side: ";
		std::cin >> a;
	} while (a <= 0);

	do
	{
		std::cout << "Set the angle at the top: ";
		std::cin >> phi;
	} while (phi < 0 || phi > 180);

	mas_obj[*count] << p;

	phi = (phi * pi) / 180;
	p.real(p.real() + a * cos(phi));
	p.imag(p.imag() + a * sin(phi));
	mas_obj[*count] << p;

	p.real(2 * p.real());
	p.imag(0);
	mas_obj[*count] << p;

	p.real(0);
	mas_obj[*count] << p;

	*count += 1;
}

//---------------------------------------------------------

//Main function
int main() 
{
	//Broken<int> mas_obj[20];
	//Broken<float> mas_obj[20];
	//Broken<double> mas_obj[20];
	Broken<std::complex<float>> mas_obj[20];
	//Broken<std::complex<double>> mas_obj[20];

	int n = 0;
	int count = 0;

	while (1)
	{	
		try {
			menu();
			std::cin >> n;
			if (n == 1) create_polyline(mas_obj, &count);
			if (n == 2) output_vertices(mas_obj, count);
			if (n == 3) len_poly(mas_obj, count);
			if (n == 4) ver_beg(mas_obj, count);
			if (n == 5) ver_back(mas_obj, count);
			if (n == 6) sum_ver(mas_obj, &count);
			if (n == 7) add_task(mas_obj, &count);
			if (n == 8) break;
			system("CLS");
		}
		catch (const char* ex)
		{
			std::cout << ex;
			system("pause");
			system("CLS");
		}
	}
}

//При определении дружественного оператора сравнения на равенство/неравенство
//мы не писали const, потому что метод real и imag НЕ ЯВЛЯЮТСЯ КОНСТАНТНЫМИ.
