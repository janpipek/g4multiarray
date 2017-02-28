#include "multi_array.hh"

#include <valarray>
#include <iostream>

using namespace std;

int main()
{
	// Basic array
	auto a = linspace<double>(1, 5, 5, true);   // -> {[1, 2, 3, 4, 5]}
	cout << a << endl;

    cout << (a + a) << endl;
    cout << (-a) << endl;

	// Operations
	cout << a * 2 << endl;                      // -> {[2, 4, 6, 8, 10]}  

	cout << pow(2, a) << endl;                  // -> {[2, 4, 8, 16, 32]}
	
	multi_array<double, 2> x({3,3});

	cout << multi_array<double, 2>({3, 3}, 1) << endl;

	x = 79;

	//x[{4, 3}] = 4.1;

	x[2][2] = 4.22;

    auto z = x[2];

	z = 22;

	z[1] = 6.7;

    x[1] *= 2;

    cout << x << endl;

	try
	{
		z[7] = 4;
	}
	catch(std::runtime_error&)
	{
		cout << "z[7] threw an error (correctly)." << endl;
	}

	// z = x[1];  -- not working yet

	cout << x << endl;

	cout << z << endl;

	// return 0;

	

	x *= 4;



	z *= 17;

	cout << x << endl;

	cout << (4.7 - z / 2) << endl;

	// cout << x[4][3] << endl;

	// x.Write(std::cout);

	cout << endl;

	// x[4].Write(std::cout);

	// cout << x[{4, 6}];

	multi_array<double, 2> y{x};

	y.ReadOnly().As<int>();

	cout << y.Copy() << endl;

	cout << y.As<int>() << endl;

	cout << pow(abs(exp(sin(y))), 5) << endl;

	cout << atan2(y, y) << endl;

	return 0;
}

int main2()
{
	return 0;
	/*
	valarray<double> x = {-1, 2, 3, -4, 5, 6};
	G4MultiArray<double, 2> arr{{2, 3}, x};
	arr = exp(abs(arr));
	cout << arr.Size() << endl;

	cout << arr[{1, 1}] << endl;
	arr[{1, 1}] = 3.7;
	cout << arr[{1, 1}] << endl;

	cout << arr[1].Copy().Flatten()[1] << endl;
	cout << arr.As<int>()[{1, 1}] << endl;


	cout << "----" << endl;
	arr = {{2, 3}, {1, 2, 3, 4, 5, 6}};

	cout << arr[{1, 1}] << endl;
	cout << arr[1][1] << endl;
	cout << arr[{0, 2}] << endl;
	cout << arr[0][2] << endl;

	arr = 0;
	cout << arr[{1, 1}] << endl;

	arr[1][1] = 7;
	cout << arr[1][1] << endl;

	cout << arr[1] << endl;

	// arr[1] = 0;

	cout << endl;

	cout << arr << endl;

	auto new_arr = arr.Reshape<3>({1, 2, 3});
	cout << new_arr << endl;

	
	auto new_arr2 = arr.Reshape<4>({1, 2, 1, 3});
	cout << new_arr2 << endl;

	G4MultiArray<double, 4> new_arr3 {{2, 2, 2, 2}};
	cout << new_arr3 << endl;

	new_arr3[0][0] = G4MultiArray<double, 2>({2, 2}, 4);
	cout << new_arr3 << endl;

	new_arr3[1][1] = G4MultiArray<double, 3>({2, 2, 2}, 5)[1];
	cout << new_arr3 << endl;

	G4MultiArray<double, 2> new_arr4 {{2, 2}};
	new_arr4 = new_arr3[0][0];
	cout << new_arr4 << endl;

	vector<vector<double>> k { {1 , 2}, {1, 4}};
	G4MultiArray<double, 2> new_arr5(k);
	cout << new_arr5 << endl;

	G4MultiArray<double, 2> new_arr6 {{{1 , 2}, {1, 4}}};
	cout << new_arr6 << endl;

	//auto arr7 = make_multiarray<double, 2>({{1, 2}});

	G4MultiArray<double, 2> arr8 {{
		{ 1,  2,  3,  4,  5,  6,  7,  8,  9, 10},
		{ 2,  4,  6,  8, 10, 12, 14, 16, 18, 20}
	}};
	cout << arr8 << endl;

	arr8 = {{
		{ 1,  2,  3,  4,  5,  6,  7,  8},
		{ 2,  4,  6,  8, 10, 12, 14, 16}
	}};
	cout << arr8 * 3 << endl;

	cout << (3.14 * tanh(arr8) - 58) << endl;

	cout << dot(arr8[0], arr8[1]) << endl;

	auto numbers = linspace(1, 10, 10);

	cout << outer(sqrt(numbers.As<double>()), numbers.As<double>()) << endl;

	cout << tanh(G4MultiArray<double, 1>(arr8[0])) << endl;*/
}
