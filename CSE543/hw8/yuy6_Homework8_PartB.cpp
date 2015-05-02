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

std::vector<char> processHTMLFile(const std::string& url, long& numOfBytes) {
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
    // convert vector of string to a flat buffer
    numOfBytes = words.size() * MAX_LENGTH;
    std::cout << "start converting: " << numOfBytes << " chars\n";
    std::vector<char> flatCharBuff(numOfBytes);
    for (size_t i = 0; i < words.size(); ++i) {
        if (words[i].size() == MAX_LENGTH) {
            for (size_t j = 0; j < MAX_LENGTH; ++j) {
                flatCharBuff[i * MAX_LENGTH + j] = words[i][j];
            }
        } else {
            size_t j = 0;
            for (; j < words[i].size(); ++j) {
                flatCharBuff[i * MAX_LENGTH + j] = words[i][j];
            }

            while (j < MAX_LENGTH) {
                flatCharBuff[i * MAX_LENGTH + j] = '\0';
                ++j;
            }
        }
    }
    std::cout << "end converting\n";

    return flatCharBuff;
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

std::vector<int> processWords(const int rank, const int numProcs,
                  const std::vector<std::string>& words,
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
    return distanceCount;
}

std::vector<std::string>
convertBuffersToStrings(const std::vector<char>& buffers,
                        const int myRank,
                        const int numProcs,
                        const long numOfBytes) {
    std::vector<std::string> words;
    int blockSize = numOfBytes / (MAX_LENGTH * numProcs);
    // each process handle part of the flat buffer
    size_t startIdx = myRank * blockSize * MAX_LENGTH;
    size_t endIdx;
    if (myRank < numProcs - 1) {
        endIdx   = startIdx + (blockSize * MAX_LENGTH);
    } else {
        endIdx   = numOfBytes;
    }
    std::cout << "In rank " << myRank << " start converting buffers\n";
    // convert flat buffer to vector of strings
    while (startIdx < endIdx) {
        std::string str(buffers.begin() + startIdx,
                        buffers.begin() + MAX_LENGTH);
        words.push_back(str);
        startIdx += MAX_LENGTH;
    }
    std::cout << "In rank " << myRank << " finish converting buffers\n";
    return words;
}

void doWorkerTask(const int myRank, const int numProcs,
                  const std::vector<std::string>& terms) {
    //std::vector<std::string> terms = preProcess(argc, argv);
    long numOfBytes = 0;
    do {
        MPI_Bcast(&numOfBytes, 1, MPI_LONG, 0, MPI_COMM_WORLD);
        if (numOfBytes != -1) {
            std::vector<char> flatBuffer(numOfBytes);
            std::cout << "In rank " << myRank << " start receiving buffers\n";
            MPI_Bcast(&flatBuffer[0], numOfBytes, MPI_CHAR, 0,
                      MPI_COMM_WORLD);
            // each process handle part of the flat buffers
            // first convert that part of buffers to vector of strings
            std::vector<std::string> words = convertBuffersToStrings(flatBuffer,
                                                  myRank, numProcs, numOfBytes);
            std::vector<int> totalDistanceCount(MAX_DISTANCE + 1, 0);
            std::vector<int> distanceCount = processWords(myRank, numProcs,
                                                  words, terms);
            MPI_Reduce(&distanceCount[0], &totalDistanceCount[0],
                       totalDistanceCount.size(), MPI_INT, MPI_SUM, MANAGER,
                       MPI_COMM_WORLD);
        }
    } while(numOfBytes != -1);
}

void doManagerTask(const int myRank, const int numProcs,
                   const std::vector<std::string>& urls,
                   const std::vector<std::string>& terms) {
    long numOfBytes = 0;
    for (const auto& url : urls) {
        std::vector<char> flatBuffer = processHTMLFile(url, numOfBytes);
        std::cout << "URL: " << url << "\n";
        MPI_Bcast(&numOfBytes, 1, MPI_LONG, 0, MPI_COMM_WORLD);
        std::cout << "end sending numOfBytes: " << numOfBytes << "\n";
        MPI_Bcast(&flatBuffer[0], flatBuffer.size(), MPI_CHAR, 0,
                  MPI_COMM_WORLD);
        std::cout << "end sending flat buffer\n";
        std::vector<std::string> words = convertBuffersToStrings(flatBuffer,
                                              myRank, numProcs, numOfBytes);
        std::vector<int> totalDistanceCount(MAX_DISTANCE + 1, 0);
        std::vector<int> distanceCount = processWords(myRank, numProcs,
                                              words, terms);
        std::cout << "start reducing\n";
        MPI_Reduce(&distanceCount[0], &totalDistanceCount[0],
                   totalDistanceCount.size(), MPI_INT, MPI_SUM, MANAGER,
                   MPI_COMM_WORLD);
        std::cout << "finish reducing\n";
    }
    numOfBytes = -1;
    MPI_Bcast(&numOfBytes, 1, MPI_LONG, 0, MPI_COMM_WORLD);
}

int main(int argc, char* argv[]) {
    int numberOfProcess, myRank;
    MPI_Init(&argc, &argv);
    std::vector<std::string> urls, terms;
    preProcess(argc, argv, urls, terms);
    if (urls.size() == 0 || terms.size() == 0) {
        std::cerr << "cannot process, need at least three arguments\n";
    } else {
        MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcess);
        MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

        if (myRank == MANAGER) {
            doManagerTask(myRank, numberOfProcess, urls, terms);
        } else {
            doWorkerTask(myRank, numberOfProcess, terms);
        }
    }

    MPI_Finalize();
    return 0;

    //processWords(words, terms);
}
