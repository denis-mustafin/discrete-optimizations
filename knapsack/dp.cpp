#include <bits/stdc++.h>
using namespace std;

vector<array<int, 3>> items;
vector<int> pref;

vector<int> calc_dp(int l, int r, int W) {
    static vector<int> dp;
    dp.resize(W + 1);
    fill(dp.begin(), dp.end(), 0);
    for (int i = l; i < r; ++i) {
        for (int w = W - items[i][1]; w >= 0; --w) {
            dp[w + items[i][1]] = max(dp[w + items[i][1]], dp[w] + items[i][0]);
        }
    }
    return dp;
}

int restore(int l, int r, int W, vector<int>& ans) {
    if (pref[r] - pref[l] <= W) {
        int res = 0;
        for (int i = l; i < r; ++i) {
            ans.push_back(items[i][2]);
            res += items[i][0];
        }
        return res;
    }
    int m = (l + r) / 2;
    static vector<int> dp_left, dp_right;
    dp_left = calc_dp(l, m, W);
    dp_right = calc_dp(m, r, W);
    int opt = -1, optw = -1;
    for (int w = W; w >= 0; --w) {
        if (dp_left[w] + dp_right[W - w] > opt) {
            opt = dp_left[w] + dp_right[W - w];
            optw = w;
        }
    }
    int res = 0;
    bool go_left = (dp_left[optw] != 0);
    bool go_right = (dp_right[W - optw] != 0);
    if (go_left) {
        res += restore(l, m, optw, ans);
    }
    if (go_right) {
        res += restore(m, r, W - optw, ans);
    }
    return res;
}

void solve_() {
    int n, W;
    cin >> n >> W;
    items.resize(n);
    pref.resize(n + 1);
    for (int i = 0; i < n; ++i) {
        cin >> items[i][0] >> items[i][1];
        items[i][2] = i;
    }

    sort(items.begin(), items.end(), [](auto a, auto b) {
        return 1ll * a[0] * b[1] > 1ll * b[0] * a[1];
    });

    pref[0] = 0;
    for (int i = 0; i < n; ++i) {
        pref[i + 1] = pref[i] + items[i][1];
    }

    vector<int> ans;
    int res = restore(0, n, W, ans);
    cout << res << "\n";
    cout << ans.size() << "\n";
    for (int i : ans) {
        cout << i << " ";
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