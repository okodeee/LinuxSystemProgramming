#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

// make my own ls program using directory-related system calls
main()
{
	DIR				*dp;
	struct dirent	*dep;

	// return pointer if OK, NULL on error
	if ((dp = opendir(".")) == NULL)  {
		perror("opendir");
		exit(0);
	}

	// reading a directory
	// return pointer if OK, NULL at end of directory or error
	while (dep = readdir(dp))  {
		printf("%s\n", dep->d_name);
	}
	
	closedir(dp);
}
