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

    // 单词计数
    typedef std::unordered_map<std::string, int> WordCount; // 无须排序
    WordCount counts;
    char word[256];
    while (!infile.eof()) {
        infile >> word;
        counts[word]++;
    }

    // 转存vector
    std::vector<std::pair<int, WordCount::const_iterator>> freq;
    freq.reserve(counts.size());
    for (auto it = counts.cbegin(); it != counts.cend(); ++it) {
        freq.push_back(std::make_pair(it->second, it));
    }

    // 排序
    std::sort(freq.begin(), freq.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.first > rhs.first;
    });

    infile.close();
}

int main(int argc, char *argv[])
{
    // 0. 从一台机器，两个文件读取数据

    // 1. 根据hash值输出到两个文件

    // 2. 对每个文件进行排序

    // 3. 二路归并排序，将文件合并为一个

}