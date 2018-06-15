#include <iostream>
#include <unordered_map>
#include <fstream>
#include <utility>   // std::make_pair
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <memory>
#include <string>   // 包含fsteam >> 的方法

class Sharder
{
public:
    Sharder(int buckets)
    {
        for (int i = 0; i < buckets; ++i) {
            char filename[256];
            snprintf(filename, sizeof filename, "sharder-%05d.txt", i);
            files_.push_back(std::unique_ptr<std::ofstream>(new std::ofstream(filename)));
        }
    }

    void outputToFile(const std::string & data, const int count) {
        int index = std::hash<std::string>()(data) % files_.size();
        *files_[index] << data.c_str() << " " << count << "\n";
    }

private:
    std::vector<std::unique_ptr<std::ofstream>> files_; // 自动释放
};

void shard(int buckets, int inputFileCount, char *files[])
{
    // 新建buckets个输出文件，hash操作
    Sharder sharder(buckets);
    std::unordered_map<std::string, int> wordCount;

    for (int i = 0; i < inputFileCount; ++i) {
        std::ifstream file(files[i]);
        wordCount.clear();

        std::string word;
        while (file >> word) {
            wordCount[word]++;
        }

        for (auto it : wordCount) {
            sharder.outputToFile(it.first, it.second);
        }
        file.close();
    }
}

void sort_shard(int buckets)
{
    char filename[256];
    char data[256];
    std::string str;
    int count = 0;
    typedef std::unordered_map<std::string, int> WordCount;
    WordCount wordCount;
    std::vector<std::pair<int, WordCount::const_iterator>> words;
    for (int i = 0; i < buckets; ++i) {
        snprintf(filename, 256, "sharder-%05d.txt", i);
        std::ifstream file(filename);

        wordCount.clear();
        words.clear();
        
        while (!file.eof()) {
            str.clear();
            file.getline(data, 256);
            str = data;
            if (str != "") {
                size_t split = str.find(" ");
                count = atoi((str.substr(split, str.size() - split)).c_str());
                str = str.substr(0, split);

                wordCount[str] += count;
            }
        }
        file.close();

        for (auto it = wordCount.cbegin(); it != wordCount.cend(); ++it) {
            words.push_back(std::make_pair(it->second, it));
        }
        std::sort(words.begin(), words.end(), [](const auto &lhs, const auto &rhs) {
            return lhs.first > rhs.first;
        });

        std::ofstream outfile(filename);
        for (auto it : words) {
            outfile << it.second->first << " " << it.first << "\n";
        }
        outfile.close();
    }
}

class Source
{
public:
    explicit Source(std::unique_ptr<std::ifstream> file)
        : file_(std::move(file))
        , word_("")
        , count_(0)
    {
    }

    Source(Source &src) {
        this->file_ = std::move(src.file_);
        this->word_ = src.word_;
        this->count_ = src.count_;
    }

    bool next() {
        if (!file_->eof()) {
            *file_ >> word_;
            count_ = strtol(word_.c_str(), NULL, 0);
            word_ = word_.substr(0, word_.find(" "));
            return true;
        }
        return false;
    }

    bool operator<(const Source& rhs) const {
        return count_ < rhs.count_;
    }

    void outputTo(std::ofstream &outfile) const {
        outfile << word_.c_str() << " " << count_ << "\n";
    }

private:
    std::unique_ptr<std::ifstream> file_;
    std::string word_;
    int count_;
};

void merge(int buckets)
{
    // 智能指针管理，自动释放，不用close
    std::vector<std::unique_ptr<std::ifstream>> inputs;   
    std::vector<Source> words;
    for (int i = 0; i < buckets; ++i) {
        char filename[256];
        snprintf(filename, 256, "sharder-%05d.txt", i);
        inputs.push_back(std::unique_ptr<std::ifstream>(new std::ifstream(filename)));
        Source src(std::move(inputs.back()));
        if (src.next()) {
            words.push_back(src);
        }
        ::_unlink(filename);
        // unlink()会删除参数pathname 指定的文件. 
        // 如果该文件名为最后连接点, 但有其他进程打开了此文件, 
        // 则在所有关于此文件的文件描述词皆关闭后才会删除. 
        // 如果参数pathname 为一符号连接, 则此连接会被删除。
    }

    std::ofstream outfile("result.txt");
    std::make_heap(words.begin(), words.end());  // 堆排序
    while (!words.empty()) {
        std::pop_heap(words.begin(), words.end());// 将最大元素放在末尾
        words.back().outputTo(outfile); // 将最大元素输出到文件
       
        if (words.back().next()) { // 从文件取数据
            std::push_heap(words.begin(), words.end());// 重新堆排序
        }
        else {
            words.pop_back();   // 文件结束，清除vector元素
        }
    }
}

