#ifndef SERVER_CONTROLS_H
#define SERVER_CONTROLS_H

#include "orderbook.h"

typedef struct OrderBook OrderBook;

typedef struct Stock {
	char* name;
	char* id;
	int amt;
	int price;
	bool isBuy;
	Stock* next;
} Stock;

typedef struct User {
	int bal;
	int num_stocks;
	char* id;
	Stock* stocklist;
} User;

User* find_user(User** user_list, int num_users, char* id);
char* generate_id();
User* create_user(int bal);
void remove_node(User* curr, char* stock_name);
Stock* create_stock_order(User* curr, char* stock_name, int buy_or_sell, int amt, int price);
void add_stock_order(User* curr, Stock* new_order);
Stock* buy_stock(User* curr, int buy_or_sell, char* stock_name, int stock_amt);
int parse_command(User* curr, char* input, OrderBook* curr_book);
char* return_position(Stock* curr);

#endif