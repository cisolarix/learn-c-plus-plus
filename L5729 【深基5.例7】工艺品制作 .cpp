// #include <iostream>
//
// using namespace std;
//
// // 定义三维数组，用来记录每个小方格的状态
// // 0 表示存在，1 表示已被蒸发
// int cube[25][25][25];
//
// int main() {
//     int w, x, h, q;
//     if (!(cin >> w >> x >> h)) return 0;
//     cin >> q;
//
//     // 初始化：由于全局变量默认就是 0，所以不需要手动初始化
//     // 如果你在函数内部定义，记得用循环或 memset 清零
//
//     // 进行 q 次切割
//     while (q--) {
//         int x1, y1, z1, x2, y2, z2;
//         cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
//
//         // 暴力枚举切割范围内的所有小方块
//         for (int i = x1; i <= x2; i++) {
//             for (int j = y1; j <= y2; j++) {
//                 for (int k = z1; k <= z2; k++) {
//                     cube[i][j][k] = 1; // 标记为已切除
//                 }
//             }
//         }
//     }
//
//     // 统计剩下的小方块
//     int count = 0;
//     for (int i = 1; i <= w; i++) {
//         for (int j = 1; j <= x; j++) {
//             for (int k = 1; k <= h; k++) {
//                 if (cube[i][j][k] == 0) {
//                     count++;
//                 }
//             }
//         }
//     }
//
//     cout << count << endl;
//
//     return 0;
// }
