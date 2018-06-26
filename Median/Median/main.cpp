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
            if (k > lower && (k <= lower + same)) { // ������
                found = true;
                break;
            }

            if (k < lower) {    // �´���
                max = guess;
            }
            else if (k >= (lower + same)) { // ��С��
                min = min + 1;
            }
            guess = min + (max - min) / 2;  // ����
        }

        std::pair<int, bool> result(guess, found);
        return result;
    }

private:
    ~Kth() {}   // ����������Ϊ˽�У�ֻ����������new��������

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

    // 1. ���������������������IP����vecotr
    // 2. �������client�����ӷ�����
    // 3. rpc��ѯstat
    // 4. ����λ����rpc��ʽsearch

    return 0;
}
