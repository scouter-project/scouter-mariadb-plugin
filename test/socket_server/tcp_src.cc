#include<iostream>
#include <sys/socket.h>
#include<arpa/inet.h>
#include <string.h>
using namespace std;

int main(int argc, char** argv) {
	
	int sockfd;
	int s1;
	struct sockaddr_in addr, peer;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	int on = 1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	memset(&addr,0,sizeof(addr));
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port=htons(6300);
	bind(sockfd,(sockaddr*)&addr,sizeof(addr));
	listen(sockfd,10);
	socklen_t len=sizeof(peer);
	char msg[1000];
	for(;;) {
		s1=accept(sockfd,(struct sockaddr*)&peer,&len);
	}
	

}
