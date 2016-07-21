/*
    G4MultiArray class, representing N-dimensional data.

    Written by: Jan Pipek (jan.pipek@gmail.com)

    Goals:
    - C++11 "feel"
    - No virtual functions
    - API similarity to std::vector
    - API similarity to numpy's ndarray
    - Interoperability with Geant4's G4ThreeVector, G4RotationMatrix, ...

    Inspired by:
    - B.Stroustrup's Matrix ( http://www.stroustrup.com/Programming/Matrix/Matrix.h )

 */


#ifndef G4MultiArray_h
#define G4MultiArray_h

#include <vector>
#include <valarray>
#include <array>
#include <functional>
#include <ostream>


// Forward-declare the types
template<typename T, size_t N> class G4MultiArray;
template<typename T, size_t N, size_t M> class G4MultiArrayView;


template<typename T, size_t N, size_t M = N> class G4MultiArrayImpl
{
public:
    using item_type = G4MultiArrayView<T, N, M-1>;

    using nested_vector_type = std::vector<typename G4MultiArrayImpl<T, N, M-1>::nested_vector_type>;

    static item_type get_item(G4MultiArray<T, N>& arr, size_t i)
    {
        return item_type(arr, i);
    }

    static G4MultiArrayView<T, N, M-1> get_item(G4MultiArrayView<T, N, M>& arr, size_t i)
    {
        return G4MultiArrayView<T, N, M-1>(arr, i);
    } 

    static item_type const get_const_item(const G4MultiArray<T, N>& arr, size_t i)
    {
        return item_type(arr, i);
    }    

    static const G4MultiArrayView<T, N, M-1> get_const_item(const G4MultiArrayView<T, N, M>& arr, size_t i)
    {
        return G4MultiArrayView<T, N, M-1>(arr, i);
    } 

    static std::array<size_t, M> get_shape(const nested_vector_type& nested_vector)
    {
        std::array<size_t, M> result;
        result[0] = nested_vector.size();
        auto daughter = G4MultiArrayImpl<T, N, M-1>::get_shape(nested_vector[0]);
        for (int i = 1; i < M; i++)
        {
            result[i] = daughter[i - 1];
        }
        return result;
    }
};

template<typename T, size_t N> class G4MultiArrayImpl<T, N, 1>
{
public:
    using item_type = T&;

    using nested_vector_type = std::vector<T>;

    static item_type get_item(G4MultiArray<T, 1>& arr, size_t i)
    {
        return arr.fData[i];
    }    

    static item_type get_item(G4MultiArrayView<T, N, 1>& arr, size_t i)
    {
        auto index = arr.make_index({i});
        // std::cout << "i" << index << std::endl;
        return arr.fArray.fData[index];
    }   

    static const item_type get_const_item(const G4MultiArray<T, 1>& arr, size_t i)
    {
        return arr.fData[i];
    }    

    static const item_type get_const_item(const G4MultiArrayView<T, N, 1>& arr, size_t i)
    {
        return arr.fArray.fData[arr.make_index({i})];
    }  

    static const item_type get_const_item(const G4MultiArray<T, 1>&& arr, size_t i)
    {
        return arr.fData[i];
    }    

    static const item_type get_const_item(const G4MultiArrayView<T, N, 1>&& arr, size_t i)
    {
        return arr.fArray[arr.make_index({i})];
    }  

    static std::array<size_t, 1> get_shape(const nested_vector_type& nested_vector)
    {
        return { nested_vector.size() };
    }
};

// TODO: use std::stride & std::gstride

