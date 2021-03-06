#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

// #include "file.h"

char webpage[] =
"HTTP/1.1 200 OK\r\n"
"Content-Type:text/html; charset=UTF-8\r\n\r\n" 
"<!DOCTYPE html>"
"<html lang=\"en\">"
"<head>"
"<meta charset=\"UTF-8\">"
"<meta name=\"viewport\" content=\"width=device-width, initial-scale =1.0\">"
"<title>Document</title>"
"</head>"
"<body>"
"<p>This is a website that is hosted by a C server</p>"
"</body>"
"</html>";

// void f(int);
// void cc(int i){
// 	f(i);
// }

int main(){
	// cc(600);
	struct sockaddr_in server_addr, client_addr;
	socklen_t sin_len = sizeof(client_addr);
	int fd_server, fd_client;
	char buf[2048];
	int fdimg;
	int on = 1;

	fd_server = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_server < 0){
		perror("socket");
		exit(1);
	}

	setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(8070);

	if(bind(fd_server, (struct sockaddr *) &server_addr, sizeof(server_addr))== -1){
		perror("bind");
		close(fd_server);
		exit(1);
	}

	if(listen(fd_server, 10) == -1){
		perror("listen");
		close(fd_server);
		exit(1);
	}

	while(1){
		fd_client = accept(fd_server, (struct sockaddr *) &client_addr, &sin_len);
		if(fd_client == -1){
			perror("Connection failed....\n");
			continue;
		}

		printf("Got client connection.....\n");
		
		if(!fork()){
			/*child process*/
			close(fd_server);
			memset(buf, 0, 2048);
			read(fd_client, buf, 2047);
			printf("%s\n", buf);

			if(!strncmp(buf, "GET /favicon.png", 16)){
				fdimg = open("favicon.png", O_RDONLY);
				sendfile(fd_client, fdimg, NULL, 4000);
				close(fdimg);
				
			}
			else if(!strncmp(buf, "GET /image.jpg", 16)){
				fdimg = open("favicon.jpg", O_RDONLY);
				sendfile(fd_client, fdimg, NULL, 6000);
				close(fdimg);	
				
			}else{
				write(fd_client, webpage, sizeof(webpage) -1);
			}
			close(fd_client);
			printf("closing....\n");
			exit(0);
		}
		/*parent process*/
		close(fd_client);
	}

	return 0;
}