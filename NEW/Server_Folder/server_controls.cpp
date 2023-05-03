#include <iostream>
#include <random>
#include <cstdlib>
#include <cstring>
#include "server_controls.h"

#define PRICE 1 

User* find_user(User** user_list, int num_users, char* id) {
	for (int x = 0; x < num_users; x++) {
		if (strcmp(user_list[x]->id, id) == 0) {
			return user_list[x];
		}
	}

	return NULL;
}

char* generate_id() {
    const int length = 4;
    const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char* userId = (char*) malloc((length + 1) * sizeof(char));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(charset) - 1);

    for (int i = 0; i < length; i++) {
        userId[i] = charset[dis(gen)];
    }

    userId[length] = '\0';

    return userId;
}

User* create_user(int bal) {
	User* new1 = (User*) malloc (sizeof(User));

	new1->id = (char*) malloc(sizeof(char) * 5);
	new1->id = generate_id();
	new1->bal = bal;
	new1->num_stocks = 0;
	new1->stocklist = NULL;

	return new1;
}

void remove_node(User* curr, char* stock_name) {

	if (curr != NULL && stock_name != NULL) {
		Stock* temp = curr->stocklist;
		Stock* prev = NULL;
		bool continue1 = true;
		while (temp != NULL && continue1) {
			if (strcmp(temp->name, stock_name) == 0) {
				if (prev == NULL) {
					curr->stocklist = temp->next;
				}
				else {
					prev->next = temp->next;
				}
				continue1 = false;
				curr->num_stocks--;
			}

			prev = temp;
			temp = temp->next;
		}

	}
}

void buy_stock(User* curr, int buy_or_sell, char* stock_name, int stock_amt) {
	Stock* temp;
	Stock* loop;
	bool continue1 = true;
	if (curr != NULL && stock_name != NULL && stock_amt > 0 && 
		(buy_or_sell == 1 || (buy_or_sell == 0 && curr->bal >= PRICE * stock_amt))) {

		
		if (curr->stocklist == nullptr && buy_or_sell == 0) {
			temp = (Stock*) malloc (sizeof(Stock));
			if (temp != NULL) {
				temp->name = (char*) malloc(strlen(stock_name) + 1);
				strcpy(temp->name, stock_name);
				temp->amt = stock_amt;
				temp->next = NULL;
				curr->stocklist = temp;
				curr->num_stocks++;
				curr->bal -= PRICE * stock_amt;
			}
		}
		else {
			loop = curr->stocklist;
			while (loop->next != nullptr && continue1) {
				if (strcmp(loop->name, stock_name) == 0) {
					continue1 = false;
					if (buy_or_sell == 0) {
						loop->amt += stock_amt;
						curr->bal -= PRICE * stock_amt;
					}
					else {
						if (loop->amt > stock_amt) {
							loop->amt -= stock_amt;
						}
						else {
							remove_node(curr, stock_name);
						}
						curr->bal += PRICE * stock_amt;
					}
				}

				loop = loop->next;
			}

			if (strcmp(loop->name, stock_name) == 0) {
				continue1 = false;
				if (buy_or_sell == 0) {
					loop->amt += stock_amt;
					curr->bal -= PRICE * stock_amt;
				}
				else {
					if (loop->amt > stock_amt) {
						loop->amt -= stock_amt;
					}
					else {
						remove_node(curr, stock_name);
					}
					curr->bal += PRICE * stock_amt;
				}
			}

			if (continue1 && buy_or_sell == 0) {
				temp = (Stock*) malloc (sizeof(Stock));
				if (temp != NULL) {
					temp->name = (char*) malloc(strlen(stock_name) + 1);
					strcpy(temp->name, stock_name);
					temp->amt = stock_amt;
					temp->next = NULL;
					loop->next = temp;
					curr->num_stocks++;
					curr->bal -= PRICE * stock_amt;
				}
			}
		}

	}
}

int parse_command(User* curr, char* input) {
	char str1[20], str2[20], str3[20];
    
    int count = sscanf(input, "%s %s %s", str1, str2, str3);
    if (strcmp(str1, "BUY") == 0) {
    	buy_stock(curr, 0, str2, atoi(str3));
    }
    else if (strcmp(str1, "SELL") == 0) {
    	buy_stock(curr, 1, str2, atoi(str3));
    }
    else if (strcmp(str1, "LOGOUT") == 0) {
    	return 1;
    }

    return 0;
}



