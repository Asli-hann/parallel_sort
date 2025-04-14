# COMP 464 Final Project: Improving MergeSort and QuickSort

## 📌 Overview

This project explores the optimization of two classical sorting algorithms — **Merge Sort** and **Quick Sort** — through **parallelization** and **cache analysis**. The primary objective is to improve runtime performance on large datasets and evaluate the **memory efficiency** of each algorithm using tools like **OpenMP** and **Cachegrind**.

---

## 🎯 Project Goals

- Implement parallel versions of Merge Sort and Quick Sort using **OpenMP**.
- Measure and compare their **runtime performance** across multiple thread counts.
- Evaluate **cache efficiency** using **Valgrind's Cachegrind** tool.
- Determine optimal thread configurations for varying dataset sizes.

---

## 🧪 Why These Algorithms?

- **Merge Sort:** Naturally lends itself to parallelization through its divide-and-conquer approach.
- **Quick Sort:** In-place and generally faster in practice, but has a worst-case of O(n²).

---

## ⚙️ Tools & Techniques

### 🔵 OpenMP
Used for parallelizing recursive function calls:
- `#pragma omp task shared(...)` to create asynchronous tasks
- `#pragma omp taskwait` to synchronize before merging
- `#pragma omp single` to initiate parallelism only once per recursion level

### 🔵 Cachegrind
Used for cache performance profiling:
- L1 instruction/data cache hits and misses
- L2 unified cache behavior
- Key metrics: L1 misses, L refs per L1 miss, D1 misses

---

## 📊 Key Findings

- **Quick Sort** generally outperformed Merge Sort in both runtime and memory usage due to its in-place nature.
- **Merge Sort** showed more instruction references and higher memory allocation due to additional array copying.
- **4–8 threads** provided optimal performance across most input sizes.
- For very large arrays, efficiency dropped due to cache capacity limits (cache thrashing).
- **Parallelization benefits** diminish after a certain thread count due to overhead.

---

## 📈 Experimental Setup

- Tested with array sizes: **10,000**, **100,000**, and **1,000,000**
- Thread counts: **1, 2, 4, 8, 16**
- Metrics recorded:
  - Runtime (execution time)
  - Cache efficiency (L1/L2 read/write/miss ratios)

---

## 🧠 What We Learned

- **OpenMP** is effective and easy to integrate for recursive algorithms.
- Diminishing returns are real in parallel computing — more threads don’t always mean faster.
- **Cache profiling** offers valuable insights into algorithm behavior beyond just runtime.
- Algorithm choice and implementation matter significantly when working with large data on multi-core systems.

---

## 📄 Conclusion

This project successfully demonstrated how **parallelization** and **cache-aware programming** can significantly influence the performance of sorting algorithms. It also highlights the importance of understanding algorithmic behavior in both **runtime** and **memory efficiency**, particularly when scaling to larger datasets.

