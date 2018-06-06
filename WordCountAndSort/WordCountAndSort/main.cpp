#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>

// 0. 预处理：读取文件，讲英文文章单词读出，存放在不同文件
void PreData()
{
    std::fstream inFile;
    std::fstream outFile;
    inFile.open("input.txt", std::ios::in); // 输入模式
    //outFile.open("output.txt", std::ios::ate | std::ios::app);  // 定位到文件末尾 | 追加模式
    outFile.open("output.txt", std::ios::trunc | std::ios::out);  // 定位到文件末尾 | 追加模式

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

// 1. 计算单词个数，并保存到文件
void WordCount(std::map<std::string, int> & wordCount)
{
    std::fstream inFile;
    std::fstream outFile;
    inFile.open("output.txt", std::ios::in);
    outFile.open("result.txt", std::ios::out | std::ios::trunc);  // 文件如果存在则覆盖

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

// 递归层次太深，CPU耗费严重？
void QuickSort(std::vector<struct WordCount> &vecWordCount, 
    std::vector<struct WordCount>::iterator start, 
    std::vector<struct WordCount>::iterator end)
{
    if (start >= end)    // i - 1， i可能为0 | i + 1, i可能为end
        return;

    int tmp = start->count;
    std::string tmpStr = start->word;
    auto i = start;
    auto j = end - 1;   // vector end是末尾的后一个
    while (i < j) {
        while (i < j && j->count <= tmp) {
            j--;
        }
        i->count = j->count;
        i->word = j->word;
        while (i < j && i->count >= tmp) {
            i++;
        }
        j->count = i->count;
        j->word = i->word;
    }
    i->count = tmp;
    i->word = tmpStr;
    QuickSort(vecWordCount, start, i);     // i - 1， i可能为0 | i + 1, i可能为end
    QuickSort(vecWordCount, i + 1, end);
}

// 2. 按单词出现次数，从大到小排序
void WordCountSort(std::map<std::string, int> & wordCount)
{
    std::vector<struct WordCount> vecWordCount;
    struct WordCount tmp;
    for (auto it : wordCount) {
        tmp.word = it.first;
        tmp.count = it.second;
        vecWordCount.push_back(tmp);
    }

    std::fstream outFile;
    outFile.open("result2.txt", std::ios::out | std::ios::trunc);

    QuickSort(vecWordCount, vecWordCount.begin(), vecWordCount.end());

    for (auto it : vecWordCount) {
        outFile << it.count << "  " << it.word.c_str() << "\n";
    }

    outFile.close();
}

int main(int argc, char *argv[])
{
    PreData();

    std::map<std::string, int> wordCount;   // 单词，个数
    WordCount(wordCount);

    WordCountSort(wordCount);
}