#include "../multi_array.hh"
#include <iostream>

using namespace std;

bool isEven(const int& n)
{
    return(n % 2 == 0);
}

bool isPrime(int n)
{
    if (n < 2) { return false; }
    for (int i = 2; i < n; i++)
    {
        if (n % i == 0) { return false; }
    }
    return true;
}

int main()
{
    std::function<bool(int)> lambda = [](int n) -> bool { return n == 2; };
    auto vectorizedLambda = vectorize(lambda);
    auto vectorizedEven = vectorize(isEven);
    auto vectorizedIsPrime = vectorize(isPrime);

    auto l = arange(20);
    cout << l << endl;

    cout << "Even numbers?" << endl;
    cout << vectorizedEven(l) << endl;

    cout << "Primes?" << endl;
    cout << vectorizedIsPrime(l) << endl;

    return 0;
}
