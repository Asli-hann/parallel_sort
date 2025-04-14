#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <algorithm>

using namespace std;

//Classic merge sort algorithm to merge left and right parts of the give array
//Nothing to be parallelize here. Maybe we can add openmp to copying data to temporary arrays later on.
void merge(vector<int>& arr, int l, int mid, int r) {
    int left = mid - l + 1;
    int right = r - mid;

    vector<int> L(left);
    vector<int> R(right);

    for (int i = 0; i < left; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < right; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0;
    int j = 0;
    int k = l;

    while (i < left && j < right) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < left) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < right) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

//Only parallelize the the recursive function when the array size is higher than 1000.
//The reason is; parallelizing smaller arrays will not be efficient overall.
void mergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int mid = l + (r - l) / 2;

        #pragma omp task shared(arr) if (r - l > 1000) 
        mergeSort(arr, l, mid);

        #pragma omp task shared(arr) if (r - l > 1000)
        mergeSort(arr, mid + 1, r);

        #pragma omp taskwait // Ensure tasks complete before merging
        merge(arr, l, mid, r);
    }
}

//Initialize the parallel environment with a thread pool.
//Ensure a single thread begins the recursive quickSort process.
void parallelMergeSort(vector<int>& arr, int l, int r) {
    #pragma omp parallel
    {
        #pragma omp single 
        {
            mergeSort(arr, l, r);
        }
    }
}


//Compiled code takes 2 arguments; array size and number of threads.
//Usage is ./mSort 100000 4 for array size 100000 and 4 threads.
//After getting the size and thread number, set the number of threads and calculate the time taken. 
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <array_size> <num_of_threads>" << endl;
        return 1;
    }

    int n = stoi(argv[1]); // Array size is n
    int num_of_threads = stoi(argv[2]); // Number of threads is num_of_threads
    omp_set_num_threads(num_of_threads);

    vector<int> arr(n);
    generate(arr.begin(), arr.end(), rand); // generate and fill the array with random numbers

    auto start_time = chrono::high_resolution_clock::now(); //start the timer

    parallelMergeSort(arr, 0, arr.size() - 1);

    auto end_time = chrono::high_resolution_clock::now();  //end the timer

    chrono::duration<double, milli> duration = end_time - start_time;

    cout << "Time taken: " << duration.count() << " ms for array size " << n << " with " << num_of_threads << " threads." << endl;

    return 0;
}
