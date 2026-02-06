#include<bits/stdc++.h>
using namespace std;

enum class ordertype { LIMIT, CANCEL, MARKET };
enum class orderside { BUY, SELL };

struct order {
    int id;
    int64_t price;
    double quantity;
    orderside side;
    time_t time;
    ordertype typ;
};

class OrderBook {
private:
    map<int64_t, list<order>> sell;
    map<int64_t, list<order>, greater<int64_t>> buy;
    unordered_map<int, pair<int64_t, orderside>> cancel_map;

public:
    void add_limit_order(order& o) {
        if (o.quantity <= 0 || o.price <= 0) return;
        if (o.side == orderside::BUY) buy[o.price].push_back(o);
        else sell[o.price].push_back(o);
        cancel_map[o.id] = {o.price, o.side};
    }

    void match(order& o) {
        if (o.side == orderside::BUY) {
            while (o.quantity > 0 && !sell.empty() && o.price >= sell.begin()->first) {
                auto it = sell.begin();
                auto& lst = it->second;
                while (!lst.empty() && o.quantity > 0) {
                    order& s = lst.front();
                    double traded = min(o.quantity, s.quantity);
                    o.quantity -= traded;
                    s.quantity -= traded;
                    if (s.quantity == 0) {
                        cancel_map.erase(s.id);
                        lst.pop_front();
                    }
                }
                if (lst.empty()) sell.erase(it);
            }
        } else {
            while (o.quantity > 0 && !buy.empty() && o.price <= buy.begin()->first) {
                auto it = buy.begin();
                auto& lst = it->second;
                while (!lst.empty() && o.quantity > 0) {
                    order& b = lst.front();
                    double traded = min(o.quantity, b.quantity);
                    o.quantity -= traded;
                    b.quantity -= traded;
                    if (b.quantity == 0) {
                        cancel_map.erase(b.id);
                        lst.pop_front();
                    }
                }
                if (lst.empty()) buy.erase(it);
            }
        }
        if (o.quantity > 0) add_limit_order(o);
        else cancel_map.erase(o.id);
    }

    void cancel_order(int id) {
        if (!cancel_map.count(id)) return;
        int64_t price = cancel_map[id].first;
        orderside side = cancel_map[id].second;

        auto& book = (side == orderside::BUY) ? buy[price] : sell[price];
        for (auto it = book.begin(); it != book.end(); ++it) {
            if (it->id == id) {
                book.erase(it);
                break;
            }
        }
        if (book.empty()) {
            if (side == orderside::BUY) buy.erase(price);
            else sell.erase(price);
        }
        cancel_map.erase(id);
    }

    void simulate_order_book(int n) {
        srand(50);
        int id = 1;

        for (int i = 0; i < n; i++) {
            int action = rand() % 100;

            if (action < 40) {
                double qty = static_cast<double>(1 + rand() % 100);
                order o = {id++, 10000 + rand() % 1000, qty,
                           orderside::BUY, time(nullptr), ordertype::LIMIT};
                add_limit_order(o);

            } else if (action < 80) {
                double qty = static_cast<double>(1 + rand() % 100);
                order o = {id++, 10000 + rand() % 1000, qty,
                           orderside::SELL, time(nullptr), ordertype::LIMIT};
                add_limit_order(o);

            } else if (action < 90 && !sell.empty()) {
                double qty = static_cast<double>(1 + rand() % 50);
                order o = {id++, 11000, qty,
                           orderside::BUY, time(nullptr), ordertype::MARKET};
                match(o);

            } else if (action < 95 && !buy.empty()) {
                double qty = static_cast<double>(1 + rand() % 50);
                order o = {id++, 0, qty,
                           orderside::SELL, time(nullptr), ordertype::MARKET};
                match(o);

            } else if (!cancel_map.empty()) {
                auto it = cancel_map.begin();
                advance(it, rand() % cancel_map.size());
                cancel_order(it->first);
            }
        }
    }

    void validate_order_book() {
        for (auto it = buy.begin(); it != buy.end(); ++it) {
            int64_t price = it->first;
            for (const auto& o : it->second) {
                assert(cancel_map.count(o.id));
                assert(cancel_map[o.id].first == price);
                assert(cancel_map[o.id].second == orderside::BUY);
            }
        }

        for (auto it = sell.begin(); it != sell.end(); ++it) {
            int64_t price = it->first;
            for (const auto& o : it->second) {
                assert(cancel_map.count(o.id));
                assert(cancel_map[o.id].first == price);
                assert(cancel_map[o.id].second == orderside::SELL);
            }
        }
        cout << "Validation Passed\n";
    }
};

int main() {
    auto start = chrono::high_resolution_clock::now();

    OrderBook ob;
    ob.simulate_order_book(100000);
    ob.validate_order_book();

    auto end = chrono::high_resolution_clock::now();
    cout << "Time(ms): "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << endl;
    return 0;
}
