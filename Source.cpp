#include<iostream>
#include<vector>
#include <complex>

#include<math.h>
#include <windows.h>
#include<conio.h>


const double PI = 3.1415;

//overloaded complex number output operator
template<typename S>
std::ostream& operator <<(std::ostream& os, const std::complex<S>& c)
{
	return os << "(" << c.real() << ", " << c.imag() << ")";
}

/*----declaration and definition of the structure----*/

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
std::ostream& operator <<(std::ostream& os, const Points<T>& p)
{
	return os << "(" << p.x << ", " << p.y << ")";
}

template<typename T>
double Points<T>::lambda = 0.000001;

/*----text visible to the user----*/

void list_of_types()
{
	std::cout << "Choose the type of data you need:\n\n";
	std::cout << "1. int\n";
	std::cout << "2. float\n";
	std::cout << "3. double\n";
	std::cout << "4. std::complex<float>\n";
	std::cout << "5. std::complex<double>\n";
	std::cout << "\nEnter the number: ";
}

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

const char* list_of_actions(const int n)
{
	if (n == 1) return "\nWhich polyline vertices do you want to get?(counting from zero): ";
	if (n == 2) return "\nWhich polyline length do you want to calculate?(counting from zero): ";
	if (n == 3) return "\nIn which polyline do I need to add a vertex to the beginning?(counting from zero): ";
	if (n == 4) return "\nIn which polyline do I need to add a vertex to the end?(counting from zero): ";
	if (n == 5) return "\nSelect the FIRST vertex (counting from zero): ";
	if (n == 6) return "\nSelect the SECOND vertex (counting from zero): ";
	if (n == 7) return "\nChoose from which polyline to display the vertex?(counting from zero): ";
	if (n == 8) return "\nChoose in which polyline the vertex should be rewritten?(counting from zero): ";
	if (n == 9) return "\nWhich vertex to display?(counting from zero): ";
	if (n == 10) return "\nChoose which vertex you want to overwrite?(counting from zero): ";
	if (n == 11) return "How many vertices do you want to add now?: ";
}

/*----declaration and definition of the class----*/

//Broken class declaration
template<typename T>
class Broken
{
private:
	std::vector<Points<T>> data;
	int n;

public:
	template<typename V> friend std::ostream& operator <<(std::ostream&, const Broken<V>&);
	template<typename V> friend void operator +=(const Points<V>&, Broken<V>&);
	Broken<T>& operator =(const Broken<T>&) = default;
	Broken<T> operator +(const Broken<T>&) const;
	Broken(const Broken<T>&) = default;
	bool operator ==(const Broken<T>&);
	bool operator !=(const Broken<T>&);
	void operator +=(const Points<T>&);
	double len_broken_complex() const;
	Points<T>& operator [](int);
	double len_broken() const;
	~Broken() = default;
	Broken(const int);
	int get_n() const;
	auto begin();
	auto end();
	Broken();	
};

template<typename T>
std::ostream& operator <<(std::ostream& os, const Broken<T>& obj)
{
	int counter_vertex = 0;
	for (const auto& i: obj.data)
	{
		os << i;
		if (counter_vertex++ < obj.n - 1) os << " -> ";
	}
	return os;
}

//Operator for adding a vertex to the beginning
template<typename T>
void operator +=(const Points<T>& p, Broken<T>& obj)
{
	std::vector<Points<T>> tmp_data(obj.n + 1);
	int index = 0;

	for (const auto& i : obj.data) tmp_data[index++ + 1] = i;

	tmp_data[0] = p;
	obj.data = tmp_data;
	obj.n++;
}

template<typename T>
Broken<T> Broken<T>:: operator +(const Broken<T>& obj) const
{
	Broken<T> tmp_obj(n + obj.n);
	int index = 0;

	for (const auto& i : data) tmp_obj[index++] = i;
	for (const auto& i : obj.data) tmp_obj[index++] = i;

	return tmp_obj;
}

template<typename T>
bool Broken<T>::operator ==(const Broken<T>& obj)
{
	if (n != obj.n) throw "\nError! Polylines do not have the same number of vertices.\n\n";
	int index = 0;

	for (auto i: obj.data) if (i != data[index++]) return false;
	return true;
}

