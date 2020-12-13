#pragma once
#include <mutex>
#include <queue>

class seller;

class buyer {
public:
    buyer();
    explicit buyer(int, std::mutex*);
    buyer(int, std::mutex*, std::queue<seller*>);
    buyer(const buyer& other);
    buyer& operator=(const buyer&);

    int getNumber();
    void run();
private:
    bool waiting = false;
    std::queue<seller*> sellers_order;

    std::mutex* cout_mutex;
	
    int number;
};
