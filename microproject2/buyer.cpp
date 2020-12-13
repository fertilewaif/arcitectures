#include "buyer.h"
#include <iostream>
#include <utility>
#include "seller.h"

buyer::buyer() : number(-1)
{
	sellers_order = std::queue<seller*>();
}

buyer::buyer(const int number) : number(number)
{
	sellers_order = std::queue<seller*>();
}

buyer::buyer(int number, std::queue<seller*> sellers) : sellers_order(std::move(sellers)), number(number)
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


int buyer::getNumber()
{
	return number;
}

void buyer::run()
{
	while (!sellers_order.empty())
	{
		seller* current_seller = sellers_order.front();
		std::cout << "Buyer #" << getNumber() << " goes to shop#" << current_seller->getNumber() << std::endl;
		std::mutex cur_mutex;
		sellers_order.pop();
		current_seller->add_buyer(this, &cur_mutex);
	}
}


