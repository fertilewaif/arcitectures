#include <random>
#include "buyer.h"

class seller {
public:
    int number;

    seller();
    explicit seller(int number);
    seller(const seller& other);
    seller& operator=(const seller& other);

    void add_buyer(buyer* new_buyer);
    void process_buyer(buyer* new_buyer);
private:
    std::mt19937 randomizer;
    std::mutex queue_mutex;
    std::queue<buyer*> current_buyers;
};
