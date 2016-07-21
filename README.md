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

Demo
----
Look in `test.cc` for the usage, compile & run. More detailed description to come.

Inspiration
-----------
- B.Stroustrup's Matrix: <http://www.stroustrup.com/Programming/Matrix/Matrix.h>

Any suggestions welcome :-)