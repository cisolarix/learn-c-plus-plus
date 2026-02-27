// #include <iostream>
// #include <string>

// using namespace std;

// int main() {
//     string expr;
//     cin >> expr;
    
//     int count = 0;
//     bool matched = true;
    
//     for (int i = 0; i < expr.length(); i++) {
//         if (expr[i] == '(') {
//             count++;
//         } else if (expr[i] == ')') {
//             count--;
//             if (count < 0) {
//                 matched = false;
//                 break;
//             }
//         } else if (expr[i] == '@') {
//             break;
//         }
//     }
    
//     if (matched && count == 0) {
//         cout << "YES" << endl;
//     } else {
//         cout << "NO" << endl;
//     }
    
//     return 0;
// }
