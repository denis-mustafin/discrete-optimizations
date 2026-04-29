#include <bits/stdc++.h>
using namespace std;

struct Vector {
    double x;
    double y;

    Vector(double x = 0, double y = 0): x(x), y(y) {}

    double sqlen() const { return x * x + y * y; }
    double len() const { return sqrt(sqlen()); }
};

Vector operator-(Vector a, const Vector& b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

double operator%(const Vector& a, const Vector& b) {
    return a.x * b.y - a.y * b.x;
}

bool hp(const Vector& v) {
    if (v.y == 0) return v.x > 0;
    return v.y > 0;
}

bool operator<(const Vector& a, const Vector& b) {
    if (hp(a) != hp(b)) {
        return hp(a);
    }
    return a % b > 0;
}

const int MAXN = 1000;
Vector pos[MAXN];
int d[MAXN];

double dist[MAXN][MAXN];

const double INF = 1e9;
const int THRESHOLD = 20;
double dp[1 << THRESHOLD][THRESHOLD];
int p[1 << THRESHOLD][THRESHOLD];

pair<double, vector<int>> tsp(vector<int> ind) {
    int n = ind.size();
    vector<Vector> arr(n);
    for (int i = 0; i < n; ++i) {
        arr[i] = pos[ind[i]];
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            dist[i][j] = (arr[i] - arr[j]).len();
        }
    }

    if (n <= THRESHOLD) {
        for (int i = 1; i < (1 << n); i += 2) {
            fill(dp[i], dp[i] + n, INF);
        }
        dp[1][0] = 0;
        for (int mask = 1; mask < (1 << n); mask += 2) {
            for (int lst = 0; lst < n; ++lst) {
                if (mask & (1 << lst)) {
                    for (int v = 0; v < n; ++v) {
                        if (mask & (1 << v)) continue;
                        if (dp[mask][lst] + dist[lst][v] < dp[mask ^ (1 << v)][v]) {
                            dp[mask ^ (1 << v)][v] = dp[mask][lst] + dist[lst][v];
                            p[mask ^ (1 << v)][v] = lst;
                        }
                    }
                }
            }
        }
        int id = -1;
        double res = INF;
        for (int i = 0; i < n; ++i) {
            if (id == -1 || dp[(1 << n) - 1][i] + dist[0][i]) {
                res = dp[(1 << n) - 1][i] + dist[0][i];
                id = i;
            }
        }
        vector<int> cyc;
        int mask = (1 << n) - 1;
        int lst = id;
        while (mask != 1) {
            cyc.push_back(lst);
            int v = p[mask][lst];
            mask ^= (1 << lst);
            lst = v;
        }
        cyc.push_back(0);
        vector<int> out(n);
        for (int i = 0; i < n; ++i) {
            out[i] = ind[cyc[i]];
        }
        reverse(out.begin(), out.end());
        return {res, out};
    }

    vector<int> ord;
    ord.push_back(0);
    vector<int> used(n, 0);
    used[0] = 1;
    double res = 0;
    for (int i = 1; i < n; ++i) {
        int id = -1;
        for (int j = 0; j < n; ++j) {
            if (used[j]) continue;
            if (id == -1 || (arr[j] - arr[ord.back()]).len() < (arr[id] - arr[ord.back()]).len()) {
                id = j;
            }
        }
        res += (arr[id] - arr[ord.back()]).len();
        ord.push_back(id);
        used[id] = true;
    }
    res += (arr[ord.back()] - arr[ord[0]]).len();
    vector<int> out(n);
    for (int i = 0; i < n; ++i) {
        out[i] = ind[ord[i]];
    }
    return {res, out};
}

vector<int> out[MAXN];

void solve_() {
    int n, v, c;
    cin >> n >> v >> c;
    for (int i = 0; i < n; ++i) {
        cin >> d[i] >> pos[i].x >> pos[i].y;
    }
    pos[n] = {0, 0};
    vector<int> ord(n);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int i, int j) { return pos[i] < pos[j]; });

    double ans = INF;

    for (int start = 0; start < 2 * (n + v) / v; ++start) {
        cerr << start << endl;
        vector<vector<int>> division(v);
        vector<int> sums(v);
        int id = start;
        int total = 0;
        for (int i = 0; i < v; ++i) {
            division[i].push_back(n);
            sums[i] = 0;
            while (total < n && sums[i] + d[ord[id]] <= c) {
                sums[i] += d[ord[id]];
                division[i].push_back(ord[id]);
                id = (id + 1) % n;
                ++total;
            }
        }

        bool ok = true;
        while (total < n) {
            bool found = false;
            for (int i = 0; i < v; ++i) {
                if (sums[i] + d[ord[id]] <= c) {
                    sums[i] += d[ord[id]];
                    division[i].push_back(ord[id]);
                    id = (id + 1) % n;
                    ++total;
                    found = true;
                    break;
                }
            }

            if (!found) {
                ok = false;
                break;
            }
        }
        if (!ok) continue;

        double cur_res = 0;
        for (int i = 0; i < v; ++i) {
            /// cerr << "i: " << i << " " << division[i].size() << endl;
            auto cur = tsp(division[i]);
            cur_res += cur.first;
            division[i] = vector<int>(cur.second.begin() + 1, cur.second.end());
        }

        if (cur_res < ans) {
            ans = cur_res;
            for (int i = 0; i < v; ++i) {
                out[i] = division[i];
            }
        }
    }
    cout << ans << "\n";
    for (int i = 0; i < v; ++i) {
        cout << out[i].size() << " ";
        for (int x : out[i]) {
            cout << x + 1 << " ";
        }
        cout << "\n";
    }
}

/// #define MULTITEST

signed main() {
#ifdef LOCAL
    freopen("../scored_data/vrp_421_41_1", "r", stdin);
    freopen("../scored_data_output/vrp_421_41_1.out", "w", stdout);
    auto start = clock();
#else
    ios_base::sync_with_stdio(false);
    cin.tie(0);
#endif

    cout << fixed << setprecision(20);

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