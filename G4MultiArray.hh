#ifndef G4MultiArray_h
#define G4MultiArray_h

#include <vector>
#include <valarray>
#include <array>
#include <functional>


// Forward-declare the types
template<typename T, size_t N> class G4MultiArray;
template<typename T, size_t N, size_t M> class G4MultiArrayView;


// TODO: use std::stride & std::gstride

// TODO: Include copy-on-write function (thread-safe?)
template<typename T, size_t N> class G4MultiArray final
{
public:
    constexpr static size_t Ndim = N;

    G4MultiArray(std::array<size_t, N> shape, std::valarray<T>& data) :
        fShape(shape), fData(data)
    {
        update_shape();   
    }

    G4MultiArray(std::array<size_t, N> shape, std::valarray<T>&& data) :
        fShape(shape), fData(data)
    {
        update_shape();
    }    

    G4MultiArray& operator=(G4MultiArray&& other)
    {
        std::swap(fData, other.fData);
        std::swap(fStrides, other.fStrides);
        std::swap(fShape, other.fShape);
        std::swap(fSize, other.fSize);
    }

    /* G4MultiArray& operator=(G4MultiArray& other)
    {
        fData = other.fData;
        fStrides = other.fStrides;
        fShape = other.fShape;
        fSize = other.fSize;
    }*/   

    size_t Size() const { return fSize; }

    const std::array<size_t, N>& Shape() const { return fShape; }

    const std::valarray<T>& Flatten() const { return fData; }


    /*void ReshapeInline(std::array<)
    {

    }*/

    template<typename U> G4MultiArray<U, N> Apply(std::function<U(T)> f) const
    {
        return G4MultiArray(fShape, fData.apply(f));
    }

    void ApplyInline(std::function<T(T)> f)
    {
        fData = fData.apply(f);
    }

    template<size_t M> G4MultiArray<T, M> Reshape(std::array<size_t, N> newShape) const
    {
        // TODO: Check sum
        return G4MultiArray<T, M>(newShape, fData);
    }

    /* template<size_t M> G4MultiArray& operator=(const G4MultiArrayView<T, M+N, N>& view)
    {

    };*/

    // TODO: Index with masked array



    template<typename AccessorType = std::array<size_t, N>> const T& At(AccessorType index) const
    {
        return fData[make_index(index)];
    }

    template<typename AccessorType = std::array<size_t, N>> T& At(AccessorType index)
    {
        return fData[make_index(index)];
    } 

    /*T& At(std::initializer_list<T> init)
    {
        return fData[make_index(std::arr)]
    }*/

    /* template<> typename std::enable_if<(N==1), T&>::type At(size_t i)
    {
        return fData[i];
    }*/


    //TODO: Resizetemplate<> const G4MultiArray
    

    // Arithmetic operations
    // friend G4MultiArray<T, N> abs (const G4MultiArray<T, N>&);

private:
    void update_shape()
    {
        fSize = 1;
        fStrides[N - 1] = 1;
        for (size_t i = 0; i < N; i++) 
        {
            fSize *= fShape[i];
            if (i > 0)
            {
                fStrides[N - 1 - i] = fStrides[N - i] * fShape[N - 1 - i];
            }
        }
        if (fSize != fData.size())
        {
            throw std::runtime_error("Invalid shape. Dimensions must match.");
        }
    }

    // TODO: Implement check/not chcek
    size_t make_index(const std::array<size_t, N>& arr) const
    {
        size_t index = 0;
        for (int i = 0; i < N; i++)
        {
            if (arr[i] >= fShape[i])
            {
                throw std::runtime_error("Index overflow");
            }
            index += fStrides[i] * arr[i];
        }
        return index;
    }

    size_t fSize;

    std::array<size_t, N> fStrides;

    std::array<size_t, N> fShape;

    // TODO: Make this a shared_ptr???
    std::valarray<T> fData;
};

template<typename T, size_t N> G4MultiArray<T, N> abs (const G4MultiArray<T, N>& arr)
{
    std::valarray<T>&& newData = abs(arr.Flatten());
    return { arr.Shape(), newData };
}

template<typename T, size_t N> G4MultiArray<T, N> exp (const G4MultiArray<T, N>& arr)
{
    std::valarray<T>&& newData = exp(arr.Flatten());
    return { arr.Shape(), newData };
}

/*template<typename T, size_t N, size_t M> class G4MultiArrayView
{
public:
    G4MultiArrayView();

    G4MultiArrayView<T, N, M - 1>& const At(size_t i);

    const G4MultiArrayView<T, N, M - 1>& const At(size_t i);

    G4MultiArray<M> Copy();

private:
    // 4MultiArray()
};

*/

#endif


