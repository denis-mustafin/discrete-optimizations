#include <bits/stdc++.h>
using namespace std;

using ld = long double;

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

int main(int argc, char* argv[]) {
    cout << fixed << setprecision(15);

    auto input = freopen(argv[1], "r", stdin);
    int n;
    cin >> n;
    vector<Vector> arr(n);
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }
    fclose(input);

    auto output = freopen(argv[2], "r", stdin);
    ld participant_len;
    if (!(cin >> participant_len)) {
        cout << "Presentation Error: No length in the output\nScore = 0\n";
        return 0;
    }

    vector<int> path(n);
    ld len = 0;
    vector<int> used(n);
    for (int i = 0; i < n; ++i) {
        if (!(cin >> path[i])) {
            cout << "Presentation Error: not enough vertices\nScore = 0\n";
            return 0;
        }
        if (path[i] < 0 || path[i] >= n) {
            cout << "Presentation Error: invalid vertex id\nScore = 0\n";
            return 0;
        }
        if (used[path[i]]) {
            cout << "Wrong Answer: Repeating vertices\nScore = 0\n";
            return 0;
        }
        if (i > 0) {
            len += (arr[path[i]] - arr[path[i - 1]]).len();
        }
    }

    len += (arr[path[0]] - arr[path[n - 1]]).len();
    if (abs(len - participant_len) > 1e-6 && abs(len - participant_len) / len > 1e-6) {
        cout << "Wrong Answer: participant claims the length is " << participant_len << " but it is actually " << len << "\nScore = 0\n";
        return 0;
    }

    cout << "OK\nScore = " << participant_len << "\n";

    return 0;
}