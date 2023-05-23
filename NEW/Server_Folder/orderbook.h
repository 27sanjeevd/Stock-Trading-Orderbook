#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "server_controls.h"

typedef struct Stock Stock;

typedef struct Orders {
	Stock* stock_info;
	Orders* next;
} Orders;

typedef struct Stock_Exchange {

	char* name;
	Orders* buy_orders;
	Orders* sell_orders;
	int num_buy;
	int num_sell;
	Stock_Exchange* next;
} Stock_Exchange;

typedef struct OrderBook {

	Stock_Exchange* stock_books;
} OrderBook;

OrderBook* create_orderbook();
Stock_Exchange* create_exchange(OrderBook* curr, char* stock_name);
Stock_Exchange* exchange_exists(OrderBook* curr, char* stock_name);
bool is_next_order(int buy_or_sell, int original_price, int new_price);
Orders* create_order(Stock* info);
void add_order(OrderBook* curr_book, Orders* new1);
bool compare_order(Orders* old_order, Orders* new_order);
void edit_order(OrderBook* curr_book, Orders* order1, int new_amt);
void remove_order(OrderBook* curr_book, Orders* order1);
#endif