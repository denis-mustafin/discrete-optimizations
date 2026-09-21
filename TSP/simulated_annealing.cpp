#include <bits/stdc++.h>
using namespace std;

using ld = double;

const ld EPS = 1e-9;

mt19937 rnd(0);
uniform_real_distribution<ld> u01(0, 1);

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

struct Node {
    int id = -1;
    int nxt = -1;
    int prv = -1;
};

const int MAXMEM = 1e6;
Node nd[MAXMEM];
int id_nd = 0;

int nn(int id = -1, int nxt = -1, int prv = -1) {
    nd[id_nd] = {id, nxt, prv};
    return id_nd++;
}

void connect(int u, int v) {
    nd[u].nxt = v;
    nd[v].prv = u;
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

ld delta_rev(int l, int r) {
    return - (arr[nd[l].id] - arr[nd[nd[l].nxt].id]).len()
           - (arr[nd[r].id] - arr[nd[nd[r].nxt].id]).len()
           + (arr[nd[l].id] - arr[nd[r].id]).len()
           + (arr[nd[nd[l].nxt].id] - arr[nd[nd[r].nxt].id]).len();
}

void apply_rev(int l, int r, ld& len) {
    len += delta_rev(l, r);
    int mem = nd[r].nxt;
    int v0 = l, v1 = r, v2 = nd[r].prv;
    while (v2 != l) {
        connect(v0, v1);
        v0 = v1;
        v1 = v2;
        v2 = nd[v2].prv;
    }
    connect(v0, v1);
    connect(v1, mem);
}

void hill_climber(vector<int> nodes, ld& len) {
    int n = nodes.size();
    bool found = true;
    while (found) {
        found = false;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (delta_rev(nodes[i], nodes[j]) < -EPS) {
                    apply_rev(nodes[i], nodes[j], len);
                    found = true;
                }
            }
        }
    }
}

const int START_SAMPLE = 100'000;

array<int, 2> gen_segment(int n) {
    while (true) {
        int l = rnd() % n;
        int r = rnd() % n;
        if (l == r) continue;
        return {l, r};
    }
}

void simulated_annealing(vector<int> nodes, ld& len) {
    int n = nodes.size();
    ld sum_delta = 0;
    int cnt_bad = 0;
    for (int i = 0; i < START_SAMPLE || cnt_bad < START_SAMPLE / 10; ++i) {
        auto cur = gen_segment(n);
        cur[0] = nodes[cur[0]]; cur[1] = nodes[cur[1]];
        ld delta = delta_rev(cur[0], cur[1]);
        if (delta > 0) {
            sum_delta += delta;
            ++cnt_bad;
        }
    }

    ld avg = sum_delta / cnt_bad;
    ld T_0 = -avg / log(0.5);
    ld T_end = 0.1;
    int N = min((int)1e8, (int)(1e11 / n));
    ld alpha = pow(T_end / T_0, 1.0 / N);

    ld T = T_0;
    for (int i = 0; i < N; ++i, T *= alpha) {
        auto cur = gen_segment(n);
        cur[0] = nodes[cur[0]]; cur[1] = nodes[cur[1]];
        ld delta = delta_rev(cur[0], cur[1]);
        if (delta < 0) {
            apply_rev(cur[0], cur[1], len);
            continue;
        }
        ld p = exp(-delta / T);
        /// cerr << T << " " << p << " (" << n << ")" << endl;
        if (u01(rnd) < p) {
            apply_rev(cur[0], cur[1], len);
        }
    }
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

    for (int i = 0; i < n; ++i) {
        nn(i);
    }
    for (int i = 0; i < n; ++i) {
        int u = res[i], v = res[(i + 1) % n];
        connect(u, v);
    }

    vector<int> nodes(n);
    iota(nodes.begin(), nodes.end(), 0);

    simulated_annealing(nodes, len);

    cout << len << "\n";
    int v = 0;
    for (int i = 0; i < n; ++i, v = nd[v].nxt) {
        cout << nd[v].id << " ";
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