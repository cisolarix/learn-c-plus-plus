// #include <iostream>
// // 引入标准输入输出流库

// using namespace std;
// // 使用标准命名空间

// int main() {
// // 主函数开始
//     int A, B, C;
//     // 定义三个整数 A, B, C，表示三个三位数的比例
//     cin >> A >> B >> C;
//     // 读取比例 A:B:C
    
//     int used[10];
//     // 定义数组 used，用于统计数字 0-9 的使用次数
//     int count = 0;
//     // 定义计数器 count，记录找到的解的数量
    
//     for (int num1 = 123; num1 <= 987; num1++) {
//     // 枚举第一个三位数，从 123 到 987
//         if (num1 % A != 0) continue;
//         // 如果 num1 不是 A 的倍数，则跳过（因为 num1/A 必须是整数）
//         int base = num1 / A;
//         // 计算基数（公约数），使得 num1 = base * A
//         int num2 = base * B;
//         // 根据比例计算第二个三位数 num2 = base * B
//         int num3 = base * C;
//         // 根据比例计算第三个三位数 num3 = base * C
        
//         if (num2 < 100 || num2 > 999 || num3 < 100 || num3 > 999) continue;
//         // 如果 num2 或 num3 不是三位数，则跳过
        
//         for (int i = 0; i <= 9; i++) {
//         // 初始化 used 数组，将所有数字的使用次数清零
//             used[i] = 0;
//             // 将第 i 个数字的使用次数设为 0
//         }
        
//         int temp = num1;
//         // 将 num1 复制到临时变量 temp
//         used[temp / 100]++;
//         // 统计 num1 的百位数字（第一位）
//         used[(temp / 10) % 10]++;
//         // 统计 num1 的十位数字（第二位）
//         used[temp % 10]++;
//         // 统计 num1 的个位数字（第三位）
        
//         temp = num2;
//         // 将 num2 复制到临时变量 temp
//         used[temp / 100]++;
//         // 统计 num2 的百位数字
//         used[(temp / 10) % 10]++;
//         // 统计 num2 的十位数字
//         used[temp % 10]++;
//         // 统计 num2 的个位数字
        
//         temp = num3;
//         // 将 num3 复制到临时变量 temp
//         used[temp / 100]++;
//         // 统计 num3 的百位数字
//         used[(temp / 10) % 10]++;
//         // 统计 num3 的十位数字
//         used[temp % 10]++;
//         // 统计 num3 的个位数字
        
//         int ok = 1;
//         // 定义标志变量 ok，初始值为 1（表示有效）
//         if (used[0] != 0) ok = 0;
//         // 如果使用了数字 0，则标记为无效（题目要求只用 1-9）
//         for (int i = 1; i <= 9; i++) {
//         // 遍历数字 1 到 9
//             if (used[i] != 1) ok = 0;
//             // 如果某个数字使用次数不等于 1，则标记为无效
//         }
        
//         if (ok) {
//         // 如果三个数恰好使用 1-9 各一次
//             cout << num1 << " " << num2 << " " << num3 << endl;
//             // 输出这三个三位数，用空格分隔
//             count++;
//             // 解的数量加 1
//         }
//     }
    
//     if (count == 0) {
//     // 如果没有找到任何解
//         cout << "No!!!" << endl;
//         // 输出 "No!!!"
//     }
    
//     return 0;
//     // 程序正常结束
// }
