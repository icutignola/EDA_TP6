#pragma once
#include <cstdlib>
#include <string.h>

#define MAX_PARSE_LENGTH 256

typedef enum { ERROR_DURING_PATH, NOT_VALID_ENTRY}error_t;

class path {
	public:
		path(char *);
		bool verifyPath(char *path); // o que reciba un string?
		
		//error_t error_function(bool is_path_correct);

	private:
		bool is_error;




};


