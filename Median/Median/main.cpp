#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <boost/test/unit_test.hpp>
#include <string>

class Kth
{
public:
    Kth() {}

    std::pair<int, bool> getKth(std::function<void(int, int&, int&)> search, int k, int min, int max)
    {
        int guess = max;
        bool found = false;

        while (min <= max) {
            int lower = 0;
            int same = 0;
            search(guess, lower, same);
            if (k > lower && (k <= lower + same)) { // 猜中了
                found = true;
                break;
            }

            if (k <= lower) {    // 猜大了
                max = guess;
            }
            else if (k > (lower + same)) { // 猜小了
                min = guess + 1;
            }
            guess = min + (max - min) / 2;  // 二分
        }

        std::pair<int, bool> result(guess, found);
        return result;
    }

//private:
    //~Kth() {}   // test 将析构设置为私有，只能派生或者new创建对象

};

static std::vector<int> nums = { 1, 2, 3, 4, 5, 6 };

void search(int guess, int &smaller, int &same)
{
    auto lower = std::lower_bound(nums.begin(), nums.end(), guess); // 小于
    auto upper = std::upper_bound(nums.begin(), nums.end(), guess); // 大于等于
    smaller = lower - nums.begin();
    same = upper - lower;
}

class TcpServer
{
public:
    TcpServer() {
        // socket
        // bind
        // listen
        // accept
        // w/r
    }

    bool listen() {

    }

private:
    int fd_;
};

class TcpClient
{
public:
    TcpClient(std::string ip, size_t port) {
        // socket
        // connect
        // w/r
    }

    bool connect() {

    }

    bool query() {

    }

    bool search() {

    }

private:
    int fd_;
};

int main(int argc, char *argv[])
{
    Kth *kth = new Kth();
    std::pair<int, bool> result = kth->getKth(search, 3, 1, 6);
    if (result.second) {
        std::cout << result.first << std::endl;
    }
    else {
        std::cout << "Not found" << std::endl;
    }
    // 1. 根据输入参数，将服务器IP放入vecotr
    std::string ip;
    size_t port;
    std::vector<std::pair<std::string, int>> ipPort;
    for (int i = 1; i < argc; ++i) {
        std::string str = argv[i];
        ip = str.substr(0, str.find(':'));
        port = static_cast<size_t>(atoi(str.substr(str.find(':'), str.length() - str.find(':')).c_str()));
        std::pair<std::string, int> pair(ip, port);
        ipPort.push_back(pair);
    }

    // 2. 创建多个client，连接服务器

    // 3. 链接protobuf, rpc查询stat

    // 4. 求中位数，rpc方式search

    return 0;
}
