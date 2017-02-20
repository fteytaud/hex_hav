/*
BSD 3-Clause License

Copyright (c) 2017, 
Fabien Teytaud, Julien Dehos, Joris Duguépéroux and Ahmad Mazyad
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef BOARD_TEST_HPP_
#define BOARD_TEST_HPP_

#include <hex_hav/Board.hpp>
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

#endif  // BOARD_TEST_HPP_

