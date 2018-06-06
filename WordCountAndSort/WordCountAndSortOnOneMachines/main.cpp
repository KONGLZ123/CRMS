#include <iostream>
#include <unordered_map>
#include <fstream>
#include <utility>   // std::make_pair
#include <vector>
#include <algorithm>

void WordCountFitRAM()
{
    std::fstream infile;
    infile.open("input.txt", std::ios::in);

    // ���ʼ���
    typedef std::unordered_map<std::string, int> WordCount; // ��������
    WordCount counts;
    char word[256];
    while (!infile.eof()) {
        infile >> word;
        counts[word]++;
    }

    // ת��vector
    std::vector<std::pair<int, WordCount::const_iterator>> freq;
    freq.reserve(counts.size());
    for (auto it = counts.cbegin(); it != counts.cend(); ++it) {
        freq.push_back(std::make_pair(it->second, it));
    }

    // ����
    std::sort(freq.begin(), freq.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.first > rhs.first;
    });

    infile.close();
}

int main(int argc, char *argv[])
{
    // 0. ��һ̨�����������ļ���ȡ����

    // 1. ����hashֵ����������ļ�

    // 2. ��ÿ���ļ���������

    // 3. ��·�鲢���򣬽��ļ��ϲ�Ϊһ��

}