//
// Created by user on 2020-11-05.
//
#include "gtest/gtest.h"
#include "pub.hpp"

TEST(Pub_tests, Cycle_for_10_clients) {
    RealTimePub p1 = RealTimePub(10);
    p1.sim();
    EXPECT_TRUE(p1.no_clients());
}