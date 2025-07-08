#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <iomanip>
#include <sstream>
using namespace std;

struct Investment {
    string stock;
    string person;
    string currency;
    int shares;
    double price;
};

struct ExchangeRate {
    string from;
    string to;
    double rate;
};

struct Query {
    string type;
    string name;
    string target_currency;
};

double convert_currency(double amount, const string& from, const string& to,
    const vector<ExchangeRate>& rates) {
    if (from == to) return amount;

    for (const auto& rate : rates) {
        if (rate.from == from && rate.to == to) return amount * rate.rate;
        if (rate.from == to && rate.to == from) return amount / rate.rate;
    }
    return 0;
}

string format_amount(double amount) {
    ostringstream oss;
    if (amount == floor(amount)) {
        oss << static_cast<long long>(amount);
    }
    else {
        oss << fixed << setprecision(2) << amount;
        string s = oss.str();
        s.erase(s.find_last_not_of('0') + 1, string::npos);
        if (s.back() == '.') s.pop_back();
        return s;
    }
    return oss.str();
}

vector<Investment> read_investments() {
    int n;
    cin >> n;
    vector<Investment> investments(n);
    for (auto& inv : investments) {
        cin >> inv.stock >> inv.person >> inv.currency >> inv.shares >> inv.price;
    }
    return investments;
}

vector<ExchangeRate> read_exchange_rates() {
    int m;
    cin >> m;
    vector<ExchangeRate> rates(m);
    for (auto& rate : rates) {
        cin >> rate.from >> rate.to >> rate.rate;
    }
    return rates;
}

vector<Query> read_queries() {
    int k;
    cin >> k;
    vector<Query> queries(k);
    for (auto& q : queries) {
        cin >> q.type >> q.name >> q.target_currency;
    }
    return queries;
}

double calculate_total(const Query& query, const vector<Investment>& investments,
    const vector<ExchangeRate>& rates) {
    double total = 0;
    for (const auto& inv : investments) {
        if ((query.type == "PERSON" && inv.person == query.name) ||
            (query.type == "STOCK" && inv.stock == query.name)) {
            total += convert_currency(inv.shares * inv.price,
                inv.currency,
                query.target_currency,
                rates);
        }
    }
    return total;
}

int main() {
    auto investments = read_investments();
    auto rates = read_exchange_rates();
    auto queries = read_queries();

    for (const auto& query : queries) {
        double total = calculate_total(query, investments, rates);
        cout << format_amount(total) << endl;
    }

    return 0;
}