template<typename T>
bool Broken<T>::operator !=(const Broken<T>& obj) { return !(*this == obj); }

//Add vertex to end operator
template<typename T>
void Broken<T>:: operator +=(const Points<T>& p)
{
	data.push_back(p);
	n++;
}

template<typename T>
double Broken<T>::len_broken_complex() const
{
	double sum = 0;
	std::vector<Points<T>> tmp = data;

	for (int i = 0; i < n - 1; i++)
	{
		if (i >= 1 && tmp[i - 1] == tmp[i + 1]) continue;

		T z1, z2;
		z1 = tmp[i].x - tmp[i + 1].x;
		z2 = tmp[i].y - tmp[i + 1].y;
		sum += sqrt(pow(std::abs(z1), 2) + pow(std::abs(z2), 2));
	}

	return sum;
}

//Vertex Read/Write Operator
template<typename T>
Points<T>& Broken<T>:: operator [](const int i)
{
	if (i < n) return data[i];
	else throw "\nError! Attempt to access by case index.\n\n";
}

template<typename T>
double Broken<T>::len_broken() const
{
	double sum = 0;
	std::vector<Points<T>> tmp = data;

	for (int i = 0; i < n - 1; i++)
	{
		if (i >= 1 && tmp[i - 1] == tmp[i + 1]) continue;

		double x = 0, y = 0;
		x = pow(tmp[i].x - tmp[i + 1].x, 2);
		y = pow(tmp[i].y - tmp[i + 1].y, 2);
		sum += sqrt(x + y);
	}

	return sum;
}

template<typename T>
Broken<T>::Broken(const int n) : n(n) { data.resize(n); }

template<typename T>
int Broken<T>::get_n() const { return n; }

template<typename T>
auto Broken<T>::begin() { return data.begin(); }

template<typename T>
auto Broken<T>::end() { return data.end(); }

//Explicit default constructor
template<typename T>
Broken<T>::Broken() :n(0) {}

/*----functions with reusable parts----*/

template<typename T>
Points<T> set_point(Points<T>& p)
{
	std::cout << "\nEnter x: ";
	std::cin >> p.x;
	std::cout << "Enter y: ";
	std::cin >> p.y;

	return p;
}

template<typename S>
Points<std::complex<S>> set_point_complex(Points<std::complex<S>>& p, S& num)
{
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

	return p;
}

int check_value_num_of_vertices(const int& n)
{
	float num = 0;

	do
	{
		std::cout << list_of_actions(n);
		std::cin >> num;
	} while (num <= 0 || num != (int)num);

	return num;
}

int check_index_value(const int count, const int& item)
{
	float index = 0;

	do
	{
		std::cout << list_of_actions(item);
		std::cin >> index;
	} while (index < 0 || index >= count || index != (int)index);

	return index;
}

/*----intermediary functions for working with class object(for all types of vertexes)----*/

template<typename T>
void create_polyline(Broken<T>* mas_obj, int& count)
{
	int num = check_value_num_of_vertices(11);
	Broken<T> tmp(num);
	Points<T> p;

	for (auto& i: tmp) i = set_point(p);
	mas_obj[count++] = tmp;
}

template<typename T>
void output_vertices_of_broken(Broken<T>* mas_obj, int count)
{
	int index_obj = check_index_value(count, 1);
	std::cout << "\nVertices: " << mas_obj[index_obj] << "\n\n";
	system("pause");
}

template<typename T>
void lenght_polyline(Broken<T>* mas_obj, int count)
{
	int index_obj = check_index_value(count, 2);
	std::cout << "The length of the polyline is " << mas_obj[index_obj].len_broken() << "\n\n";
	system("pause");
}

template<typename T>
void add_vertex_begin(Broken<T>* mas_obj, int count)
{
	Points<T> p;
	int index_obj = check_index_value(count, 3);
	set_point(p) += mas_obj[index_obj];
}

