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


/// source: https://github.com/denis-mustafin/Yolki-palki-kactl/blob/main/content/graph/MinCostMaxFlow.h
/**
 * Author: Tikhon Evteev/Vsevolod Nagibin
 * Date: 2024-04-08
 * Description: Min-cost max-flow.
 * Status: not tested
 */

// Warning: inf flow and cost!
// Each step works in O(E log V), sometimes O(V^2) may be better
// Dijkstra can be replaced
const ld INF = 1e18, EPS = 1e-9;

struct CostEdge {
  int from, to;
  int flow;
  int cap;
  ld cost;
  CostEdge(int from, int to, int flow, int cap, ld cost): from(from), to(to), flow(flow), cap(cap), cost(cost) {}
};

struct MinCost {
  vector<CostEdge> edges;
  vector<vector<int>> G;
  vector<ld> dist;
  vector<ld> potential;
  vector<int> par;
    vector<bool> used;
  int N;
  int s, t;

  MinCost(int N, int s, int t): edges(), G(N), dist(N), potential(N), par(N), N(N), s(s), t(t), used(N) {}

  void add_dir_edge(int from, int to, int cap, ld cost) {
    G[from].emplace_back(edges.size());
    edges.emplace_back(from, to, 0, cap, cost);
    G[to].emplace_back(edges.size());
    edges.emplace_back(to, from, 0, 0, -cost);
  }

    void calc_potential() {
      fill(potential.begin(), potential.end(), INF);
      potential[s] = 0;
    for (int i = 0; i < N; ++i) {
      for (auto e : edges) {
        if (e.flow < e.cap)
          potential[e.to] = min(potential[e.to], potential[e.from] + e.cost);
      }
    }
  }

  int flow;
  ld cost;

  bool step() {
    fill(dist.begin(), dist.end(), INF);
    fill(par.begin(), par.end(), -1);
      fill(used.begin(), used.end(), false);
    dist[s] = 0;

    for (int _ = 1; _ < N; ++_) {
        int v = -1;
        for (int u = 0; u < N; ++u) {
            if (used[u] || dist[u] > INF / 2) continue;
            if (v == -1 || dist[u] < dist[v]) {
                v = u;
            }
        }
        if (v == -1) break;
        used[v] = true;
        for (int i : G[v]) {
            if (edges[i].cap > edges[i].flow) {
                int u = edges[i].to;
                ld opt = dist[v] + edges[i].cost + potential[v] - potential[u];
                if (dist[u] > opt + EPS) {
                    par[u] = i;
                    dist[u] = opt;
                }
            }
        }
    }


    if (dist[t] == INF)
      return false;
    int mn = (int)1e9;
    int cur = t;
    while (cur != s) {
      mn = min(mn, edges[par[cur]].cap - edges[par[cur]].flow);
      cur = edges[par[cur]].from;
    }
    cur = t;
    while (cur != s) {
      edges[par[cur]].flow += mn;
      edges[par[cur] ^ 1].flow -= mn;
      cur = edges[par[cur]].from;
    }
    cost += mn * (dist[t] + potential[t] - potential[s]);
      flow += mn;
    for (int i = 0; i < N; ++i)
      potential[i] += dist[i];
    return true;
  }

  pair<ld, int> min_cost_max_flow() {
    flow = 0;
    cost = 0;
    calc_potential();
    while (step()) {}
    return make_pair(cost, flow);
  }
};

const int MAXN = 3e3;
Vector shop_pos[MAXN];
int c[MAXN];
ld cost[MAXN];
int d[MAXN];
Vector person_pos[MAXN];

ld fullness[MAXN][MAXN];
int id_ed[MAXN][MAXN];

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
            if (id == -1 || fullness[jd][shops[id]] < fullness[jd][shops[i]]) {
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

    MinCost mc(n + m + 2, n + m, n + m + 1);
    for (int i = 0; i < m; ++i) {
        mc.add_dir_edge(mc.s, i, d[i], 0);
        for (int j = 0; j < n; ++j) {
            id_ed[i][j] = mc.edges.size();
            mc.add_dir_edge(i, j + m, (int)1e9, (person_pos[i] - shop_pos[j]).len() / d[i]);
        }
    }
    vector<int> ids(n);
    for (int j = 0; j < n; ++j) {
        ids[j] = mc.edges.size();
        mc.add_dir_edge(j + m, mc.t, c[j], cost[j] / c[j]);
    }

    auto relax = mc.min_cost_max_flow();
    cerr << "cost: " << relax.first << "\n";
    cerr << "capacity: " << relax.second << "\n";

    vector<ld> openness(n);
    for (int i = 0; i < n; ++i) {
        openness[i] = mc.edges[ids[i]].flow * 1.0 / mc.edges[ids[i]].cap;
    }
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            fullness[i][j] = mc.edges[id_ed[i][j]].flow * 1.0 / mc.edges[id_ed[i][j]].cap;
        }
    }


    vector<int> open_order(n);
    iota(open_order.begin(), open_order.end(), 0);
    sort(open_order.begin(), open_order.end(), [&](int i, int j) { return openness[i] > openness[j]; });
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