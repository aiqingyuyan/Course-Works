#include "HTMLDownloader.h"
#include <mpi.h>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>

const int MANAGER            = 0;
const int MAX_LENGTH         = 15;
const int MAX_DISTANCE       = 4;
const std::string httpPrefix = "http";

void preProcess(int argc, char** const argv,
                std::vector<std::string>& urls,
                std::vector<std::string>& terms) {
    if (argc < 3) {
        return ;
    }

    for (int i = 1; i < argc; ++i) {
        std::string str(argv[i]);
        if (str.find(httpPrefix) != std::string::npos) {
            urls.push_back(str);
        } else {
            terms.push_back(str);
        }
    }
}

std::vector<std::string> processHTMLFile(const std::string& url) {
    HTMLDownloader htmlDownloader(url);
    // istream pointer which points to the stream object
    std::istream* data = &(htmlDownloader.getText());
    // constructor istream iterator
    std::istream_iterator<std::string> is_iter(*data), eof;
    // copy all strings with length less than or equal to 15 characters
    std::vector<std::string> words;
    std::copy_if(is_iter, eof, std::back_inserter(words),
                 [](const std::string& str) {
                       return str.size() <= MAX_LENGTH;
                 });
    std::transform(words.begin(), words.end(), words.begin(),
                   [](std::string& str) {
                     std::transform(str.begin(), str.end(), str.begin(),
                                    ::tolower);
                     return str;
                   });
    //
    std::vector<char*> _words(words.size());
    for (size_t i = 0; i < _words.size(); ++i) {
        _words[i] = new char[MAX_LENGTH];
        strcpy(_words[i], words[i].data());
    }
    return words;
}

/**
 * function to calculate the Levenshtenin Distance
 * between two strings
 */
int levenshteninDistance(const std::string& str1, const std::string& str2,
                         int length1, int length2) {
    if (length1 == 0 || length2 == 0) {
        return std::max(length1, length2);
    }
    int distanceOne = 1 +
                  levenshteninDistance(str1, str2, length1 - 1, length2);
    int distanceTwo = 1 +
                  levenshteninDistance(str1, str2, length1, length2 - 1);
    int distanceThree = (str1[length1 - 1] == str2[length2 - 1] ? 0 : 1) +
                  levenshteninDistance(str1, str2, length1 - 1, length2 - 1);
    return std::min(std::min(distanceOne, distanceTwo), distanceThree);
}

void processWords(const std::vector<std::string>& words,
                  const std::vector<std::string>& terms) {
    std::vector<int> distanceCount(MAX_DISTANCE + 1, 0);
    for (size_t i = 0; i < terms.size(); ++i) {
        for (size_t j = 0; j < words.size(); ++j) {
            int distance = levenshteninDistance(words[j],
                                                terms[i],
                                                words[j].size(),
                                                terms[i].size());
            if (distance <= MAX_DISTANCE) {
                ++distanceCount[distance];
            }

        }
    }
    std::cout << "Using words: ";
    for_each(terms.begin(), terms.end(),
             [](const std::string& term) {
                 std::cout << term << " ";
             });
    std::cout << "\n";
    for (size_t i = 0; i <= MAX_DISTANCE; ++i) {
        std::cout << "Words with distance=" << i << ": "
                  << distanceCount[i] << "\n";
    }
}

void doWorkerTask(const int myRank, const int numProcs,
                  const std::vector<std::string>& terms) {
    //std::vector<std::string> terms = preProcess(argc, argv);
}

void doManagerTask(const int myRank, const int numProcs,
                   const std::vector<std::string>& urls,
                   const std::vector<std::string>& terms) {
    for (const auto& url : urls) {
        std::vector<std::string> words = processHTMLFile(url);
        std::cout << "URL: " << url << "\n";
        processWords(words, terms);
    }
}

int main(int argc, char* argv[]) {
    int numberOfProcess, myRank;
    MPI_Init(&argc, &argv);
    std::vector<std::string> urls, terms;
    preProcess(argc, argv, urls, terms);
    if (urls.size() == 0 || terms.size() == 0) {
        std::cerr << "cannot process, need at least three arguments\n";
    } else {
      //MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcess);
      //MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

      //if (myRank == MANAGER) {
        doManagerTask(myRank, numberOfProcess, urls, terms);
      //} else {
          //doWorkerTask(myRank, numberOfProcess);
      //}
    }

    MPI_Finalize();
    return 0;

    //processWords(words, terms);
}