template<typename T>
void add_vertex_end(Broken<T>* mas_obj, int count)
{
	Points<T> p;
	int index_obj = check_index_value(count, 4);
	mas_obj[index_obj] += set_point(p);
}

template<typename T>
void concatenation_of_polylines(Broken<T>* mas_obj, int& count)
{
	int index_obj_1 = check_index_value(count, 5);
	int index_obj_2 = check_index_value(count, 6);
	mas_obj[count++] = mas_obj[index_obj_1] + mas_obj[index_obj_2];
}

template<typename T>
void create_triangle_from_polyline(Broken<T>* mas_obj, int& count)
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


	mas_obj[count] += p;

	phi = PI/2 - ((phi * PI) / 360);
	p.x = p.x + a * cos(phi);
	p.y = p.y + a * sin(phi);
	mas_obj[count] += p;

	p.x = 2 * p.x;
	p.y = 0;
	mas_obj[count] += p;

	p.x = 0;
	mas_obj[count++] += p;
}

template<typename T>
void rewrite_vertex(Broken<T>* mas_obj, int count)
{
	Points<T> p;
	int index_vertex = 0;
	int index_obj = check_index_value(count, 8);

	do { index_vertex = check_value_num_of_vertices(10); } while (index_vertex >= mas_obj[index_obj].get_n());

	mas_obj[index_obj][index_vertex] = set_point(p);
}

template<typename T>
void input_vertex(Broken<T>* mas_obj, int count)
{
	int index_obj = check_index_value(count, 7);
	int index_vertex = 0;

	do { index_vertex = check_value_num_of_vertices(9); } while (index_vertex >= mas_obj[index_obj].get_n());

	std::cout << "\nVertex: " << mas_obj[index_obj][index_vertex] << "\n\n";
	system("pause");
}

template<typename T>
void compare_broken(Broken<T>* mas_obj, int count)
{
	int index_obj_1 = check_index_value(count, 5);
	int index_obj_2 = 0;

	do { index_obj_2 = check_index_value(count, 6); } while (index_obj_1 == index_obj_2);

	if (mas_obj[index_obj_1] == mas_obj[index_obj_2]) std::cout << "Broken lines are equal\n\n";
	else std::cout << "Broken lines are not equal\n\n";
	system("pause");
}

template<typename T>
void change_precision()
{
	Points<T> p;
	double num = 0;

	std::cout << "Now the accuracy is " << p._lambda();
	std::cout << "\nEnter new precision: ";
	std::cin >> num;

	p._lambda() = num;
}

/*----intermediary functions for working with class object(only for complex vertices)----*/

template<typename S>
void create_polyline(Broken<std::complex<S>>* mas_obj, int& count)
{
	int n = check_value_num_of_vertices(11);
	S num = 0;

	Broken<std::complex<S>> tmp(n);
	Points<std::complex<S>> p;

	for (auto& i : tmp) i = set_point_complex(p, num);
	mas_obj[count++] = tmp;
}

template<typename S>
void rewrite_vertex(Broken<std::complex<S>>* mas_obj, int count)
{
	S num = 0;
	int index_vertex = 0;
	Points<std::complex<S>> p;
	int index_obj = check_index_value(count, 8);

	do { index_vertex = check_value_num_of_vertices(10); } while (index_vertex >= mas_obj[index_obj].get_n());

	mas_obj[index_obj][index_vertex] = set_point_complex(p, num);
}

template<typename S>
void add_vertex_begin(Broken<std::complex<S>>* mas_obj, int count)
{
	S num = 0;
	Points<std::complex<S>> p;
	int index_obj = check_index_value(count, 3);
	set_point_complex(p, num) += mas_obj[index_obj];
}

template<typename S>
void add_vertex_end(Broken<std::complex<S>>* mas_obj, int count)
{
	S num = 0;
	Points<std::complex<S>> p;
	int index_obj = check_index_value(count, 4);
	mas_obj[index_obj] += set_point_complex(p, num);
}

template<typename S>
void lenght_polyline(Broken<std::complex<S>>* mas_obj, int count)
{
	int index_obj = check_index_value(count, 2);
	std::cout << "The length of the polyline is " << mas_obj[index_obj].len_broken_complex() << "\n\n";
	system("pause");
}

