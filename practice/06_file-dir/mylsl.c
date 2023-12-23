#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define MAX_PATH	256

main()
{
	DIR				*dp;
	struct dirent	*dep;
	struct stat		statbuf;

	// return pointer if OK, NULL on error
	if ((dp = opendir(".")) == NULL)  {
		perror("opendir");
		exit(0);
	}

	// reading a directory
	// return pointer if OK, NULL at end of directory or error
	while (dep = readdir(dp))  {
		if (lstat(dep->d_name, &statbuf) < 0) {
			perror("lstat");
			exit(1);
		}

		
		// Type
		char mode;
        if (S_ISREG(statbuf.st_mode))
            mode = '-';
        else if (S_ISDIR(statbuf.st_mode))
            mode = 'd';
        else if (S_ISCHR(statbuf.st_mode))
            mode = 'c';
        else if (S_ISBLK(statbuf.st_mode))
            mode = 'b';
        else if (S_ISFIFO(statbuf.st_mode))
            mode = 'p';
        else if (S_ISLNK(statbuf.st_mode))
            mode = 'l';
        else if (S_ISSOCK(statbuf.st_mode))
            mode = 's';
        printf("%c", mode);
		
		// Permission
		printf( (statbuf.st_mode & S_IRUSR) ? "r" : "-");
        printf( (statbuf.st_mode & S_IWUSR) ? "w" : "-");
        printf( (statbuf.st_mode & S_IXUSR) ? "x" : "-");
        printf( (statbuf.st_mode & S_IRGRP) ? "r" : "-");
        printf( (statbuf.st_mode & S_IWGRP) ? "w" : "-");
        printf( (statbuf.st_mode & S_IXGRP) ? "x" : "-");
        printf( (statbuf.st_mode & S_IROTH) ? "r" : "-");
        printf( (statbuf.st_mode & S_IWOTH) ? "w" : "-");
        printf( (statbuf.st_mode & S_IXOTH) ? "x" : "-");
		printf(" ");

		// Number of links to the file
		printf("%d ", statbuf.st_nlink);

		// Owner
		struct passwd *pwd;
		pwd = getpwuid(statbuf.st_uid);
		printf("%s ", pwd->pw_name);

		// Group
		struct group *g;
		g = getgrgid(statbuf.st_gid);
		printf("%s ", g->gr_name);

		// Size in bytes, for regular files
		printf("%5d ", statbuf.st_size);

		// Modified date
		struct tm *lt;
		lt = localtime(&statbuf.st_mtime);
		char buf[40];
		strftime(buf, sizeof(buf), "%b %d %H:%M", lt);
		printf("%s ", buf);

		// Name
		printf("%s\n", dep->d_name);
	}
	
	closedir(dp);
}
