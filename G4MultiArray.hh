/*
    G4MultiArray class, representing N-dimensional data.

    Written by: Jan Pipek (jan.pipek@gmail.com), 2016
 */


#ifndef G4MultiArray_h
#define G4MultiArray_h

#include <vector>
#include <valarray>
#include <array>
#include <functional>
#include <ostream>


namespace g4_multi_array
{
    // Forward-declare the types
    template<typename T, size_t N> class G4MultiArray;
	template<typename T, size_t N, template<typename> arrayT> typename multi_array;

    template<typename T, size_t N, size_t M> class G4MultiArrayView;

    template<typename T, size_t N, size_t M = N> class G4MultiArrayImpl
    {
    public:
        using item_type = G4MultiArrayView<T, N, M - 1>;

        using nested_vector_type = std::vector<typename G4MultiArrayImpl<T, N, M - 1>::nested_vector_type>;

        static item_type get_item(G4MultiArray<T, N>& arr, size_t i)
        {
            return item_type(arr, i);
        }

        static G4MultiArrayView<T, N, M - 1> get_item(G4MultiArrayView<T, N, M>& arr, size_t i)
        {
            return G4MultiArrayView<T, N, M - 1>(arr, i);
        }

        static item_type const get_const_item(const G4MultiArray<T, N>& arr, size_t i)
        {
            return item_type(arr, i);
        }

        static const G4MultiArrayView<T, N, M - 1> get_const_item(const G4MultiArrayView<T, N, M>& arr, size_t i)
        {
            return G4MultiArrayView<T, N, M - 1>(arr, i);
        }

        static std::array<size_t, M> get_shape(const nested_vector_type& nested_vector)
        {
            std::array<size_t, M> result;
            result[0] = nested_vector.size();
            auto daughter = G4MultiArrayImpl<T, N, M - 1>::get_shape(nested_vector[0]);
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
            auto index = arr.make_index({ i });
            // std::cout << "i" << index << std::endl;
            return arr.fArray.fData[index];
        }

        static const item_type get_const_item(const G4MultiArray<T, 1>& arr, size_t i)
        {
            return arr.fData[i];
        }

        static const item_type get_const_item(const G4MultiArrayView<T, N, 1>& arr, size_t i)
        {
            return arr.fArray.fData[arr.make_index({ i })];
        }

        static const item_type get_const_item(const G4MultiArray<T, 1>&& arr, size_t i)
        {
            return arr.fData[i];
        }

        static const item_type get_const_item(const G4MultiArrayView<T, N, 1>&& arr, size_t i)
        {
            return arr.fArray[arr.make_index({ i })];
        }

        static std::array<size_t, 1> get_shape(const nested_vector_type& nested_vector)
        {
            return{ nested_vector.size() };
        }
    };

    // TODO: use std::stride & std::gstride

	template<size_t N> size_t get_product(const std::array<size_t, N>& arr)
	{
		size_t total = 1;
		for (size_t i = 0; i < N; i++)
		{
			total *= arr[i];
		}
		return total;
	}

	template<size_t N> std::array<size_t, N> get_strides(const std::array<size_t, N>& arr)
	{
		index_type result;
		result[N - 1] = 1;
		for (size_t i = 1; i < N; i++)
		{
			result[N - 1 - i] = result[N - i] * arr[N - i];
		}
		return result;
	}

	template<size_t N> std::gslice get_gslice(size_t offset, std::array<size_t, N> shape, std::array<size_t, N> strides)
	{
		std::valarray<size_t> shape_(N);
		std::valarray<size_t> strides_(N);
		std::copy(shape.begin(), shape.end(), std::begin(shape_));
		std::copy(strides.begin(), strides.end(), std::begin(strides_));
		return std::gslice(offset, shape_, strides_);
	}

	template<typename T, size_t N> class multi_array_subtypes
	{
	public:
		using item_type = multi_array<T, N - 1, std::gslice_array>;

		using masked_type = multi_array<T, 1, std::mask_array>;

		using indirect_type = multi_array<T, 1, std::indirect_array>;

		using nested_vector_type = std::vector<typename multi_array_subtypes<T, N - 1>::nested_vector_type>;
	};

	template<typename T> class multi_array_subtypes<T, 1>
	{
	public:
		using item_type = T&;

		using masked_type = multi_array<T, 1, std::mask_array>;

		using indirect_type = multi_array<T, 1, std::indirect_array>;

		using nested_vector_type = std::vector<T>;
	};

	template<typename T> class multi_array_storage
	{
	public:
		using real_data_type = std::valarray<T>&;
	};

	template<typename T> class multi_array_storage<std::valarray<T>>
	{
	public:
		using real_data_type = std::valarray<T>;
	};

	template<typename T, size_t N, template<typename> typename arrayT> class multi_array_accessor
	{
		
	};

