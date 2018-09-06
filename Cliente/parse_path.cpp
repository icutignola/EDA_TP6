#include "parse_path.h"

bool path::verifyPath(char *path)
{
	bool isValid = false;
	for (int i = 0; (i<MAX_PARSE_LENGTH) && (isValid == false); i++)
	{
		if (path[i] == '/')
		{
			isValid = true;
		}
	}
	return isValid;
}

//error_t error_function(bool is_path_correct)
