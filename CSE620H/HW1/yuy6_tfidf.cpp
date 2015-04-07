/*
 * Copyright [2014] <Yan Yu>
 */

#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <vector>
#include <map>
#include <sstream>
#include <unordered_map>
#include <string>

using std::cin;
using std::cout;
using std::string;
using std::endl;

void print(string &i) {
    cout << i << endl;
}

void trimTerm(string &str, const string &punctuations) {
    // get rid of the punctuations in the string
    for_each(punctuations.begin(), punctuations.end(),
             [&](char c){str.erase(std::remove(
                                   str.begin(), str.end(), c), str.end());});
    // convert term to lowercase
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

bool readTerms(string &file, std::unordered_map<string, int> &word_count) {
    if (file.empty()) {
        return true;
    }
    const string numbers = "0123456789";
    const string punctuation = ";,.'[]^%$#@()/?\"";
    string term;
    std::ifstream in(file);
    if (!in.good()) return false;
    while ((in >> term)) {
        size_t i;
        for (i = 0; i < term.length(); i++) {
            if (numbers.find(term[i]) != string::npos)
                break;
        }
        if (i == term.length() && term.find("http") != 0) {
            // if string is good, then trim it
            trimTerm(term, punctuation);
            if (word_count.find(term) == word_count.end()) word_count[term] = 1;
            else
               word_count[term]++;
        }
    }
    return true;
}

bool loadCorpus(string corpus, string search_file,
                std::unordered_map<string, int> &word_count_corpus,
                std::unordered_map<string, int> &word_count_search,
                int &num_of_files) {
    cout << "Loading corpus using files listed in " << corpus << endl;
    string file;
    std::ifstream in(corpus);
    if (!in.good()) {
        std::cerr << "unable to open " << corpus << endl;
        return false;
    }
    std::vector<string> files;
    std::copy(std::istream_iterator<string>(in),
              std::istream_iterator<string>(), back_inserter(files));
    num_of_files = files.size();

    // load corpus
    for (auto key : files) {
        if (!readTerms(key, word_count_corpus)) {
            std::cerr << "unable to open " << key << endl;
        }
    }

    // load search file
    readTerms(search_file, word_count_search);

    cout << "Loaded corpus of " << word_count_corpus.size() << " words from "
         << files.size() << " file(s)." << endl;

    // for(auto key : word_count_search) {
    //    cout << key.first << " -> " << key.second << endl;
    // }

    return true;
}

// calculate TF_IDF for each term
void calculateTF_IDF(std::map<double, string> &percentage_word,
                     std::unordered_map<string, int> &word_count_search,
                     std::unordered_map<string, int> &word_count_corpus,
                     int num_of_files) {
    for (auto element : word_count_search) {
        double percentage = static_cast<double>(element.second)
                * (log(num_of_files /
                static_cast<double>(word_count_corpus[element.first])) /
                log(10));
        percentage_word[percentage] = element.first;
        // cout << element.first << endl;
    }
}

void processLines(const string &search_file,
                  const string &significantWords) {
    std::ifstream in(search_file);
    string line;
    int line_num = 1;   // line number start with 1
    cout << "Lines with 2 or more significant words:" << endl;
    while (std::getline(in, line)) {             // get one line
        int num_of_significantWords_in_line = 0;
        // std::istringstream iss(line);
        std::istringstream iss(significantWords);
        string tmp, line_tmp(line);
        // std::transform(line.begin(), line.end(),
        //          line_tmp.begin(), ::tolower);
        while (iss >> tmp) {
          // if(std::find(topSignificantWords.begin(),
          // topSignificantWords.end(), tmp) !=
          // topSignificantWords.end())
          if (line.find(tmp) != string::npos)
              num_of_significantWords_in_line++;
        }
        if (num_of_significantWords_in_line  >= 2) {
            cout << line_num << "["<< num_of_significantWords_in_line << "]:"
               << "\t" << line << endl;
        }
        line_num++;
    }
}

void analyze(std::unordered_map<string, int> &word_count_corpus,
             std::unordered_map<string, int> &word_count_search,
             int numTerm, int num_of_files, string search_file) {
    cout << "------[ Start analysis]------" << endl;

    // calculate the TF-IDF for each term
    std::map<double, string> percentage_word;
    calculateTF_IDF(percentage_word, word_count_search,
                    word_count_corpus, num_of_files);

    cout << "Top " << numTerm << " significant words..." << endl;
    unsigned int j = 0;
    // std::vector<string> topSignificantWords;
    string significantWords = "";
    for (std::map<double, string>::reverse_iterator it =
           percentage_word.rbegin();
           (j < numTerm && it != percentage_word.rend());
           j++, it++) {
        if (it->first == 0.0) continue;
        cout << it->second << ": " << it->first << endl;
        // topSignificantWords.push_back(it->second);
        significantWords += (it->second + " ");
    }
    // for_each(topSignificantWords.begin(), topSignificantWords.end(), print);

    // Process Lines:
    processLines(search_file, significantWords);
}

int main(int argc, char *argv[]) {
    string corpus_file = argv[1];
    string search_file = argv[2];
    int num_of_terms = std::stoi(argv[3]);
    int num_of_files = 0;

    std::unordered_map<string, int> word_count_corpus;
    std::unordered_map<string, int> word_count_search;

    if (!loadCorpus(argv[1], argv[2], word_count_corpus,
         word_count_search, num_of_files))
           return 1;

    analyze(word_count_corpus, word_count_search,
             num_of_terms, num_of_files, search_file);
    // for_each(words_in_search_file.begin(),
    //          words_in_search_file.end(), print);
}
