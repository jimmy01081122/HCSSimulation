// centralized_queue.cpp
// Centralized task queue scheduling benchmark for gem5.
// Employs a global queue guarded by a spinlock and sorts tasks upon insertion to model scheduling overhead.

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

struct Task {
    int request_id;
    int token_id;
    int expert_id;
    int priority;
};

class SimpleSpinLock {
private:
    volatile int flag = 0;
public:
    void lock() {
        while (__sync_lock_test_and_set(&flag, 1)) {
            // spin-wait
        }
    }
    void unlock() {
        __sync_lock_release(&flag);
    }
};

class CentralizedQueue {
private:
    std::vector<Task> tasks;
    SimpleSpinLock spinlock;
public:
    void push(const Task& t) {
        spinlock.lock();
        tasks.push_back(t);
        // Sort to optimize locality (Centralized alignment overhead)
        std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
            return a.expert_id < b.expert_id;
        });
        spinlock.unlock();
    }

    bool pop(Task& t) {
        spinlock.lock();
        if (tasks.empty()) {
            spinlock.unlock();
            return false;
        }
        t = tasks.back();
        tasks.pop_back();
        spinlock.unlock();
        return true;
    }
};

int main() {
    const int num_tasks = 100;
    std::cout << "Starting Centralized Queue CPU Benchmark..." << std::endl;

    std::vector<Task> test_tasks;
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist_req(0, 100);
    std::uniform_int_distribution<int> dist_expert(0, 63);

    for (int i = 0; i < num_tasks; ++i) {
        test_tasks.push_back({
            dist_req(rng),
            i % 32,
            dist_expert(rng),
            1
        });
    }

    CentralizedQueue global_queue;
    for (const auto& t : test_tasks) {
        global_queue.push(t);
    }

    volatile unsigned long long total_ops = 0;
    Task temp;
    for (int i = 0; i < num_tasks; ++i) {
        if (global_queue.pop(temp)) {
            total_ops += temp.expert_id;
        }
    }

    std::cout << "Benchmark complete. Hash ops check: " << total_ops << std::endl;
    return 0;
}
