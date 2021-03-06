#include <catch.hpp>

#include <Math/Common.hpp>
#include <Math/Spinor.hpp>
#include <Math/Rect.hpp>
#include <Math/Vector.hpp>

using namespace Math;

TEST_CASE("Common tests", "[math]")
{
    SECTION("Float comparisons")
    {
        CHECK(!FloatCompare(0.f, 0.0000001f, 0.0000001f));
        CHECK(!FloatCompare(0.f, 0.00001f, 0.0000001f));
        CHECK(!FloatCompare(0.f, 0.001f, 0.0000001f));

        CHECK(FloatCompare(0.f, 0.0000001f, 0.01f));
        CHECK(FloatCompare(0.f, 0.00001f, 0.01f));
        CHECK(FloatCompare(0.f, 0.001f, 0.01f));
    }
}

TEST_CASE("Spinor tests", "[math][spinor]")
{
    Spinor s1, s2;

    INFO("Implement these");
}

TEST_CASE("Rect tests", "[math][rect]")
{
    Rect r1, r2;

    INFO("Implement these");
}

TEST_CASE("Vector tests", "[math][vector]")
{
    Vector2 v1, v2;

    INFO("Implement these");
}
