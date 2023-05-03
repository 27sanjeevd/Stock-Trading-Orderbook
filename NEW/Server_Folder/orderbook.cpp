#include <iostream>
#include <random>
#include <cstdlib>
#include <cstring>
#include "orderbook.h"

OrderBook* create_orderbook() {
	OrderBook* output = (OrderBook*) malloc(sizeof(OrderBook));
	if (output != NULL) {
		output->stock_books = NULL;
	}

	return output;
}

Stock_Exchange* create_exchange(OrderBook* curr, char* stock_name) {
	Stock_Exchange* new1;
	Stock_Exchange* temp;
	if (curr != NULL && stock_name != NULL) {
		new1 = (Stock_Exchange*) malloc(sizeof(Stock_Exchange));
		if (new1 != NULL) {
			new1->name = (char*) malloc(strlen(stock_name) + 1);
			new1->buy_orders = NULL;
			new1->sell_orders = NULL;
			new1->next = NULL;

			if (curr->stock_books == NULL) {
				curr->stock_books = new1;
			}
			else {
				temp = curr->stock_books;
				while (temp->next != NULL) {
					temp = temp->next;
				}

				temp->next = new1;
				return new1;
			}
		}
	}
	return NULL;
}


Stock_Exchange* exchange_exists(OrderBook* curr, char* stock_name) {
	Stock_Exchange* temp;
	if (curr != NULL && stock_name != NULL) {
		temp = curr->stock_books;
		while (temp != NULL) {
			if (strcmp(temp->name, stock_name) == 0) {
				return temp;
			}

			temp = temp->next;
		}
	}

	return NULL;
}


bool is_next_order(int buy_or_sell, int original_price, int new_price) {
	if (buy_or_sell == 0) {
		return new_price > original_price;
	}
	return new_price < original_price;
}


Orders* create_order(char* user_id, char* name, int amt, int price, bool buy) {
	bool check1 = false;
	Orders* output = (Orders*) malloc(sizeof(Orders));
	if (output != NULL) {
		output->id = (char*) malloc(strlen(user_id) + 1);
		output->name = (char*) malloc(strlen(name) + 1);
		if (output->id != NULL && output->name != NULL) {
			strcpy(output->id, user_id);
			strcpy(output->name, name);
			output->amt = amt;
			output->price = price;
			output->next = NULL;
			output->isBuy = buy;
			check1 = true;
		} 
	}

	if (check1) {
		return output;
	}

	return NULL;
}


void add_order(OrderBook* curr_book, Orders* new1) {
	Stock_Exchange* curr_exchange;

	if (curr_book != NULL && new1 != NULL) {
		curr_exchange = exchange_exists(curr_book, new1->name);
		if (curr_exchange == NULL) {
			curr_exchange = create_exchange(curr_book, new1->name);
		}

		Orders* loop;
		bool continue1 = true;
		if (new1->isBuy) {
			if (curr_exchange->buy_orders == NULL) {
				curr_exchange->buy_orders = new1;
			}
			else {
				Orders* curr_order = curr_exchange->buy_orders;
				Orders* prev_order = NULL;

				while (curr_order != NULL && is_next_order(1, curr_order->price, new1->price)) {
					prev_order = curr_order;
					curr_order = curr_order->next;
				}
				if (prev_order == NULL) {
					curr_exchange->buy_orders = new1;
				}
				else {
					prev_order->next = new1;
				}
				new1->next = curr_order;

			}
		}
		else {
			if (curr_exchange->sell_orders == NULL) {
				curr_exchange->sell_orders = new1;
			}
			else {
				Orders* curr_order = curr_exchange->sell_orders;
				Orders* prev_order = NULL;

				while (curr_order != NULL && is_next_order(0, curr_order->price, new1->price)) {
					prev_order = curr_order;
					curr_order = curr_order->next;
				}
				if (prev_order == NULL) {
					curr_exchange->sell_orders = new1;
				}
				else {
					prev_order->next = new1;
				}
				new1->next = curr_order;

			}
		}

	}
}








