#include <iostream>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <thread>
#include <mutex>

std::mutex arr_mutex;
std::mutex max_mutex;

size_t arr_size;
int *arr;
int max_i = -1, max_j;
long long max_sum;

void func(int startIndex) {
	int max_index = startIndex + 1;

	arr_mutex.lock();
	const int temp_size = arr_size;
    arr_mutex.unlock();

    arr_mutex.lock();
    long long cur_sum = arr[startIndex] - arr[startIndex + 1];
    arr_mutex.unlock();
    long long cur_max_sum = cur_sum;

    for (auto offset = 2; startIndex + offset < temp_size; offset++) {
	    arr_mutex.lock();
        int elem = arr[startIndex + offset];
        arr_mutex.unlock();
        if (offset % 2 == 0) {
            cur_sum += elem;
        }
        else {
            cur_sum -= elem;
        }
        if (cur_sum >= cur_max_sum) {
            cur_max_sum = cur_sum;
            max_index = startIndex + offset;
        }
    }

    max_mutex.lock();
    if (max_i == -1 || cur_max_sum > max_sum) {
        max_i = startIndex;
        max_j = max_index;
        max_sum = cur_max_sum;
    }
    max_mutex.unlock();
}

int main(int argc, char **argv) {
    if (argc != 2)
    {
        std::cout << "Wrong usage: main.exe <input_path>" << std::endl;
    }
	
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


    std::thread *threads = new std::thread[arr_size - 1];

    clock_t startTime = clock();

    for (size_t i = 0; i < arr_size - 1; i++) {
        threads[i] = std::thread(func, i);
    }
    
    for (size_t i = 0; i < arr_size - 1; i++) {
        threads[i].join();
    }

    clock_t endTime = clock();
    std::cout << "Max sum occurs with i = " << max_i << ", j = " << max_j << " and is equal to " << max_sum << std::endl;
    std::cout << "Calculation time = " << 1.0 * (endTime - startTime) / CLOCKS_PER_SEC << " seconds" << std::endl;

    delete[] arr;
    delete[] threads;
	return 0;
}
