#include <iostream>
#include <functional>

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

            if (k < lower) {    // 猜大了
                max = guess;
            }
            else if (k >= (lower + same)) { // 猜小了
                min = min + 1;
            }
            guess = min + (max - min) / 2;  // 二分
        }

        std::pair<int, bool> result(guess, found);
        return result;
    }

private:
    ~Kth() {}   // 将析构设置为私有，只能派生或者new创建对象

};

//class kk : public Kth
//{
//public:
//    kk(){}
//    ~kk() {}
//};

void search(int guess, int &smaller, int &same)
{

}

int main(int argc, char *argv[])
{
    int a[] = { 2, 1, 5, 6, 3, 4 };

    Kth *kth = new Kth();
    kth->getKth(search, 3, 1, 6);

    // 1. 根据输入参数，将服务器IP放入vecotr
    // 2. 创建多个client，连接服务器
    // 3. rpc查询stat
    // 4. 求中位数，rpc方式search

    return 0;
}
