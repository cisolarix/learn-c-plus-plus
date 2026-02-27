// #include <iostream>
// #include <string>

// using namespace std;

// string passward(string str, int pass) {
//     pass = pass % 26;
//     for (int i = 0; i < str.length(); i++) {
//         if (str[i] == ' ') {
//             continue;
//         } else if (str[i] + pass > 'z') {
//             str[i] = 'a' + (pass - ('z' - str[i])) - 1;
//         } else {
//             str[i] = str[i] + pass;
//         }
//     }
//     return str;
// }

// int main() {
//     string str;
//     getline(cin, str);
//     cout << passward(str, 6) << endl;
//     cin.get();
//     return 0;
// }
