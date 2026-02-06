// #include <iostream>
//
// using namespace std;
//
// // 定义普通数组来存储三科成绩和总分
// // 假设 N 最大为 1000
// int chinese[1010];
// int math[1010];
// int english[1010];
// int total[1010];
//
// int main() {
//     int n;
//     // 输入学生人数
//     if (!(cin >> n)) return 0;
//
//     // 1. 输入成绩并预存总分
//     for (int i = 0; i < n; i++) {
//         cin >> chinese[i] >> math[i] >> english[i];
//         total[i] = chinese[i] + math[i] + english[i];
//     }
//
//     int count = 0;
//
//     // 2. 双重循环比较
//     for (int i = 0; i < n; i++) {
//         for (int j = i + 1; j < n; j++) {
//             // 计算各科分差（手动处理绝对值）
//             int diffC = chinese[i] - chinese[j];
//             if (diffC < 0) diffC = -diffC; // 如果是负数，取反变正
//
//             int diffM = math[i] - math[j];
//             if (diffM < 0) diffM = -diffM;
//
//             int diffE = english[i] - english[j];
//             if (diffE < 0) diffE = -diffE;
//
//             int diffT = total[i] - total[j];
//             if (diffT < 0) diffT = -diffT;
//
//             // 3. 判断是否满足“旗鼓相当”的所有条件
//             if (diffC <= 5 && diffM <= 5 && diffE <= 5 && diffT <= 10) {
//                 count++;
//             }
//         }
//     }
//
//     // 输出最终对数
//     cout << count << endl;
//
//     return 0;
// }
