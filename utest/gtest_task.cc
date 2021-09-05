/*************************************************************************
 @Copyright (c), ke Inc
 @Created Time : 2021-02-24 08:15:43 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-05 08:30:01 PM
 @Modified By: dongzhixiong
************************************************************************/
#include <iostream>

#include "concurrency/task.h"
#include "base_gtest.h"


static int add_sum;
static int str_len;

void Add(int i, int j) {
    add_sum = i + j;
}

void CalStrSize(const std::string &st) {
    str_len = st.size();
}


void TestSample01() {
    int i = 2;
    int j = 1;
    auto task_fun = [i, j]() {
        Add(i, j);
    };
    kbms::TaskPtr task = std::shared_ptr<kbms::Task>(new kbms::Task(task_fun));
    task->state_ = kbms::EXECUTING;
    task->Run();
    H_TEST_ASSERT(add_sum == 3);
}

void TestSample02() {
    std::string text = "hello ke.com";
    auto task_fun = [text]() {
        CalStrSize(text);
    };
    kbms::TaskPtr task = std::shared_ptr<kbms::Task>(new kbms::Task(task_fun));
    task->state_ = kbms::EXECUTING;
    task->Run();
    H_TEST_ASSERT(str_len == 12);

}


TEST_UNIT(gtestSample01) {
    for ( int i = 0; i < 1; ++i) {
        std::cout << "gtestSample01 i=" << i;
        TestSample01();
    }
}

TEST_UNIT(gtestSample02) {
    for ( int i = 0; i < 1; ++i) {
        std::cout << "gtestSample02 i=" << i;
        TestSample02();
    }
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);

    auto result = RUN_ALL_TESTS();
    return result;
}
