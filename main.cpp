// #include <iostream>
// using namespace std;
// int main() {
//     int a , b , c;
//     cin >> a >> b >> c;
//     if  ( a > b ) {
//         if ( a > c ) {
//             if ( b > c ) {
//                 cout << c << " " << b << " " << a;
//             } else {
//                 cout << b << " " << c << " " <<  a;
//             }
//         } else if ( b > c ) {
//             if ( a > c ) {
//                 if ( a > b ) {
//                     cout << b << " " << a << " " << c;
//                 } else {
//                     cout << a << " " << b << " " << c;
//                 }
//             }
//         }
//         else if ( a > c ) {
//             if ( b > c ) {
//                 if ( a > b ) {
//                     cout << b << a << c;
//                 } else {
//                     cout << a << " " << b << " " << c;
//                 }
//             } else {
//                 if ( a > b ) {
//                     if ( c > b ) {
//                         cout << b << " " << c << "" << a;
//                     }
//                 }
//             }
//         }
//         else if
//         ( b > c ) {
//             if ( a > c ) {
//                 if ( a > b ) {
//                     cout << b << " " << a << " " << c;
//                 } else {
//                     cout << a << " " << b << " " << c;
//                 }
//             } else {
//                 cout << a << " " << b << " " << c;
//             }
//         }
//         return 0;
//     }
// }


//#include <iostream>
// using namespace std;

//int main() {
    // int a, b, c;
    // cin >> a >> b >> c;

    // if (a <= b && a <= c) { // a 是最小的
        // if (b <= c) {
    //         cout << a << " " << b << " " << c << endl;
    //     } else {
    //         cout << a << " " << c << " " << b << endl;
    //     }
    // }
    // else if (b <= a && b <= c) { // b 是最小的
    //     if (a <= c) {
    //         cout << b << " " << a << " " << c << endl;
    //     } else {
    //         cout << b << " " << c << " " << a << endl;
    //     }
    // }
    // else { // c 是最小的
    //     if (a <= b) {
    //         cout << c << " " << a << " " << b << endl;
    //     } else {
    //         cout << c << " " << b << " " << a << endl;
    //     }
    // }
    //
    // return 0;
// }

// #include <iostream>
// using namespace std;
// int main() {
//     int a;
//     cin >> a;
//     if ( a % 2 == 0 ) {
//         if ( 4 < a && a <=12) {
//             cout << "1100" << endl;
//         } else {
//             cout << "0110" << endl;
//         }
//     }
//     else if ( a % 2 == 1 ) {
//         if ( 4 < a && a <= 12 ) {
//             cout << "0110" << endl;
//         } else {
//             cout << "0001" << endl;
//         }
//     }
//     return 0;
// }

// #include <iostream>
//
// using namespace std;
//
// int main() {
//     int a;
//     cin >> a;
//     if (a % 2 == 0 && (4 < a && a <= 12)) {
//         cout << 1 << " ";
//     } else {
//         cout << 0 << " ";
//     }
//
//     if (a % 2 == 0 || (4 < a && a <= 12)) {
//         cout << 1 << " ";
//     } else {
//         cout << 0 << " ";
//     }
//
//     if ((a % 2 == 0 && !(4 < a && a <= 12)) || (a % 2 != 0 && (4 < a && a <= 12))) {
//         cout << 1 << " ";
//     } else {
//         cout << 0 << " ";
//     }
//
//     if (a % 2 != 0 && !(4 < a && a <= 12)) {
//         cout << 1 << " ";
//     } else {
//         cout << 0 << " ";
//     }
//     return 0;
// }

// #include <iostream>
// using namespace std;
// int main () {
//     int n;
//     cin >> n;
//     if(n == 1 || n == 0) {
//         cout<<"Today, I ate" << n << "apple.";
//     } else {
//         cout<<"Today, I ate "<< n << "apples.";
//         return 0;
//     }
// }

// #include <iostream>
// using namespace std;
// int main (){
//     int n;
//     cin >> n;
//     if (5 * n < (11 + 3 * n)) {
//         cout << "Local" << endl;
//     } else {
//         cout << "Luogu" << endl;
//     }
//     return 0;
// }

// #include <iostream>
// using namespace std;
// int main() {
// #include <iostream>
//     using namespace std;
//     int main(); {
//         int a, b;
//         int c = 0; // 记录目前为止最大的学习总时长
//         int d = 0;         // 记录最不高兴的是周几
//
//         for (int day = 1; day <= 7; ++day) {
//             cin >> a >> b;
//             int total = 0;
//             // 判断是否超过 8 小时，并且是否比之前记录的更“不高兴”
//             if (total > 8 && total > c) {
//                 c = total;
//                 d = day;
//             }
//         }
//
//         // 如果 targetDay 没被更新过，初始值就是 0，符合题意
//         cout << d << endl;
//         return 0;
//     }
// }

// #include <iostream>
// using namespace std;
// int main() {
//     int a , b , c;
//     cin >> a >> b >> c;
//     if ((a + b) <= c || (a + c) <= b || (b + c) <= a) {
//         cout << "Not triangle" << endl;
//     }
//     if (((a * a + b * b) == c * c) || ((b * b + c * c) == a * a) || ((c * c + a * a) == b * b)) {
//         cout << "Right triangle" << endl;
//     }
//     if (((a * a + b * b) <= c * c) || ((b * b + c * c) <= a * a )) {
//         cout << "Acute triangle" << endl;
//     }
//     if ((a * a + b * b) >= c * c || (b * b + c * c) == a * a ) {
//         cout << "Obtuse triangle" << endl;
//     }
//     if (a == b && b == c && c == a) {
//         cout << "Isosceles triangle" << endl;
//     }
//     if (a == b && a == c && b == c) {
//         cout << "Equilateral triangle" << endl;
//     }
//     return 0;
// }

