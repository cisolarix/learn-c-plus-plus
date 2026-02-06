// #include <iostream>
//
// using namespace std;
//
// int main() {
//     int q;
//     string doc; // 我们的“文档”字符串
//
//     if (!(cin >> q >> doc)) return 0;
//
//     while (q--) {
//         int op;
//         cin >> op;
//
//         if (op == 1) {
//             // 操作 1：后接插入
//             string str;
//             cin >> str;
//             doc += str; // 直接使用 += 在末尾拼接
//             cout << doc << endl;
//         } else if (op == 2) {
//             // 操作 2：截取文档
//             int a, b;
//             cin >> a >> b;
//             // substr(起始位置, 长度) 返回截取后的子串
//             doc = doc.substr(a, b);
//             cout << doc << endl;
//         } else if (op == 3) {
//             // 操作 3：插入片段
//             int a;
//             string str;
//             cin >> a >> str;
//             // insert(插入位置, 插入字符串)
//             doc.insert(a, str);
//             cout << doc << endl;
//         } else if (op == 4) {
//             // 操作 4：查找子串
//             string str;
//             cin >> str;
//             // find(子串) 返回第一次出现的位置，找不到返回 string::npos
//             int pos = doc.find(str);
//             if (pos != string::npos) {
//                 cout << pos << endl;
//             } else {
//                 cout << -1 << endl;
//             }
//         }
//     }
//
//     return 0;
// }