	template<typename T, size_t N, template<typename> typename arrayT> class multi_array_base
		: protected multi_array_subtypes<T, N>,
		  protected multi_array_storage<arrayT<T>>,
		  protected multi_array_accessor<T, N, array<T>>
	{
	public:    // *** Type definitions
		static_assert(N > 0, "Cannot have 0-dimensional arrays");

		using index_type = std::array<size_t, N>;

		using value_type = T;

		using data_type = arrayT<T>;

		using multi_array_storage<data_type>::real_data_type;

		using multi_array_subtypes<T, N>::item_type;

		using multi_array_subtypes<T, N>::nested_vector_type;

		constexpr static size_t ndim = N;

	public:    // *** Friends
		template<typename, size_t> friend std::ostream& operator << (std::ostream&, const multi_array_base&);

	protected:    // *** Constructors
		multi_array_base() { }

		multi_array_base(real_data_type& data, const index_type& shape, size_t offset, const index_type& strides)
			: fData(data), fShape(shape), fOffset(offset), fStrides(strides), fSize(get_product(shape))
		{
			if (data.size() != fSize)
			{
				throw std::runtime_error("Invalid size of data.");
			}
		}

	public:    // *** Functions
		size_t Size() const { return fSize; }

		const index_type& Shape() const { return fShape; }

		const data_type& Data() const { return fData; }

		multi_array<T, N> Copy() const { return multi_array<T, N>(*this); }

	public:    // *** Item access
		/* item_type operator[](size_t i)
		{
			return G4MultiArrayImpl<T, N>::get_item(*this, i);
		}
		*/

		/*item_type operator[](size_t i) const
		{
			return G4MultiArrayImpl<T, N>::get_const_item(*this, i);
		}*/

	protected:   // *** Data
		real_data_type fData;

		index_type fStrides;

		index_type fShape;

		size_t fSize;

		size_t fOffset{ 0 };
	};

	template<typename T, size_t N> class multi_array final : public multi_array_base<T, N, std::valarray>
	{
	public:
		using base_type = multi_array_base<T, N, std::valarray>;

		explicit multi_array(const index_type& shape)
			: base_type(
				real_data_type(get_product(shape)),
				shape,
				0,
				get_strides(shape))
		{ }

		multi_array(const index_type& shape, const data_type& data)
			: base_type(
				data,
				shape,
				0,
				get_strides(shape))
		{}

		multi_array(const index_type& shape, const T& value)
			: base_type(
				real_data_type(value, get_product(shape)),
				shape,
				0,
				get_strides(shape))
		{ }

		multi_array(const index_type& shape, const T* data)
			: base_type(
				real_data_type(data, get_product(shape)),
				shape,
				0,
				get_strides(shape)
			)
		{ }

		template<template <typename> arrayT> multi_array(const multi_array_base<T, N, arrayT>& other)
			: base_type(
				other.Data(),
				other.Shape(),
				0,
				get_product(other.Shape())
			)
		{ }

		/* multi_array(const nested_vector_type& nested_vector)
		{
			// TODO: Finish implementation
			fShape = G4MultiArrayImpl<T, N>::get_shape(nested_vector);
			fData = data_type(get_product(fShape));
			// update_shape();

			for (size_t i = 0; i < fShape[0]; i++)
			{
				(*this)[i] = nested_vector[i];
			}
		}*/

		// TODO: Include multi_array_base&& constructor
		// TODO: Include data_typee&& constructor
	};

	template<typename T, size_t N> class multi_array_view : public multi_array_base<T, N, std::gslice_array> final
	{
	public: 
		using base_type = multi_array_base<T, N, std::gslice_array>;

		template<template<typename> typename arrayT> multi_array_view(multi_array_base<T, N, arrayT>& upper, size_t i)
		{
			for (size_t i = 0; i < N; i++)
			{
				fShape[i] = upper.fShape[i + 1];
				fStrides[i] = upper.fStrides[i + 1];
			}
			// fOffsets[0] = i;
			fGlobalOffset = upper.fGlobalOffset + i * upper.fStrides[0];

			// update_gslice();
		}
	};

    // TODO: Include copy-on-write function (thread-safe?)
    template<typename T, size_t N> class G4MultiArray final
    {
    public:
        

        /*template<typename, size_t, size_t> friend class G4MultiArrayView;

        template<typename, size_t, size_t> friend class G4MultiArrayImpl;

        template<typename, size_t> friend std::ostream& operator << (std::ostream&, const G4MultiArray&); */

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

                os << fData[i];
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
            os << "])";
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
		
        template<typename U> G4MultiArray<U, N> As() const
        {
            std::valarray<U> result;
            result.resize(fSize);
            for (size_t i = 0; i < fSize; i++)
            {
                result[i] = U(fData[i]);
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
                throw std::runtime_error("Incompatible shapes for division.");
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
                throw std::runtime_error("Incompatible shapes for addition.");
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
                throw std::runtime_error("Incompatible shapes for subtraction.");
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
        static 

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

    template<typename T, size_t N, size_t M> class G4MultiArrayView
    {
    public:
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
            return *this;
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

        G4MultiArrayView(G4MultiArrayView<T, N, M + 1>& upper, size_t i) : fArray(upper.fArray)
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

        item_type operator[](size_t i)
        {
            return G4MultiArrayImpl<T, N, M>::get_item(*this, i);
        }

        item_type operator[](size_t i) const
        {
            return G4MultiArrayImpl<T, N, M>::get_const_item(*this, i);
        }


        // const data_type& Flatten() const { return fData; }

    private:
        static 

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
}

using g4_multi_array::G4MultiArray;


#endif


