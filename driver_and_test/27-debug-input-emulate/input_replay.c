
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_REPLAY 0
#define INPUT_TAG    1


void print_usage(char *file)
{
	printf("Usage:\r\n");
	printf("%s write <file>\r\n",file);
	printf("%s replay \r\n",file);
	printf("%s tag <string>\r\n",file);
}

int main(int argc,char **argv)
{
	int fd;
	int fd_data;
	int len;
	int buf[100];

	if (argc != 2 && argc != 3){
		print_usage(argv[0]);
		return -1;
	}

	fd = open("/dev/input_emu",O_RDWR);
	if (fd < 0){
		printf("can't open /dev/input_emu\r\n");
		return -1;
	}

	if (strcmp(argv[1],"replay") == 0){
		ioctl(fd,INPUT_REPLAY);
	}else if (strcmp(argv[1],"write") == 0){
		if (argc != 3){
			print_usage(argv[0]);
			return -1;
		}

		fd_data = open(argv[2],O_RDONLY);
		if (fd_data < 0){
			printf("can't open %s\r\n",argv[2]);
			return -1;
		}

		while(1){
			len = read(fd_data,buf,100);
			if (len == 0){
				printf("write ok\r\n");
				break;
			}
			else
			{
				write(fd, buf, len);				
			}
		}
	}else if(strcmp(argv[1],"tag") == 0){
		if (argc != 3){
			print_usage(argv[0]);
			return -1;
		}
		ioctl(fd,INPUT_TAG,argv[2]);
	}else{
		print_usage(argv[0]);
		return -1;
	}

	return 0;
}