template<typename S>
void create_triangle_from_polyline(Broken<std::complex<S>>* mas_obj, int& count)
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

	mas_obj[count] += p;
	phi = PI / 2 - ((phi * PI) / 360);

	abs = a * cos(phi);
	abs *= abs;
	p.x.real(sqrt(abs / 2)); p.x.imag(sqrt(abs / 2));
	abs = a * sin(phi);
	abs *= abs;
	p.y.real(sqrt(abs / 2)); p.y.imag(sqrt(abs / 2));
	mas_obj[count] += p;

	p.y.real(0); p.y.imag(0);
	p.x.real(p.x.real() * 2);
	p.x.imag(p.x.imag() * 2);
	mas_obj[count] += p;

	p.x.real(0); p.x.imag(0);
	mas_obj[count++] += p;
}

/*----vertex comparison operators----*/

template<typename T>
bool operator!=(Points<T>& p1, Points<T>& p2)
{
	return !(p1 == p2);
}

template<typename T>
bool operator==(Points<T>& p1, Points<T>& p2)
{
	if (fabs(p1.x - p2.x) > p1._lambda()) return false;
	if (fabs(p1.y - p2.y) > p1._lambda()) return false;
	return true;
}

template<typename S>
bool operator!=(Points<std::complex<S>>& p1, Points<std::complex<S>>& p2)
{
	return !(p1 == p2);
}

template<typename S>
bool operator==(Points<std::complex<S>>& p1, Points<std::complex<S>>& p2)
{
	if (fabs(p1.x.real() - p2.x.real()) > p1._lambda()) return false;
	if (fabs(p1.x.imag() - p2.x.imag()) > p1._lambda()) return false;
	if (fabs(p1.y.real() - p2.y.real()) > p1._lambda()) return false;
	if (fabs(p1.y.imag() - p2.y.imag()) > p1._lambda()) return false;
	return true;
}

/*----program interface----*/

template<typename T>
void functions_of_program(Broken<T>* mas_obj)
{
	int num_of_objects = 0, item_num = 0;

	while (1)
	{
		try {
			system("CLS");
			menu();
			std::cin >> item_num;
			if (item_num == 1) create_polyline(mas_obj, num_of_objects);
			if (item_num == 2) rewrite_vertex(mas_obj, num_of_objects);
			if (item_num == 3) input_vertex(mas_obj, num_of_objects);
			if (item_num == 4) output_vertices_of_broken(mas_obj, num_of_objects);
			if (item_num == 5) lenght_polyline(mas_obj, num_of_objects);
			if (item_num == 6) add_vertex_begin(mas_obj, num_of_objects);
			if (item_num == 7) add_vertex_end(mas_obj, num_of_objects);
			if (item_num == 8) concatenation_of_polylines(mas_obj, num_of_objects);
			if (item_num == 9) create_triangle_from_polyline(mas_obj, num_of_objects);
			if (item_num == 10) compare_broken(mas_obj, num_of_objects);
			if (item_num == 11) change_precision<T>();
			if (item_num == 12) break;
		}
		catch (const char* ex)
		{
			std::cout << ex;
			system("pause");
			system("CLS");
		}
	}
}

int main() 
{
	int type_num = 0;
	list_of_types();
	std::cin >> type_num;

	if (type_num == 1)
	{
		Broken<int> mas_obj[20];
		functions_of_program(mas_obj);
	}

	else if (type_num == 2)
	{
		Broken<float> mas_obj[20];
		functions_of_program(mas_obj);
	}

	else if (type_num == 3)
	{
		Broken<double> mas_obj[20];
		functions_of_program(mas_obj);
	}

	else if (type_num == 4)
	{
		Broken<std::complex<float>> mas_obj[20];
		functions_of_program(mas_obj);
	}

	else if (type_num == 5)
	{
		Broken<std::complex<double>> mas_obj[20];
		functions_of_program(mas_obj);
	}

	else std::cout << "\n\nPlease enter the correct number next time.\n\n";
}
