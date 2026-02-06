// #include <iostream>
//
// using namespace std;
//
// int main() {
//     int n;
//     cin >> n;
//
//     // 使用二维数组（或vector）存储
//     int a[21][21] = {0};
//
//     for (int i = 0; i < n; i++) {
//         // 每一行的首尾都是 1
//         a[i][0] = 1;
//         a[i][i] = 1;
//
//         // 计算中间的数字
//         for (int j = 1; j < i; j++) {
//             a[i][j] = a[i - 1][j - 1] + a[i - 1][j];
//         }
//     }
//
//     // 格式化输出
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j <= i; j++) {
//             cout << a[i][j] << " ";
//         }
//         cout << endl;
//     }
//
//     return 0;
// }