// TODO: Include copy-on-write function (thread-safe?)
template<typename T, size_t N> class G4MultiArray final
{
public:
    static_assert(N > 0, "Cannot have 0-dimensional arrays");

    using index_type = std::array<size_t, N>;

    using value_type = T;

    using data_type = std::valarray<T>;

    using item_type = typename G4MultiArrayImpl<T, N>::item_type;

    using view_type = G4MultiArrayView<T, N, N>;

    constexpr static size_t Ndim = N;   

    template<typename, size_t, size_t> friend class G4MultiArrayView;

    template<typename, size_t, size_t> friend class G4MultiArrayImpl;

    template<typename, size_t> friend std::ostream& operator << (std::ostream&, const G4MultiArray&);

    void Write(std::ostream& os) const
    {
        os << "G4MultiArray([";
        size_t i = 0;
        while (i < fSize)
        {
            if (i == 0)
            {
                for (size_t j = 0; j < N; j++)
                {
                    os << "[";
                }
            }
            else
            {
                for (size_t j = 0; j+1 < N; j++)
                {
                    if (i % fStrides[j] == 0)
                    {
                        for (size_t k = j; k+1 < N; k++)
                        {
                            os << "\n";    
                        }
                        os << "               ";
                        for (size_t k = 0; k < j; k++)
                        {
                            os << " ";
                        }
                        for (size_t k = j; k+1 < N; k++)
                        {
                            os << "[";    
                        }                        
                        break;
                    }
                }    
            }

            os << fData[i];
            i++;

            size_t j = 0;
            // size_t k = 0;
            for (; j+1 < N; j++)
            {
                if (i % fStrides[j] == 0)
                {
                    break;
                }
            }
            for (size_t k = j; k+1 < N; k++)
            {
                os << "]";
            }
            if (i != fSize)
            {
                os << ", ";
            }
        }
        os << "])";
    }

    G4MultiArray(const index_type& shape, const T& value) :
        fShape(shape),
        fData(value, get_product(shape)),
        fSize(get_product(shape)),
        fStrides(get_strides(shape))
    { }

    G4MultiArray(const index_type& shape) :
        fShape(shape),
        fData(get_product(shape)),
        fSize(get_product(shape)),
        fStrides(get_strides(shape))
    { }    

    G4MultiArray(const index_type& shape, const T* data) :
        fShape(shape),
        fData(data, get_product(shape)),
        fSize(get_product(shape)),
        fStrides(get_strides(shape))
    { }

    G4MultiArray(const index_type& shape, const data_type& data) :
        fShape(shape), fData(data)
    {
        update_shape();   
    }

    G4MultiArray(const typename G4MultiArrayImpl<T, N>::nested_vector_type& nested_vector)
    {
        fShape = G4MultiArrayImpl<T, N>::get_shape(nested_vector);
        fData = data_type(get_product(fShape));
        update_shape();

        for (size_t i = 0; i < fShape[0]; i++)
        {
            (*this)[i] = nested_vector[i];
        }
    }

    //template<size_t... Ms> G4MultiArray()

    G4MultiArray(const index_type& shape, data_type&& data) :
        fShape(shape), fData(data)
    {
        update_shape();
    }  

    G4MultiArray(const G4MultiArray& other) :
        fShape(other.fShape), fData(other.fData), fSize(other.fSize), fStrides(other.fStrides)
    {
    }  

    G4MultiArray& operator=(G4MultiArray&& other)
    {
        std::swap(fData, other.fData);
        std::swap(fStrides, other.fStrides);
        std::swap(fShape, other.fShape);
        std::swap(fSize, other.fSize);
        return *this;
    }

    template<size_t N2> G4MultiArray& operator=(const G4MultiArrayView<T, N2, N>& other)
    {
        G4MultiArrayView<T, N, N>(*this) = other;
        return *this;
    }

    G4MultiArray& operator=(const T& value)
    {
        fData = value;
        return *this;
    }

    size_t Size() const { return fSize; }

    const index_type& Shape() const { return fShape; }

    const data_type& Flatten() const { return fData; }

    G4MultiArray Copy() const
    {
        return G4MultiArray<T, N>(fShape, fData);
    }

    template<typename U> G4MultiArray<U, N> As() const
    {
        std::valarray<U> result;
        result.resize(fSize);
        for (size_t i = 0; i < fSize; i++)        
        {
            result[i] = U{fData[i]};
        }
        return G4MultiArray<U, N>(fShape, std::move(result));
    }

    G4MultiArray<T, N> Apply(std::function<T(T)> f) const
    {
        return G4MultiArray(fShape, fData.apply(f));
    }

    void ApplyInline(std::function<T(T)> f)
    {
        for (size_t i = 0; i < fSize; i++)
        {
            fData[i] = f(fData[i]);
        }
    }

    template<size_t M> G4MultiArray<T, M> Reshape(const std::array<size_t, M>& newShape) const
    {
        return G4MultiArray<T, M>(newShape, fData);
    }

    void ReshapeInline(const index_type& newShape)
    {
        if (get_product(newShape) != fSize)
        {
            throw std::runtime_error("Invalid shape. Dimensions must match.");
        }
        fShape = newShape;
        fStrides = get_strides(newShape);
    }

    T& operator[](const index_type& index)
    {
        return fData[make_index(index, false)];
    }

    const T& operator[](const index_type& index) const
    {
        return fData[make_index(index, false)];
    }   

    item_type operator[](size_t i)
    {
        return G4MultiArrayImpl<T, N>::get_item(*this, i);
    }

    item_type operator[](size_t i) const
    {
        return G4MultiArrayImpl<T, N>::get_const_item(*this, i);
    }   

    G4MultiArray& operator*= (const G4MultiArray& other) 
    {
        if (fShape != other.fShape)
        {
            throw std::runtime_error("Incompatible shapes for multiplication.");
        }
        fData *= other;
        return *this;        
    }

    G4MultiArray& operator*= (const T& other) 
    {
        fData *= other;   
        return *this;   
    }    

    template<typename U> G4MultiArray operator* (const U& other) const
    {
        auto result = Copy();
        result *= other;
        return result;
    }

    G4MultiArray& operator/= (const G4MultiArray& other) 
    {
        if (fShape != other.fShape)
        {
            throw std::runtime_error("Incompatible shapes for multiplication.");
        }
        fData /= other;
        return *this;        
    }

    G4MultiArray& operator/= (const T& other) 
    {
        fData /= other;   
        return *this;   
    }    

    template<typename U> G4MultiArray operator/ (const U& other) const
    {
        auto result = Copy();
        result /= other;
        return result;
    } 

    G4MultiArray& operator+= (const G4MultiArray& other) 
    {
        if (fShape != other.fShape)
        {
            throw std::runtime_error("Incompatible shapes for multiplication.");
        }
        fData += other;
        return *this;        
    }

    G4MultiArray& operator+= (const T& other) 
    {
        fData += other;   
        return *this;   
    }    

    template<typename U> G4MultiArray operator+ (const U& other) const
    {
        auto result = Copy();
        result += other;
        return result;
    }   

    G4MultiArray& operator-= (const G4MultiArray& other) 
    {
        if (fShape != other.fShape)
        {
            throw std::runtime_error("Incompatible shapes for multiplication.");
        }
        fData -= other;
        return *this;        
    }

    G4MultiArray& operator-= (const T& other) 
    {
        fData -= other;   
        return *this;   
    }    

    template<typename U> G4MultiArray operator- (const U& other) const
    {
        auto result = Copy();
        result -= other;
        return result;
    }       

private:
    static size_t get_product(const index_type& arr)
    {
        size_t total = 1;
        for (size_t i = 0; i < N; i++) 
        {
            total *= arr[i];
        }        
        return total;
    }

    index_type get_strides(const index_type& arr)
    {
        index_type result;
        result[N - 1] = 1;
        for (size_t i = 1; i < N; i++)
        {
            result[N - 1 - i] = result[N - i] * arr[N - i];
        }
        return result;
    }

    void update_shape()
    {
        size_t size = get_product(fShape);
        if (size != fData.size())
        {
            throw std::runtime_error("Invalid shape. Dimensions must match.");
        }

        fSize = size;
        fStrides = get_strides(fShape);
    }

    // TODO: Implement check/not chcek
    // TODO: Add support for negative indices
    size_t make_index(const index_type& arr, bool check_index = true) const
    {
        size_t index = 0;
        // std::cout << "multi: ";
        for (int i = 0; i < N; i++)
        {
            if (check_index && (arr[i] >= fShape[i]))
            {
                throw std::runtime_error("Index overflow.");
            }
            // std::cout << fStrides[i] << "*" << arr[i] << " ";
            index += fStrides[i] * arr[i];
        }
        // std::cout << " = " << index << std::endl;
        return index;
    }

    size_t fSize;

    index_type fStrides;

    index_type fShape;

    // TODO: Make this a shared_ptr???
    data_type fData;
};

