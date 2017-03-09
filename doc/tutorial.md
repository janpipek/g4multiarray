# `multi_array` tutorial

## Indexing & slicing

There four main indexing approaches:

* `array[size_t]` - sub-array view, indexed along the first axis (scalar for 1-D case).
* `array.At(const std::array<size_t)&)` - scalar element, indexed along all axes.
* `array.slice<I>(args...)` - reduce or select sub-array along the I-th axis.
* `array(args...)` - multi-functional indexing, selecting elements or ranges along any axis. This approach is most inspired by numpy.

All of them do (or should) work both with const and non-const arrays and views.

**Planned:** masked arrays (using booleans or predicates)

## Creating arrays

There are several constructors:

* `multi_array<T,N>(const std::array<size_t, N>&)` - create a zero-filled array with a shape
* ...others, **TODO**: update this

To convert data, the universal `asarray` function should be used.

Other help functions can be used to create specific matrices. They are all inspired
by similar numpy functions but they are less flexible in parameters.

* `zeros`
* `ones`
* `linspace`
* `arange`
* `geomspace`
* `logspace`

## Arithmetic operations

Most arithemtic operations (+, -, *, /) are defined both element-wise for two
arrays of the same shape and for a combination of array and scalar.

**Planned:** All other operations and logical combinations.

## Mathematical functions

The standard functions defined in `<cmath>` are adapted to work with arrays element-wise,
i.e. `cos(array)` should return an array of cosines. The following functions are
(currently) available:

`abs`, `exp`, `log`, `log10`, `pow`, `sqrt`, `sin`, `cos`, `tan`, `asin`, `acos`,
`atan`, `atan2`, `sinh`, `cosh`, `tanh`

## Output

* `ostream << array` - write all elements to a stream
