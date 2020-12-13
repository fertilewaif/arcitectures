#include <mutex>
#include <queue>

class seller;

class buyer {
public:
    int number;

    buyer();
    explicit buyer(int number);
    buyer(int number, std::queue<seller*> sellers);
    buyer(const buyer& other);
    buyer& operator=(const buyer& other);

    void notify();
    void run();
private:
    std::mutex wait_mutex;
    bool waiting = false;
    std::queue<seller*> sellers_order;

    bool is_waiting();
    void set_waiting(bool waiting);
};
