// #include <iostream>
// #include <string>

// using namespace std;

// int main() {
//     string a, b;
//     cin >> a >> b;
    
//     string result = "";
//     int carry = 0;
    
//     int i = a.length() - 1;
//     int j = b.length() - 1;
    
//     while (i >= 0 || j >= 0 || carry > 0) {
//         int digitA = 0;
//         int digitB = 0;
        
//         if (i >= 0) {
//             digitA = a[i] - '0';
//             i--;
//         }

//         if (j >= 0) {
//             digitB = b[j] - '0';
//             j--;
//         }
        
//         int sum = digitA + digitB + carry;
//         carry = sum / 10;
//         result = (char)('0' + sum % 10) + result;
//     }
    
//     cout << result << endl;
    
//     return 0;
// }
