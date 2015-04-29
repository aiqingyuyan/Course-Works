#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <mpi.h>

const int NUMBER_TAG  = 1;
const int FACTORS_TAG = 2;

const int MANAGER_RANK = 0;

/* bool preProcess(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Input format: ./program <file name>\n";
        return false;
    }
    return true;
} */

/**
   This is a simple function that can be used to determine the number
   of factors for a given integer i.
*/
int getFactorCount(const long long n) {
    long long i = 0;
    int factorCount = 0;
    
    for(i = 1; (i <= n); i++) {
        if (!(n % i)) {
            factorCount++;
        }
    }
    return factorCount;
}

void doWorkerTasks() {
    long long number;
    do {
        /* read number from the manager (rank == 0) */
        MPI_Recv(&number, 1, MPI_LONG_LONG_INT, MANAGER_RANK, NUMBER_TAG,
                 MPI_COMM_WORLD, NULL);
        if (number != -1) {
            int factors = getFactorCount(number);
            /* send number of factors back to the manager */
            MPI_Send(&factors, 1, MPI_INT, MANAGER_RANK, FACTORS_TAG,
                     MPI_COMM_WORLD);
        }
    } while(number != -1);
}

void doManagerTasks(const std::string& file, const int numberOfWorkers) {
    std::ifstream data(file);
    if (!data.good()) {
        std::cout << "Unable to open numbers.txt for input. "
                  << "Manager aborting.\n";
    } else {
        std::vector<int> worker_value(numberOfWorkers);
        std::vector<long long> numbers;
        std::istream_iterator<long long> if_iter(data), eof;
        std::copy(if_iter, eof, std::back_inserter(numbers));
        size_t Bound = numberOfWorkers < numbers.size() ?
                       numberOfWorkers : numbers.size();
        size_t i = 0, worker;
        // std::cout << "Bound: " << Bound << "\n";
        for (worker = 1; i < Bound; ++i, ++worker) {
            // std::cout << "worker rank: " << worker << "\n";
            MPI_Send(&numbers[i], 1, MPI_LONG_LONG_INT, worker, NUMBER_TAG,
                     MPI_COMM_WORLD);
            // keep track of current value for the worker
            worker_value[worker - 1] = numbers[i];
        }
        //
        MPI_Status status;
        int factors;
        for (; i < numbers.size(); ++i) {
            MPI_Recv(&factors, 1, MPI_INT, MPI_ANY_SOURCE, FACTORS_TAG,
                     MPI_COMM_WORLD, &status);
            worker = status.MPI_SOURCE;
            // print results
            std::cout << "Number " << worker_value[worker - 1] << " has "
                      << factors << " factors.\n";
            MPI_Send(&numbers[i], 1, MPI_LONG_LONG_INT, worker, NUMBER_TAG,
                     MPI_COMM_WORLD);
            worker_value[worker - 1] = numbers[i];
        }
    }
    // send -1 to worker to inform worker to end processing
    long long number = -1;
    for (int worker = 1; worker <= numberOfWorkers; ++worker) {
        MPI_Send(&number, 1, MPI_LONG_LONG_INT, worker, NUMBER_TAG,
                 MPI_COMM_WORLD);
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int numberOfProcess = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcess);
    if (numberOfProcess < 2) {
        std::cerr << "This program must be run with at least 2 processes!\n";
    } else {
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        switch(rank) {
            case 0: doManagerTasks(argv[1], numberOfProcess - 1); break;
            default: doWorkerTasks(); break;
        }
    }
    MPI_Finalize(); 
    return 0;
}