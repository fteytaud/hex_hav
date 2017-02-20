// Copyright © 2014 Teytaud & Dehos <{teytaud,dehos}@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef BOARD_TEST_HPP_
#define BOARD_TEST_HPP_

#include "Board.hpp"
#include <cxxtest/TestSuite.h>

class Cell_test :  public CxxTest::TestSuite 
{
    public:
        void test_constructor1() 
        {
            Cell c1;
            TS_ASSERT_EQUALS(c1._i, 0);
            TS_ASSERT_EQUALS(c1._j, 0);
            Cell c2 = c1;
            TS_ASSERT_EQUALS(c2._i, 0);
            TS_ASSERT_EQUALS(c2._j, 0);
            Cell c3;
            c3 = c2;
            TS_ASSERT_EQUALS(c3._i, 0);
            TS_ASSERT_EQUALS(c3._j, 0);
        }
        void testi_constructor2() 
        {
            Cell c1(1, 2);
            TS_ASSERT_EQUALS(c1._i, 1);
            TS_ASSERT_EQUALS(c1._j, 2);
            Cell c2 = c1;
            TS_ASSERT_EQUALS(c2._i, 1);
            TS_ASSERT_EQUALS(c2._j, 2);
            Cell c3;
            c3 = c2;
            TS_ASSERT_EQUALS(c3._i, 1);
            TS_ASSERT_EQUALS(c3._j, 2);
        }
        void test_equal() 
        {
            Cell c1(-1, -2);
            Cell c2(4, 3);
            Cell c3(-1, 3);
            Cell c4(4, -2);
            Cell c5(-1, -2);
            TS_ASSERT_EQUALS(c1, c1);
            TS_ASSERT_EQUALS(c1, c5);
            TS_ASSERT_DIFFERS(c1, c2);
            TS_ASSERT_DIFFERS(c1, c3);
            TS_ASSERT_DIFFERS(c1, c4);
        }
};
#endif

