#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <random>
#include <algorithm>

// 模擬 MoE 任務結構
struct Task {
    int request_id;
    int token_id;
    int expert_id;
    int priority;
};

// 簡單的自定義鎖，便於在 SE 模式下測試
class SimpleSpinLock {
private:
    volatile int flag = 0;
public:
    void lock() {
        while (__sync_lock_test_and_set(&flag, 1)) {
            // spin
        }
    }
    void unlock() {
        __sync_lock_release(&flag);
    }
};

// 1. 集中式佇列 (Centralized Global Queue)
class CentralizedQueue {
private:
    std::vector<Task> tasks;
    SimpleSpinLock spinlock;
public:
    void push(const Task& t) {
        spinlock.lock();
        tasks.push_back(t);
        // 模擬排程排序 (Expert locality alignment)
        std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b){
            return a.expert_id < b.expert_id; // 按 expert 排序以提升 cache 命中率
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

// 2. 分散式/Affinity-based 佇列 (Distributed Expert-Affinity Queue)
// 每個 Expert 分派到特定的 Queue 中，減少全局鎖的爭搶並提高 CPU 緩存局部性
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
        // 先嘗試自己 preferred 的 expert 佇列
        int q_idx = preferred_expert_id % NUM_QUEUES;
        locks[q_idx].lock();
        if (!queues[q_idx].empty()) {
            t = queues[q_idx].back();
            queues[q_idx].pop_back();
            locks[q_idx].unlock();
            return true;
        }
        locks[q_idx].unlock();

        // Work stealing: 若空，輪詢其他佇列
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

int main(int argc, char* argv[]) {
    int num_tasks = 1000;
    bool use_distributed = false;
    
    if (argc > 1) {
        std::string mode = argv[1];
        if (mode == "dist") {
            use_distributed = true;
        }
    }

    std::cout << "Starting CPU Scheduling Benchmark..." << std::endl;
    std::cout << "Mode: " << (use_distributed ? "Distributed Expert-Affinity Queue" : "Centralized Global Queue") << std::endl;
    std::cout << "Tasks: " << num_tasks << std::endl;

    // 產生隨機任務需求
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

    volatile unsigned long long total_ops = 0;

    if (!use_distributed) {
        CentralizedQueue global_queue;
        // 模擬 CPU scheduling 的 push 和 pop 過程
        for (const auto& t : test_tasks) {
            global_queue.push(t);
        }
        Task temp;
        for (int i = 0; i < num_tasks; ++i) {
            if (global_queue.pop(temp)) {
                total_ops += temp.expert_id; // 模擬運算
            }
        }
    } else {
        DistributedQueue dist_queue;
        for (const auto& t : test_tasks) {
            dist_queue.push(t);
        }
        Task temp;
        for (int i = 0; i < num_tasks; ++i) {
            if (dist_queue.pop(temp, i % 64)) {
                total_ops += temp.expert_id;
            }
        }
    }

    std::cout << "Benchmark complete. Hash ops check: " << total_ops << std::endl;
    return 0;
}
