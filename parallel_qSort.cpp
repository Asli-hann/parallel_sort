#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

//Nothing to be parallel here, same with the original quick sort algorithm
//Used bits/stdc++ library for the swap function.
int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

//Only parallelize the the recursive function when the array size is higher than 1000.
//The reason is, we will not be able to see any chnages for smaller array sizes, although time can even increase.
void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        #pragma omp task shared(arr) if (high - low > 1000)
        quickSort(arr, low, pi - 1);

        #pragma omp task shared(arr) if (high - low > 1000)
        quickSort(arr, pi + 1, high);
    }
}

//Initialize the parallel environment with a thread pool.
//Ensure a single thread begins the recursive quickSort process.
void parallelQuickSort(std::vector<int>& arr, int low, int high) {
    #pragma omp parallel
    {
        #pragma omp single
        {
            quickSort(arr, low, high);
        }
    }
}


//Compiled code takes 2 arguments; array size and number of threads.
//Usage is ./qSort 100000 4 for array size 100000 and 4 threads.
//After getting the size and thread number, set the number of threads and calculate the time taken. 
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <array_size> <num_of_threads>" << std::endl;
        return 1;
    }

    int n = std::stoi(argv[1]); // Array size is n 
    int num_of_threads = std::stoi(argv[2]); // Number of threads is num_of_threads
    omp_set_num_threads(num_of_threads);

    std::vector<int> arr(n);
    std::generate(arr.begin(), arr.end(), rand); // Fill the array with the random values

    auto start_time = std::chrono::high_resolution_clock::now(); //start the timer

    parallelQuickSort(arr, 0, arr.size() - 1);

    auto end_time = std::chrono::high_resolution_clock::now(); //end the timer
    std::chrono::duration<double, std::milli> duration = end_time - start_time;

    std::cout << "Time taken: " << duration.count() << " ms for array size " << n << " with " << num_of_threads << " threads." << std::endl;

    return 0;
}
