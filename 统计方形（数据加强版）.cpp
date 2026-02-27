// #include <iostream>

// using namespace std;

// int main() {
//     long long n, m;
//     cin >> n >> m;
    
//     long long total = n * (n + 1) / 2 * m * (m + 1) / 2;
    
//     long long square = 0;
//     long long minSize = (n < m ? n : m);
//     for (long long k = 1; k <= minSize; k++) {
//         square += (n - k + 1) * (m - k + 1);
//     }
    
//     long long rectangle = total - square;
    
//     cout << square << " " << rectangle << endl;
    
//     return 0;
// }
