#include<iostream>
#include<vector>
#include <complex>
#include<math.h>
#include <windows.h>
#include<conio.h>


const double PI = 3.1415;

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
	std::vector<Points<T>> data;
	int n;

public:
	Broken();
	Broken(const int);
	Broken(const Broken<T>&) = default;
	~Broken() = default;
	Broken<T>& operator =(const Broken<T>&) = default;
	double len_broken() const;
	double len_broken_complex() const;
	Broken<T> operator +(const Broken<T>&) const;
	void operator +=(const Points<T>&);
	template<typename V> friend void operator +=(const Points<V>&, Broken<V>&);
	template<typename V> friend std::ostream& operator <<(std::ostream&, const Broken<V>&);
	bool operator ==(const Broken<T>&);
	bool operator !=(const Broken<T>&);
	Points<T>& operator [](int);
	auto begin();
	auto end();
	int get_n() const;
};

//Explicit default constructor
template<typename T>
Broken<T>::Broken() :n(0) {}

//Constructor
template<typename T>
Broken<T>::Broken(const int n): n(n)
{
	data.resize(n);
}

//Calculating the length of a polyline
template<typename T>
double Broken<T>::len_broken() const
{
	double sum = 0;
	std::vector<Points<T>> tmp = data;

	for (int i = 0; i < n - 1; i++)
	{
		if (i >= 1 && tmp[i-1] == tmp[i+1]) continue;
		
		double x = 0, y = 0;
		x = pow(tmp[i].x - tmp[i+1].x, 2);
		y = pow(tmp[i].y - tmp[i+1].y, 2);
		sum += sqrt(x + y);
	}

	return sum;
}

//Calculating the length of a polyline (for std::complex<>)
template<typename T>
double Broken<T>::len_broken_complex() const
{
	double sum = 0;
	std::vector<Points<T>> tmp = data;

	for (int i = 0; i < n - 1; i++)
	{
		if (i >= 1 && tmp[i-1] == tmp[i+1]) continue;
		
		T z1, z2;
		z1 = tmp[i].x - tmp[i+1].x;
		z2 = tmp[i].y - tmp[i+1].y;
		sum += sqrt(pow(std::abs(z1), 2) + pow(std::abs(z2), 2));
	}

	return sum;
}

//Two-polyline addition operator
template<typename T>
Broken<T> Broken<T>:: operator +(const Broken<T>& obj) const
{
	Broken<T> tmp_obj(n + obj.n);
	tmp_obj.n = n + obj.n;
	int c = 0;

	for (const auto& i : data) tmp_obj[c++] = i;
	for (const auto& i : obj.data) tmp_obj[c++] = i;

	return tmp_obj;
}

//Add vertex to end operator
template<typename T>
void Broken<T>:: operator +=(const Points<T>& p)
{
	data.push_back(p);
	n++;
}

//Operator for adding a vertex to the beginning
template<typename T>
void operator +=(const Points<T>& p, Broken<T>& obj)
{
	std::vector<Points<T>> tmp_data(obj.n + 1);
	int c = 0;
	
	for (const auto& i: obj.data) tmp_data[c++ + 1] = i;
	
	tmp_data[0] = p;
	obj.data = tmp_data;
	obj.n++;
}

//Vertex Output Operator Overload
template<typename T>
std::ostream& operator <<(std::ostream& os, const Broken<T>& obj)
{
	int c = 0;

	for (auto& i: obj.data)
	{
		os << i;
		if (c < obj.n - 1) os << " -> ";
		c++;
	}
	return os;
}

//Comparison operator for equality of broken lines
template<typename T>
bool Broken<T>::operator ==(const Broken<T>& obj)
{
	if (n != obj.n) throw "\n\nThe broken lines are incomparable, due to the different number of vertices.\n\n";
	int c = 0;

	for (auto i: obj.data) if (i != data[c++]) return false;

	return true;
}

//Comparison operator for broken line inequality
template<typename T>
bool Broken<T>::operator !=(const Broken<T>& obj)
{
	return !(*this == obj);
}

