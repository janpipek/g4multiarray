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
	return{ arr.Shape(), newData };
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
#endif // ! G4MultiArrayTools_h

