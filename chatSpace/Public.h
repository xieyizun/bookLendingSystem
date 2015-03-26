#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

enum Type { LOGIN = 0x01, LOGOUT = 0x02, LISTALLUSER = 0x03, USERLIST = 0x04, LOGIN_SUCCESS = 0x05, \
	LOGOUT_SUCCESS = 0x06, USERLOGIN = 0x07, USERLOGOUT = 0x08 };

typedef struct Message {
	int type;
	int len;
	char content[1024];
} Message;

void ERR_REPORT(const char *m) {
	do {
		perror(m);
		exit(EXIT_FAILURE);
	} while(0);
}

//readn, writen用于处理tcp的粘包问题
//方法1:读取和发送定长包
ssize_t readn(int fd, void *buf, size_t count) {
	size_t nleft = count;//还有多少没读
	char *bufp = (char*)buf;
	ssize_t nread;
	while (nleft > 0) {
		if ((nread = read(fd, bufp, nleft)) < 0) {
			if (errno == EINTR)//接收到信号
				continue;
			return -1;
		} else if (nread == 0)//读完
			return count - nleft;
		bufp += nread;
		nleft -= nread;
	}
	return count;//while循环成功退出,则读取了定长count个字节
}

//只有发送缓存区的大小大于要发送的数据,要发送的数据都会成功发送到
//发送缓存区,而不会阻塞
ssize_t writen(int fd, const void *buf, size_t count) {
	size_t nleft = count;//还有多少发送
	char *bufp = (char*)buf;
	ssize_t nwrite;
	while (nleft > 0) {
		if ((nwrite = write(fd, bufp, nleft)) < 0) {
			if (errno == EINTR)//接收到信号中断
				continue;
			return -1;
		} else if (nwrite == 0)//什么都没有发生过一样
			continue;
		bufp += nwrite;
		nleft -= nwrite;
	}
	return count;//while循环成功退出,则发送了定长count个字节
}
