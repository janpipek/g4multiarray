#include "G4MultiArray.hh"

#include <valarray>

using namespace std;

int main()
{
	valarray<double> x = {1, 2, 3, 4, 5, 6};
	G4MultiArray<double, 2> arr{{2, 3}, x};
}