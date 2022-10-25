#include "../gtest/gtest.h"
#include "../Numbers/Rational.h"

class Rational_Checking : public ::testing::Test {
protected:
    virtual void SetUp() {
        auto second_num = RationalNumber(-500);
        auto cool_num = RationalNumber("3", "5");
        auto simplify_num = RationalNumber(10, -5);
        auto super_simplify = RationalNumber(-25, 500);
};


TEST(Rational_Checking)
{
    ASSERT_EQ((RationalNumber(0) - RationalNumber(53)).to_string(), "-53");
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*
TEST_F(Rational_Checking, distanceCheck)
{
    edges path = {{{"A", "C"}}, {{"C", "F"}}, {{"F", "E"}}};
    ASSERT_EQ(shortest_way(con_graph, "A", "E"), path);
    path = {{{"A", "C"}}, {{"C", "F"}}};
    ASSERT_EQ(shortest_way(con_graph, "A", "F"), path);
    path = {{{"B", "D"}}, {{"D", "E"}}};
    ASSERT_EQ(shortest_way(con_graph, "B", "E"), path);
    path = {{{"A", "B"}}, {{"B", "C"}}, {{"C", "D"}}};
    ASSERT_EQ(shortest_way(con_long_graph, "A", "D"), path);
    ASSERT_THROW(shortest_way(noncon_graph, "A", "F"), std::logic_error);
}
*/