#pragma once
#include <queue>

class seller;

class buyer {
public:
    buyer();
    explicit buyer(int);
    buyer(int number, std::queue<seller*>);
    buyer(const buyer& other);
    buyer& operator=(const buyer&);

    int getNumber();
    void run();
private:
    bool waiting = false;
    std::queue<seller*> sellers_order;
	
    int number;
};
