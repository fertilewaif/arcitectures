#include <iostream>
#include <ctime>
#include <fstream>
#include <omp.h>

int main(int argc, char **argv) {
    if (argc != 2)
    {
        std::cout << "Wrong usage: main.exe <input_path>" << std::endl;
    }
	
    size_t arr_size;
    int *arr;
    int max_i = -1, max_j;
    long long max_sum;

    char *file_name = argv[1];
    std::ifstream input(file_name);
    if (!input.is_open()) {
        std::cout << "wrong file" << std::endl;
        return 1;
    }
    input >> arr_size;

    if (arr_size <= 10) {
        std::cout << "Incorrect size of vector = " << arr_size << std::endl;
        return 1;
    }
	
    arr = new int[arr_size];
	
    try {
        for (size_t i = 0; i < arr_size; i++) {
            input >> arr[i];
        }
    }
    catch (...)
    {
        std::cout << "wrong content of file" << std::endl;
        input.close();
        return 1;
    }
    input.close();

    std::cout << arr_size << std::endl;
    for (size_t i = 0; i < arr_size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    clock_t startTime = clock();

    #pragma omp parallel for 
    #pragma omp shared(arr, arr_size)
    for (size_t i = 0; i < arr_size - 1; i++) {
        int max_index = i + 1;

        long long cur_sum = arr[i] - arr[i + 1];
        long long cur_max_sum = cur_sum;

        for (size_t offset = 2; i + offset < arr_size; offset++) {
            int elem = arr[i + offset];
            if (offset % 2 == 0) {
                cur_sum += elem;
            }
            else {
                cur_sum -= elem;
            }
            if (cur_sum >= cur_max_sum) {
                cur_max_sum = cur_sum;
                max_index = i + offset;
            }
        }

        #pragma critical
        {
            if (max_i == -1 || cur_max_sum > max_sum) {
                max_i = i;
                max_j = max_index;
                max_sum = cur_max_sum;
            }
        }

        int thread_num = omp_get_thread_num();
        std::cout << "Thread #" << thread_num << " with beginning index " << i << " has finished working with ending index " << max_index << " and sum " << cur_max_sum << std::endl;  
    }

    clock_t endTime = clock();
    std::cout << "Max sum occurs with i = " << max_i << ", j = " << max_j << " and is equal to " << max_sum << std::endl;
    std::cout << "Calculation time = " << 1.0 * (endTime - startTime) / CLOCKS_PER_SEC << " seconds" << std::endl;

    delete[] arr;
    return 0;
}