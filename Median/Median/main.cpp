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
            if (k > lower && (k <= lower + same)) { // ������
                found = true;
                break;
            }

            if (k <= lower) {    // �´���
                max = guess;
            }
            else if (k > (lower + same)) { // ��С��
                min = guess + 1;
            }
            guess = min + (max - min) / 2;  // ����
        }

        std::pair<int, bool> result(guess, found);
        return result;
    }

//private:
    //~Kth() {}   // test ����������Ϊ˽�У�ֻ����������new��������

};

static std::vector<int> nums = { 1, 2, 3, 4, 5, 6 };

void search(int guess, int &smaller, int &same)
{
    auto lower = std::lower_bound(nums.begin(), nums.end(), guess); // С��
    auto upper = std::upper_bound(nums.begin(), nums.end(), guess); // ���ڵ���
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
    // 1. ���������������������IP����vecotr
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

    // 2. �������client�����ӷ�����

    // 3. ����protobuf, rpc��ѯstat

    // 4. ����λ����rpc��ʽsearch

    return 0;
}
