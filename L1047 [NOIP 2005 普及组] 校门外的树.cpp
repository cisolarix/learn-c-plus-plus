// #include <iostream>
//
// using namespace std;
//
// // 在 main 函数外定义大数组是一个好习惯，可以避免栈溢出
// // 假设 L 最大为 10000，我们开 10001 + 10 以防万一
// int road[10010];
//
// int main() {
//     int l, m;
//
//     // 输入 L 和 M，输入后直接按回车即可
//     if (!(cin >> l >> m)) return 0;
//
//     // 第一步：初始化，把 0 到 l 的位置都种上树（标记为 0）
//     // 我们约定：0 表示有树，1 表示树被移走了
//     for (int i = 0; i <= l; i++) {
//         road[i] = 0;
//     }
//
//     // 第二步：处理 M 个区域
//     for (int i = 0; i < m; i++) {
//         int u, v;
//         cin >> u >> v; // 每次循环读取一组区间，按回车或空格分隔均可
//
//         // 将区间 [u, v] 内的树标记为“移走”
//         for (int j = u; j <= v; j++) {
//             road[j] = 1;
//         }
//     }
//
//     // 第三步：统计剩余
//     int remainingTrees = 0;
//     for (int i = 0; i <= l; i++) {
//         if (road[i] == 0) {
//             remainingTrees++;
//         }
//     }
//
//     // 输出结果
//     cout << remainingTrees << endl;
//
//     return 0;
// }
