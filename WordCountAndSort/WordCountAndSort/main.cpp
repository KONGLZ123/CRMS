#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>

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

struct WordCount
{
    std::string word;
    int count;
};

void QuickSort(int *a, int start, int end)
{
    if (start == end)
        return;

    int tmp = a[start];
    int i = start;
    int j = end - 1;
    while (i != j) {

    }
    a[i] = tmp;
    QuickSort(a, start, i - 1);
    QuickSort(a, i + 1, end);
}

int main(int argc, char *argv[])
{
    //PreData();

    //std::map<std::string, int> wordCount;   // 单词，个数
    //WordCount(wordCount);
    //
    std::vector<struct WordCount> vecWordCount;
    //struct WordCount tmp;
    //for (auto it : wordCount) {
    //    tmp.word = it.first;
    //    tmp.count = it.second;
    //    vecWordCount.push_back(tmp);
    //}

    std::fstream outFile;
    outFile.open("result2.txt", std::ios::out | std::ios::trunc);

    struct WordCount tmp;
    tmp.count = 3;
    tmp.word = "C";
    vecWordCount.push_back(tmp);
    tmp.count = 1;
    tmp.word = "A";
    vecWordCount.push_back(tmp);
    tmp.count = 4;
    tmp.word = "D";
    vecWordCount.push_back(tmp);
    tmp.count = 2;
    tmp.word = "B";
    vecWordCount.push_back(tmp);

    int count = vecWordCount.size();
    auto i = vecWordCount.begin(), j = vecWordCount.end();
    int n = 0;
    std::string tmpWord = vecWordCount[0].word;
    int tmpCnt = vecWordCount[0].count;
    int t = 0;
    std::string tmpStr;
    
    //while (n < count) {
    //    i = vecWordCount.begin();
    //    j = vecWordCount.end() - 1;
    //    while (i != j) {
    //        if (j->count <= tmpCnt) {
    //            j--;
    //        }
    //        else {
    //            i++;
    //        }

    //        if (i == j) {
    //            break;
    //        }

    //        if (i->count >= tmpCnt) {
    //            i++;
    //        }
    //        else {
    //            j--;
    //        }

    //        if (i == j) {
    //            break;
    //        }
    //    }

    //    t = vecWordCount[i - vecWordCount.begin()].count;
    //    vecWordCount[i - vecWordCount.begin()].count = tmpCnt;
    //    tmpCnt = t;

    //    tmpStr = vecWordCount[i - vecWordCount.begin()].word;
    //    vecWordCount[i - vecWordCount.begin()].word = tmpWord;
    //    tmpWord = tmpStr;
    //    n++;
    //}

    for (auto it : vecWordCount) {
        outFile << it.count << "  " << it.word.c_str() << "\n";
    }

    outFile.close();
    //3. 排序
    // 3, 1, 5, 2, 4, 6
    // i              j
    // 快速排序，从大到小
    int a[] = { 3, 1, 4, 2, 2, 6, 8, 11, 3};
    int cnt = sizeof(a) / sizeof(int);
    QuickSort(a, 0, cnt);
    for (int i = 0; i < cnt; ++i) {
        std::cout << a[i] << std::endl;
    }
}