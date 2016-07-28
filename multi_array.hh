#ifndef G4MultiArray_h
#define G4MultiArray_h

#include <valarray>
#include <array>
#include <ostream>

// Forward definition of types
template<typename T, size_t N> class multi_array;
template<typename T, size_t N> class multi_array_view;
template<typename T, size_t N> class multi_array_view_const;
template<typename T, size_t N, template<typename, size_t> class data_policy> class multi_array_base;

template<size_t N> size_t get_product(const std::array<size_t, N>& arr)
{
    size_t total = 1;
    for (size_t i = 0; i < N; i++)
    {
        total *= arr[i];
    }
    return total;
}

/** Creates strides for a regular array. **/
template<size_t N> std::array<size_t, N> get_strides(const std::array<size_t, N>& arr)
{
    std::array<size_t, N> result;
    result[N - 1] = 1;
    for (size_t i = 1; i < N; i++)
    {
        result[N - 1 - i] = result[N - i] * arr[N - i];
    }
    return result;
}

/** Creates gslice for a view. **/
template<size_t N> std::gslice get_gslice(size_t offset, std::array<size_t, N> shape, std::array<size_t, N> strides)
{
    std::valarray<size_t> shape_(N);
    std::valarray<size_t> strides_(N);
    std::copy(shape.begin(), shape.end(), std::begin(shape_));
    std::copy(strides.begin(), strides.end(), std::begin(strides_));
    return std::gslice(offset, shape_, strides_);
}

template<size_t N> class index_impl
{
public:
    using index_type = std::array<size_t, N>;

    index_impl(const index_type& shape)
        : fShape(shape), fStrides(get_strides(shape)), fSize(get_product(shape))
    { }

    index_impl(const index_type& shape, const index_type& strides, size_t offset)
        : fShape(shape), fStrides(strides), fSize(get_product(shape)), fOffset(offset)
    { }

    size_t Size() const { return fSize; }

    const index_type& Shape() const { return fShape; }

protected:
    size_t make_index(const index_type& arr, bool check_index = true) const
    {
        size_t index = fOffset;
        for (int i = 0; i < N; i++)
        {
            if (check_index && (arr[i] >= fShape[i]))
            {
                throw std::runtime_error("Index overflow.");
            }
            index += fStrides[i] * arr[i];
        }
        return index;        
    }

    index_type fShape;

    index_type fStrides;

    size_t fSize;

    size_t fOffset{ 0 };    
};

template<typename T, size_t N> class array_owner_impl : public index_impl<N>
{
public:
    // Type aliases
    using data_type = std::valarray<T>;
    using base_type = index_impl<N>;
    using typename base_type::index_type;

    array_owner_impl(const data_type& data, const index_type& shape)
        : base_type(shape), fData(data)
    { }

    array_owner_impl(const data_type&& data, const index_type& shape)
        : base_type(shape), fData(data)
    { }    

    constexpr static bool read_write_access = true;

    const std::valarray<T>& Data() const { return fData; }

protected:
    std::valarray<T>& get_data_array() { return fData; }

    data_type fData;
};

template<typename T, size_t N, bool is_const> class t_array_view_impl : public index_impl<N>
{
public:
    constexpr static bool read_write_access = is_const;

    // Type aliases
    using data_type = typename std::conditional<is_const, const std::valarray<T>&, std::valarray<T>&>::type;
    using base_type = index_impl<N>;
    using typename base_type::index_type;

    // Import base members
    using index_impl<N>::fStrides;
    using index_impl<N>::fShape;
    using index_impl<N>::fOffset;

    t_array_view_impl(data_type data, const index_type& shape, const index_type& strides, size_t offset)
        : base_type(shape, strides, offset), fData(data)
    {  }

    std::valarray<T> Data() const
    {
        return fData[get_gslice_()];
    }
private:
    std::gslice get_gslice_() const
    {
        return get_gslice(fOffset, fShape, fStrides);
    }    

protected:
    data_type fData;

    auto get_data_array() -> decltype(this->fData[std::gslice()])
    {
        return fData[get_gslice_()]; 
    }
};


template<typename T, size_t N> using array_view_impl = t_array_view_impl<T, N, false>;

template<typename T, size_t N> using array_const_view_impl = t_array_view_impl<T, N, true>;

template<typename T, size_t N> class array_accessor_impl
{
public:
    using const_item_type = multi_array_view_const<T, N-1>;
    using item_type = multi_array_view<T, N-1>;

    template<template<typename, size_t> class data_policy> static const_item_type get_const_item(const multi_array_base<T, N, data_policy>& array, size_t i)
    {
        return const_item_type(array, i);
    }

    template<template<typename, size_t> class data_policy> static item_type get_item(multi_array_base<T, N, data_policy>& array, size_t i)
    {
        return item_type(array, i);
    }    
};

template<typename T> class array_accessor_impl<T, 1>
{
public:
    using const_item_type = const T&;
    using item_type = T&;

    template<template<typename, size_t> class data_policy> static const_item_type get_const_item(const multi_array_base<T, 1, data_policy>& array, size_t i)
    {
        return array[{i}];
    }

    template<template<typename, size_t> class data_policy> static item_type get_item(multi_array_base<T, 1, data_policy>& array, size_t i)
    {
        return array[{i}];
    }    
};