template<typename T, size_t N> G4MultiArray<T, N> abs (const G4MultiArray<T, N>& arr)
{
    typename G4MultiArray<T, N>::data_type&& newData = abs(arr.Flatten());
    return { arr.Shape(), newData };
}

template<typename T, size_t N> G4MultiArray<T, N> exp (const G4MultiArray<T, N>& arr)
{
    typename G4MultiArray<T, N>::data_type&& newData = exp(arr.Flatten());
    return { arr.Shape(), newData };
}

template<typename T, size_t N, size_t M> class G4MultiArrayView
{
public:
    using array_type = G4MultiArray<T, N>;

    using data_type = typename array_type::data_type;

    using index_type = typename G4MultiArray<T, M>::index_type;

    using item_type = typename G4MultiArrayImpl<T, N, M>::item_type;

    template<typename, size_t, size_t> friend class G4MultiArrayImpl;

    template<typename, size_t, size_t> friend class G4MultiArrayView;

    G4MultiArrayView(array_type& array) : fArray(array)
    {
        static_assert(N == M, "Slicing array must result in a view of the same dimension.");
        fStrides = array.fStrides;
        fShape = array.fShape;
    }

    G4MultiArrayView(array_type& array, std::slice a_slice) : fArray(array)
    {
        static_assert(N == M, "Slicing array must result in a view of the same dimension.");
        fStrides = array.fStrides;
        fShape = array.fShape;
        // fOffsets[0] = a_slice.start;
        fGlobalOffset = a_slice.start * array.fStrides[0];
        fShape[0] = a_slice.size;
        fStrides[0] *= a_slice.stride;

        //update_gslice();
    }

    G4MultiArrayView& operator=(const T& value)
    {
        fArray.fData[get_gslice(fGlobalOffset, fShape, fStrides)] = value;
        return *this;
    }

    template<size_t N2> G4MultiArrayView& operator=(const G4MultiArrayView<T, N2, M>& other)
    {
        if (other.Shape() != fShape)
        {
            throw std::runtime_error("Cannot assign array of different shapes.");
        }
        *this = other.Copy();
        return *this;
    }

    G4MultiArrayView& operator=(const G4MultiArray<T, M>& other)
    {
        if (other.Shape() != fShape)
        {
            throw std::runtime_error("Cannot assign array of different shapes.");
        }
        fArray.fData[get_gslice(fGlobalOffset, fShape, fStrides)] = other.fData;
        return *this;
    }    

    G4MultiArrayView& operator=(const typename G4MultiArrayImpl<T, N, M>::nested_vector_type& nested_vector)
    {
        if (fShape[0] != nested_vector.size())
        {
            throw std::runtime_error("Nested vector used to initialize G4MultiArray must be regular");
        }
        for (size_t i = 0; i < fShape[0]; i++)
        {
            (*this)[i] = nested_vector[i];
        }
    }

    G4MultiArrayView(array_type& array, size_t i) : fArray(array)
    {
        static_assert(N == M + 1, "Subscripting array must result in a view of a smaller dimension.");
        // std::cout << "Strides ";
        for (size_t i = 0; i < M; i++)
        {
            fShape[i] = array.fShape[i + 1];
            fStrides[i] = array.fStrides[i + 1];
            // std::cout << fStrides[i];
        }
        // std::cout <<  std::endl;
        // fOffsets[0] = i;

        fGlobalOffset = i * array.fStrides[0];

        // std::cout <<  "Gloval offset: " << fGlobalOffset << std::endl;

        //update_gslice();
    }    

    G4MultiArrayView(G4MultiArrayView<T, N, M+1>& upper, size_t i) : fArray(upper.fArray)
    {
        for (size_t i = 0; i < M; i++)
        {
            fShape[i] = upper.fShape[i + 1];
            fStrides[i] = upper.fStrides[i + 1];
        }
        // fOffsets[0] = i;
        fGlobalOffset = upper.fGlobalOffset + i * upper.fStrides[0];

        // update_gslice();
    }

    operator G4MultiArray<T, M>() const
    {
        data_type data = fArray.fData[get_gslice(fGlobalOffset, fShape, fStrides)];
        return G4MultiArray<T, M>(fShape, std::move(data));
    }

    G4MultiArray<T, M> Copy() const
    {
        return G4MultiArray<T, M>(*this);   // Uses reference???
    }

    item_type operator[](size_t i)
    {
        return G4MultiArrayImpl<T, N, M>::get_item(*this, i);
    }  

    item_type operator[](size_t i) const
    {
        return G4MultiArrayImpl<T, N, M>::get_const_item(*this, i);
    }    

    const index_type& Shape() const { return fShape; }

    // const data_type& Flatten() const { return fData; }

private:
    static std::gslice get_gslice(size_t offset, index_type shape, index_type strides)
    {
        std::valarray<size_t> shape_(M);
        std::valarray<size_t> strides_(M);
        std::copy(shape.begin(), shape.end(), std::begin(shape_));
        std::copy(strides.begin(), strides.end(), std::begin(strides_));
        return std::gslice(offset, shape_, strides_);
    }

    size_t make_index(const index_type& arr, bool check_index = true) const
    {
        size_t index = fGlobalOffset;
        // std::cout << "multi: " << fGlobalOffset << " + ";
        for (int i = 0; i < M; i++)
        {
            // std::cout << i << ", ";
            if (false && (arr[i] >= fShape[i]))
            {
                throw std::runtime_error("Index overflow.");
            }
            // std::cout << fStrides[i] << "*" << arr[i] << " ";
            index += fStrides[i] * arr[i];
        }
        // std::cout << " = " << index << std::endl;
        // std::cout << " = " << index << std::endl;
        return index;
    }    

    static size_t get_dot(const index_type& arr1, const index_type& arr2)
    {
        size_t total = 0;
        for (size_t i = 0; i < N; i++) 
        {
            total += arr1[i] * arr2[i];
        }        
        return total;
    }    

    array_type& fArray;

    index_type fStrides;

    index_type fShape;    

    // index_type fOffsets;

    size_t fGlobalOffset;

    // std::gslice fSlice;

    // std::gslice_array<T> fData;

    size_t fSize;
};

template<typename T, size_t N> G4MultiArray<T, N> operator*(const T& t, const G4MultiArray<T, N>& array)
{
    return array * t;
}

template<typename T, size_t N> G4MultiArray<T, N> operator/(const T& t, const G4MultiArray<T, N>& array)
{
    return array / t;
}

template<typename T, size_t N> G4MultiArray<T, N> operator+(const T& t, const G4MultiArray<T, N>& array)
{
    return array + t;
}

template<typename T, size_t N> G4MultiArray<T, N> operator-(const T& t, const G4MultiArray<T, N>& array)
{
    return array - t;
}

template<typename T, size_t N> std::ostream& operator << (std::ostream& os, const G4MultiArray<T, N>& array)
{
    array.Write(os);
    return os;
}

template<typename T, size_t N, size_t M> std::ostream& operator << (std::ostream& os, const G4MultiArrayView<T, N, M>& array)
{
    array.Copy().Write(os);
    return os;
}
#endif


