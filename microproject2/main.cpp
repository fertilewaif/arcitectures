#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include "buyer.h"
#include "seller.h"
#include <random>

const int max_purchases = 10;
const int sellers_amount = 3;

int main()
{
	int buyers_amount;
	std::cout << "Enter amount of buyers: ";
	std::cin >> buyers_amount;
	if (buyers_amount <= 0)
	{
		std::cout << "Amount of buyers should be positive number";
		return 0;
	}

	std::mutex cout_mutex;
	
	std::vector<seller*> sellers(sellers_amount);
	std::vector<std::thread> sellers_threads(sellers_amount);
	for (int i = 0; i < sellers_amount; i++)
	{
		auto new_seller = new seller(i + 1, &cout_mutex);
		sellers[i] = new_seller;
		sellers_threads[i] = std::thread(&seller::run, new_seller);
	}
	
	std::vector<buyer*> buyers(buyers_amount);
	std::vector<std::thread> buyers_threads(buyers_amount);

	for (int i = 0; i < buyers_amount; i++)
	{
		std::mt19937 randomizer;
		std::uniform_int_distribution<int> range(1, max_purchases);
		int len = range(randomizer);
		
		std::vector<int> buyings_order(len);
		std::queue<seller*> sellers_order;
		for (int i = 0; i < len; i++)
		{
			std::uniform_int_distribution<int> range(1, sellers_amount);
			buyings_order[i] = range(randomizer);
			sellers_order.push(sellers[buyings_order[i] - 1]);
		}
		auto new_buyer = new buyer(i + 1, &cout_mutex, sellers_order);
		buyers.emplace_back(new_buyer);
		buyers_threads.emplace_back(std::thread(&buyer::run, new_buyer));
	}

	for (int i = 0; i < buyers_amount; i++)
	{
		buyers_threads[i].join();
	}
	for (int i = 0; i < sellers_amount; i++)
	{
		sellers[i]->cancel();
		sellers_threads[i].join();
	}

	for (int i = 0; i < buyers_amount; i++)
	{
		delete buyers[i];
	}
	for (int i = 0; i < sellers_amount; i++)
	{
		delete sellers[i];
	}
}