template<typename T, size_t N, template<typename, size_t> class data_policy> class multi_array_base
    : public data_policy<T, N>
{
public:
    // Type aliases
    using base_type = data_policy<T, N>;
    using typename base_type::index_type;
    using const_item_type = typename std::conditional<N == 1, const T&, multi_array_view_const<T, N-1>>::type;
    using item_type = typename std::conditional<N == 1, T&, multi_array_view<T, N-1>>::type;
    using accessor_type = array_accessor_impl<T, N>;

    // Friends
    template<typename, size_t> friend class array_accessor_impl;
    template<typename, size_t, template<typename, size_t> class> friend class multi_array_base;
    template<typename, size_t> friend class multi_array;
    template<typename, size_t> friend class multi_array_view;
    template<typename, size_t> friend class multi_array_view_const;
    // template<typename, size_t> friend std::ostream& operator << (std::ostream&, const multi_array_base&);

protected:
    // Import members
    using index_impl<N>::fStrides;
    using index_impl<N>::fShape;
    using index_impl<N>::fSize;
    using index_impl<N>::fOffset; 
    using base_type::fData;    
    using index_impl<N>::make_index;

    using base_type::base_type;

public:
    T& operator[] (const index_type& i) { return fData[make_index(i)]; }

    const T& operator[] (const index_type& i) const { return fData[make_index(i)]; }   

    const_item_type operator[] (size_t i) const { return accessor_type::get_const_item(*this, i); }

    item_type operator[] (size_t i) { return accessor_type::get_item(*this, i); }

    multi_array<T, N> Copy() const { return multi_array<T, N>(*this); }
};

template<typename T, size_t N> class multi_array : public multi_array_base<T, N, array_owner_impl>
{
public:
    // Type aliases
    using base_type = multi_array_base<T, N, array_owner_impl>;
    using typename base_type::index_type;
    using typename base_type::data_type;    // std::valarray<T>

protected:
    // Import members
    using index_impl<N>::fStrides;
    using index_impl<N>::fShape;
    using index_impl<N>::fSize;
    using index_impl<N>::fOffset; 
    using base_type::fData;   

public:
    explicit multi_array(const index_type& shape)
        : base_type(
            data_type(get_product(shape)),
            shape)
    { }

    multi_array(const index_type& shape, const std::valarray<T>& data)
        : base_type(
            data,
            shape)
    {}

    multi_array(const index_type& shape, const T& value)
        : base_type(
            data_type(value, get_product(shape)),
            shape)
    { }

    multi_array(const index_type& shape, const T* data)
        : base_type(
            data_type(data, get_product(shape)),
            shape)
    { }

    template<template <typename, size_t> class data_policy> multi_array(const multi_array_base<T, N, data_policy>& other)
        : base_type(
            other.Data(),
            other.Shape()
        )
    { }

    void Write(std::ostream& os) const; // TODO: Generalize
};

template<typename T, size_t N> class multi_array_view : public multi_array_base<T, N, array_view_impl>
{
public:
    // Type aliases
    using base_type = multi_array_base<T, N, array_view_impl>;
    using typename base_type::index_type;
    using typename base_type::data_type;    // std::valarray<T>

    //     t_array_view_impl(data_type data, const index_type& shape, const index_type& strides, size_t offset)
    //    : base_type(shape, strides, offset), fData(data)

    template<template<typename, size_t> class data_policy> multi_array_view(multi_array_base<T, N+1, data_policy>& upper, size_t i)
        : base_type(
            upper.fData,
            get_shape(upper, i),
            get_strides(upper, i),
            get_offset(upper, i)
        )
    {

    }

private:
    template<template<typename, size_t> class data_policy> static index_type get_shape(const multi_array_base<T, N+1, data_policy>& upper, size_t i)
    {
        index_type shape;
        for (size_t i = 0; i < N; i++)
        {
            shape[i] = upper.fShape[i + 1];
        }
        return shape;
    }

    template<template<typename, size_t> class data_policy> static index_type get_strides(const multi_array_base<T, N+1, data_policy>& upper, size_t i)
    {
        index_type strides;
        for (size_t i = 0; i < N; i++)
        {
            strides[i] = upper.fStrides[i + 1];
        }
        return strides;
    }  

    template<template<typename, size_t> class data_policy> static size_t get_offset(const multi_array_base<T, N+1, data_policy>& upper, size_t i)
    {
        return upper.fOffset + i * upper.fStrides[1];
    }   

};

template<typename T, size_t N> class multi_array_view_const : public multi_array_base<T, N, array_const_view_impl>
{

};

template<typename T, size_t N> void multi_array<T, N>::Write(std::ostream& os) const
{
    os << "{";
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
            for (size_t j = 0; j + 1 < N; j++)
            {
                if (i % fStrides[j] == 0)
                {
                    for (size_t k = j; k + 1 < N; k++)
                    {
                        os << "\n";
                    }
                    os << "               ";
                    for (size_t k = 0; k < j; k++)
                    {
                        os << " ";
                    }
                    for (size_t k = j; k + 1 < N; k++)
                    {
                        os << "[";
                    }
                    break;
                }
            }
        }

        os << fData[fOffset + i];
        i++;

        size_t j = 0;
        // size_t k = 0;
        for (; j + 1 < N; j++)
        {
            if (i % fStrides[j] == 0)
            {
                break;
            }
        }
        for (size_t k = j; k + 1 < N; k++)
        {
            os << "]";
        }
        if (i != fSize)
        {
            os << ", ";
        }
    }
    os << "}";
}

#endif