#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>

// 0. Ԥ������ȡ�ļ�����Ӣ�����µ��ʶ���������ڲ�ͬ�ļ�
void PreData()
{
    std::fstream inFile;
    std::fstream outFile;
    inFile.open("input.txt", std::ios::in); // ����ģʽ
    //outFile.open("output.txt", std::ios::ate | std::ios::app);  // ��λ���ļ�ĩβ | ׷��ģʽ
    outFile.open("output.txt", std::ios::trunc | std::ios::out);  // ��λ���ļ�ĩβ | ׷��ģʽ

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

// 1. ���㵥�ʸ����������浽�ļ�
void WordCount(std::map<std::string, int> & wordCount)
{
    std::fstream inFile;
    std::fstream outFile;
    inFile.open("output.txt", std::ios::in);
    outFile.open("result.txt", std::ios::out | std::ios::trunc);  // �ļ���������򸲸�

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

struct WordCount
{
    std::string word;
    int count;
};

// �ݹ���̫�CPU�ķ����أ�
void QuickSort(std::vector<struct WordCount> &vecWordCount, 
    std::vector<struct WordCount>::iterator start, 
    std::vector<struct WordCount>::iterator end)
{
    if (start >= end)    // i - 1�� i����Ϊ0 | i + 1, i����Ϊend
        return;

    int tmp = start->count;
    std::string tmpStr = start->word;
    auto i = start;
    auto j = end - 1;   // vector end��ĩβ�ĺ�һ��
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
    QuickSort(vecWordCount, start, i);     // i - 1�� i����Ϊ0 | i + 1, i����Ϊend
    QuickSort(vecWordCount, i + 1, end);
}

// 2. �����ʳ��ִ������Ӵ�С����
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

    std::map<std::string, int> wordCount;   // ���ʣ�����
    WordCount(wordCount);

    WordCountSort(wordCount);
}