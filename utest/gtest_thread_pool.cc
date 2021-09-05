/*************************************************************************
 @Copyright (c), ke Inc
 @Created Time : 2021-02-24 08:15:43 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-05 08:31:15 PM
 @Modified By: dongzhixiong
************************************************************************/
#include <iostream>

#include "concurrency/task.h"
#include "concurrency/thread_pool.h"
#include "base_gtest.h"


class Sample {
public:
    Sample() {}

    ~Sample() {}

    void Add(int i, int j) {
        add_sum = i + j;
    }

public:
    int add_sum;
};

void TestSample01() {
    std::shared_ptr<kbms::ThreadPool> thread_pool = std::shared_ptr<kbms::ThreadPool>(new kbms::ThreadPool(5));
    thread_pool->Init();
    int i = 200;
    int j = 100;
    std::shared_ptr<Sample> sample = std::make_shared<Sample>();
    auto task_fun = [i, j, sample]() {
        sample->Add(i, j);
    };
    kbms::TaskPtr task = std::shared_ptr<kbms::Task>(new kbms::Task(task_fun));
    thread_pool->AddTask(task);
    usleep(10);
    H_TEST_ASSERT(sample->add_sum == 300);

}

TEST_UNIT(gtestNormalHander) {
    for ( int i = 0; i < 4; ++i) {
        std::cout << "gtestSample01 i=" << i;
        TestSample01();
    }
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);

    auto result = RUN_ALL_TESTS();
    return result;
}
