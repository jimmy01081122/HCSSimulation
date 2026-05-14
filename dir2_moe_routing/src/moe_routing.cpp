#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <gem5/m5ops.h> // 包含 gem5 的特殊指令，用於標記量測區段

/*
 * MoE (Mixture of Experts) 路由邏輯通常包含對 Expert 分數進行 Softmax 與 Top-K 選擇。
 * 這裡我們模擬這個邏輯，並使用 gem5 的 m5_work 指令來精確獲取統計數據。
 */

void softmax_routing(int num_experts, const std::vector<float>& scores) {
    std::vector<float> exp_scores(num_experts);
    float sum = 0.0;

    // 1. 計算 Exponential 與總和 (Softmax 第一步)
    for (int i = 0; i < num_experts; ++i) {
        exp_scores[i] = std::exp(scores[i]);
        sum += exp_scores[i];
    }

    // 2. 正規化 (Softmax 第二步)
    for (int i = 0; i < num_experts; ++i) {
        exp_scores[i] /= sum;
    }

    // 3. 模擬 Top-K 選擇 (這裡簡單尋找最大值)
    auto it = std::max_element(exp_scores.begin(), exp_scores.end());
    int selected_expert = std::distance(exp_scores.begin(), it);

    std::cout << "選擇的 Expert 編號: " << selected_expert << " (機率: " << *it << ")" << std::endl;
}

int main() {
    int num_experts = 64; // 假設有 64 個專家
    std::vector<float> scores(num_experts);

    // 初始化隨機分數
    for (int i = 0; i < num_experts; ++i) {
        scores[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }

    std::cout << "--- 開始純 CPU 運算測試 ---" << std::endl;

    /*
     * m5_work_begin(0, 0) 會在 gem5 的 stats.txt 中重設統計數據，
     * 讓我們可以單獨看這一段代碼的效能，排除了程式初始化的干擾。
     */
    m5_work_begin(0, 0); 
    
    for (int i = 0; i < 1000; ++i) { // 重複多次以放大觀察效果
        softmax_routing(num_experts, scores);
    }

    m5_work_end(0, 0); // 標記結束

    std::cout << "--- 運算完成 ---" << std::endl;

    return 0;
}
