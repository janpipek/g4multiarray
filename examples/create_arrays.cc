#include "../multi_array.hh"
#include <iostream>

using namespace std;

int main()
{
    cout << "1D array with 4 zeros: multi_array<double, 1> {{4}}" << endl;
    multi_array<double, 1> array1 {{4}};
    cout << array1 << endl << endl;

    cout << "The same empty 1D array: zeros<double>(4)" << endl;
    auto array2 = zeros<double>(4);
    cout << array2 << endl << endl;

    cout << "2x2 matrix with ones array: multi_array<double, 1> {{2, 2}, 1}" << endl;
    multi_array<double, 2> array3 {{2, 2}, 1};
    cout << array3 << endl << endl;

    cout << "The same matrix: ones<double>(2, 2)" << endl;
    auto array4 = ones<double>(2, 2);
    cout << array4 << endl << endl;

    cout << "Numbers from 1 to 10: linspace<int>(1, 10, 10)" << endl;
    auto array5 = linspace<int>(1, 10, 10);
    cout << array5 << endl << endl;

    cout << "Numbers from 0 to 9: arange(10)" << endl;
    auto array6 = arange(10);
    cout << array6 << endl << endl;

    cout << "Powers of ten: geomspace(1, 1000000, 7)" << endl;
    auto array7 = geomspace(1, 1000000, 7);
    cout << array7 << endl << endl;


    return 0;
}
