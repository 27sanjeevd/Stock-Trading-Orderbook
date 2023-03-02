#include <iostream>
#include <queue>
#include <functional>

using namespace std;

struct Order {
    int quantity;
    double price;
    bool side;
    int id;
};

struct BuyOrderComparator {
    bool operator()(const Order& a, const Order& b) const {
        return a.price > b.price;
    }
};

struct SellOrderComparator {
    bool operator()(const Order& a, const Order& b) const {
        return a.price < b.price;
    }
};

class Orderbook {
    public:

        void addOrder(Order order, int id) {
            order.id = id;
            if (order.side == true){
                buys.push(order);
            }
            else {
                sells.push(order);
            }
        }

        void removeOrder(int id){
            priority_queue<Order, vector<Order>, BuyOrderComparator> newBuys;
            priority_queue<Order, vector<Order>, SellOrderComparator> newSells;

            while (!buys.empty()){
                Order order = buys.top();
                buys.pop();
                if (order.id != id){
                    newBuys.push(order);
                }
            }

            while (!sells.empty()){
                Order order = sells.top();
                sells.pop();
                if (order.id != id){
                    newSells.push(order);
                }
            }

            buys = newBuys;
            sells = newSells;
        }

        void printOrders(){
            priority_queue<Order, vector<Order>, BuyOrderComparator> copyBuys = buys;
            priority_queue<Order, vector<Order>, SellOrderComparator> copySells = sells;

            while (!copyBuys.empty()){
                Order order = copyBuys.top();
                copyBuys.pop();
                cout << "Buy -> Amount: " << order.quantity << " Price: " << order.price << " ID: " << order.id << endl;
            }

            while (!copySells.empty()){
                Order order = copySells.top();
                copySells.pop();
                cout << "Sell -> Amount: " << order.quantity << " Price: " << order.price << " ID: " << order.id << endl;
            }
        }

    private:
        priority_queue<Order, vector<Order>, BuyOrderComparator> buys;
        priority_queue<Order, vector<Order>, SellOrderComparator> sells;
};

int main() {
    int id = 0;
    Orderbook book1;
    book1.addOrder({15, 105.5, true}, id++);
    book1.addOrder({12, 2.5, true}, id++);
    book1.addOrder({115, 96, true}, id++);
    book1.addOrder({17, 105.5, false}, id++);
    book1.printOrders();
    book1.removeOrder(2);
    cout << endl;
    book1.printOrders();

    return 0;

}