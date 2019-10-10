#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 10000

char snBf[100];
char rcvBf[100];

int main(){
	int c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len;
	int n;
	int chlen; //strlen -- 처리용
	char chlen_str[100]; //chlen을 write 하는 도중 sprintf을 위한 변수 
	int chcmp; //	strcmp -- 처리용
	int result;
	char cmp_str[100];	//cmp결과 담을 변수

	// 1. 서버 소켓 생성
	//서버 소켓 = 클라이언트의 접속 요청을 처리(허용)해 주기 위한 소켓
	s_socket = socket(PF_INET, SOCK_STREAM, 0); //TCP/IP 통신을 위한 서버 소켓 생성
	
	//2. 서버 소켓 주소 설정
	memset(&s_addr, 0, sizeof(s_addr)); //s_addr의 값을 모두 0으로  초기화
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP 주소 설정
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);

	//3. 서버 소켓바인딩
	if(bind(s_socket,(struct sockaddr *) &s_addr, sizeof(s_addr)) == -1){ 
		//바인딩 작업 실패 시, Cannot Bind 메시지 출력 후 프로그램 종료
		printf("Cannot Bind\n");
		return -1;
	}
	
	//4.listen() 함수 실행
	if(listen(s_socket, 5) == -1){
		printf("listen Fail\n");
		return -1;
	}

	//5. 클라이언트 요청 처리
	// 요청을 허용한 후, Hello World 메세지를 전송함
	while(1){ //무한 루프
		len = sizeof(c_addr);
		printf("클라이언트 접속을 기다리는 중....\n");
		c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len); 
		//클라이언트의 요청이 오면 허용(accept)해 주고, 해당 클라이언트와 통신할 수 있도록 클라이언트 소켓(c_socket)을 반환함.
		printf("/client is connected\n");
		printf("클라이언트 접속 허용\n");

		while(1){	

			n=read(c_socket, rcvBf, sizeof(rcvBf));
			rcvBf[n]='\0'; //개행문자포함시켜서 오류막기
			printf("Received Data: %s\n",rcvBf);  // 체크용도

			if(strncasecmp(rcvBf,"quit",4)==0 || strncasecmp(rcvBf,"kill server",11)==0)
				break;

			if(strncasecmp(rcvBf,"안녕하세요\n",n)==0||strncasecmp(rcvBf,"안녕하세요.\n",n)==0){
				strcpy(snBf,"안녕하세요. 만나서 반가워요!\n");
				write(c_socket,snBf,strlen(snBf));
			}
			else if(strncasecmp(rcvBf,"이름이 머야?\n",n)==0||strncasecmp(rcvBf,"이름이 머야?\n",n)==0){
				strcpy(snBf,"내 이름은 홍성용이야.\n");
				write(c_socket,snBf,strlen(snBf));			
			}
			else if(strncasecmp(rcvBf,"몇 살이야?\n",n)==0||strncasecmp(rcvBf,"몇 살이야?\n",n)==0){
				strcpy(snBf,"나는 23살이야.\n");
				write(c_socket,snBf,strlen(snBf));
			}
			else if(strncasecmp(rcvBf,"strlen",6)==0){
				chlen=strlen(rcvBf)-8;
				sprintf(chlen_str,"문자열의 길이= %d\n",chlen);
				strcpy(snBf,chlen_str);
				write(c_socket,snBf,strlen(snBf));
			}
			else if(strncasecmp(rcvBf,"strcmp",6)==0){
				char *ptr=strtok(rcvBf," ");
				ptr=strtok(NULL," ");
				char *str1=ptr;
				ptr=strtok(NULL," ");
				char *str2=ptr;
				str2[strlen(str2)-1]='\0';

				printf("%s %d ",str1,strlen(str1));
				printf("%s %d\n",str2,strlen(str2));

				if (strcmp(str1,str2)==0){
					result=0;
					sprintf(cmp_str,"비교결과= %d ---> [일치]\n",result);
					strcpy(snBf,cmp_str);
					write(c_socket,snBf,strlen(snBf));
				}
				else if(strcmp(str1,str2)!=0){
					result=strcmp(str1,str2);
					sprintf(cmp_str,"비교결과= %d ---> [불일치]\n",result);
					strcpy(snBf,cmp_str);
					write(c_socket,snBf,strlen(snBf));
				}
			}
			else
				write(c_socket, rcvBf, n); //모든 조건을 만족하지 않는경우 => 에코
		}
		close(c_socket);
		if(strncasecmp(rcvBf,"kill server",11)==0)
			break;
	}
	close(s_socket);
	return 0;	
}
