#pragma once
#include <random>
#include <mutex>
#include "buyer.h"

class seller {
public:
    seller();
    explicit seller(int);
    seller(const seller&);
    seller& operator=(const seller&);

    int getNumber();
    std::string to_string();
	
    void add_buyer(buyer*, std::mutex*);
    void process_buyer(buyer*, std::mutex*);

    void run();
    void cancel();
private:
    int number;
    std::mt19937 randomizer;

	std::mutex queue_mutex;
    std::queue<std::pair<buyer*, std::mutex*>> current_buyers;

    std::mutex working_mutex;
    bool working = true;

    bool is_working();
	
    static std::vector<int> queueToVector(std::queue<std::pair<buyer*, std::mutex*>>);
};
