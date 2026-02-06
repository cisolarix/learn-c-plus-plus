// #include <iostream>
// #include <string>
//
// using namespace std;
//
// /**
//  * hanoi 递归函数
//  * n: 盘子数量
//  * from: 起始杆
//  * to: 目标杆
//  * aux: 辅助杆
//  */
// void hanoi(int n, string from, string to, string aux) {
//     if (n == 1) {
//         // 只有一个盘子时，直接移动并结束递归
//         cout << from << "->1->" << to << endl;
//         return;
//     }
//
//     // 1. 将 n-1 个盘子从 from 移到 aux (目标变辅助，辅助变目标)
//     hanoi(n - 1, from, aux, to);
//
//     // 2. 将第 n 个盘子从 from 移到 to
//     cout << from << "->" << n << "->" << to << endl;
//
//     // 3. 将刚才那 n-1 个盘子从 aux 移到 to (起始变辅助，辅助变起始)
//     hanoi(n - 1, aux, to, from);
// }
//
// int main() {
//     int n;
//     string a, b, c;
//
//     // 输入盘子数和三个杆子的名称
//     if (cin >> n >> a >> b >> c) {
//         // 调用递归：目标是将 n 个盘子从 a 移到 b，c 作为辅助
//         hanoi(n, a, b, c);
//     }
//
//     return 0;
// }
