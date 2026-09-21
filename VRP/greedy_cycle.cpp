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

ld operator%(const Vector& a, const Vector& b) {
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

struct TSP {
    struct Node {
        int id = -1;
        int nxt = -1;
        int prv = -1;
    };

    vector<Node> nd;
    int id_nd = 0;

    int nn(int id = -1, int nxt = -1, int prv = -1) {
        nd.push_back({id, nxt, prv});
        return id_nd++;
    }

    void connect(int u, int v) {
        nd[u].nxt = v;
        nd[v].prv = u;
    }

    vector<Vector> arr;

    vector<int> used;

    TSP (const vector<Vector>& arr): arr(arr), used(vector<int>(arr.size(), 0)) {}

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

    const int START_SAMPLE = 1'000'000;

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

        ld mem_len = len;
        vector<int> mem_nodes(n);
        for (int i = 0; i < n; ++i) {
            mem_nodes[i] = nn(i);
        }
        for (int v : nodes) {
            connect(mem_nodes[nd[v].id], mem_nodes[nd[nd[v].nxt].id]);
        }

        vector<ld> all_bad;
        for (int i = 0; i < START_SAMPLE || all_bad.size() < START_SAMPLE / 10; ++i) {
            auto cur = gen_segment(n);
            cur[0] = nodes[cur[0]]; cur[1] = nodes[cur[1]];
            ld delta = delta_rev(cur[0], cur[1]);
            if (delta > 0) {
                all_bad.push_back(delta);
            }
        }

        sort(all_bad.begin(), all_bad.end());
        ld avg = all_bad[all_bad.size() * 0.5];
        ld T_0 = -avg / log(0.5);
        ld T_end = 0.001;
        int N = min((int)3e8, (int)(5e10 / n));
        ld alpha = pow(T_end / T_0, 1.0 / N);

        ld T = T_0;
        int cnt_in_red = 0;
        for (int i = 0; i < N; ++i, T *= alpha) {
            if (len < mem_len - EPS) {
                mem_len = len;
                for (int v : nodes) {
                    connect(mem_nodes[nd[v].id], mem_nodes[nd[nd[v].nxt].id]);
                }
                cnt_in_red = 0;
            } else if (len > mem_len + EPS) {
                ++cnt_in_red;
                if (cnt_in_red > N / 100) {
                    len = mem_len;
                    for (int j = 0; j < n; ++j) {
                        nd[nodes[j]].id = j;
                    }
                    for (int v : mem_nodes) {
                        connect(nodes[nd[v].id], nodes[nd[nd[v].nxt].id]);
                    }
                    cnt_in_red = 0;
                }
            }


            auto cur = gen_segment(n);
            cur[0] = nodes[cur[0]]; cur[1] = nodes[cur[1]];
            ld delta = delta_rev(cur[0], cur[1]);
            if (delta < 0) {
                apply_rev(cur[0], cur[1], len);
                continue;
            }
            ld p = exp(-delta / T);
            if (u01(rnd) < p) {
                apply_rev(cur[0], cur[1], len);
            }
        }

        len = mem_len;
        for (int j = 0; j < n; ++j) {
            nd[nodes[j]].id = j;
        }
        for (int v : mem_nodes) {
            connect(nodes[nd[v].id], nodes[nd[nd[v].nxt].id]);
        }
    }

    pair<ld, vector<int>> solve() {
        int n = arr.size();
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

        vector<int> ans(n);
        int v = 0;
        for (int i = 0; i < n; ++i, v = nd[v].nxt) {
            ans[i] = nd[v].id;
        }
        return {len, ans};
    }
};

const int MAXN = 1000;
Vector pos[MAXN];
int d[MAXN];

double dist[MAXN][MAXN];

const double INF = 1e9;
const int THRESHOLD = 20;
double dp[1 << THRESHOLD][THRESHOLD];
int p[1 << THRESHOLD][THRESHOLD];

pair<ld, vector<int>> tsp(vector<int> ind) {
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

    TSP solver(arr);
    auto res = solver.solve();
    int pos = -1;
    for (int i = 0; i < n; ++i) {
        if (res.second[i] == 0) {
            pos = i;
        }
    }
    rotate(res.second.begin(), res.second.begin() + pos, res.second.end());
    for (int i = 0; i < n; ++i) {
        res.second[i] = ind[res.second[i]];
    }
    return res;
}

vector<int> out[MAXN];

void solve_() {
    int n, v, c;
    cin >> n >> v >> c;
    for (int i = 0; i < n; ++i) {
        cin >> d[i] >> pos[i].x >> pos[i].y;
    }
    for (int i = 1; i < n; ++i) {
        pos[i] = pos[i] - pos[0];
    }
    pos[0] = {0, 0};
    vector<int> ord(n - 1);
    iota(ord.begin(), ord.end(), 1);
    sort(ord.begin(), ord.end(), [&](int i, int j) { return pos[i] < pos[j]; });

    double ans = INF;

    for (int start = 0; start < 2 * (n + v) / v; ++start) {
        vector<vector<int>> division(v);
        vector<int> sums(v);
        int id = start % (n - 1);
        int total = 0;
        for (int i = 0; i < v; ++i) {
            division[i].push_back(0);
            sums[i] = 0;
            while (total < n - 1 && sums[i] + d[ord[id]] <= c) {
                sums[i] += d[ord[id]];
                division[i].push_back(ord[id]);
                id = (id + 1) % (n - 1);
                ++total;
            }
        }

        bool ok = true;
        vector<int> unfit;
        while (total < n - 1) {
            unfit.push_back(ord[id]);
            id = (id + 1) % (n - 1);
            ++total;
        }

        while (!unfit.empty()) {
            int id = unfit.back();
            unfit.pop_back();
            bool found = false;
            for (int i = 0; i < v; ++i) {
                if (sums[i] + d[id] <= c) {
                    sums[i] += d[id];
                    division[i].push_back(id);
                    found = true;
                    break;
                }
            }
            if (found) continue;

            for (int i = 0; i < v; ++i) {
                sort(division[i].begin(), division[i].end(), [&](int a, int b) {
                    if (d[a] >= sums[i] + d[id] - c && d[b] >= sums[i] + d[id] - c) {
                        return d[a] > d[b];
                    } else if (d[a] >= sums[i] + d[id] - c) {
                        return false;
                    } else if (d[b] >= sums[i] + d[id] - c) {
                        return true;
                    } else {
                        return d[a] < d[b];
                    }
                });

                if (d[division[i].back()] < d[id] && sums[i] + d[id] - d[division[i].back()] <= c) {
                    unfit.push_back(division[i].back());
                    sums[i] -= d[division[i].back()];
                    division[i].pop_back();
                    sums[i] += d[id];
                    division[i].push_back(id);
                    found = true;
                    break;
                }
            }
            if (found) continue;

            ok = false;
            break;
        }
        if (!ok) continue;

        double cur_res = 0;
        for (int i = 0; i < v; ++i) {
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
            cout << x << " ";
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