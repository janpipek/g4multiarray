#include "../multi_array.hh"
#include <iostream>

using namespace std;

bool isEven(const int& n)
{
    return(n % 2 == 0);
}

int main()
{
    std::function<bool(int)> lambda = [](int n) -> bool { return n == 2; };
    auto vectorizedLambda = vectorize(lambda);
    auto vectorizedEven = vectorize(isEven);

    auto l = arange(20);
    cout << vectorizedEven(l) << endl;

    return 0;
}
