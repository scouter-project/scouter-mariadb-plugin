#include<iostream>
#include <sys/socket.h>
#include<arpa/inet.h>
#include <string.h>
using namespace std;

int main(int argc, char** argv) {
	
	int sockfd;
	struct sockaddr_in addr, peer;
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	memset(&addr,0,sizeof(addr));
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port=htons(6200);
	bind(sockfd,(sockaddr*)&addr,sizeof(addr));
	socklen_t len=sizeof(peer);
	char msg[1000];
	
	for(;;) {
		recvfrom(sockfd,msg,100,0,(sockaddr*)&peer,&len);	
	}

}
