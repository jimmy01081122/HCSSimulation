// distributed_queue.cpp
// Distributed expert-affinity task queue scheduling benchmark for gem5.
// Distributes tasks into multiple independent queues to minimize lock contention.

#include <iostream>
#include <vector>
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

class DistributedQueue {
private:
    static const int NUM_QUEUES = 8;
    std::vector<Task> queues[NUM_QUEUES];
    SimpleSpinLock locks[NUM_QUEUES];
public:
    void push(const Task& t) {
        int q_idx = t.expert_id % NUM_QUEUES;
        locks[q_idx].lock();
        queues[q_idx].push_back(t);
        locks[q_idx].unlock();
    }

    bool pop(Task& t, int preferred_expert_id) {
        int q_idx = preferred_expert_id % NUM_QUEUES;
        locks[q_idx].lock();
        if (!queues[q_idx].empty()) {
            t = queues[q_idx].back();
            queues[q_idx].pop_back();
            locks[q_idx].unlock();
            return true;
        }
        locks[q_idx].unlock();

        // Work stealing loop
        for (int i = 0; i < NUM_QUEUES; ++i) {
            if (i == q_idx) continue;
            locks[i].lock();
            if (!queues[i].empty()) {
                t = queues[i].back();
                queues[i].pop_back();
                locks[i].unlock();
                return true;
            }
            locks[i].unlock();
        }
        return false;
    }
};

int main() {
    const int num_tasks = 100;
    std::cout << "Starting Distributed Queue CPU Benchmark..." << std::endl;

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

    DistributedQueue dist_queue;
    for (const auto& t : test_tasks) {
        dist_queue.push(t);
    }

    volatile unsigned long long total_ops = 0;
    Task temp;
    for (int i = 0; i < num_tasks; ++i) {
        if (dist_queue.pop(temp, i % 64)) {
            total_ops += temp.expert_id;
        }
    }

    std::cout << "Benchmark complete. Hash ops check: " << total_ops << std::endl;
    return 0;
}
