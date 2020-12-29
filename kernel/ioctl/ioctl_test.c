/* test ioctl char driver */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "test.h"

#define TEST_COUNT 10000
#define PATH "/dev/test"
#define SIZEMAX 64

int test_read(int fd,char * buff){
	return read(fd,buff,SIZEMAX);
}

int ioctl_test(int fd)
{
    int result = 1001;
    char buf ;
    if (ioctl(fd, FC_TGC_IOC_W, &result) < 0) {
        printf("ioctl CSDN_IOC_W failed!\n");
        return 0;
    }

    if (ioctl(fd, FC_TGC_IOC_R, &result) < 0) {
        printf("ioctl CSDN_IOC_R failed!\n");
        return 0;
    }
    printf("CSDN_IOC_R result: %d\n", result);

    result = 2002;
    if (ioctl(fd, FC_TGC_IOC_RW, &result) < 0) {
        printf("ioctl CSDN_IOC_R failed!\n");
        return 0;
    }
    printf("CSDN_IOC_WR result: %d\n", result);

    if (ioctl(fd, FC_TGC_IOC_CW, &buf) < 0) {
        printf("ioctl CSDN_IOC_R failed!\n");
        return 0;
    }
    printf("CSDN_IOC_WR result: %c\n", buf);
}

int main(int argc,char * argv[]){

	int i = 0;
	char * buf = (char *)malloc(sizeof(char) * SIZEMAX);

	if(argc < 2 && !argv[1]){
		printf("Please input dev\n");
		memcpy(buf, PATH, SIZEMAX);
	}else{
		memcpy(buf, argv[1],SIZEMAX);
	}

//	printf("argv[1]:%s\n",argv[1]);
	int fd = open(buf,O_RDWR);
	printf("fd:%d\n",fd);

	printf("pid:%d\n",getpid());
	if(fd <= 0)
		printf("open is failed\n");
#if 0
	while(i < TEST_COUNT){
		i++;
		test_read(fd,buf);
	}
#endif	
	ioctl_test(fd);
	close(fd);
	return 0;
}
