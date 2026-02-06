// #include <bits/stdc++.h> // 用于保留小数
// #include <algorithm> // 用于 max 和 min 函数
//
// using namespace std;
//
// int main() {
//     int n, m;
//     if (!(cin >> n >> m)) return 0;
//
//     double max_total_avg = 0.0; // 记录全场最高平均分
//
//     for (int i = 0; i < n; i++) {
//         int sum = 0;
//         int max_s = -1; // 评分范围 0-10，初始设为最小值
//         int min_s = 11; // 初始设为最大值
//
//         for (int j = 0; j < m; j++) {
//             int score;
//             cin >> score;
//             sum += score;
//             if (score > max_s) max_s = score;
//             if (score < min_s) min_s = score;
//         }
//
//         // 计算当前同学去掉最高分和最低分后的平均分
//         // 注意：计算平均值时要除以 (m-2.0) 以确保结果是浮点数
//         double current_avg = (double) (sum - max_s - min_s) / (m - 2);
//
//         // 更新全场最高分
//         if (current_avg > max_total_avg) {
//             max_total_avg = current_avg;
//         }
//     }
//
//     // 输出结果，保留两位小数
//     cout << fixed << setprecision(2) << max_total_avg << endl;
//
//     return 0;
// }
