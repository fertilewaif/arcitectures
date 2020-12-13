#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include "buyer.h"
#include "seller.h"
#include <random>

const int max_purchases = 2;
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
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		std::mt19937 randomizer(seed);
		
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

		std::cout << "Buyer #" << i + 1 << " goes in following order: ";
		for (auto seller_number : buyings_order)
		{
			std::cout << seller_number << " ";
		}
		std::cout << std::endl;
		
		auto new_buyer = new buyer(i + 1, &cout_mutex, sellers_order);
		buyers[i] = new_buyer;
		buyers_threads[i] = std::thread(&buyer::run, new_buyer);
	}

	for (int i = 0; i < buyers_amount; i++)
	{
		if (buyers_threads[i].joinable()) {
			buyers_threads[i].join();
		}
	}
	for (int i = 0; i < sellers_amount; i++)
	{
		sellers[i]->cancel();
		if (sellers_threads[i].joinable()) {
			sellers_threads[i].join();
		}
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