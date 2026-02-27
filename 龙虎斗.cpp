#include <iostream>

using namespace std;

int main() {
    int n;
    cin >> n;
    
    long long c[100005];
    for (int i = 1; i <= n; i++) {
        cin >> c[i];
    }
    
    int m, p1;
    long long s1, s2;
    cin >> m >> p1 >> s1 >> s2;
    
    if (p1 != m) {
        c[p1] += s1;
    }
    
    long long dragon = 0;
    for (int i = 1; i < m; i++) {
        dragon += c[i] * (m - i);
    }
    
    long long tiger = 0;
    for (int i = m + 1; i <= n; i++) {
        tiger += c[i] * (i - m);
    }
    
    long long minDiff = 1e18;
    int bestP2 = 1;
    
    for (int p2 = 1; p2 <= n; p2++) {
        long long newDragon = dragon;
        long long newTiger = tiger;
        
        if (p2 < m) {
            newDragon += s2 * (m - p2);
        } else if (p2 > m) {
            newTiger += s2 * (p2 - m);
        }
        
        long long diff = (newDragon - newTiger >= 0 ? newDragon - newTiger : newTiger - newDragon);
        
        if (diff < minDiff) {
            minDiff = diff;
            bestP2 = p2;
        }
    }
    
    cout << bestP2 << endl;
    
    return 0;
}
