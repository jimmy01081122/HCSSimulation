#include <stdio.h>
#include <stdlib.h>

/* 
 * 這個程式模擬一個需要大量數據搬運的場景。
 * 在真實的異構系統中，CPU 通常不直接搬運數據，
 * 而是通知 DMA (Direct Memory Access) 控制器來處理，
 * 這樣 CPU 就可以同時進行其他運算。
 */

#define DATA_SIZE (1024 * 1024) // 搬運 1MB 的數據

int main() {
    // 分配來源與目的端記憶體空間
    // 在模擬環境中，這些空間會被映射到 gem5 的實體記憶體
    float *src = (float*)malloc(DATA_SIZE * sizeof(float));
    float *dst = (float*)malloc(DATA_SIZE * sizeof(float));

    if (src == NULL || dst == NULL) {
        printf("記憶體分配失敗！\n");
        return -1;
    }

    printf("開始初始化數據...\n");
    for (int i = 0; i < DATA_SIZE; i++) {
        src[i] = (float)i;
    }

    /*
     * 這裡我們模擬「觸發」DMA 動作。
     * 在簡單的 SE (Syscall Emulation) 模式下，我們雖然直接用程式碼操作，
     * 但在 gem5 的 Python 配置中，我們會觀察 Bus 的流量。
     * 之後可以進一步透過 m5 ops 或是讀寫特定的 DMA 暫存器地址來模擬真實硬體行為。
     */
    printf("模擬觸發 DMA 數據搬運 (1MB)...\n");
    for (int i = 0; i < DATA_SIZE; i++) {
        dst[i] = src[i];
    }

    printf("數據搬運完成，驗證最後一筆數據: %f\n", dst[DATA_SIZE-1]);

    free(src);
    free(dst);
    return 0;
}