//Vertex Read/Write Operator
template<typename T>
Points<T>& Broken<T>:: operator [](const int i)
{
	if (i < n) return data[i];
	else throw "Error! Attempt to access by case index.\n\n";
}

//Iterator to start element
template<typename T>
auto Broken<T>::begin()
{
	return data.begin();
}

//Iterator to last element
template<typename T>
auto Broken<T>::end()
{
	return data.end();
}

//Getting the number of vertices in an array
template<typename T>
int Broken<T>::get_n() const { return n; }

//----------------------------------------------------------

//Create a polyline
template<typename T>
void create_polyline(Broken<T>* mas_obj, int* count)
{
	int n = 0;

	do
	{
		std::cout << "How many vertices do you want to add now?: ";
		std::cin >> n;
	} while (n <= 0 || n != (int)n);
	
	Broken<T> tmp(n);

	for (auto& i: tmp)
	{
		std::cout << "\nEnter x: ";
		std::cin >> i.x;
		std::cout << "Enter y: ";
		std::cin >> i.y;
	}

	mas_obj[*count] = tmp;
	*count += 1;
}

//Displaying polyline vertices
template<typename T>
void output_vertices(Broken<T>* mas_obj, int count)
{
	int n = 0;

	do
	{
		std::cout << "\nWhich polyline vertices do you want to get?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count || n != (int)n);
	

	std::cout << "\nVertices: ";
	std::cout << mas_obj[n] << "\n\n";

	system("pause");
}

//Getting the length of a polyline
template<typename T>
void len_poly(Broken<T>* mas_obj, int count)
{
	int n = 0;

	do
	{
		std::cout << "\nWhich polyline length do you want to calculate?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count || n != (int)n);

	std::cout << "The length of the polyline is " << mas_obj[n].len_broken() << "\n\n";
	system("pause");
}

//Adding a vertex to the beginning of a polyline
template<typename T>
void ver_beg(Broken<T>* mas_obj, int count)
{
	int n = 0;
	Points<T> p;

	do
	{
		std::cout << "\nIn which polyline do I need to add a vertex to the beginning?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count || n != (int)n);

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

	do
	{
		std::cout << "\nIn which polyline do I need to add a vertex to the beginning?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count || n != (int)n);

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
	
	do
	{
		std::cout << "\nSelect the FIRST vertex (counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= *count || n != (int)n);

	do
	{
		std::cout << "\nSelect the SECOND vertex (counting from zero): ";
		std::cin >> m;
	} while (m < 0 || m >= *count || m != (int)m);

	mas_obj[*count] = mas_obj[n] + mas_obj[m];
	*count += 1;
}

//Additional task to create a triangle
template<typename T>
void add_task(Broken<T>* mas_obj, int* count)
{
	Points<T> p;
	double phi = 0, a = 0;

	do
	{
		std::cout << "\nSet the LEIGHT of the side: ";
		std::cin >> a;
	} while (a <= 0);

	do
	{
		std::cout << "\nSet the ANGLE at the top: ";
		std::cin >> phi;
	} while (phi < 0 || phi > 180);


	mas_obj[*count] += p;

	phi = PI/2 - ((phi * PI) / 360);
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

	do
	{
		std::cout << "\nChoose in which polyline the vertex should be rewritten?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count || n != (int)n);

	do
	{
		std::cout << "\nChoose which vertex you want to overwrite?(counting from zero): ";
		std::cin >> m;
	} while (m < 0 || m >= mas_obj[n].get_n() || m != (int)m);

	std::cout << "\nEnter x: ";
	std::cin >> mas_obj[n][m].x;
	std::cout << "Enter y: ";
	std::cin >> mas_obj[n][m].y;
}

//Displays the vertex of the polyline
template<typename T>
void input_vertex(Broken<T>* mas_obj, int count)
{
	int n = 0, m = 0;

	do
	{
		std::cout << "\nChoose from which polyline to display the vertex?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count || n != (int)n);

	do
	{
		std::cout << "\nWhich vertex to display?(counting from zero): ";
		std::cin >> m;
	} while (m < 0 || m >= mas_obj[n].get_n() || m != (int)m);

	std::cout << "\nVertex: " << mas_obj[n][m] << "\n\n";
	system("pause");
}

template<typename T>
void compare_broken(Broken<T>* mas_obj, int count)
{
	int n = 0, m = 0;

	do
	{
		std::cout << "\nSelect the FIRST polyline (counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count || n != (int)n);

	do
	{
		std::cout << "\nSelect the SECOND polyline (counting from zero): ";
		std::cin >> m;
	} while (m < 0 || m >= count || m == n || m != (int)m);

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
	S num = 0;

	do
	{
		std::cout << "\nHow many vertices do you want to add now?: ";
		std::cin >> n;
	} while (n <= 0 || n != (int)n);

	Broken<std::complex<S>> tmp(n);

	for (auto& i: tmp)
	{
		std::cout << "\nEnter x: ";

		std::cout << "\n\tEnter real: ";
		std::cin >> num;
		i.x.real(num);

		std::cout << "\tEnter imag: ";
		std::cin >> num;
		i.x.imag(num);


		std::cout << "\nEnter y: ";

		std::cout << "\n\tEnter real: ";
		std::cin >> num;
		i.y.real(num);

		std::cout << "\tEnter imag: ";
		std::cin >> num;
		i.y.imag(num);
	}

	mas_obj[*count] = tmp;
	*count += 1;
}

//Rewrites the vertex(for std::complex<>)
template<typename S>
void rewrite_vertex(Broken<std::complex<S>>* mas_obj, int count)
{
	int n = 0, m = 0;
	S num = 0;

	do
	{
		std::cout << "\nChoose in which polyline the vertex should be rewritten?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count || n != (int)n);

	do
	{
		std::cout << "\nChoose which vertex you want to overwrite?(counting from zero): ";
		std::cin >> m;
	} while (m < 0 || m >= mas_obj[n].get_n() || m != (int)m);

	std::cout << "\nEnter x: ";

	std::cout << "\n\tEnter real: ";
	std::cin >> num;
	mas_obj[n][m].x.real(num);

	std::cout << "\tEnter imag: ";
	std::cin >> num;
	mas_obj[n][m].x.imag(num);


	std::cout << "\nEnter y: ";

	std::cout << "\n\tEnter real: ";
	std::cin >> num;
	mas_obj[n][m].y.real(num);

	std::cout << "\tEnter imag: ";
	std::cin >> num;
	mas_obj[n][m].y.imag(num);
}

//Adding a vertex to the beginning of a polyline (for std::complex<>)
template<typename S>
void ver_beg(Broken<std::complex<S>>* mas_obj, int count)
{
	int n = 0;
	Points<std::complex<S>> p;
	S num = 0;

	do
	{
		std::cout << "\nIn which polyline do I need to add a vertex to the beginning?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count || n != (int)n);

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

	do
	{
		std::cout << "\nIn which polyline do I need to add a vertex to the beginning?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count || n != (int)n);

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

	do
	{
		std::cout << "\nWhich polyline length do you want to calculate?(counting from zero): ";
		std::cin >> n;
	} while (n < 0 || n >= count || n != (int)n);

	std::cout << "The length of the polyline is " << mas_obj[n].len_broken_complex() << "\n\n";
	system("pause");
}

//Additional task to create a triangle (only std::complex)
template<typename S>
void add_task(Broken<std::complex<S>>* mas_obj, int* count)
{
	Points<std::complex<S>> p;
	double phi = 0, a = 0, abs = 0;

	do
	{
		std::cout << "\nSet the LEIGHT of the side: ";
		std::cin >> a;
	} while (a <= 0);

	do
	{
		std::cout << "\nSet the ANGLE at the top: ";
		std::cin >> phi;
	} while (phi < 0 || phi > 180);

	mas_obj[*count] += p;
	phi = PI / 2 - ((phi * PI) / 360);

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
	return !(p1 != p2);
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
	return !(p1 != p2);
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
