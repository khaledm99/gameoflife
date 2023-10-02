#include "gtest/gtest.h"
#include "gameoflife.h"

TEST(ModTest, NonNegativeValues)
{
    EXPECT_EQ(GameOfLife::mod_floor(1, 1), 0);
    EXPECT_EQ(GameOfLife::mod_floor(0, 1), 0);
    EXPECT_EQ(GameOfLife::mod_floor(2,4), 2);
    EXPECT_EQ(GameOfLife::mod_floor(4,2), 0);
    EXPECT_EQ(GameOfLife::mod_floor(5,2), 1);
}

TEST(ModTest, NegativeValues)
{
    EXPECT_EQ(GameOfLife::mod_floor(-1,5), 4);
    EXPECT_EQ(GameOfLife::mod_floor(-2,5), 3);
}

TEST(RoundTest, ZeroValues)
{
    EXPECT_ANY_THROW(GameOfLife::find_nearest(1,0));
    EXPECT_EQ(GameOfLife::find_nearest(0,1), 0);
}

TEST(RoundTest, NonZeroValues)
{
    EXPECT_EQ(GameOfLife::find_nearest(1,1), 1);    // no round
    EXPECT_EQ(GameOfLife::find_nearest(1,2), 1);    // round up 
    EXPECT_EQ(GameOfLife::find_nearest(1,3), 0);    // round down
}
