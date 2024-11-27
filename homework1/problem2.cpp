// HW1 problem2

#include <iostream>
using namespace std;

// 遞迴函數生成冪集
void powerset(int set[], int subset[], int set_size, int subset_size, int idx) {
    // 如果索引超出集合大小，列印當前子集
    if (idx == set_size) {
        cout << "{ ";
        for (int i = 0; i < subset_size; i++) {
            if (i != 0) cout << ", ";
            cout << subset[i];
        }
        cout << " }" << endl;
        return;
    }

    // 情況1：不包含當前元素
    powerset(set, subset, set_size, subset_size, idx + 1);

    // 情況2：包含當前元素
    subset[subset_size] = set[idx];  // 將當前元素加入子集
    powerset(set, subset, set_size, subset_size + 1, idx + 1);
}

int main() {
    int set[] = {1, 2, 3};  // 示例集合
    int subset[3];  // 用於儲存當前子集

    int set_size = sizeof(set) / sizeof(set[0]);  // 計算集合大小

    cout << "冪集為: " << endl;
    powerset(set, subset, set_size, 0, 0);  // 生成並列印冪集

    return 0;
}
