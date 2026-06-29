// lock_free_queue.cpp
// Lock-free task queue/stack scheduling benchmark for gem5.
// Employs std::atomic and Compare-And-Swap (CAS) for synchronization-free scheduling.

#include <iostream>
#include <vector>
#include <random>
#include <atomic>

struct Task {
    int request_id;
    int token_id;
    int expert_id;
    int priority;
};

class LockFreeStack {
private:
    struct Node {
        Task data;
        Node* next;
        Node(const Task& t) : data(t), next(nullptr) {}
    };
    std::atomic<Node*> head;

public:
    LockFreeStack() : head(nullptr) {}

    ~LockFreeStack() {
        Node* curr = head.load();
        while (curr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
    }

    void push(const Task& t) {
        Node* new_node = new Node(t);
        new_node->next = head.load();
        // Atomic compare-and-swap retry loop
        while (!head.compare_exchange_weak(new_node->next, new_node)) {
            // spin-wait/retry
        }
    }

    bool pop(Task& t) {
        Node* old_head = head.load();
        while (old_head && !head.compare_exchange_weak(old_head, old_head->next)) {
            // spin-wait/retry
        }
        if (old_head) {
            t = old_head->data;
            delete old_head;
            return true;
        }
        return false;
    }
};

int main() {
    const int num_tasks = 100;
    std::cout << "Starting Lock-Free Queue CPU Benchmark..." << std::endl;

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

    LockFreeStack lf_stack;
    for (const auto& t : test_tasks) {
        lf_stack.push(t);
    }

    volatile unsigned long long total_ops = 0;
    Task temp;
    for (int i = 0; i < num_tasks; ++i) {
        if (lf_stack.pop(temp)) {
            total_ops += temp.expert_id;
        }
    }

    std::cout << "Benchmark complete. Hash ops check: " << total_ops << std::endl;
    return 0;
}
