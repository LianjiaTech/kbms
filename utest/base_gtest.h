/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-08-03 06:00:36 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 06:10:40 PM
 @Modified By: dongzhixiong
************************************************************************/
#pragma once

#include <gtest/gtest.h>

#define TEST_UNIT(name)  \
    class GtestObjectClass_##name : public testing::Test{ \
    public: \
        GtestObjectClass_##name() {} \
        ~GtestObjectClass_##name() {} \
        virtual void SetUp() {} \
        virtual void TearDown() {} \
    }; \
    TEST_F(GtestObjectClass_##name, name)

#define H_TEST_ASSERT ASSERT_TRUE
#define H_TEST_EQUAL(x, y) H_TEST_ASSERT((x)==(y))
#define H_EXPECT_EQUAL(x, y) H_TEST_ASSERT((x)==(y))
#define H_EXPECT_TRUE(x) H_TEST_ASSERT(x)
#define H_EXPECT_FALSE(x) H_TEST_ASSERT(!(x))
