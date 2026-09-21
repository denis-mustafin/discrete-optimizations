#include <bits/stdc++.h>
using namespace std;

using ld = double;

struct Vector {
    ld x;
    ld y;

    Vector(ld x = 0, ld y = 0): x(x), y(y) {}

    ld sqlen() const { return x * x + y * y; }
    ld len() const { return hypotl(x, y); }
};

istream& operator>>(istream& in, Vector& v) {
    in >> v.x >> v.y;
    return in;
}

Vector operator-(const Vector& a, const Vector& b) {
    return {a.x - b.x, a.y - b.y};
}

const ld INF = 1e18;

const int MAXN = 1e4;
Vector shop_pos[MAXN];
int c[MAXN];
ld cost[MAXN];
int d[MAXN];
Vector person_pos[MAXN];

pair<ld, vector<vector<int>>> greedy_match(vector<int> people, const vector<int>& shops) {
    int n = shops.size(), m = people.size();
    vector<vector<int>> res(n);
    vector<int> cap(n);
    for (int i = 0; i < n; ++i) {
        cap[i] = c[shops[i]];
    }
    ld ans = 0;
    for (int i = 0; i < n; ++i) {
        ans += cost[shops[i]];
    }

    sort(people.begin(), people.end(), [](int i, int j) { return d[i] > d[j]; });
    for (int j = 0; j < m; ++j) {
        int jd = people[j];
        int id = -1;
        for (int i = 0; i < n; ++i) {
            if (cap[i] < d[jd]) continue;
            if (id == -1 || ((shop_pos[shops[i]] - person_pos[jd]).sqlen() < (shop_pos[shops[id]] - person_pos[people[jd]]).sqlen())) {
                id = i;
            }
        }
        if (id == -1) {
            return {INF, res};
        }
        cap[id] -= d[jd];
        res[id].push_back(jd);
        ans += (shop_pos[shops[id]] - person_pos[jd]).len();
    }
    return {ans, res};
}

void solve_() {
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        cin >> cost[i] >> c[i] >> shop_pos[i];
    }
    for (int i = 0; i < m; ++i) {
        cin >> d[i] >> person_pos[i];
    }

    vector<int> open_order(n);
    iota(open_order.begin(), open_order.end(), 0);
    sort(open_order.begin(), open_order.end(), [](int i, int j) { return cost[i] * c[j] < cost[j] * c[i]; });
    vector<int> people(m);
    iota(people.begin(), people.end(), 0);

    ld ans = INF;
    vector<pair<int, vector<int>>> out;
    for (int cnt = 1; cnt <= n; ++cnt) {
        auto res = greedy_match(people, vector<int>(open_order.begin(), open_order.begin() + cnt));
        if (res.first >= INF / 2) continue;
        for (int i = 0; i < res.second.size(); ++i) {
            if (res.second[i].empty()) {
                res.first -= cost[open_order[i]];
            }
        }
        if (res.first >= ans) continue;
        ans = res.first;
        out.clear();
        for (int i = 0; i < cnt; ++i) {
            if (res.second[i].empty()) continue;
            out.emplace_back(open_order[i], res.second[i]);
        }
    }

    cout << ans << "\n";
    cout << out.size() << "\n";
    for (auto elem : out) {
        cout << elem.first << " " << elem.second.size() << " ";
        for (int x : elem.second) {
            cout << x << " ";
        }
        cout << "\n";
    }
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