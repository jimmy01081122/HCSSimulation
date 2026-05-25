#include <iostream>
#include <vector>
#include <pthread.h>
#include <gem5/m5ops.h>

/*
 * 方向三：KV Cache 軟體預取模擬
 * 在 LLM 推理中，KV Cache 的讀取往往是效能瓶頸。
 * 我們模擬雙核系統：
 * - Core 0 (Main Thread): 負責計算並讀取 KV 數據。
 * - Core 1 (Prefetch Thread): 負責走訪即將被使用的地址，將其拉入 L2 Cache。
 */

#define ARRAY_SIZE (512 * 1024) // 512KB 的數據量
float kv_data[ARRAY_SIZE];
volatile int ready = 0;

// Core 1: 預取執行緒
void* prefetch_thread(void* arg) {
    // 等待主執行緒信號
    while (!ready);

    /*
     * 模擬「分散收集 (Scatter-Gather)」前的預取動作。
     * 透過簡單的讀取動作，觸發硬體快取機制。
     */
    for (int i = 0; i < ARRAY_SIZE; i += 16) { // 以 Cache Line 為單位跳躍
        float temp = kv_data[i]; 
    }
    
    return NULL;
}

int main() {
    // 初始化數據
    for (int i = 0; i < ARRAY_SIZE; i++) {
        kv_data[i] = (float)i;
    }

    pthread_t thread;
    pthread_create(&thread, NULL, prefetch_thread, NULL);

    std::cout << "--- 開始 KV Cache 預取實驗 ---" << std::endl;

    m5_work_begin(0, 0); // 標記 ROI

    ready = 1; // 啟動預取執行緒

    // Core 0: 模擬核心計算與數據存取
    float sum = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sum += kv_data[i];
    }

    m5_work_end(0, 0);

    pthread_join(thread, NULL);
    std::cout << "計算結果: " << sum << std::endl;

    return 0;
}
