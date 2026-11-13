# CS 3360 Assignment #5: Real-Time DAG Multiprocessor Scheduling & Parallel Programming

## Project Overview

This project focuses on real-time (DAG) scheduling and multi-threaded parallel programming.

The assignment is has two parts:

- **Part 1 – Real-Time DAG Scheduling:** Analyze a DAG task by calculating its total work, critical path length, deadline feasibility, and processor allocation using Federated Scheduling. Amdahl's Law is also used to analyze theoretical speedup.

- **Part 2 – Multithreaded Parallel Programming:** Implement a C++ program that finds the minimum and maximum values in an array of 1,000,000 randomly generated integers using 1, 2, 4, and 8 threads. Mutex synchronization is used to prevent data races when updating shared variables.

The program measures execution time, calculates measured and theoretical speedup, and evaluates parallel efficiency for each thread configuration.

## C++ Implementation

The program uses multithreading to find the minimum and maximum values in an array of 1,000,000 randomly generated integers ranging from 1 to 1,000,000,000.

### Thread Configuration

The program tests four thread configurations:

- 1 Thread (Baseline)
- 2 Threads
- 4 Threads
- 8 Threads

The array is divided into chunks based on the number of threads. Each thread processes its assigned portion of the array to find the local minimum and maximum values.

### Thread Synchronization

Each thread uses private variables **localMin** and **localMax** to calculate the minimum and maximum values within its assigned chunk.

A mutex **resultMutex** protects the shared variables **globalMin** and **globalMax** when threads update the final results. This prevents data races by allowing only one thread to update the shared values at a time.

The program uses **join()** to wait for all threads to finish before calculating the execution time.

### Performance Measurements

The program measures and calculates the following metrics:

- **Execution Time:** The time required to create the threads, process the array, and wait for all threads to finish.
- **Measured Speedup:** The baseline execution time divided by the execution time of the current thread configuration.
- **Theoretical Speedup:** Calculated using Amdahl's Law with a serial fraction of 5%.
- **Parallel Efficiency:** The measured speedup divided by the number of threads, multiplied by 100%.

Execution time is measured in milliseconds using **chrono::steady_clock**.

