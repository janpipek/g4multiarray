#ifndef  G4MultiArrayTools_h
#define G4MultiArrayTools_h

#include "G4MultiArray.hh"

template<typename T, size_t N> G4MultiArray<T, N> abs(const G4MultiArray<T, N>& arr)
{
    typename G4MultiArray<T, N>::data_type&& newData = abs(arr.Flatten());
    return{ arr.Shape(), newData };
}

template<typename T, size_t N> G4MultiArray<T, N> exp(const G4MultiArray<T, N>& arr)
{
    typename G4MultiArray<T, N>::data_type&& newData = exp(arr.Flatten());
    return{ arr.Shape(), newData };
}

template<typename T, size_t N> G4MultiArray<T, N> log(const G4MultiArray<T, N>& arr)
{
    typename G4MultiArray<T, N>::data_type&& newData = log(arr.Flatten());
    return{ arr.Shape(), newData };
}

template<typename T, size_t N> G4MultiArray<T, N> log10(const G4MultiArray<T, N>& arr)
{
    typename G4MultiArray<T, N>::data_type&& newData = log10(arr.Flatten());
    return{ arr.Shape(), newData };
}

template<typename T, size_t N> G4MultiArray<T, N> pow(const G4MultiArray<T, N>& arr, const T& value)
{
    typename G4MultiArray<T, N>::data_type&& newData = pow(arr.Flatten(), value);
    return{ arr.Shape(), newData };
}

template<typename T, size_t N> G4MultiArray<T, N> sqrt(const G4MultiArray<T, N>& arr)
{
    typename G4MultiArray<T, N>::data_type&& newData = sqrt(arr.Flatten());
    return{ arr.Shape(), newData };
}

template<typename T, size_t N> G4MultiArray<T, N> sin(const G4MultiArray<T, N>& arr)
{
    typename G4MultiArray<T, N>::data_type&& newData = sin(arr.Flatten());
    return{ arr.Shape(), newData };
}

template<typename T, size_t N> G4MultiArray<T, N> cos(const G4MultiArray<T, N>& arr)
{
    typename G4MultiArray<T, N>::data_type&& newData = cos(arr.Flatten());
    return{ arr.Shape(), newData };
}

template<typename T, size_t N> G4MultiArray<T, N> tan(const G4MultiArray<T, N>& arr)
{
    typename G4MultiArray<T, N>::data_type&& newData = tan(arr.Flatten());
    return{ arr.Shape(), newData };
}

template<typename T, size_t N> G4MultiArray<T, N> asin(const G4MultiArray<T, N>& arr)
{
    typename G4MultiArray<T, N>::data_type&& newData = asin(arr.Flatten());
    return{ arr.Shape(), newData };
}

template<typename T, size_t N> G4MultiArray<T, N> acos(const G4MultiArray<T, N>& arr)
{
    typename G4MultiArray<T, N>::data_type&& newData = acos(arr.Flatten());
    return{ arr.Shape(), newData };
}

template<typename T, size_t N> G4MultiArray<T, N> atan(const G4MultiArray<T, N>& arr)
{
    typename G4MultiArray<T, N>::data_type&& newData = atan(arr.Flatten());
    return{ arr.Shape(), newData };
}

template<typename T, size_t N> G4MultiArray<T, N> atan2(const G4MultiArray<T, N>& arr1, const G4MultiArray<T, N>& arr2)
{
    typename G4MultiArray<T, N>::data_type&& newData = atan2(arr1.Flatten(), arr2.Flatten());
    return{ arr1.Shape(), newData };
}

template<typename T, size_t N> G4MultiArray<T, N> sinh(const G4MultiArray<T, N>& arr)
{
    typename G4MultiArray<T, N>::data_type&& newData = sinh(arr.Flatten());
    return{ arr.Shape(), newData };
}

template<typename T, size_t N> G4MultiArray<T, N> cosh(const G4MultiArray<T, N>& arr)
{
    typename G4MultiArray<T, N>::data_type&& newData = cosh(arr.Flatten());
    return{ arr.Shape(), newData };
}

template<typename T, size_t N> G4MultiArray<T, N> tanh(const G4MultiArray<T, N>& arr)
{
    typename G4MultiArray<T, N>::data_type&& newData = tanh(arr.Flatten());
    return{ arr.Shape(), newData };
}

template<typename T, size_t N, size_t M> T dot(const g4_multi_array::G4MultiArrayView<T, N, 1>& arr1, const g4_multi_array::G4MultiArrayView<T, M, 1>& arr2)
{
    return (arr1.Flatten() * arr2.Flatten()).sum();
}

// using inner = dot;

template<typename T, size_t N1, size_t M1, size_t N2, size_t M2> G4MultiArray<T, M1 + M2> outer(const g4_multi_array::G4MultiArrayView<T, N1, M1>& arr1, const g4_multi_array::G4MultiArrayView<T, N2, M2>& arr2)
{
    std::array<size_t, M1 + M2> shape;
    for (int i = 0; i < M1; i++)
    {
        shape[i] = arr1.Shape()[0];
    }
    for (int i = 0; i < M2; i++)
    {
        shape[i+M1] = arr2.Shape()[0];   
    }

    std::valarray<T> newData(arr1.Size() * arr2.Size());
    std::valarray<T> data1 = arr1.Flatten();
    std::valarray<T> data2 = arr2.Flatten();

    for (int i = 0; i < data1.size(); i++)
    {
        for (int j = 0; j < data2.size(); j++)
        {
            newData[i * data2.size() + j] = data1[i] * data2[j];
        }
    }

    return G4MultiArray<T, M1 + M2>(shape, std::move(newData));
}

template<typename T, size_t N, size_t M> G4MultiArray<T, N+M> outer(const G4MultiArray<T, N>& arr1, const G4MultiArray<T, M>& arr2)
{
    std::array<size_t, N + M> shape;
    for (int i = 0; i < N; i++)
    {
        shape[i] = arr1.Shape()[0];
    }
    for (int i = 0; i < M; i++)
    {
        shape[i+N] = arr2.Shape()[0];   
    }

    std::valarray<T> newData(arr1.Size() * arr2.Size());
    std::valarray<T> data1 = arr1.Flatten();
    std::valarray<T> data2 = arr2.Flatten();

    for (int i = 0; i < data1.size(); i++)
    {
        for (int j = 0; j < data2.size(); j++)
        {
            newData[i * data2.size() + j] = data1[i] * data2[j];
        }
    }

    return { shape, std::move(newData) };
}

// As numpy.linspace
template<typename T> G4MultiArray<T, 1> linspace(const T& start, const T& stop, size_t num, bool endpoint = true)
{
    std::valarray<T> data(num);
    T step = (stop - start) / (endpoint ? (num - 1) : num);
    for (size_t i = 0; i < num; i++)
    {
        data[i] = start + i * step;
    }
    if (endpoint) { data[num-1] = stop; } // To make the endpoint precise
    return { { num }, std::move(data) };
}


#endif // ! G4MultiArrayTools_h

