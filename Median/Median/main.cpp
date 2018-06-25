#include <iostream>
#include <functional>

int main(int argc, char *argv[])
{
    int a[] = { 2, 1, 5, 6, 3, 4 };

    int guess = a[0];
    int smaller = 0;
    int equal = 0;
    for (int i = 0; i < 6; ++i) {
        if (guess > a[i])
            smaller++;
        else if (guess == a[i])
            equal++;

        if (smaller < guess && smaller + equal >= guess)
            break;
    }

    // 1. 根据输入参数，将服务器IP放入vecotr
    // 2. 创建多个client，连接服务器
    // 3. rpc查询stat
    // 4. 求中位数，rpc方式search

    return 0;
}

std::pair<int, bool> getKth(std::function<int, int, int> search, int count, int min, int max)
{
    int guess = 0;
    int found = false;
    int lower = 0, same = 0;

    while (min < max) {

        search(guess, lower, same);
        if (guess > lower && guess <= same) {
            found = true;
            break;
        }

    }

    return std::pair<int, bool>(guess, found);
}