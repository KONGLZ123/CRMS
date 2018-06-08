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

// ����һ�������hashfile�ж�����ָ����
void hasher(std::fstream &infile, std::fstream outfile[], int outfileCnt)
{
    char data[256];
    while (!infile.eof()) {
        infile >> data;
        std::size_t hash = std::hash<std::string>()(data) % outfileCnt;
        outfile[hash] << data << "\n";
    }
}

typedef std::unordered_map<std::string, int> WordCount;

int main(int argc, char *argv[])
{
    // 1. ��һ̨�����������ļ���ȡ���ݣ�����hashֵ����������ļ�
    std::fstream infile1;
    std::fstream infile2;
    const int bucketCount = 3;
    std::fstream hashfile[bucketCount];
    infile1.open("input1.txt", std::ios::in);
    infile2.open("input2.txt", std::ios::in);

    char filename[256];
    for (int i = 0; i < bucketCount; ++i) {
        sprintf_s(filename, "hashfile%d.txt", i);
        hashfile[i].open(filename, std::ios::out | std::ios::trunc);
    }

    hasher(infile1, hashfile, bucketCount);
    hasher(infile2, hashfile, bucketCount);

    // 2. ��ÿ���ļ����������ⲿ����
    char data[256];
    WordCount counts;
    std::vector<std::pair<int, WordCount::const_iterator>> freq;
    for (int i = 0; i < bucketCount; ++i) {
        freq.clear();
        counts.clear();
        
        // ���㵥�ʸ���
        while (!hashfile[i].eof()) {
            hashfile[i] >> data;
            counts[data]++;
        }

        // ��mapת�浽vector���Ա���������
        freq.reserve(counts.size());
        for (auto it = counts.cbegin(); it != counts.cend(); ++it) {
            freq.push_back(std::make_pair(it->second, it));
        }

        // ���ڴ��ڶ��ļ�����
        std::sort(freq.begin(), freq.end(), [](const auto &lhs, const auto &rhs) {
            return lhs.first > rhs.first;
        });

        // �ر��ļ����½�������ļ�����
        hashfile[i].close();
        sprintf_s(filename, "hashfile%d.txt", i);
        hashfile[i].open(filename, std::ios::out | std::ios::trunc);

        // ������ļ�
        for (auto it = freq.begin(); it != freq.end(); ++it) {
            hashfile[i] << it->second->first.c_str() << " " << it->first << "\n";
        }
    }

    // 3. ��·�鲢���򣬽��ļ��ϲ�Ϊһ��
    int a[] = { 4, 100, 3, 6, 5, 123, 111 };

    int i, j;
    
    int index = 0;
    int cnt = sizeof(a) / sizeof(int);
    int tmp[7];
    int iEnd, jEnd;
    for (int step = 1; step <= cnt; step *= 2) {
        i = 0;
        j = i + step;
        iEnd = j;
        jEnd = iEnd + step;
        index = 0;
        while (i < j && j <= cnt) {
            while (i < iEnd && j < std::min(cnt, jEnd)) {
                if (a[i] >= a[j]) {
                    tmp[index++] = a[i];
                    i++;
                }
                else if (a[i] < a[j]) {
                    tmp[index++] = a[j];
                    j++;
                }
            }
            while (i < iEnd) {
                tmp[index++] = a[i];
                i++;
            }
            while (j < std::min(cnt, jEnd)) {
                tmp[index++] = a[j];
                j++;
            }
            i = j;
            j = i + step;
            iEnd = j;
            jEnd = iEnd + step;
        }
        //std::swap(a, tmp);    // ���������swap����a
        for (int i = 0; i < cnt; ++i) {
            a[i] = tmp[i];
        }
    }

    // 4. �ͷ���Դ
    infile1.close();
    infile2.close();

    for (int i = 0; i < bucketCount; ++i) {
        hashfile[i].close();
    }
}