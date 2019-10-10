#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 10000
#define IPADDR "127.0.0.1"
#define BUFSIZE 100

int main(){
	int c_socket; 
	struct sockaddr_in c_addr;
	int n;
	char rcvBuffer[BUFSIZE];
	char sendBuffer[BUFSIZE] = "Hi, I'm client.";
	c_socket = socket(PF_INET, SOCK_STREAM, 0); 
	memset(&c_addr, 0, sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr(IPADDR); 
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(PORT);

	if(connect(c_socket, (struct sockaddr *) &c_addr, sizeof(c_addr)) == -1){
		printf("Cannot Connect\n");
		close(c_socket);
		return -1; 
	}
	while(1){
		fgets(sendBuffer, sizeof(sendBuffer), stdin);
		write(c_socket, sendBuffer, strlen(sendBuffer));
		if(strncasecmp(sendBuffer, "quit", 4) == 0 || strncasecmp(sendBuffer, "kill server", 11) == 0)
			break;
		n = read(c_socket, rcvBuffer, sizeof(rcvBuffer)); 
		if (n < 0){
			printf("Read Failed\n");
			return -1;
		}
		rcvBuffer[n] = '\0';
		printf("received data: %s\n", rcvBuffer);
		printf("rcvBuffer length: %d\n", n);
	}
	close(c_socket);
	return 0;	
}
