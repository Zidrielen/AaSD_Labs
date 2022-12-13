#include<math.h>
#include<iostream>
#include <windows.h>
#include<conio.h>
#include <complex>
#include<math.h>


template<typename T>
struct Points 
{
private:
	static double lambda;
public:
	T x, y;
	double& _lambda() { return lambda; }
	template<typename V> friend std::ostream& operator <<(std::ostream&, const Points<V>&);
};

template<typename T>
double Points<T>::lambda = 0.000001;

template<typename T>
std::ostream& operator <<(std::ostream& os, const Points<T>& p)
{
	return os << "(" << p.x << ", " << p.y << ")";
}

//Menu for selecting data type
void choose_type()
{
	std::cout << "Choose the type of data you need:\n\n";
	std::cout << "1. int\n";
	std::cout << "2. float\n";
	std::cout << "3. double\n";
	std::cout << "4. std::complex<float>\n";
	std::cout << "5. std::complex<double>\n";
	std::cout << "\nEnter the number: ";
}

//Main menu
void menu()
{
	std::cout << "1. Create a polyline\n";
	std::cout << "2. Overwrite the vertex\n";
	std::cout << "3. Output the vertex\n";
	std::cout << "4. Get all vertices of a particular polyline\n";
	std::cout << "5. Get the length of a specific polyline\n";
	std::cout << "6. Add a vertex to the beginning of a specific polyline\n";
	std::cout << "7. Add a vertex to the end of a certain polyline\n";
	std::cout << "8. Create a polyline that is the 'sum' of two polylines\n";
	std::cout << "9. Create a polyline in the form of an isosceles triangle\n";
	std::cout << "10. Compare broken lines\n";
	std::cout << "11. Change precision\n";
	std::cout << "12. Exit the program\n";
	std::cout << "\nEnter the number: ";
}

//----------------------------------------------------------------

//Broken class declaration
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
	double len_broken() const;
	double len_broken_complex() const;
	Broken<T> operator +(const Broken<T>&) const;
	void operator =(const Broken<T>&);
	void operator +=(const Points<T>&);
	template<typename V> friend void operator +=(const Points<V>&, Broken<V>&);
	template<typename V> friend std::ostream& operator <<(std::ostream&, const Broken<V>&);
	bool operator ==(const Broken<T>&);
	bool operator !=(const Broken<T>&);
	Points<T>& operator [](int);
	int get_cap() const;
	int get_n() const;
	void set_n();
};

//Allocating memory for an array of vertices
template<typename T>
void Broken<T>::memory()
{
	cap += 2;
	Points<T>* tmp_data = new Points<T>[cap];

	for (int i = 0; i < n; i++) {
		if (i < cap) tmp_data[i] = data[i];
		else throw "Error! Access outside of allocated memory.\n\n";
	}

	delete[] data;
	data = tmp_data;
}

//Explicit default constructor
template<typename T>
Broken<T>::Broken() :cap(0), n(0), data(NULL) {}

//Constructor for allocating memory for vertices
template<typename T>
Broken<T>::Broken(const int cap) : cap(cap), n(0)
{ 
	data = new Points<T>[cap];
}

//Destructor
template<typename T>
Broken<T>::~Broken() { delete[] data; }

//Сopy constructor
template<typename T>
Broken<T>::Broken(const Broken<T>& obj) : cap(obj.cap), n(obj.n)
{
	data = new Points<T>[cap];

	for (int i = 0; i < n; i++) {
		if (i < cap) data[i] = obj.data[i];
		else throw "Error! Access outside of allocated memory.\n\n";
	}
}

//Calculating the length of a polyline
template<typename T>
double Broken<T>::len_broken() const
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

//Calculating the length of a polyline (for std::complex<>)
template<typename T>
double Broken<T>::len_broken_complex() const
{
	double sum = 0;

	for (int i = 0; i < n - 1; i++) {
		if (i >= 1 && i < n - 1 && data[i - 1] == data[i + 1]) continue;

		T z1, z2;

		z1 = data[i].x - data[i + 1].x;
		z2 = data[i].y - data[i + 1].y;

		sum += sqrt(pow(std::abs(z1), 2) + pow(std::abs(z2), 2));
	}

	return sum;
}

//Two-polyline addition operator
template<typename T>
Broken<T> Broken<T>:: operator +(const Broken<T>& obj) const
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

