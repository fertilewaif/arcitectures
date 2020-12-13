#include "buyer.h"
#include <iostream>
#include <utility>
#include "seller.h"

buyer::buyer() : number(-1)
{
	sellers_order = std::queue<seller*>();
}

buyer::buyer(const int number, std::mutex* out_mutex) : cout_mutex(out_mutex), number(number)
{
	sellers_order = std::queue<seller*>();
}

buyer::buyer(int number, std::mutex* out_mutex, std::queue<seller*> sellers) : sellers_order(std::move(sellers)), cout_mutex(out_mutex), number(number)
{

}

buyer::buyer(const buyer& other)
{
	number = other.number;
	sellers_order = other.sellers_order;
	cout_mutex = other.cout_mutex;
}

buyer& buyer::operator=(const buyer& other)
{
	number = other.number;
	sellers_order = other.sellers_order;
	cout_mutex = other.cout_mutex;
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

		cout_mutex->lock();
		std::cout << "Buyer #" << getNumber() << " goes to shop#" << current_seller->getNumber() << std::endl;
		cout_mutex->unlock();

		std::mutex cur_mutex;
		sellers_order.pop();
		current_seller->add_buyer(this, &cur_mutex);
	}
}


