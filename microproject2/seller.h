#pragma once
#include "buyer.h"

class seller {
public:
    seller();
    explicit seller(int);
    seller(const seller&);
    seller& operator=(const seller&);

    int getNumber();
	
    void add_buyer(buyer*);
    void process_buyer(buyer*);
};