//Assignment operator
template<typename T>
void Broken<T>:: operator =(const Broken<T>& obj)
{
	delete [] data;
	data = new Points<T>[obj.cap];
	n = obj.n;
	cap = obj.cap;
	for (int i = 0; i < n; i++) {
		if (i < cap) data[i] = obj.data[i];
		else throw "Error! Access outside of allocated memory.\n\n";
	}
}

//Add vertex to end operator
template<typename T>
void Broken<T>:: operator +=(const Points<T>& p)
{
	if (cap <= n) memory();
	data[n++] = p;
}

//Operator for adding a vertex to the beginning
template<typename T>
void operator +=(const Points<T>& p, Broken<T>& obj)
{
	if (obj.cap <= obj.n) obj.memory();
	
	Points<T>* tmp_data = new Points<T>[obj.cap];
	
	for (int i = 0; i < obj.n; i++) 
	{
		if (i < obj.cap) tmp_data[i + 1] = obj.data[i];
		else throw "Error! Access outside of allocated memory.";
	}
	
	tmp_data[0] = p;
	
	delete[] obj.data;
	obj.data = tmp_data;
	obj.n++;
}

//Vertex Output Operator Overload
template<typename T>
std::ostream& operator <<(std::ostream& os, const Broken<T>& obj)
{
	for (int i = 0; i < obj.n; i++)
	{
		os << obj.data[i];
		if (i < obj.n - 1) os << " -> ";
	}
	return os;
}

//Comparison operator for equality of broken lines
template<typename T>
bool Broken<T>::operator ==(const Broken<T>& obj)
{
	if (n != obj.n) throw "\n\nThe broken lines are incomparable, due to the different number of vertices.\n\n";

	for (int i = 0; i < n; i++)
	{
		if (data[i] != obj.data[i]) return false;
	}

	return true;
}

//Comparison operator for broken line inequality
template<typename T>
bool Broken<T>::operator !=(const Broken<T>& obj)
{
	if (n != obj.n) throw "\n\nThe broken lines are incomparable, due to the different number of vertices.\n\n";

	for (int i = 0; i < n; i++)
	{
		if (data[i] == obj.data[i]) return false;
	}

	return true;
}

//Vertex Read/Write Operator
template<typename T>
Points<T>& Broken<T>:: operator [](const int i)
{
	if (i < n) return data[i];
	else throw "Error! Attempt to access by case index.\n\n";
}

//Getting the capacity of an array of vertices
template<typename T>
int Broken<T>::get_cap() const { return cap; }

//Getting the number of vertices in an array
template<typename T>
int Broken<T>::get_n() const { return n; }

//Increment by one of the vertex counter in the array
template<typename T>
void Broken<T>::set_n() { n++; }

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
	std::cout << mas_obj[n];

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
	Points<T> p;

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

	p += mas_obj[n];
}

//Adding a vertex to the end of a polyline
template<typename T>
void ver_back(Broken<T>* mas_obj, int count)
{
	int n = 0;
	Points<T> p;

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

	mas_obj[n] += p;
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


	mas_obj[*count] += p;

	phi = pi/2 - ((phi * pi) / 360);
	p.x = p.x + a * cos(phi);
	p.y = p.y + a * sin(phi);
	mas_obj[*count] += p;

	p.x = 2 * p.x;
	p.y = 0;
	mas_obj[*count] += p;

	p.x = 0;
	mas_obj[*count] += p;

	*count += 1;
}

