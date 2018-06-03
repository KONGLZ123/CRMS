#include <iostream>
#include <fstream>
#include <map>

void PreData()
{
    // 0. 预处理：读取文件，讲英文文章单词读出，存放在不同文件
    std::fstream inFile;
    std::fstream outFile;
    inFile.open("input.txt", std::ios::in); // 输入模式
    outFile.open("output.txt", std::ios::ate | std::ios::app);  // 定位到文件末尾 | 追加模式

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
    outFile.open("result.txt", std::ios::out | std::ios::trunc);  // 文件如果存在则覆盖

    // 1. 计算单词个数，并保存到文件
    char tmp[256];
    while (!inFile.eof()) {
        inFile.getline(tmp, 256);       // getline 默认去除\n换行
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

    //std::map<std::string, int> wordCount;   // 单词，个数
    //WordCount(wordCount);

    //3. 排序
    // 3, 1, 5, 2, 4, 6
    // i              j
    // 快速排序，从大到小
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