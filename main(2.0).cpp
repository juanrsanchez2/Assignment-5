
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
#include <chrono>
#include <climits>
#include <iomanip>
#include <random>
using namespace std;

const int ARRAY_SIZE = 1000000;
const int MAX_VALUE = 1000000000;

int arr[ARRAY_SIZE];

int globalMin = INT_MAX;
int globalMax = INT_MIN;

mutex resultMutex;

// Calculates the minimum and maximum values in a thread assigned chunk
void workerThread(int threadId, int start, int end) {
    int localMin = INT_MAX;
    int localMax = INT_MIN;

    // Searches the assigned chunk for the minimum and maximum value
    for (int i = start; i < end; i++) {
        if (arr[i] < localMin) {
            localMin = arr[i];
        }

        if (arr[i] > localMax) {
            localMax = arr[i];
        }
    }

    // Lock the mutex before updating the shared minimum and maximum
    resultMutex.lock();

    if (localMin < globalMin) {
        globalMin = localMin;
    }

    if (localMax > globalMax) {
        globalMax = localMax;
    }

    resultMutex.unlock();
}

int main() {

    // Generate one million random integer between 1 and 1 billion
    mt19937 generator(time(0));
    uniform_int_distribution<int> distribution(1, MAX_VALUE);

    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = distribution(generator);
    }

    // Store the thread counts and their execution time
    int threadCounts[4] = {1, 2, 4, 8};
    double executionTimes[4];

    // Displays the benchmarking table header
    cout << "### Parallel Array Reduction Benchmarking Summary (N = 1,000,000 Elements)\n\n";

    cout << left << setw(13) << "Threads" << " | "
         << right << setw(19) << "Execution Time (ms)" << " | "
         << setw(16) << "Measured Speedup" << " | "
         << setw(25) << "Theoretical Amdahl (S=5%)" << " | "
         << setw(14) << "Efficiency (%)" << endl;

    cout << "--------------|---------------------|------------------|---------------------------|---------------\n";

    cout << fixed << setprecision(2);

    // Runs the benchmark using 1, 2, 4, and 8 thread
    for (int t = 0; t < 4; t++) {
        int numThreads = threadCounts[t];
        int chunkSize = ARRAY_SIZE / numThreads;

        // Resets the shared minimum and maximum before each run
        globalMin = INT_MAX;
        globalMax = INT_MIN;

        thread workers[8];

        // Starts measuring the execution time
        auto startTime = chrono::steady_clock::now();

        // Creates threads and assigns each one a portion of the array
        for (int i = 0; i < numThreads; i++) {
            int start = i * chunkSize;
            int end = start + chunkSize;


            if (i == numThreads - 1) {
                end = ARRAY_SIZE;
            }

            workers[i] = thread(workerThread, i, start, end);
        }

        // Waits for all threads to finish processing
        for (int i = 0; i < numThreads; i++) {
            workers[i].join();
        }

        // store the execution time in milliseconds
        auto endTime = chrono::steady_clock::now();

        chrono::duration<double, milli> elapsedTime = endTime - startTime;
        executionTimes[t] = elapsedTime.count();

        // Calculates measured speedup, theoretical speedup, and efficiency
        double measuredSpeedup = executionTimes[0] / executionTimes[t];
        double theoreticalSpeedup = 1.0 / (0.05 + 0.95 / numThreads);
        double efficiency = measuredSpeedup / numThreads * 100;

        // Displays the benchmarking results for the current thread count
        cout << left << setw(13) << (numThreads == 1 ? "1 (Baseline)" : to_string(numThreads) + " Threads") << " | "
             << right << setw(16) << executionTimes[t] << " ms | "
             << setw(15) << measuredSpeedup << "x | "
             << setw(24) << theoreticalSpeedup << "x | "
             << setw(13) << efficiency << "%" << endl;
    }

    return 0;
}
