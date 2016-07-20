#include "G4MultiArray.hh"

#include <valarray>
#include <iostream>

using namespace std;

int main()
{
	valarray<double> x = {-1, 2, 3, -4, 5, 6};
	G4MultiArray<double, 2> arr{{2, 3}, x};
	arr = exp(abs(arr));
	cout << arr.Size() << endl;

	cout << arr.At({1, 1}) << endl;
	arr.At({1, 1}) = 3.7;
	cout << arr.At({1, 1}) << endl;

	cout << arr.Flatten()[3] << endl;
	cout << arr.As<int>().At({1, 1}) << endl;


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

	

	// std::slice zm {0,0,0};
}