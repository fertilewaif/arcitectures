#include <buyer.h>
#include <iostream>
#include <seller.h>
#include <string>

seller::seller()
{
	number = -1;
	current_buyers = std::queue<std::pair<buyer*, std::mutex*>>();

	std::random_device device;
	randomizer.seed(device());
}

seller::seller(int number) : number(number)
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
}

seller& seller::operator=(const seller& other)
{
	number = other.number;
	current_buyers = other.current_buyers;
	randomizer = other.randomizer;
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

void seller::process_buyer(buyer* cur_buyer, std::mutex* cur_mutex)
{
	std::cout << "Seller #" << getNumber()
		<< " processes client #"
		<< current_buyers.front().first->getNumber()
		<< std::endl
		<< "Current condition"
		<< std::endl
		<< to_string()
		<< std::endl;
				
	
	std::uniform_int_distribution<int> range(1, 3);
	int seconds = range(randomizer);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000 * seconds));
	cur_mutex->unlock();
}

std::string seller::to_string()
{
	std::string res('-', 3);
	res += '\n';
	res += '|' + std::to_string(getNumber()) + "|\n";
	std::vector<int> visitorsNumbers = queueToVector(current_buyers);
	for (auto visitorNumber : visitorsNumbers)
	{
		res += ' ' + std::to_string(visitorNumber) + '\n';
	}
	return res;
}

std::vector<int> seller::queueToVector(std::queue<std::pair<buyer*, std::mutex*>> q)
{
	std::vector<int> res(q.size());
	while (!q.empty())
	{
		res.emplace_back(q.front().first->getNumber());
		q.pop();
	}
	return res;
}


