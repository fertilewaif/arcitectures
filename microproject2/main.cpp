#include <iostream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <random>

class buyer;

class seller;

class buyer {
public:
    int number;

    buyer();
    explicit buyer(int number);
    buyer(int number, std::queue<seller*> sellers);
    buyer(const buyer& other);
    buyer& operator=(const buyer& other);
	
    void notify();
    void run();
private:
    std::mutex wait_mutex;
    bool waiting = false;
    std::queue<seller*> sellers_order;

    bool is_waiting();
    void set_waiting(bool waiting);
};

buyer::buyer()
{
    number = -1;
    sellers_order = std::queue<seller*>();
}

buyer::buyer(const int number) : number(number)
{
    sellers_order = std::queue<seller*>();
}

buyer::buyer(int number, std::queue<seller*> sellers) : number(number), sellers_order(sellers)
{
}

buyer::buyer(const buyer& other)
{
    number = other.number;
    sellers_order = other.sellers_order;
}

buyer& buyer::operator=(const buyer& other)
{
    number = other.number;
    sellers_order = other.sellers_order;

    return *this;
}

void buyer::notify()
{
}

void buyer::run()
{
	while (!sellers_order.empty())
	{
        seller* current_seller = sellers_order.front();
        sellers_order.pop();
		current_seller
	}
}

bool buyer::is_waiting()
{
    wait_mutex.lock();
    bool res = waiting;
    wait_mutex.unlock();
    return res;
}

void buyer::set_waiting(bool is_waiting)
{
    wait_mutex.lock();
    waiting = is_waiting;
    wait_mutex.unlock();
}

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

seller::seller()
{
    number = -1;
    current_buyers = std::queue<buyer*>();

    std::random_device device;
    randomizer.seed(device());
}

seller::seller(int number) : number(number)
{
    current_buyers = std::queue<buyer*>();
    std::random_device device;
    randomizer.seed(device());
}

seller::seller(const seller& other)
{
    number = other.number;
    current_buyers = other.current_buyers;
    randomizer = other.randomizer;
}

seller& seller::operator=(const seller& other)
{
    number = other.number;
    current_buyers = other.current_buyers;
    randomizer = other.randomizer;
    return *this;
}

void seller::add_buyer(buyer* new_buyer)
{
    queue_mutex.lock();
    current_buyers.push(new_buyer);
    queue_mutex.unlock();
}

void seller::process_buyer(buyer* new_buyer)
{
	// симулируем полезную работу
    std::uniform_int_distribution<int> range(1, 3);
    int seconds = range(randomizer);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * seconds));
    new_buyer->notify();
}

std::vector<buyer> buyer_queue_to_vector(std::queue<buyer> q) {
    std::vector<buyer> res(q.size());
    while (!q.empty()) {
        res.emplace_back(q.front());
        q.pop();
    }
    return res;
}
