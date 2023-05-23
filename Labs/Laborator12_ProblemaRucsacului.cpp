#include <algorithm> // for std::find
#include <iostream>
#include <vector>

static constexpr int M = 10;

struct Stare {
    int w;
    int p;

    // requires C++20
    bool operator==(Stare const& another) const = default;

    bool operator<(Stare const& another) const
    {
        return w < another.w;
    }
};

void interclasare_grafice(std::vector<Stare> const& Sc, std::vector<Stare> const& Tc, int M, std::vector<Stare>& R)
{
    int i = 0, j = 0;

    while (i < Sc.size() and j < Tc.size()) {
        if (Sc[i].w <= Tc[j].w) {
            if (Sc[i].w <= M and (R.size() == 0 or Sc[i].p > R.back().p)) {
                R.push_back(Sc[i]);
            }
            i++;
        } else {
            if (Tc[j].w <= M and (R.size() == 0 or Tc[j].p > R.back().p)) {
                R.push_back(Tc[j]);
            }
            j++;
        }
    }

    while (i < Sc.size()) {
        if (Sc[i].w <= M and (R.size() == 0 or Sc[i].p > R.back().p)) {
            R.push_back(Sc[i]);
        }
        i++;
    }

    while (j < Tc.size()) {
        if (Tc[j].w <= M and (R.size() == 0 or Tc[j].p > R.back().p)) {
            R.push_back(Tc[j]);
        }
        j++;
    }
}

void rucsac(int M, int n, std::vector<int> const& weights, std::vector<int> const& profits, std::vector<bool>& x)
{
    std::vector<std::vector<Stare>> S;
    S.resize(n + 1);
    std::vector<std::vector<Stare>> T;
    T.resize(n + 1);

    // Algorithm begins
    S[0] = { Stare { 0, 0 } };
    T[0] = { Stare { weights[0], profits[0] } };

    for (int i = 1; i <= n; ++i) {
        interclasare_grafice(S[i - 1], T[i - 1], M, S[i]);

        for (int j = 0; j < S[i].size(); ++j) {
            Stare temp;
            temp.w = S[i][j].w + weights[i];
            temp.p = S[i][j].p + profits[i];
            T[i].push_back(temp);
        }
    }

    // determina maximul
    Stare XiJi = S[n].back();

    for (int i = n - 1; i >= 0; --i) {
        if (std::find(S[i].begin(), S[i].end(), XiJi) != S[i].end()) {
            x[i] = false;
        } else {
            x[i] = true;
            XiJi.w -= weights[i];
            XiJi.p -= profits[i];
        }
    }
}

int main()
{
    std::vector<int> weights = { 3, 5, 6 };
    std::vector<int> profits = { 10, 30, 20 };
    std::vector<bool> x;
    x.resize(3);
    rucsac(M, 3, weights, profits, x);

    std::cout << "In your backpack we'll pack: " << std::endl;
    for (int i = 0; i < 3; ++i) {
        if (x[i]) {
            std::cout << "\tobject #" << i << " with a profit of " << profits[i] << " and weight of " << weights[i] << std::endl;
        }
    }

    return 0;
}
