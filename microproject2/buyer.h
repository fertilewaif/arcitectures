#pragma once
#include <queue>

class seller;

class buyer {
public:
    buyer();
    explicit buyer(int number);
    buyer(int number, std::queue<seller*> sellers);
    buyer(const buyer& other);
    buyer& operator=(const buyer& other);

    int getNumber();
	
    void notify();
    void run();
private:	
    bool is_waiting();
    void set_waiting(bool waiting);
};
