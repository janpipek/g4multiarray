#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../multi_array.hh"

#include <vector>

using namespace std;

TEST_CASE("Constructing using asarray")
{
	SECTION("From std::vector")
	{
		vector<double> v{ 1, 2, 3, 4, 5 };
		multi_array<double, 1> a = asarray(v);
		array<size_t, 1> expectedShape{ 5 };

		REQUIRE(a.Shape() == expectedShape);
		REQUIRE(a(4) == 5.0);
	}

	SECTION("From std::array")
	{
		array<double, 5> v{ 1, 2, 3, 4, 5 };
		multi_array<double, 1> a = asarray(v);
		array<size_t, 1> expectedShape{ 5 };

		REQUIRE(a.Shape() == expectedShape);
		REQUIRE(a(4) == 5.0);
	}
}