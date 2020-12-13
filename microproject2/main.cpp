#include <iostream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <random>

#include "buyer.h"
#include "seller.h"




std::vector<buyer> buyer_queue_to_vector(std::queue<buyer> q) {
    std::vector<buyer> res(q.size());
    while (!q.empty()) {
        res.emplace_back(q.front());
        q.pop();
    }
    return res;
}
