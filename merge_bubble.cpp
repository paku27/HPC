#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>
using namespace std;

void bubbleSortSequential(vector<int>& arr) {
    int n = arr.size();
    bool swapped = true;

    while (swapped) {
        swapped = false;
        for (int i = 0; i < n - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
    }
}

void bubbleSortParallel(vector<int>& arr) {
    int n = arr.size();
    bool swapped = true;

    while (swapped) {
        swapped = false;
        #pragma omp parallel for
        for (int i = 0; i < n - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                #pragma omp critical
                {
                    swap(arr[i], arr[i + 1]);
                    swapped = true;
                }
            }
        }
    }
}

void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> temp;
    int i = l, j = m + 1;

    while (i <= m && j <= r) {
        if (arr[i] <= arr[j]) temp.push_back(arr[i++]);
        else temp.push_back(arr[j++]);
    }

    while (i <= m) temp.push_back(arr[i++]);
    while (j <= r) temp.push_back(arr[j++]);

    for (int k = l; k <= r; k++)
        arr[k] = temp[k - l];
}

void mergeSortSequential(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSortSequential(arr, l, m);
        mergeSortSequential(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void mergeSortParallel(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSortParallel(arr, l, m);
            #pragma omp section
            mergeSortParallel(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}

void printArray(const vector<int>& arr) {
    for (int num : arr) cout << num << " ";
    cout << endl;
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> input(n);
    cout << "Enter the elements: ";
    for (int i = 0; i < n; i++) cin >> input[i];

    vector<int> bseq = input;
    vector<int> bpar = input;
    vector<int> mseq = input;
    vector<int> mpar = input;

    // Bubble Sort Sequential
    double start = omp_get_wtime();
    bubbleSortSequential(bseq);
    double end = omp_get_wtime();
    cout << "Sequential Bubble Sort Time: " << (end - start) << " seconds\n";
    cout << "Sorted array by Sequential Bubble Sort: ";
    printArray(bseq);

    // Bubble Sort Parallel
    start = omp_get_wtime();
    bubbleSortParallel(bpar);
    end = omp_get_wtime();
    cout << "Parallel Bubble Sort Time: " << (end - start) << " seconds\n";
    cout << "Sorted array by Parallel Bubble Sort: ";
    printArray(bpar);

    // Merge Sort Sequential
    start = omp_get_wtime();
    mergeSortSequential(mseq, 0, n - 1);
    end = omp_get_wtime();
    cout << "Sequential Merge Sort Time: " << (end - start) << " seconds\n";
    cout << "Sorted array by Sequential Merge Sort: ";
    printArray(mseq);

    // Merge Sort Parallel
    start = omp_get_wtime();
    mergeSortParallel(mpar, 0, n - 1);
    end = omp_get_wtime();
    cout << "Parallel Merge Sort Time: " << (end - start) << " seconds\n";
    cout << "Sorted array by Parallel Merge Sort: ";
    printArray(mpar);

    return 0;
}
