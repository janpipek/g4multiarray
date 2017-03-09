G4MultiArray
============
Proposal of a multidimensional array to be used in Geant4.

Goals
-----
- C++11 "feel"
- No virtual functions
- API similarity to std::vector
- API similarity to numpy's ndarray
- Interoperability with Geant4's G4ThreeVector, G4RotationMatrix, ...
- Compilability with GCC 4.8+, Visual Studio

Installation
------------
The library is header-only. Just download multi_array.hh and include it in your project.

Examples
--------

See the examples directory.

Inspiration
-----------
- B.Stroustrup's Matrix: <http://www.stroustrup.com/Programming/Matrix/Matrix.h>

Any suggestions welcome :-)
