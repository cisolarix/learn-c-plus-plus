// #include <iostream>
// using namespace std;
//
// int main() {
//     int a, b, c;
//
//     cout << "Enter two numbers: " << endl;
//
//     cin >> a >> b >> c;
//
//     if (a <= b && a <= c) { // a 是最小的
//         if (b <= c) {
//             cout << a << " " << b << " " << c << endl;
//         } else {
//             cout << a << " " << c << " " << b << endl;
//         }
//     }
//     else if (b <= a && b <= c) { // b 是最小的
//         if (a <= c) {
//             cout << b << " " << a << " " << c << endl;
//         } else {
//             cout << b << " " << c << " " << a << endl;
//         }
//     }
//     else { // c 是最小的
//         if (a <= b) {
//             cout << c << " " << a << " " << b << endl;
//         } else {
//             cout << c << " " << b << " " << a << endl;
//         }
//     }
//
//     return 0;
// }