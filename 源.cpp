#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cmath>  // Added this line for floor function

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

double convertCurrency(double amount, const string& from, const string& to, const vector<ExchangeRate>& rates) {
    if (from == to) return amount;

    // Check for direct conversion
    for (const auto& rate : rates) {
        if (rate.from == from && rate.to == to) {
            return amount * rate.rate;
        }
        if (rate.from == to && rate.to == from) {
            return amount / rate.rate;
        }
    }

    // If no direct conversion, try to find a path through CNY (assuming CNY is the base currency)
    double toCNY = amount;
    if (from != "CNY") {
        bool found = false;
        for (const auto& rate : rates) {
            if (rate.from == from && rate.to == "CNY") {
                toCNY = amount * rate.rate;
                found = true;
                break;
            }
            if (rate.from == "CNY" && rate.to == from) {
                toCNY = amount / rate.rate;
                found = true;
                break;
            }
        }
        if (!found) return 0; // No conversion path found
    }

    if (to == "CNY") return toCNY;

    for (const auto& rate : rates) {
        if (rate.from == "CNY" && rate.to == to) {
            return toCNY * rate.rate;
        }
        if (rate.from == to && rate.to == "CNY") {
            return toCNY / rate.rate;
        }
    }

    return 0; // No conversion path found
}

int main() {
    int n, m, k;
    vector<Investment> investments;
    vector<ExchangeRate> exchangeRates;

    // Read investments
    cin >> n;
    for (int i = 0; i < n; ++i) {
        Investment inv;
        cin >> inv.stock >> inv.person >> inv.currency >> inv.shares >> inv.price;
        investments.push_back(inv);
    }

    // Read exchange rates
    cin >> m;
    for (int i = 0; i < m; ++i) {
        ExchangeRate rate;
        cin >> rate.from >> rate.to >> rate.rate;
        exchangeRates.push_back(rate);
    }

    // Read queries
    cin >> k;
    vector<pair<string, string>> queries; // type+name, target currency
    for (int i = 0; i < k; ++i) {
        string type, name, currency;
        cin >> type >> name >> currency;
        queries.emplace_back(type + " " + name, currency);
    }

    // Process queries
    for (const auto& query : queries) {
        string type_name = query.first;
        string targetCurrency = query.second;

        double total = 0;

        size_t space_pos = type_name.find(' ');
        string type = type_name.substr(0, space_pos);
        string name = type_name.substr(space_pos + 1);

        for (const auto& inv : investments) {
            if ((type == "PERSON" && inv.person == name) ||
                (type == "STOCK" && inv.stock == name)) {
                double value = inv.shares * inv.price;
                double converted = convertCurrency(value, inv.currency, targetCurrency, exchangeRates);
                total += converted;
            }
        }

        // Format output to avoid scientific notation and unnecessary decimals
        ostringstream oss;
        if (total == floor(total)) {
            oss << static_cast<long long>(total);
        }
        else {
            oss << fixed << setprecision(2) << total;
            string s = oss.str();
            // Remove trailing zeros and possible . if no decimals left
            s.erase(s.find_last_not_of('0') + 1, string::npos);
            if (s.back() == '.') {
                s.pop_back();
            }
            oss.str("");
            oss << s;
        }
        cout << oss.str() << endl;
    }

    return 0;
}