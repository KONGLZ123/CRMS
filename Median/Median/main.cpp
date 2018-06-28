#define BOOST_TEST_MAIN
//#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "kth.h"

std::vector<int64_t> g_data;

template<int N>
void setVec(int64_t(&arr)[N])
{
    g_data.assign(arr, arr + N);
    std::sort(g_data.begin(), g_data.end());
}

void search(int64_t guess, int64_t* smaller, int64_t* same)
{
    std::vector<int64_t>::iterator it = std::lower_bound(g_data.begin(), g_data.end(), guess);
    *smaller = it - g_data.begin();
    *same = std::upper_bound(g_data.begin(), g_data.end(), guess) - it;
}

bool check()
{
    for (size_t i = 0; i < g_data.size(); ++i)
    {
        std::pair<int64_t, bool> result = getKth(search, i + 1, g_data.size(), g_data.front(), g_data.back());
        if (!(result.second && result.first == g_data[i]))
        {
            std::cout << "i = " << i << std::endl;
            std::copy(g_data.begin(), g_data.end(), std::ostream_iterator<int64_t>(std::cout, " "));
            std::cout << std::endl;
            return false;
        }
        //LOG_INFO << "***** Result is " << result.first;
    }
    return true;
}

const int64_t values[] =
{
    std::numeric_limits<int64_t>::min(),
    std::numeric_limits<int64_t>::min() + 1,
    std::numeric_limits<int64_t>::min() + 2,
    -2, -1, 0, 1, 2,
    std::numeric_limits<int64_t>::max() - 2,
    std::numeric_limits<int64_t>::max() - 1,
    std::numeric_limits<int64_t>::max()
};

const int n_values = static_cast<int>(sizeof(values) / sizeof(values[0]));


BOOST_AUTO_TEST_CASE(testOneElement)
{
    for (int i = 0; i < n_values; ++i)
    {
        std::cout << values[i] << "\t";
        int64_t data[] = { values[i] };
        setVec(data);
        BOOST_CHECK_EQUAL(check(), true);
    }
    std::cout << "\n";
}


BOOST_AUTO_TEST_CASE(testTwoElements)
{
    for (int i = 0; i < n_values; ++i)
        for (int j = 0; j < n_values; ++j)
        {
            int64_t data[] = { values[i], values[j] };
            setVec(data);
            BOOST_CHECK_EQUAL(check(), true);
        }
}

BOOST_AUTO_TEST_CASE(testThreeElements)
{
    int indices[] = { 0, 0, 0, };
    while (indices[0] < n_values)
    {
        int64_t data[] = { values[indices[0]], values[indices[1]], values[indices[2]], };
        setVec(data);
        BOOST_CHECK_EQUAL(check(), true);

        ++indices[2];
        if (indices[2] >= n_values)
        {
            indices[2] = 0;
            ++indices[1];
            if (indices[1] >= n_values)
            {
                indices[1] = 0;
                ++indices[0];
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(testFourElements)
{
    int indices[] = { 0, 0, 0, 0, };
    bool running = true;
    while (running)
    {
        // std::copy(indices, indices+4, std::ostream_iterator<int>(std::cout, " "));
        // std::cout << std::endl;
        int64_t data[] = { values[indices[0]], values[indices[1]], values[indices[2]], values[indices[3]], };
        setVec(data);
        BOOST_CHECK_EQUAL(check(), true);

        int d = 3;
        while (d >= 0)
        {
            ++indices[d];
            if (indices[d] >= n_values)
            {
                if (d == 0)
                {
                    running = false;
                }
                indices[d] = 0;
                --d;
            }
            else
            {
                break;
            }
        }
    }
}


//#define BOOST_TEST_MODULE MyTest
//#include <boost/test/unit_test.hpp>
//
//int add(int i, int j) { return i - j; }

//BOOST_AUTO_TEST_CASE(my_test)
//{
//    // seven ways to detect and report the same error:
//    BOOST_CHECK(add(2, 2) == 4);        // #1 continues on error
//
//    BOOST_REQUIRE(add(2, 2) == 4);      // #2 throws on error
//
//    if (add(2, 2) != 4)
//        BOOST_ERROR("Ouch...");            // #3 continues on error
//
//    if (add(2, 2) != 4)
//        BOOST_FAIL("Ouch...");             // #4 throws on error
//
//    if (add(2, 2) != 4) throw "Ouch..."; // #5 throws on error
//
//    BOOST_CHECK_MESSAGE(add(2, 2) == 4,  // #6 continues on error
//        "add(..) result: " << add(2, 2));
//
//    BOOST_CHECK_EQUAL(add(2, 2), 4);   // #7 continues on error
//}


//#include <iostream>
//#include <functional>
//#include <vector>
//#include <algorithm>
//#include <boost/test/unit_test.hpp>

//
//class Kth
//{
//public:
//    Kth() {}
//
//    std::pair<int, bool> getKth(std::function<void(int, int&, int&)> search, int k, int min, int max)
//    {
//        int guess = max;
//        bool found = false;
//
//        while (min <= max) {
//            int lower = 0;
//            int same = 0;
//            search(guess, lower, same);
//            if (k > lower && (k <= lower + same)) { // 猜中了
//                found = true;
//                break;
//            }
//
//            if (k <= lower) {    // 猜大了
//                max = guess;
//            }
//            else if (k > (lower + same)) { // 猜小了
//                min = guess + 1;
//            }
//            guess = min + (max - min) / 2;  // 二分
//        }
//
//        std::pair<int, bool> result(guess, found);
//        return result;
//    }
//
//private:
//    ~Kth() {}   // test 将析构设置为私有，只能派生或者new创建对象
//
//};
//
//static std::vector<int> nums = { 1, 2, 3, 4, 5, 6 };
//
//void search(int guess, int &smaller, int &same)
//{
//    auto lower = std::lower_bound(nums.begin(), nums.end(), guess); // 小于
//    auto upper = std::upper_bound(nums.begin(), nums.end(), guess); // 大于等于
//    smaller = lower - nums.begin();
//    same = upper - lower;
//}
//
//int main(int argc, char *argv[])
//{
//    Kth *kth = new Kth();
//    std::pair<int, bool> result = kth->getKth(search, 3, 1, 6);
//    if (result.second) {
//        std::cout << result.first << std::endl;
//    }
//    else {
//        std::cout << "Not found" << std::endl;
//    }
//    // 1. 根据输入参数，将服务器IP放入vecotr
//    // 2. 创建多个client，连接服务器
//    // 3. rpc查询stat
//    // 4. 求中位数，rpc方式search
//
//    return 0;
//}
