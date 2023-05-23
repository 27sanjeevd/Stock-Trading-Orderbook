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
			new1->num_buy = 0;
			new1->num_sell = 0;

			if (curr->stock_books == NULL) {
				curr->stock_books = new1;

				return curr->stock_books;
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

Orders* create_order(Stock* info) {
	Orders* temp = (Orders*) malloc(sizeof(Orders));
	temp->stock_info = info;
	temp->next = NULL;

	return temp;
}


void add_order(OrderBook* curr_book, Orders* new1) {
	Stock_Exchange* curr_exchange;
	if (curr_book != NULL && new1 != NULL) {
		curr_exchange = exchange_exists(curr_book, new1->stock_info->name);
		if (curr_exchange == NULL) {
			curr_exchange = create_exchange(curr_book, new1->stock_info->name);
		}
		Orders* loop;
		bool continue1 = true;
		if (new1->stock_info->isBuy) {
			curr_exchange->num_buy++;
			if (curr_exchange->buy_orders == NULL) {
				curr_exchange->buy_orders = new1;
			}
			else {
				Orders* curr_order = curr_exchange->buy_orders;
				Orders* prev_order = NULL;
				while (curr_order != NULL && 
					is_next_order(1, curr_order->stock_info->price, new1->stock_info->price)) {

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
			curr_exchange->num_sell++;
			if (curr_exchange->sell_orders == NULL) {
				curr_exchange->sell_orders = new1;
			}
			else {
				Orders* curr_order = curr_exchange->sell_orders;
				Orders* prev_order = NULL;

				while (curr_order != NULL && 
					is_next_order(0, curr_order->stock_info->price, new1->stock_info->price)) {
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


bool compare_order(Orders* old_order, Orders* new_order) {
	bool output = true;
	if (strcmp(old_order->stock_info->name, new_order->stock_info->name) != 0) {
		output = false;
	}
	if (strcmp(old_order->stock_info->id, new_order->stock_info->id) != 0) {
		output = false;
	}
	if (old_order->stock_info->amt != new_order->stock_info->amt || 
		old_order->stock_info->price != new_order->stock_info->price ||
		old_order->stock_info->isBuy != new_order->stock_info->isBuy) {
		output = false;
	}

	return output;
}

void edit_order(OrderBook* curr_book, Orders* order1, int new_amt) {
	Stock_Exchange* curr_exchange;
	if (curr_book != NULL && order1 != NULL && new_amt > 0) {
		curr_exchange = exchange_exists(curr_book, order1->stock_info->name);

		if (curr_exchange != NULL) {

			bool continue1 = true;
			if (order1->stock_info->isBuy) {

				Orders* loop_order = curr_exchange->buy_orders;
				while (loop_order != NULL && continue1) {
					if (compare_order(loop_order, order1)) {
						loop_order->stock_info->amt = new_amt;
						continue1 = false;
					}
					loop_order = loop_order->next;
				}
			}
			else {

				Orders* loop_order = curr_exchange->sell_orders;
				while (loop_order != NULL && continue1) {
					if (compare_order(loop_order, order1)) {
						loop_order->stock_info->amt = new_amt;
						continue1 = false;
					}
					loop_order = loop_order->next;
				}
			}
		}

	}
}


void remove_order(OrderBook* curr_book, Orders* order1) {
	Stock_Exchange* curr_exchange;
	if (curr_book != NULL && order1 != NULL) {
		curr_exchange = exchange_exists(curr_book, order1->stock_info->name);

		if (curr_exchange != NULL) {

			bool continue1 = true;
			if (order1->stock_info->isBuy) {

				Orders* loop_order = curr_exchange->buy_orders;
				Orders* prev = NULL;

				while (loop_order != NULL && continue1) {
					if (compare_order(loop_order, order1)) {
						if (prev == NULL) {
							curr_exchange->buy_orders = loop_order->next;
						}
						else {
							prev->next = loop_order->next;
						}
						continue1 = false;
					}
					prev = loop_order;
					loop_order = loop_order->next;
				}
			}
			else {

				Orders* loop_order = curr_exchange->sell_orders;
				Orders* prev = NULL;

				while (loop_order != NULL && continue1) {
					if (compare_order(loop_order, order1)) {
						if (prev == NULL) {
							curr_exchange->sell_orders = loop_order->next;
						}
						else {
							prev->next = loop_order->next;
						}
						continue1 = false;
					}
					prev = loop_order;
					loop_order = loop_order->next;
				}
			}
		}

	}
}


