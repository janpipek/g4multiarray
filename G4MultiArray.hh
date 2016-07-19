#ifndef G4MultiArray_h
#define G4MultiArray_h

#include <vector>
#include <valarray>
#include <array>
#include <functional>
// #include <stdalg>

template<typename T, size_t N> class G4MultiArray;
template<typename T, size_t N, size_t M> class G4MultiArrayView;


// TODO: use std::stride & std::gstride

// TODO: Include copy-on-write function (thread-safe?)
template<typename T, size_t N> class G4MultiArray final
{
public:
    explicit G4MultiArray(std::array<size_t, N> shape, std::valarray<T>& data) :
        fShape(shape), fData(data)
    {
        fSize = 1;
        for (size_t element : shape) 
        {
            fSize *= element;
        }
        if (fSize != data.size())
        {
            // TODO: raise something
        }
    }

    size_t Size() const { return fSize; }

    const std::array<size_t, N>& Shape() const { return fShape; }

    // const std::

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

    template<size_t M> G4MultiArray& operator=(const G4MultiArrayView<T, M+N, N>& view)
    {

    };

    T& At(std::array<size_t, N> index)
    {

    }

    const T& At(std::array<size_t, N> index) const
    {

    }

    // template<



    //TODO: Resizetemplate<> const G4MultiArray
    

    // Arithmetic operations

private:
    size_t fSize;

    std::valarray<size_t> fStrides;

    std::array<size_t, N> fShape;

    // TODO: Make this a shared_ptr???
    std::valarray<T> fData;
};



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


