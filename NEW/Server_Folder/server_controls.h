#ifndef SERVER_CONTROLS_H
#define SERVER_CONTROLS_H

typedef struct Stock {
	char* name;
	int amt;
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
void buy_stock(User* curr, int buy_or_sell, char* stock_name, int stock_amt);
int parse_command(User* curr, char* input);

#endif