//Rewrites the vertex
template<typename T>
void rewrite_vertex(Broken<T>* mas_obj, int count)
{
	int n = 0, m = 0;
	Points<T> p;

	std::cout << "\n";
	do
	{
		std::cout << "Choose in which polyline the vertex should be rewritten?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	std::cout << "\n";
	do
	{
		std::cout << "Choose which vertex you want to overwrite?(counting from zero): ";
		std::cin >> m;
	} while (m < 0 || m >= mas_obj[n].get_n());

	std::cout << "\nEnter x: ";
	std::cin >> p.x;
	std::cout << "Enter y: ";
	std::cin >> p.y;

	mas_obj[n][m] = p;
}

//Displays the vertex of the polyline
template<typename T>
void input_vertex(Broken<T>* mas_obj, int count)
{
	int n = 0, m = 0;

	std::cout << "\n";
	do
	{
		std::cout << "Choose from which polyline to display the vertex?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	std::cout << "\n";
	do
	{
		std::cout << "Which vertex to display?(counting from zero): ";
		std::cin >> m;
	} while (m < 0 || m >= mas_obj[n].get_n());

	std::cout << "\nVertex: " << mas_obj[n][m] << "\n\n";
	system("pause");
}

template<typename T>
void compare_broken(Broken<T>* mas_obj, int count)
{
	int n = 0, m = 0;

	std::cout << "\n";
	do
	{
		std::cout << "Select the first vertex (counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	do
	{
		std::cout << "Select the second vertex (counting from zero): ";
		std::cin >> m;
	} while (m < 0 || m >= count || m == n);

	if (mas_obj[n] == mas_obj[m]) std::cout << "Broken lines are equal\n\n";
	else std::cout << "Broken lines are not equal\n\n";

	system("pause");
}

//---------------------------------------------------------

//Create a polyline (for std::complex<>)
template<typename S>
void create_polyline(Broken<std::complex<S>>* mas_obj, int* count)
{
	int n = 0;
	Points<std::complex<S>> p;
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

		std::cout << "\n\tEnter real: ";
		std::cin >> num;
		p.x.real(num);

		std::cout << "\tEnter imag: ";
		std::cin >> num;
		p.x.imag(num);


		std::cout << "\nEnter y: ";

		std::cout << "\n\tEnter real: ";
		std::cin >> num;
		p.y.real(num);

		std::cout << "\tEnter imag: ";
		std::cin >> num;
		p.y.imag(num);

		tmp[i] = p;
	}

	mas_obj[*count] = tmp;
	*count += 1;
}

//Rewrites the vertex(for std::complex<>)
template<typename S>
void rewrite_vertex(Broken<std::complex<S>>* mas_obj, int count)
{
	int n = 0, m = 0;
	Points<std::complex<S>> p;
	S num = 0;

	std::cout << "\n";
	do
	{
		std::cout << "Choose in which polyline the vertex should be rewritten?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	std::cout << "\n";
	do
	{
		std::cout << "Choose which vertex you want to overwrite?(counting from zero): ";
		std::cin >> m;
	} while (m < 0 || m >= mas_obj[n].get_n());

	std::cout << "\nEnter x: ";

	std::cout << "\n\tEnter real: ";
	std::cin >> num;
	p.x.real(num);

	std::cout << "\tEnter imag: ";
	std::cin >> num;
	p.x.imag(num);


	std::cout << "\nEnter y: ";

	std::cout << "\n\tEnter real: ";
	std::cin >> num;
	p.y.real(num);

	std::cout << "\tEnter imag: ";
	std::cin >> num;
	p.y.imag(num);

	mas_obj[n][m] = p;
}

//Adding a vertex to the beginning of a polyline (for std::complex<>)
template<typename S>
void ver_beg(Broken<std::complex<S>>* mas_obj, int count)
{
	int n = 0;
	Points<std::complex<S>> p;
	S num = 0;

	std::cout << "\n";
	do
	{
		std::cout << "In which polyline do I need to add a vertex to the beginning?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	std::cout << "\nEnter x: ";

	std::cout << "\n\tEnter real: ";
	std::cin >> num;
	p.x.real(num);

	std::cout << "\tEnter imag: ";
	std::cin >> num;
	p.x.imag(num);


	std::cout << "\nEnter y: ";

	std::cout << "\n\tEnter real: ";
	std::cin >> num;
	p.y.real(num);

	std::cout << "\tEnter imag: ";
	std::cin >> num;
	p.y.imag(num);

	p += mas_obj[n];
}

//Adding a vertex to the end of a polyline (for std::complex)
template<typename S>
void ver_back(Broken<std::complex<S>>* mas_obj, int count)
{
	int n = 0;
	Points<std::complex<S>> p;
	S num = 0;

	std::cout << "\n";
	do
	{
		std::cout << "In which polyline do I need to add a vertex to the beginning?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	std::cout << "\nEnter x: ";

	std::cout << "\n\tEnter real: ";
	std::cin >> num;
	p.x.real(num);

	std::cout << "\tEnter imag: ";
	std::cin >> num;
	p.x.imag(num);


	std::cout << "\nEnter y: ";

	std::cout << "\n\tEnter real: ";
	std::cin >> num;
	p.y.real(num);

	std::cout << "\tEnter imag: ";
	std::cin >> num;
	p.y.imag(num);

	mas_obj[n] += p;
}

//Getting the length of a polyline(for std::complex<>)
template<typename S>
void len_poly(Broken<std::complex<S>>* mas_obj, int count)
{
	int n = 0;

	std::cout << "\n";
	do
	{
		std::cout << "Which polyline length do you want to calculate?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count);

	std::cout << "The length of the polyline is " << mas_obj[n].len_broken_complex();
	std::cout << "\n\n";
	system("pause");
}

//Additional task to create a triangle (only std::complex)
template<typename S>
void add_task(Broken<std::complex<S>>* mas_obj, int* count)
{
	Points<std::complex<S>> p;
	double phi = 0, a = 0, pi = 3.1415 ,abs = 0;

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

	mas_obj[*count] += p;
	phi = pi / 2 - ((phi * pi) / 360);

	abs = a * cos(phi);
	abs *= abs;
	p.x.real(sqrt(abs / 2)); p.x.imag(sqrt(abs / 2));
	abs = a * sin(phi);
	abs *= abs;
	p.y.real(sqrt(abs / 2)); p.y.imag(sqrt(abs / 2));
	mas_obj[*count] += p;

	p.y.real(0); p.y.imag(0);
	p.x.real(p.x.real() * 2);
	p.x.imag(p.x.imag() * 2);
	mas_obj[*count] += p;

	p.x.real(0); p.x.imag(0);
	mas_obj[*count] += p;

	*count += 1;
}

//---------------------------------------------------------

//Precision change
template<typename T>
void change_precision()
{
	Points<T> p;
	double n = 0;

	std::cout << "Now the accuracy is " << p._lambda();
	std::cout << "\nEnter new precision: ";
	std::cin >> n;

	p._lambda() = n;
}

//Equality comparison operator
template<typename T>
bool operator==(Points<T>& p1, Points<T>& p2)
{
	if (fabs(p1.x - p2.x) > p1._lambda()) return false;
	if (fabs(p1.y - p2.y) > p1._lambda()) return false;
	return true;
}

//Comparison operator for inequality
template<typename T>
bool operator!=(Points<T>& p1, Points<T>& p2)
{
	if (fabs(p1.x - p2.x) < p1._lambda()) return false;
	if (fabs(p1.y - p2.y) < p1._lambda()) return false;
	return true;
}

//Equality comparison operator(for std::complex<>)
template<typename S>
bool operator==(Points<std::complex<S>>& p1, Points<std::complex<S>>& p2)
{
	if (fabs(p1.x.real() - p2.x.real()) > p1._lambda()) return false;
	if (fabs(p1.x.imag() - p2.x.imag()) > p1._lambda()) return false;
	if (fabs(p1.y.real() - p2.y.real()) > p1._lambda()) return false;
	if (fabs(p1.y.imag() - p2.y.imag()) > p1._lambda()) return false;
	return true;
}

//Comparison operator for inequality(for std::complex<>)
template<typename S>
bool operator!=(Points<std::complex<S>>& p1, Points<std::complex<S>>& p2)
{
	if (fabs(p1.x.real() - p2.x.real()) < p1._lambda()) return false;
	if (fabs(p1.x.imag() - p2.x.imag()) < p1._lambda()) return false;
	if (fabs(p1.y.real() - p2.y.real()) < p1._lambda()) return false;
	if (fabs(p1.y.imag() - p2.y.imag()) < p1._lambda()) return false;
	return true;
}

//Complex number output operator overload
template<typename S>
std::ostream& operator <<(std::ostream& os, const std::complex<S>& c)
{
	return os << "(" << c.real() << ", " << c.imag() << ")";
}

//----------------------------------------------------------

//Main function
int main() 
{
	int n = 0;
	int count = 0;

	choose_type();
	std::cin >> n;

	if (n == 1)
	{
		Broken<int> mas_obj[20];
		while (1)
		{
			try {
				system("CLS");
				menu();
				std::cin >> n;
				if (n == 1) create_polyline(mas_obj, &count);
				if (n == 2) rewrite_vertex(mas_obj, count);
				if (n == 3) input_vertex(mas_obj, count);
				if (n == 4) output_vertices(mas_obj, count);
				if (n == 5) len_poly(mas_obj, count);
				if (n == 6) ver_beg(mas_obj, count);
				if (n == 7) ver_back(mas_obj, count);
				if (n == 8) sum_ver(mas_obj, &count);
				if (n == 9) add_task(mas_obj, &count);
				if (n == 10) compare_broken(mas_obj, count);
				if (n == 11) change_precision<int>();
				if (n == 12) break;
			}
			catch (const char* ex)
			{
				std::cout << ex;
				system("pause");
				system("CLS");
			}
		}
	}

	else if (n == 2)
	{
		Broken<float> mas_obj[20];
		while (1)
		{
			try {
				system("CLS");
				menu();
				std::cin >> n;
				if (n == 1) create_polyline(mas_obj, &count);
				if (n == 2) rewrite_vertex(mas_obj, count);
				if (n == 3) input_vertex(mas_obj, count);
				if (n == 4) output_vertices(mas_obj, count);
				if (n == 5) len_poly(mas_obj, count);
				if (n == 6) ver_beg(mas_obj, count);
				if (n == 7) ver_back(mas_obj, count);
				if (n == 8) sum_ver(mas_obj, &count);
				if (n == 9) add_task(mas_obj, &count);
				if (n == 10) compare_broken(mas_obj, count);
				if (n == 11) change_precision<float>();
				if (n == 12) break;
			}
			catch (const char* ex)
			{
				std::cout << ex;
				system("pause");
				system("CLS");
			}
		}
	}

	else if (n == 3)
	{
		Broken<double> mas_obj[20];
		while (1)
		{
			try {
				system("CLS");
				menu();
				std::cin >> n;
				if (n == 1) create_polyline(mas_obj, &count);
				if (n == 2) rewrite_vertex(mas_obj, count);
				if (n == 3) input_vertex(mas_obj, count);
				if (n == 4) output_vertices(mas_obj, count);
				if (n == 5) len_poly(mas_obj, count);
				if (n == 6) ver_beg(mas_obj, count);
				if (n == 7) ver_back(mas_obj, count);
				if (n == 8) sum_ver(mas_obj, &count);
				if (n == 9) add_task(mas_obj, &count);
				if (n == 10) compare_broken(mas_obj, count);
				if (n == 11) change_precision<double>();
				if (n == 12) break;
			}
			catch (const char* ex)
			{
				std::cout << ex;
				system("pause");
				system("CLS");
			}
		}
	}

	else if (n == 4)
	{
		Broken<std::complex<float>> mas_obj[20];
		while (1)
		{
			try {
				system("CLS");
				menu();
				std::cin >> n;
				if (n == 1) create_polyline(mas_obj, &count);
				if (n == 2) rewrite_vertex(mas_obj, count);
				if (n == 3) input_vertex(mas_obj, count);
				if (n == 4) output_vertices(mas_obj, count);
				if (n == 5) len_poly(mas_obj, count);
				if (n == 6) ver_beg(mas_obj, count);
				if (n == 7) ver_back(mas_obj, count);
				if (n == 8) sum_ver(mas_obj, &count);
				if (n == 9) add_task(mas_obj, &count);
				if (n == 10) compare_broken(mas_obj, count);
				if (n == 11) change_precision<std::complex<float>>();
				if (n == 12) break;
			}
			catch (const char* ex)
			{
				std::cout << ex;
				system("pause");
				system("CLS");
			}
		}
	}

	else if (n == 5)
	{
		Broken<std::complex<double>> mas_obj[20];
		while (1)
		{
			try {
				system("CLS");
				menu();
				std::cin >> n;
				if (n == 1) create_polyline(mas_obj, &count);
				if (n == 2) rewrite_vertex(mas_obj, count);
				if (n == 3) input_vertex(mas_obj, count);
				if (n == 4) output_vertices(mas_obj, count);
				if (n == 5) len_poly(mas_obj, count);
				if (n == 6) ver_beg(mas_obj, count);
				if (n == 7) ver_back(mas_obj, count);
				if (n == 8) sum_ver(mas_obj, &count);
				if (n == 9) add_task(mas_obj, &count);
				if (n == 10) compare_broken(mas_obj, count);
				if (n == 11) change_precision<std::complex<double>>();
				if (n == 12) break;
			}
			catch (const char* ex)
			{
				std::cout << ex;
				system("pause");
				system("CLS");
			}
		}
	}

	else std::cout << "\n\nPlease enter the correct number next time.\n\n";
}
