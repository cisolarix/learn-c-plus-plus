// // #include <iostream>
// //
// // using namespace std;
// //
// // // 使用静态数组，假设n不超过1000
// // int diff[1002][1002] = {0}; // 初始化为0
// //
// // int main() {
// //     int n, m;
// //     cin >> n >> m;
// //
// //     // 读取地毯信息并更新差分数组
// //     for (int i = 0; i < m; i++) {
// //         int x1, y1, x2, y2;
// //         cin >> x1 >> y1 >> x2 >> y2;
// //         // 二维差分更新
// //         diff[x1][y1] += 1;
// //         diff[x1][y2 + 1] -= 1;
// //         diff[x2 + 1][y1] -= 1;
// //         diff[x2 + 1][y2 + 1] += 1;
// //     }
// //
// //     // 计算前缀和得到每个格子的覆盖数
// //     for (int i = 1; i <= n; i++) {
// //         for (int j = 1; j <= n; j++) {
// //             diff[i][j] += diff[i - 1][j] + diff[i][j - 1] - diff[i - 1][j - 1];
// //             cout << diff[i][j] << " ";
// //         }
// //         cout << "\n";
// //     }
// //     return 0;
// // }
//
// #include <iostream>
//
// using namespace std;
//
// int a[1005][1005];
//
// int main() {
//     int n, m;
//     cin >> n >> m;
//
//     while (m--) {
//         int x1, y1, x2, y2;
//         cin >> x1 >> y1 >> x2 >> y2;
//         for (int i = x1; i <= x2; i++) {
//             for (int j = y1; j <= y2; j++) {
//                 a[i][j]++;
//             }
//         }
//     }
//
//     for (int i = 1; i <= n; i++) {
//         for (int j = 1; j <= n; j++) {
//             cout << a[i][j] << (j == n ? "" : " ");
//         }
//         cout << endl;
//     }
//
//     return 0;
// }
