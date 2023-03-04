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
    bool operator()(Order& a, Order& b) {
        return a.price < b.price;
    }
};

struct SellOrderComparator {
    bool operator()(Order& a, Order& b) {
        return a.price > b.price;
    }
};

class Orderbook {
    public:

        void addOrder(Order order) {
            if (order.side == true){
                order.id = buy_order += 2;
            }
            else {
                order.id = sell_order += 2;
            }

            if (order.side == true){
                buys.push(order);
            }
            else {
                sells.push(order);
            }
        }

        void changeQuantity(int id, int quantity){
            if (id % 2 == 0){
                priority_queue<Order, vector<Order>, BuyOrderComparator> newBuys;

                while (!buys.empty()){
                    Order order = buys.top();
                    buys.pop();
                    if (order.id == id){
                        order.quantity = quantity;
                    }
                    newBuys.push(order);
                }

                buys = newBuys;
            }
            else {
                priority_queue<Order, vector<Order>, SellOrderComparator> newSells;


                while (!sells.empty()){
                    Order order = sells.top();
                    sells.pop();
                    if (order.id == id){
                        order.quantity = quantity;
                    }
                    newSells.push(order);
                }

                sells = newSells;
            }
        }
        

        void removeOrder(int id){
            if (id % 2 == 0){
                priority_queue<Order, vector<Order>, BuyOrderComparator> newBuys;

                while (!buys.empty()){
                    Order order = buys.top();
                    buys.pop();
                    if (order.id != id){
                        newBuys.push(order);
                    }
                }

                buys = newBuys;
            }
            else {
                priority_queue<Order, vector<Order>, SellOrderComparator> newSells;


                while (!sells.empty()){
                    Order order = sells.top();
                    sells.pop();
                    if (order.id != id){
                        newSells.push(order);
                    }
                }

                sells = newSells;
            }
        }

        void printOrders(){
            priority_queue<Order, vector<Order>, BuyOrderComparator> copyBuys = buys;
            priority_queue<Order, vector<Order>, SellOrderComparator> copySells = sells;

            while (!copyBuys.empty()){
                Order order = copyBuys.top();
                copyBuys.pop();
                cout << "Buy ->\tAmount: " << order.quantity << "\tPrice: " << order.price << "\tID: " << order.id << endl;
            }

            while (!copySells.empty()){
                Order order = copySells.top();
                copySells.pop();
                cout << "Sell ->\tAmount: " << order.quantity << "\tPrice: " << order.price << "\tID: " << order.id << endl;
            }
        }

        void matchOrders() {
            bool continue1 = true;
            while (continue1) {
                Order buy = buys.top();
                Order sell = sells.top();
                if (buy.price < sell.price){
                    continue1 = false;
                }
                else {
                    int buy_amt = buy.quantity;
                    int sell_amt = sell.quantity;

                    if (buy_amt > sell_amt){
                        buy.quantity -= sell_amt;
                        buys.pop();
                        sells.pop();
                        addOrder(buy);
                        cout << buy.id << " and " << sell.id << " matched at amount " << sell_amt << endl;
                    }
                    else {
                        sell.quantity -= buy_amt;
                        buys.pop();
                        sells.pop();
                        addOrder(sell);
                        cout << buy.id << " and " << sell.id << " matched at amount " << buy_amt << endl;
                    }
                }
            }
        }

    private:
        priority_queue<Order, vector<Order>, BuyOrderComparator> buys;
        priority_queue<Order, vector<Order>, SellOrderComparator> sells;

        int buy_order = 0;
        int sell_order = 1;
};

int main() {
    Orderbook book1;
    book1.addOrder({15, 105.5, true});
    book1.addOrder({12, 122.5, true});
    book1.addOrder({115, 96, true});
    book1.addOrder({17, 105.5, false});
    book1.addOrder({19, 95, false});
    book1.printOrders();
    book1.removeOrder(2);
    cout << endl;
    book1.printOrders();
    book1.matchOrders();
    book1.printOrders();

    return 0;

}