#include "buyer.h"
#include <iostream>
#include <chrono>
#include <thread>
#include "seller.h"
#include <string>

seller::seller()
{
	number = -1;
	current_buyers = std::queue<std::pair<buyer*, std::mutex*>>();

	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	randomizer.seed(seed);
}

seller::seller(int number, std::mutex* out_mutex) : number(number), cout_mutex(out_mutex)
{
	current_buyers = std::queue<std::pair<buyer*, std::mutex*>>();

	std::random_device device;
	randomizer.seed(device());
}

seller::seller(const seller& other)
{
	number = other.number;
	current_buyers = other.current_buyers;
	randomizer = other.randomizer;
	cout_mutex = other.cout_mutex;
}

seller& seller::operator=(const seller& other)
{
	number = other.number;
	current_buyers = other.current_buyers;
	randomizer = other.randomizer;
	cout_mutex = other.cout_mutex;
	return *this;
}

void seller::add_buyer(buyer* new_buyer, std::mutex* new_mutex)
{
	queue_mutex.lock();
	new_mutex->lock();
	current_buyers.push({ new_buyer, new_mutex });
	queue_mutex.unlock();
}

int seller::getNumber()
{
	return number;
}

bool seller::is_working()
{
	working_mutex.lock();
	auto res = working;
	working_mutex.unlock();
	return res;
}


void seller::process_buyer(buyer* cur_buyer, std::mutex* cur_mutex)
{
	cout_mutex->lock();
	std::cout << "Seller #" << getNumber()
		<< " processes client #"
		<< cur_buyer->getNumber()
		<< std::endl
		<< "Current condition"
		<< std::endl
		<< to_string()
		<< std::endl;
	cout_mutex->unlock();
	
	 std::uniform_int_distribution<int> range(500, 1500);
	 int milliseconds = range(randomizer);
	 std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	cur_mutex->unlock();
}

std::string seller::to_string()
{
	std::string res = "---\n";
	res += '|' + std::to_string(getNumber()) + "|\n";
	res += "-/-\n";
	std::vector<int> visitorsNumbers = queueToVector(current_buyers);
	for (auto visitorNumber : visitorsNumbers)
	{
		res += ' ' + std::to_string(visitorNumber) + '\n';
	}
	return res;
}

std::vector<int> seller::queueToVector(std::queue<std::pair<buyer*, std::mutex*>> q)
{
	std::vector<int> res;
	while (!q.empty())
	{
		res.emplace_back(q.front().first->getNumber());
		q.pop();
	}
	return res;
}

void seller::run()
{
	while (is_working())
	{
		queue_mutex.lock();
		auto is_empty = current_buyers.empty();
		queue_mutex.unlock();
		if (is_empty)
		{
			continue;
		}

		queue_mutex.lock();
		auto cur_pair = current_buyers.front();
		queue_mutex.unlock();

		process_buyer(cur_pair.first, cur_pair.second);
		
		queue_mutex.lock();
		current_buyers.pop();
		queue_mutex.unlock();
	}
	while (!current_buyers.empty())
	{
		current_buyers.front().second->unlock();
		current_buyers.pop();
	}
}

void seller::cancel()
{
	working_mutex.lock();
	working = false;
	working_mutex.unlock();
}



