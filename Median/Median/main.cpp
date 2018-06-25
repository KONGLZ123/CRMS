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

    // 1. ���������������������IP����vecotr
    // 2. �������client�����ӷ�����
    // 3. rpc��ѯstat
    // 4. ����λ����rpc��ʽsearch

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