int main(int argc, char *argv[])
{
    int buckets = 10;
    char *file[] = { "input1.txt", "input2.txt" };
    shard(buckets, 2, file);
    sort_shard(buckets);
    merge(buckets);
}

//void WordCountFitRAM()
//{
//    std::fstream infile;
//    infile.open("input.txt", std::ios::in);
//
//     单词计数
//    typedef std::unordered_map<std::string, int> WordCount; // 无须排序
//    WordCount counts;
//    char word[256];
//    while (!infile.eof()) {
//        infile >> word;
//        counts[word]++;
//    }
//
//     转存vector
//    std::vector<std::pair<int, WordCount::const_iterator>> freq;
//    freq.reserve(counts.size());
//    for (auto it = counts.cbegin(); it != counts.cend(); ++it) {
//        freq.push_back(std::make_pair(it->second, it));
//    }
//
//     排序
//    std::sort(freq.begin(), freq.end(), [](const auto &lhs, const auto &rhs) {
//        return lhs.first > rhs.first;
//    });
//
//    infile.close();
//}
//
// 放在一个函数里，hashfile有多个如何指定？
//void hasher(std::fstream &infile, std::fstream outfile[], int outfileCnt)
//{
//    char data[256];
//    while (!infile.eof()) {
//        infile >> data;
//        std::size_t hash = std::hash<std::string>()(data) % outfileCnt;
//        outfile[hash] << data << "\n";
//    }
//}
//
//void MergeSort()
//{
//    int a[] = { 4, 100, 3, 6, 5, 123, 111 };
//
//    int i, j;
//    int index = 0;
//    int cnt = sizeof(a) / sizeof(int);
//    int tmp[7];
//    int iEnd, jEnd;
//    for (int step = 1; step <= cnt; step *= 2) {
//        i = 0;
//        j = i + step;
//        iEnd = j;
//        jEnd = iEnd + step;
//        index = 0;
//        while (i < j && j <= cnt) {
//            while (i < iEnd && j < std::min(cnt, jEnd)) {
//                if (a[i] >= a[j]) {
//                    tmp[index++] = a[i];
//                    i++;
//                }
//                else if (a[i] < a[j]) {
//                    tmp[index++] = a[j];
//                    j++;
//                }
//            }
//            while (i < iEnd) {
//                tmp[index++] = a[i];
//                i++;
//            }
//            while (j < std::min(cnt, jEnd)) {
//                tmp[index++] = a[j];
//                j++;
//            }
//            i = j;
//            j = i + step;
//            iEnd = j;
//            jEnd = iEnd + step;
//        }
//        std::swap(a, tmp);    // 最后结果可能swap不到a
//        for (int i = 0; i < cnt; ++i) {
//            a[i] = tmp[i];
//        }
//    }
//}
//
//typedef std::unordered_map<std::string, int> WordCount;
//
//std::string getNumStr(std::fstream &file, char data[]) {
//    char data[256];
//    std::string str("");
//    file.getline(data, 256);
//    if (strcmp(data, "")) {
//        str = data;
//        str = str.substr(str.find(' '), (str.size() - str.find(' ')));
//    }
//
//    return str;
//}
//
//int main(int argc, char *argv[])
//{
//     1. 从一台机器，两个文件读取数据，根据hash值输出到两个文件
//    std::fstream infile1;
//    std::fstream infile2;
//    const int bucketCount = 3;
//    std::fstream hashfile[bucketCount];
//    infile1.open("input1.txt", std::ios::in);
//    infile2.open("input2.txt", std::ios::in);
//
//    char filename[256];
//    for (int i = 0; i < bucketCount; ++i) {
//        sprintf_s(filename, "hashfile%d.txt", i);
//        hashfile[i].open(filename, std::ios::out | std::ios::trunc);
//    }
//
//    hasher(infile1, hashfile, bucketCount);
//    hasher(infile2, hashfile, bucketCount);
//
//     2. 对每个文件进行排序，外部排序？
//    char data[256];
//    WordCount counts;
//    std::vector<std::pair<int, WordCount::const_iterator>> freq;
//    for (int i = 0; i < bucketCount; ++i) {
//        freq.clear();
//        counts.clear();
//        
//         计算单词个数
//        while (!hashfile[i].eof()) {
//            hashfile[i] >> data;
//            counts[data]++;
//        }
//
//         将map转存到vector，以便用来排序
//        freq.reserve(counts.size());
//        for (auto it = counts.cbegin(); it != counts.cend(); ++it) {
//            freq.push_back(std::make_pair(it->second, it));
//        }
//
//         在内存内对文件排序
//        std::sort(freq.begin(), freq.end(), [](const auto &lhs, const auto &rhs) {
//            return lhs.first > rhs.first;
//        });
//
//         关闭文件再新建，清空文件内容
//        hashfile[i].close();
//        sprintf_s(filename, "hashfile%d.txt", i);
//        hashfile[i].open(filename, std::ios::out | std::ios::trunc);
//
//         输出到文件
//        for (auto it = freq.begin(); it != freq.end(); ++it) {
//            hashfile[i] << it->second->first.c_str() << " " << it->first << "\n";
//        }
//    }
//
//     3. 多路归并排序，将文件合并为一个
//    for (int i = 0; i < bucketCount; ++i) {
//        sprintf_s(filename, "hashfile%d.txt", i);
//        hashfile[i].close();
//        hashfile[i].open(filename, std::ios::in);
//    }
//
//    char data1[256];
//    char data2[256];
//    char data3[256];
//    std::fstream resultFile("result.txt", std::ios::out | std::ios::trunc);
//
//    int i = 0, j = 1, k = 3;
//    std::string str1, str2, str3;
//
//    hashfile[i].seekg(0, std::ios::beg);
//    hashfile[j].seekg(0, std::ios::beg);
//    hashfile[k].seekg(0, std::ios::beg);
//    while (!hashfile[i].eof() && !hashfile[j].eof() && !hashfile[k].eof()) {
//        str1 = getNumStr(hashfile[i], data1);
//        str2 = getNumStr(hashfile[j], data2);
//        str3 = getNumStr(hashfile[k], data3);
//
//        while (!hashfile[i].eof()
//            && atoi(str1.c_str()) >= atoi(str2.c_str()) && atoi(str1.c_str()) >= atoi(str3.c_str())) {
//            resultFile << data1 << "\n";
//            str1 = getNumStr(hashfile[i], data1);
//        }
//
//        while (!hashfile[j].eof()
//            && atoi(str2.c_str()) >= atoi(str1.c_str()) && atoi(str2.c_str()) >= atoi(str3.c_str())) {
//            resultFile << data2 << "\n";
//            str2 = getNumStr(hashfile[j], data2);
//        }
//
//        while (!hashfile[k].eof()
//            && atoi(str3.c_str()) >= atoi(str1.c_str()) && atoi(str3.c_str()) >= atoi(str1.c_str())) {
//            resultFile << data3 << "\n";
//            str3 = getNumStr(hashfile[k], data3);
//        }
//    }
//    while (!hashfile[i].eof()) {
//        hashfile[i].getline(data1, 256);
//        resultFile << data1 << "\n";
//    }
//    while (!hashfile[j].eof()) {
//        hashfile[j].getline(data2, 256);
//        resultFile << data2 << "\n";
//    }
//    while (!hashfile[k].eof()) {
//        hashfile[k].getline(data3, 256);
//        resultFile << data3 << "\n";
//    }
//
//     4. 释放资源
//    infile1.close();
//    infile2.close();
//    resultFile.close();
//
//    for (int i = 0; i < bucketCount; ++i) {
//        hashfile[i].close();
//    }
//}