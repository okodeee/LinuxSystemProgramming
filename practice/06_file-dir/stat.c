#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>


// 파일에 대한 i-node 정보를 읽어와서 출력
main(int argc, char *argv[])
{
	struct stat	statbuf;
	char		*mode;
	int			i;

	for (i = 1 ; i < argc ; i++)  {
		printf("%s: ", argv[i]);

		/*
		return information about a  file,  in  the  buffer pointed  to  by  statbuf. lstat()  is identical to stat(), except that if pathname is a symbolic link, then it returns information about the link itself, not the file that it refers to.
		*/
		if (lstat(argv[i], &statbuf) < 0)  {
			perror("lstat");
			continue;
		}
		// file type macros
		if (S_ISREG(statbuf.st_mode))
			mode = "regular";
		else if (S_ISDIR(statbuf.st_mode))
			mode = "directory";
		else if (S_ISCHR(statbuf.st_mode))
			mode = "character special";
		else if (S_ISBLK(statbuf.st_mode))
			mode = "block special";
		else if (S_ISFIFO(statbuf.st_mode))
			mode = "FIFO";
		else if (S_ISLNK(statbuf.st_mode))
			mode = "symbolic link";
		else if (S_ISSOCK(statbuf.st_mode))
			mode = "socket";
		printf("%s\n", mode);

		// file type & mode (permissions)
		printf("\tst_mode = %d\n", statbuf.st_mode);
		// i-node number (serial number)
		printf("\tst_ino = %d\n", statbuf.st_ino);
		// device number (file system)
		printf("\tst_dev = %d\n", statbuf.st_dev);
                // device number for special files
		printf("\tst_rdev = %d\n", statbuf.st_rdev);
                // number of links
		printf("\tst_nlink = %d\n", statbuf.st_nlink);
                // user ID of owner
		printf("\tst_uid = %d\n", statbuf.st_uid);
                // group ID of owner
		printf("\tst_gid = %d\n", statbuf.st_gid);
                // size in bytes, for regular files
		printf("\tst_size = %d\n", statbuf.st_size);
                // time of last access
		printf("\tst_atime = %d\n", statbuf.st_atime);
                // time of last modification
		printf("\tst_mtime = %d\n", statbuf.st_mtime);
                // time of last file status change
		printf("\tst_ctime = %d\n", statbuf.st_ctime);
                // best I/O block size
		printf("\tst_blksize = %d\n", statbuf.st_blksize);
                // no. of 512-byte blocks allocated
		printf("\tst_blocks = %d\n", statbuf.st_blocks);
	}
}
