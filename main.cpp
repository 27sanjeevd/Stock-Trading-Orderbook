#include <iostream>
#include <map>
#include <string>
#include <queue>

using namespace std;

struct Order {
    Order(double p, int v, int i, string& n) : price(p), volume(v), id(i), name(n) {}
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

        void AddOrder(double price, int volume, int id, string name) {
            priority_queue<Order, vector<Order>, compare> &order = price > 0 ? buys : sells;
            order.push({price, volume, id, name});

        }
        //make it so the function has some way of determining if an id corresponds to a buy or a sell (first few nums for example), to minimize costs
        void RemoveOrder(double price, int id) {
            priority_queue<Order, vector<Order>, compare> &order = price > 0 ? buys : sells;
            priority_queue<Order, vector<Order>, compare> tempQueue;
            while (!order.empty()) {
                Order temp1 = order.top();
                if (temp1.id == id) {
                    order.pop();
                    break;
                }
                tempQueue.push(temp1);
                order.pop();
            }

            while (!tempQueue.empty()){
                Order tempOrder = tempQueue.top();
                order.push(tempOrder);
                tempQueue.pop();
            }
        }

        void UpdateOrder(double price, int volume, int id){
            priority_queue<Order, vector<Order>, compare> &order = price > 0 ? buys : sells;
            priority_queue<Order, vector<Order>, compare> tempQueue;
            while (!order.empty()) {
                Order temp1 = order.top();
                if (temp1.id == id) {
                    tempQueue.push({temp1.price, volume, temp1.id, temp1.name});
                    order.pop();
                    break;
                }
                tempQueue.push(temp1);
                order.pop();
            }

            while (!tempQueue.empty()){
                Order tempOrder = tempQueue.top();
                order.push(tempOrder);
                tempQueue.pop();
            }
        }

        void Matching(){

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
    book.AddOrder(93.0, 5, 4, "Henry");
    book.AddOrder(115.2, 5, 5, "Austin");
    book.AddOrder(13.0, 5, 6, "Manny");
    book.AddOrder(-115.2, 5, 7, "Edward");
    book.AddOrder(-13.0, 5, 9, "Arthur");
    book.RemoveOrder(93.0, 4);
    book.UpdateOrder(13.0, 10, 6);
    book.PrintOrders();

    book.Matching();
    return 0;
}
