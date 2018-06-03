#include <iostream>
#include <fstream>
#include <map>

void PreData()
{
    // 0. Ԥ������ȡ�ļ�����Ӣ�����µ��ʶ���������ڲ�ͬ�ļ�
    std::fstream inFile;
    std::fstream outFile;
    inFile.open("input.txt", std::ios::in); // ����ģʽ
    outFile.open("output.txt", std::ios::ate | std::ios::app);  // ��λ���ļ�ĩβ | ׷��ģʽ

    char ch;
    bool flag = false;
    while (!inFile.eof()) {
        ch = inFile.get();
        if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z') {
            outFile << ch;
            flag = true;
        }
        else {
            if (flag) {
                outFile << "\n";
            }
            flag = false;
        }
    }

    inFile.close();
    outFile.close();
}

void WordCount(std::map<std::string, int> & wordCount)
{
    std::fstream inFile;
    std::fstream outFile;
    inFile.open("output.txt", std::ios::in);
    outFile.open("result.txt", std::ios::out | std::ios::trunc);  // �ļ���������򸲸�

    // 1. ���㵥�ʸ����������浽�ļ�
    char tmp[256];
    while (!inFile.eof()) {
        inFile.getline(tmp, 256);       // getline Ĭ��ȥ��\n����
        wordCount[std::string(tmp)]++;
    }

    for (auto it : wordCount) {
        outFile << it.first.c_str() << " " << it.second << "\n";
    }

    inFile.close();
    outFile.close();
}

int main(int argc, char *argv[])
{
    //PreData();

    //std::map<std::string, int> wordCount;   // ���ʣ�����
    //WordCount(wordCount);

    //3. ����
    // 3, 1, 5, 2, 4, 6
    // i              j
    // �������򣬴Ӵ�С
    int a[] = { 3, 1, 5, 2, 4, 6 };
    int tmp = a[0];
    int t;

    int i = 0, j = 5;
    int n = 0;
    while (n < 5) {
        i = 0;
        j = 5;
        while (i != j) {
            if (tmp > a[j])
                j--;
            else
                i++;

            if (i == j) {
                t = a[i];
                a[i] = tmp;
                tmp = t;
                break;
            }

            if (tmp < a[i])
                i++;
            else
                j--;

            if (i == j) {
                t = a[i];
                a[i] = tmp;
                tmp = t;
                break;
            }
        } 
        n++;
    }
}