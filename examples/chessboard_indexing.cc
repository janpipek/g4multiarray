#include "../multi_array.hh"
#include <iostream>

using namespace std;

int main()
{
    cout << "Generate a chessboard with numbers 1-64:" << endl;
    auto chessboard = linspace(1, 64, 64).Resize(8, 8);
    cout << chessboard << endl << endl;

    cout << "4th row: chessboard.slice<0>(3)" << endl;
    cout << chessboard.slice<0>(3) << endl << endl;

    cout << "Last two columns: chessboard.slice<1>(6, 8)" << endl;
    cout << chessboard.slice<1>(6, 8) << endl << endl;

    cout << "Middle square: chessboard.slice<1>(3, 5).slice<0>(3, 5)" << endl;
    cout << chessboard.slice<1>(3, 5).slice<0>(3, 5) << endl << endl;

    cout << "4 edge squares: chessboard.slice<1>(0, 8, 7).slice<0>(0, 8, 7)" << endl;
    cout << chessboard.slice<1>(0, 8, 7).slice<0>(0, 8, 7) << endl << endl;

    cout << "Each second row and column: chessboard.slice<1>(1, 8, 2).slice<0>(0, 8, 2)" << endl;
    cout << chessboard.slice<1>(1, 8, 2).slice<0>(0, 8, 2) << endl << endl;

    cout << "One element only (lower right): chessboard.slice<1>(7).slice<0>(7)" << endl;
    cout << chessboard.slice<1>(7).slice<0>(7) << endl << endl;

    cout << "The same element using operator(): chessboard(7, 7)" << endl;
    cout << chessboard(7, 7) << endl << endl;

    cout << "Last column using operator(): chessboard(_, 7)" << endl;
    cout << chessboard(_, 7) << endl << endl;

    cout << "Last column using negative index: chessboard(_, -1)" << endl;
	cout << chessboard(_, -1) << endl << endl;

    cout << "Select all even columns using operator(): chessboard(_, _(0, 7, 2))" << endl;
    cout << chessboard(_, _(0, 7, 2)) << endl << endl;

    cout << "Select middle square: chessboard(_(3, 5), _(3, 5))" << endl;
    cout << chessboard(_(3, 5), _(3, 5)) << endl << endl;

    chessboard(_(1, 7), _(1, 7)) = 0;
    cout << "Set all non-edge elements to 0: chessboard(_(1, 7), _(1, 7)) = 0" << endl;
    cout << chessboard << endl;

	return 0;
}
