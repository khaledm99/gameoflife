#include "gtest/gtest.h"
#include "gameoflife.h"
#include "util.h"

TEST(ModTest, NonNegativeValues)
{
    EXPECT_EQ(mod_floor(1, 1), 0);
    EXPECT_EQ(mod_floor(0, 1), 0);
    EXPECT_EQ(mod_floor(2,4), 2);
    EXPECT_EQ(mod_floor(4,2), 0);
    EXPECT_EQ(mod_floor(5,2), 1);
}

TEST(ModTest, NegativeValues)
{
    EXPECT_EQ(mod_floor(-1,5), 4);
    EXPECT_EQ(mod_floor(-2,5), 3);
}

TEST(RoundTest, ZeroValues)
{
    EXPECT_ANY_THROW(find_nearest(1,0));
    EXPECT_EQ(find_nearest(0,1), 0);
}

TEST(RoundTest, NonZeroValues)
{
    EXPECT_EQ(find_nearest(1,1), 1);    // no round
    EXPECT_EQ(find_nearest(1,2), 1);    // round up 
    EXPECT_EQ(find_nearest(1,3), 0);    // round down
}

TEST(GOLConstructorTest, NonPosVals)
{
    EXPECT_ANY_THROW(new GameOfLife(0,0));
    EXPECT_ANY_THROW(new GameOfLife(-1,1));
    EXPECT_ANY_THROW(new GameOfLife(1,-1));
}
TEST(GOLConstructorTest, ValidConstructor)
{
    GameOfLife g(1,1);
    EXPECT_EQ(g.getHeight(),1);
    EXPECT_EQ(g.getWidth(),1);
    EXPECT_EQ(g.getSpeed(),100);
    EXPECT_EQ(g.getWorld().size(),1);
    EXPECT_EQ((g.getWorld())[0].size(),1);
}


TEST(SpeedTest, IncreaseSpeed)
{
   GameOfLife g(1,1);
   EXPECT_EQ(g.getSpeed(), 100);
   g.changeSpeed(-20);
   EXPECT_EQ(g.getSpeed(), 80);
   g.changeSpeed(-100);
   EXPECT_EQ(g.getSpeed(), 0);
}
TEST(SpeedTest, DecreaseSpeed)
{
    GameOfLife g(1,1);
    EXPECT_EQ(g.getSpeed(), 100);
    g.changeSpeed(20);
    EXPECT_EQ(g.getSpeed(), 120);
    g.changeSpeed(10000);
    EXPECT_EQ(g.getSpeed(), 500);
}

TEST(SetViewTest, InvalidSettings)
{
   //GameOfLife g(1,1);
   Display d = Display(1,1,1,1);
   SDL_Rect initialview = *d.getView();
   EXPECT_ANY_THROW(d.setView(0,0,10,10));
   EXPECT_ANY_THROW(d.setView(0,1,10,10));
   EXPECT_ANY_THROW(d.setView(1,0,10,10));
   EXPECT_ANY_THROW(d.setView(1,1,-1,-1));
   EXPECT_ANY_THROW(d.setView(1,1,0,-1));
   EXPECT_ANY_THROW(d.setView(1,1,-1,0));
   EXPECT_EQ(d.getView()->x, initialview.x);
   EXPECT_EQ(d.getView()->y, initialview.y);
   EXPECT_EQ(d.getView()->w, initialview.w);
   EXPECT_EQ(d.getView()->h, initialview.h);
}

TEST(SetViewTest, ValidSettings)
{
    Display d = Display(1,1,1,1);
    d.setView(1920,720,0,0);
    EXPECT_EQ(d.getView()->x, 0);
    EXPECT_EQ(d.getView()->y, 0);
    EXPECT_EQ(d.getView()->w, 1920);
    EXPECT_EQ(d.getView()->h, 720);
}

TEST(SetWinTest, InvalidSettings)
{
   Display d = Display(1,1,1,1);
   SDL_Rect initialview = *d.getWin();
   EXPECT_ANY_THROW(d.setWin(0,0,10,10));
   EXPECT_ANY_THROW(d.setWin(0,1,10,10));
   EXPECT_ANY_THROW(d.setWin(1,0,10,10));
   EXPECT_ANY_THROW(d.setWin(1,1,-1,-1));
   EXPECT_ANY_THROW(d.setWin(1,1,0,-1));
   EXPECT_ANY_THROW(d.setWin(1,1,-1,0));
   EXPECT_EQ(d.getWin()->x, initialview.x);
   EXPECT_EQ(d.getWin()->y, initialview.y);
   EXPECT_EQ(d.getWin()->w, initialview.w);
   EXPECT_EQ(d.getWin()->h, initialview.h);
}

TEST(SetWinTest, ValidSettings)
{
    Display d = Display(1,1,1,1);
    d.setWin(1920,720,0,0);
    EXPECT_EQ(d.getWin()->x, 0);
    EXPECT_EQ(d.getWin()->y, 0);
    EXPECT_EQ(d.getWin()->w, 1920);
    EXPECT_EQ(d.getWin()->h, 720);
}

TEST(NextFrameTest, LineOscillator)
{
    std::vector<std::vector<int>> testvec =
    { {0,0,0,0},
      {1,1,1,0},
      {0,0,0,0},
      {0,0,0,0}};

    GameOfLife g(4,4);
    g.setCell(1,0);
    g.setCell(1,1);
    g.setCell(1,2);
    g.next();
    EXPECT_TRUE(g.getWorld()==testvec);
    for(auto &r : g.getWorld()) 
    {
        for(auto &c : r) printf("%d",c);
    }
}

TEST(DisplayConstructorTest, InvalidParams)
{
    EXPECT_ANY_THROW(new Display(0,0,0,0));
    EXPECT_ANY_THROW(new Display(-1,-1,-1,-1));
}

TEST(DisplayConstructorTest, ValidParams)
{
    Display d(10,20,30,40);
    EXPECT_EQ(d.getWin()->w, 10);
    EXPECT_EQ(d.getWin()->h,20);
    EXPECT_EQ(d.getView()->w, 30);
    EXPECT_EQ(d.getView()->h, 40);
    EXPECT_NE(d.getWindow(), nullptr);
    EXPECT_NE(d.getRenderer(), nullptr);
    EXPECT_NE(d.getTexture(), nullptr);
} 



