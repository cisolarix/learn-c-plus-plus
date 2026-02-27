// #include <iostream>

// using namespace std;

// int main() {
//     int weights[6] = {1, 2, 3, 5, 10, 20};
    
//     int num[6];
    
//     for (int i = 0; i < 6; i++) {
//         cin >> num[i];
//     }
    
//     int canReach[1001];
    
//     for (int i = 0; i <= 1000; i++) {
//         canReach[i] = 0;
//     }
    
//     for (int n1 = 0; n1 <= num[0]; n1++) {
//         for (int n2 = 0; n2 <= num[1]; n2++) {
//             for (int n3 = 0; n3 <= num[2]; n3++) {
//                 for (int n4 = 0; n4 <= num[3]; n4++) {
//                     for (int n5 = 0; n5 <= num[4]; n5++) {
//                         for (int n6 = 0; n6 <= num[5]; n6++) {
//                             int totalWeight = n1 * weights[0] + n2 * weights[1] + n3 * weights[2] + n4 * weights[3] + n5 * weights[4] + n6 * weights[5];
//                             if (totalWeight > 0 && totalWeight <= 1000) {
//                                 canReach[totalWeight] = 1;
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//     }
    
//     int count = 0;
    
//     for (int i = 1; i <= 1000; i++) {
//         if (canReach[i]) {
//             count++;
//         }
//     }
    
//     cout << "Total=" << count << endl;
    
//     return 0;
// }

