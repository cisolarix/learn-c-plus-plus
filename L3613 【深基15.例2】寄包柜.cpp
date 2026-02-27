// #include <iostream>
// #include <vector>

// using namespace std;

// struct Node {
//     int id;
//     int val;
// };

// vector<Node> locker[100005];

// int main() {
//     ios::sync_with_stdio(false);
//     cin.tie(0);

//     int n, q;
//     cin >> n >> q;

//     while (q--) {
//         int opt, i, j, k;
//         cin >> opt;
//         if (opt == 1) {
//             cin >> i >> j >> k;
//             bool found = false;
//             for (auto &node: locker[i]) {
//                 if (node.id == j) {
//                     node.val = k;
//                     found = true;
//                     break;
//                 }
//             }
//             if (!found && k != 0) {
//                 locker[i].push_back({j, k});
//             }
//         } else {
//             cin >> i >> j;
//             int ans = 0;
//             for (auto &node: locker[i]) {
//                 if (node.id == j) {
//                     ans = node.val;
//                     break;
//                 }
//             }
//             cout << ans << "\n";
//         }
//     }
//     return 0;
// }
