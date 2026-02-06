// #include <bits/stdc++.h>
//
// using namespace std;
//
// // n <= 10, 2^10 = 1024, 数组开略大一些防止越界
// int matrix[1024][1024];
//
// /**
//  * x, y: 当前子矩阵的左上角坐标
//  * size: 当前子矩阵的边长
//  */
// void release(int x, int y, int size) {
//     // 递归边界：当矩阵无法再分（边长为1）时返回
//     if (size == 1) return;
//
//     int half = size / 2;
//
//     // 1. 赦免左上角的矩阵：将该区域全部置为 0
//     for (int i = x; i < x + half; i++) {
//         for (int j = y; j < y + half; j++) {
//             matrix[i][j] = 0;
//         }
//     }
//
//     // 2. 递归处理另外三个小矩阵
//     release(x + half, y, half); // 右上角
//     release(x, y + half, half); // 左下角
//     release(x + half, y + half, half); // 右下角
// }
//
// int main() {
//     int n;
//     if (!(cin >> n)) return 0;
//
//     int L = 1 << n;
//
//     // 初始化：假设所有人都不被赦免，全部设为 1
//     for (int i = 0; i < L; i++) {
//         for (int j = 0; j < L; j++) {
//             matrix[i][j] = 1;
//         }
//     }
//
//     // 开始递归分治
//     release(0, 0, L);
//
//     // 打印最终矩阵
//     for (int i = 0; i < L; i++) {
//         for (int j = 0; j < L; j++) {
//             cout << matrix[i][j];
//             if (j != L - 1) {
//                 cout << " ";
//             }
//         }
//         cout << endl;
//     }
//
//     return 0;
// }
