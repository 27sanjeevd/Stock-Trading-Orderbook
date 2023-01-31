#include <iostream>
#include <map>
#include <string>
#include <queue>

using namespace std;

struct Order {
    Order(double p, int v, int i, std::string n) : price(p), volume(v), id(i), name(n) {}
    double price;
    int volume;
    int id;
    std::string name;
};

struct compare {
    bool operator()(const Order &a, const Order &b){
        return a.price < b.price;
    }
};

class OrderBook {
    private:
        map<double, Order> orders_;
        priority_queue<Order, vector<Order>, compare> buys;
        priority_queue<Order, vector<Order>, compare> sells;

    public:
        Order *order = nullptr;

        void AddOrder(double price, int volume, int id, string name) {
            order = new Order(price, volume, id, name);
            priority_queue<Order, vector<Order>, compare> &order1 = price > 0 ? buys : sells;
            order1.push({price, volume, id, name});

            orders_.insert({price, *order});
            delete order;
            order = nullptr;
        }

        void RemoveOrder(double price) {
            auto it = orders_.find(price);
            if (it != orders_.end()) {
                orders_.erase(it);
            }
        }

        void PrintOrders() {
            priority_queue<Order, vector<Order>, compare> copy = buys;

            while (!copy.empty()) {
                cout << copy.top().price << "\t" << copy.top().volume << "\t" << copy.top().id << "\t" << copy.top().name << endl;
                copy.pop();
            }

            copy = sells;

            while (!copy.empty()) {
                cout << copy.top().price << "\t" << copy.top().volume << "\t" << copy.top().id << "\t" << copy.top().name << endl;
                copy.pop();
            }
        }
};

int main() {
    OrderBook book;
    book.AddOrder(100.0, 10, 1, "John");
    book.AddOrder(105.0, 20, 2, "Jane");
    book.AddOrder(102.0, 5, 3, "Jim");
    book.AddOrder(93.0, 5, 3, "Henry");
    book.AddOrder(115.2, 5, 3, "Austin");
    book.AddOrder(13.0, 5, 3, "Manny");
    book.PrintOrders();
    return 0;
}
