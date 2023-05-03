#ifndef ORDERBOOK_H
#define ORDERBOOK_H

typedef struct Orders {
	char* name;
	char* id;
	int amt;
	int price;
	bool isBuy;
	Orders* next;
} Orders;

typedef struct Stock_Exchange {

	char* name;
	Orders* buy_orders;
	Orders* sell_orders;
	Stock_Exchange* next;
} Stock_Exchange;

typedef struct OrderBook {

	Stock_Exchange* stock_books;
} OrderBook;

OrderBook* create_orderbook();
Stock_Exchange* create_exchange(OrderBook* curr, char* stock_name);
Stock_Exchange* exchange_exists(OrderBook* curr, char* stock_name);
bool is_next_order(int buy_or_sell, int original_price, int new_price);
Orders* create_order(char* user_id, int amt, int price);
void add_order(OrderBook* curr_book, Orders* new1);

#endif