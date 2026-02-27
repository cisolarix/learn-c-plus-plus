// #include <iostream>
// #include <string>
// using namespace std;
//
// int main() {
//     int n, m;
//     cin >> n >> m;
//
//     string grid[105];
//     for (int i = 0; i < n; i++) {
//         cin >> grid[i];
//     }
//
//     int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
//     int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
//
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < m; j++) {
//             if (grid[i][j] == '?') {
//                 int count = 0;
//                 for (int k = 0; k < 8; k++) {
//                     int ni = i + dx[k];
//                     int nj = j + dy[k];
//                     if (ni >= 0 && ni < n && nj >= 0 && nj < m && grid[ni][nj] == '*') {
//                         count++;
//                     }
//                 }
//                 cout << count;
//             } else {
//                 cout << grid[i][j];
//             }
//         }
//         cout << "\n";
//     }
//
//     return 0;
// }
