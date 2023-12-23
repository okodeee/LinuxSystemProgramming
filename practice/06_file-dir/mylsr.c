#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define	MAX_PATH	256

// make my own ls -R program using directory-related system calls

void
JustDoIt(char *path)
{
	DIR				*dp;
	struct dirent	*dep;
	struct stat		statbuf;
	char			fullpath[MAX_PATH];

	/* The opendir() function opens a directory stream  corresponding  to the directory name, and returns a pointer to the directory stream The stream is positioned at the first entry in the directory */
	if ((dp = opendir(path)) == NULL)  {
		perror("opendir");
		exit(0);
	}

	/* returns  a pointer to a dirent structure representing the next directory  entry  in  the  directory  stream pointed  to  by  dirp It returns NULL on reaching the end of the directory stream or if an error occurred */
	printf("\n%s:\n", path);
	while (dep = readdir(dp))  {
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		printf("%s\n", dep->d_name);
	}

	// ^ ls


	// resets  the  position of the directory stream dirp to the beginning of the directory
	rewinddir(dp);
	while (dep = readdir(dp))  {
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0) 
			continue;
		strcpy(fullpath, path);
		strcat(fullpath, "/");
		strcat(fullpath, dep->d_name);
		if (lstat(fullpath, &statbuf) < 0)  {
			perror("lstat");
			exit(1);
		} // 재귀
		if (S_ISDIR(statbuf.st_mode))  {
			JustDoIt(fullpath);
		}
	}
	
	closedir(dp);
}

main()
{
	JustDoIt(".");
}
