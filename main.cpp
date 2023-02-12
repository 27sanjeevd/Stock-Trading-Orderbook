#include <iostream>
#include <map>
#include <string>
#include <queue>

using namespace std;

enum OrderType {
    BUY = 0,
    SELL = 0,
};

struct Order {
    double amount;
    OrderType type;
    double price;
    int id;
};

struct OrderCompare {
    bool operator()(const Order &a, const Order &b) const {
        if (a.type == BUY) {
            return a.price > b.price;
        } else {
            return a.price < b.price;
        }
    }
};