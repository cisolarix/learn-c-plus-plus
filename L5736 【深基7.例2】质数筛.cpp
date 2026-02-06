// #include <bits/stdc++.h>
//
// using namespace std;
//
// const int MAX_VAL = 100005;
// bool is_prime[MAX_VAL];
//
// // 预处理函数：筛选出所有质数
// void sieve() {
//     fill(is_prime + 2, is_prime + MAX_VAL, true); // 先假设全是质数
//     for (int p = 2; p * p < MAX_VAL; p++) {
//         if (is_prime[p]) {
//             // 将 p 的所有倍数标记为非质数
//             for (int i = p * p; i < MAX_VAL; i += p)
//                 is_prime[i] = false;
//         }
//     }
// }
//
// int main() {
//     sieve(); // 先筛好，后面直接查表
//
//     int n;
//     cin >> n;
//     while (n--) {
//         int a;
//         cin >> a;
//         if (a >= 2 && is_prime[a]) {
//             cout << a << " ";
//         }
//     }
//     return 0;
// }
