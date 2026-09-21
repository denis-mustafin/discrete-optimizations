#include <bits/stdc++.h>
using namespace std;

using ld = double;

struct Vector {
    ld x;
    ld y;

    Vector(ld x = 0, ld y = 0): x(x), y(y) {}

    ld sqlen() const { return x * x + y * y; }
    ld len() const { return sqrt(sqlen()); }
};

istream& operator>>(istream& in, Vector& v) {
    in >> v.x >> v.y;
    return in;
}

Vector operator-(const Vector& a, const Vector& b) {
    return {a.x - b.x, a.y - b.y};
}

const int MAXN = 1e5;
Vector arr[MAXN];
int used[MAXN];

vector<int> greedy(vector<int> vert, int start, int end = -1) {
    vector<int> path;
    int n = vert.size();
    for (int x : vert) {
        used[x] = 0;
    }
    used[start] = 1;
    for (int i = 1; i < n; ++i) {
        int nxt = -1;
        for (int v : vert) {
            if (used[v] || (v == end && i != n - 1)) continue;
            if (nxt == -1 || (arr[start] - arr[nxt]).sqlen() > (arr[start] - arr[v]).sqlen()) {
                nxt = v;
            }
        }
        path.push_back(nxt);
        used[nxt] = true;
        start = nxt;
    }
    return path;
}

void solve_() {
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }
    vector<int> all(n);
    iota(all.begin(), all.end(), 0);
    auto res = greedy(all, 0);
    res.push_back(0);
    ld len = 0;
    for (int i = 0; i < n; ++i) {
        len += (arr[res[i]] - arr[res[(i + 1) % n]]).len();
    }
    cout << len << "\n";
    for (int v : res) {
        cout << v << " ";
    }
    cout << "\n";
}

/// #define MULTITEST

signed main() {
#ifdef LOCAL
    freopen("../input.txt", "r", stdin);
    freopen("../output.txt", "w", stdout);
    auto start = clock();
#else
    ios_base::sync_with_stdio(false);
    cin.tie(0);
#endif

    cout << fixed << setprecision(15);

    int tst = 1;
#ifdef MULTITEST
    cin >> tst;
#endif // MULTITEST
    while (tst--) {
        solve_();
    }

#ifdef LOCAL
    cerr << "\ntime: ";
    cerr << setprecision(4) << (clock() - start) * 1.0 / CLOCKS_PER_SEC << endl;
#endif // LOCAL
    return 0;
}