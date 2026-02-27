// #include <iostream>
// #include <unordered_map>

// using namespace std;

// int main() {
//     ios::sync_with_stdio(false);
//     cin.tie(0);
    
//     int T;
//     cin >> T;
    
//     while (T--) {
//         int n;
//         cin >> n;
        
//         unordered_map<int, bool> appeared;
        
//         bool first = true;
        
//         for (int i = 0; i < n; i++) {
//             int num;
//             cin >> num;
            
//             if (!appeared[num]) {
//                 if (!first) {
//                     cout << " ";
//                 }
//                 cout << num;
//                 first = false;
                
//                 appeared[num] = true;
//             }
//         }
        
//         cout << "\n";
//     }
    
//     return 0;
// }
