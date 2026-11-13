# CS 3360 Assignment #5: Real-Time DAG Multiprocessor Scheduling & Parallel Programming

## Project Overview

This project focuses on real-time (DAG) scheduling and multi-threaded parallel programming.

The assignment is has two parts:

- **Part 1 – Real-Time DAG Scheduling:** Analyze a DAG task by calculating its total work, critical path length, deadline, and processor allocation. Amdahl's Law is used to analyze theoretical speedup.

- **Part 2 – Multithreaded Parallel Programming:** Implement a C++ program that finds the minimum and maximum values in an array of 1,000,000 randomly generated integers using 1, 2, 4, and 8 threads. Mutex synchronization is used to prevent data races when updating shared variables.

The program measures execution time, calculates measured and theoretical speedup, and evaluates parallel efficiency for each thread count.

## C++ Implementation

The program uses multithreading to find the minimum and maximum values in an array of 1,000,000 randomly generated integers ranging from 1 to 1,000,000,000.

### Thread Configuration

The program tests four thread configurations:

- 1 Thread (Baseline)
- 2 Threads
- 4 Threads
- 8 Threads

### Thread Synchronization

Each thread uses private variables **localMin** and **localMax** to calculate the minimum and maximum values within its assigned part.

A mutex **resultMutex** protects the shared variables **globalMin** and **globalMax** when threads update the final results. This prevents data races by allowing only one thread to update the shared values at a time.

The program uses **join()** to wait for all threads to finish before calculating the execution time.

### Performance Measurements

The program measures and calculates the following metrics:

- **Execution Time:** The time required to create the threads, process the array, and wait for all threads to finish.
- **Measured Speedup:** The baseline execution time divided by the execution time of the current thread count 1/2/4/8.
- **Theoretical Speedup:** Calculated using Amdahl's Law with a serial fraction of 5%.
- **Parallel Efficiency:** The measured speedup divided by the number of threads, multiplied by 100%.

Execution time is measured in milliseconds using **chrono::steady_clock**.

## Compilation and Execution

The program is written in C++ and uses the standard threading library.

### Running with Code::Blocks

1. Open Code::Blocks.
2. Open the project containing **main.cpp(2.0)**.
3. Select **Build and Run** (F9).
4. The program will display the benchmarking results in the console.

### Program Output

The program displays a benchmarking table containing:

- Number of threads
- Execution time in milliseconds
- Measured speedup
- Theoretical Amdahl speedup
- Parallel efficiency

The execution times may vary between runs depending on system performance.


## Benchmarking Results

The program was tested using 1, 2, 4, and 8 threads to compare execution time, measured speedup, theoretical speedup, and parallel efficiency.

## Benchmarking Summary

**Array Size:** 1,000,000 Elements

| Threads | Execution Time (ms) | Measured Speedup | Theoretical Amdahl (S=5%) | Efficiency (%) |
|---|---:|---:|---:|---:|
| 1 (Baseline) | 2.95 ms | 1.00x | 1.00x | 100.00% |
| 2 Threads | 1.82 ms | 1.62x | 1.90x | 80.89% |
| 4 Threads | 1.18 ms | 2.49x | 3.48x | 62.30% |
| 8 Threads | 1.35 ms | 2.19x | 5.93x | 27.39% |

The results show that execution time decreased as the thread count increased from 1 to 4 threads. However, the 8-thread had a higher execution time than the 4-thread.

## Task 2.1: Data Race Analysis

The original pseudocode contains a data race since multiple threads can access and update the shared variables global_min and global_max at the same time. To stop this, a mutex protects the shared variables when each thread updates the final results, ensuring only one thread can modify them at a time.

## Part 1: Real-Time DAG Scheduling

### Task 1.1: DAG Analysis

**Given in Assignment 5 Description:**

- Deadline: D₁ = 15 ms
- Period: T₁ = 20 ms
- Node execution times:
  - v₁ = 3 ms
  - v₂ = 4 ms
  - v₃ = 6 ms
  - v₄ = 5 ms
  - v₅ = 3 ms
  - v₆ = 4 ms
  - v₇ = 2 ms

#### 1. Total Work (C₁)

The total work is the sum of all node execution times.


C_1 = 3 + 4 + 6 + 5 + 3 + 4 + 2


C_1 = 27ms

#### 2. Critical Path Length (L₁)

The critical path is the longest execution path through the DAG.

| Path | Calculation | Length |
|---|---|---|
| v₁ → v₂ → v₅ → v₇ | 3 + 4 + 3 + 2 | 12 ms |
| v₁ → v₃ → v₅ → v₇ | 3 + 6 + 3 + 2 | 14 ms |
| v₁ → v₃ → v₆ → v₇ | 3 + 6 + 4 + 2 | 15 ms |
| v₁ → v₄ → v₆ → v₇ | 3 + 5 + 4 + 2 | 14 ms |

The longest path is 15 ms

#### 3. Deadline Feasibility

For the DAG task to meet its deadline, the critical path length must not exceed the deadline.

The necessary condition is satisfied because the critical path length equals the deadline. However, this does not mean that the entire DAG can be scheduled successfully.

#### 4. High Workload

A DAG task is classified as high workload when **C₁ / D₁ > 1**. Since 27 / 15 = 1.8 and **1.8 > 1**. The DAG is classified as a high-workload task.

#### 5. Federated Scheduling

The number of dedicated cores required under Federated Scheduling is calculated using:

**m₁ = ceil((C₁ - L₁) / (D₁ - L₁))**

Substituting the values:

m₁ = ceil((27 - 15) / (15 - 15))

m₁ = ceil(12 / 0)

Since the denominator is zero, the formula does not produce a finite number of cores.

In this special case, the formula is valid only when the total work equals the critical path length (C₁ = L₁).

Since C₁ = 27 ms and L₁ = 15 ms, the task is **not schedulable under the specified Federated Scheduling policy**.


### Task 1.2: Amdahl's Law

Amdahl's Law calculates the theoretical speedup of a program based on how much of the program must run sequentially and how much can run in parallel.

**Given:**

- S = 0.20 (20% of the program must run sequentially)
- 1 - S = 0.80 (80% of the program can run in parallel)
- N = m₁ (Number of processors)

#### 1. Theoretical Speedup

Amdahl's Law is:

**Speedup(N) = 1 / (S + (1 - S) / N)**

**Putting in the Values:** Speedup(N) = 1 / (0.20 + 0.80 / N)

Since the Federated Scheduling calculation did not produce a finite value for m₁, the theoretical speedup for N = m₁ cannot be determined.

#### 2. Maximum Theoretical Speedup

As the number of processors approaches infinity (N → ∞):

**Maximum Speedup = 1 / S**

**Putting in:** S = 0.20:

Maximum Speedup = 1 / 0.20

**Maximum Speedup = 5.00x**

#### 3. Explanation

Adding more processors reduces the time needed for the parallel part but does not reduce the time required for the sequential part. So this theoretical limit does not change the fact that the DAG task is not schedulable under the specified Federated Scheduling policy.








