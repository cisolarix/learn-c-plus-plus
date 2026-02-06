// #include <iostream>
//
// using namespace std;
//
// int matrix[45][45];
//
// int main() {
//     int n;
//     if (!(cin >> n)) return 0;
//
//     // row: 行, column: 列
//     int r = 0;
//     int c = n / 2;
//     matrix[r][c] = 1;
//
//     for (int k = 2; k <= n * n; k++) {
//         int prev_r = r;
//         int prev_c = c;
//
//         // 规则 1：在第一行但不在最后一列
//         if (prev_r == 0 && prev_c != n - 1) {
//             r = n - 1;
//             c = prev_c + 1;
//         } else if (prev_c == n - 1 && prev_r != 0) {
//             r = prev_r - 1;
//             c = 0;
//         } else if (prev_r == 0 && prev_c == n - 1) {
//             r = prev_r + 1;
//             c = prev_c;
//         } else if (prev_r != 0 && prev_c != n - 1) {
//             int next_r = prev_r - 1;
//             int next_c = prev_c + 1;
//
//             if (matrix[next_r][next_c] == 0) {
//                 r = next_r;
//                 c = next_c;
//             } else {
//                 r = prev_r + 1;
//                 c = prev_c;
//             }
//         }
//         matrix[r][c] = k;
//     }
//
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < n; j++) {
//             cout << matrix[i][j];
//             if (j != n - 1) cout << " ";
//         }
//         cout << endl;
//     }
//
//     return 0;
// }
