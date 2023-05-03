#include <iostream>
#include "client_controls.h"

#define LSH_INPUT_SIZE 10;
char* user_input() {
	int pos = 0;
	int input_size = LSH_INPUT_SIZE;
	char* output = (char*) malloc(sizeof(char) * input_size);
	int c;

	while (1) {
		c = getchar();
		if (pos >= input_size) {
			input_size += LSH_INPUT_SIZE;
			output = (char*) realloc(output, input_size);
			if (!output) {
				std::cout << "LSH FAILURE" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		else {
			if (c == EOF || c == '\n') {
				output[pos] = '\0';
				return output;
			}
			else {
				output[pos] = c;
				pos++;
			}
		}
	}
}

char* user_main_actions() {
	const int MAX_RESULT_LENGTH = 300;
    char* result = new char[MAX_RESULT_LENGTH];

    std::cout << "(BUY) or (SELL) a stock? or (LOGOUT): ";
    char* action = user_input();

    if (strcmp(action, "LOGOUT") != 0) {
	    std::cout << "Which stock? ";
	    char* stock = user_input();

	    std::cout << "How much? ";
	    char* amount = user_input();

	    std::snprintf(result, MAX_RESULT_LENGTH, "%s %s %s", action, stock, amount);
	    free(action);
	    free(stock);
	    free(amount);

	    return result;
	}
	return action;
}