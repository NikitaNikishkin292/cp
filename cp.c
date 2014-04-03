#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

void fileCopy (char *dir1, char *dir2) {
	char buffer;
	int readQuantity;
	int fd1 = open (dir1, O_RDONLY );
	int fd2 = open(dir2, O_WRONLY | O_CREAT, 0777 );
	if (fd1 < 0 || fd2 < 0) {
		printf ("Error. Can't open the file\n");
		exit(1);
	}
	if (readQuantity = read( fd1, &buffer, sizeof(buffer)) > 0) {
		if (write(fd2, &buffer, readQuantity) < 0) {
			printf ("Can't write\n");
			exit(1);
		}
	}
	close(fd1);
	close(fd2);
}

void directoryCopy (char *dir1, char *dir2) {
	DIR *d1;
	struct dirent *direntQ;
	struct stat statQ;

	d1 = opendir (dir1);
	if (!d1) {
		printf("Error. Can't open the directory\n");
		exit(1);
	}


	if (mkdir(dir2, 0777) < 0) {
		printf ("Error. Can't create such a directory\n");
		exit(1);
	}

	char path1[PATH_MAX + 1];
	char path2[PATH_MAX + 1];
	
	while ((direntQ = readdir(d1)) != NULL) {
		if (direntQ -> d_name[0] != '.')
			continue;
		
		 strncpy(path1, dir1, PATH_MAX);	
        	 strncpy(path2, dir2, PATH_MAX);
        	 strncat(path1, "/", PATH_MAX);
        	 strncat(path2, "/", PATH_MAX);
        	 strncat(path1, direntQ -> d_name, PATH_MAX);
        	 strncat(path2, direntQ -> d_name, PATH_MAX);

		if (stat (path1, &statQ)) {
			printf ("Error, can't open the file\n");
			exit(1);
		}

		if (statQ.st_mode & S_IFREG) {
			fileCopy(path1, path2);
		}
		if (statQ.st_mode & S_IFDIR) {
			directoryCopy(path1, path2);
		}

		closedir(d1);

}


int main( int argc, char **argv) {
    
	struct stat statQ;
    
	if (argc < 3) {
		printf("Too few arguments\n");
		exit (1);
	}
    
	if (stat( argv[1] , &statQ ) < 0) {
		printf(" Cannot open file\n");
		exit(1);
	}
    
	if (statQ.st_mode & S_IFREG)
		fileCopy (argv[1], argv[2]);
	else {
		if (opendir(argv[2])) {
			printf("This name already exists '%s'\n", argv[2]);
			exit(1);
		}
		
		directoryCopy (argv[1], argv[2]);
	}
	exit(0);
}

















