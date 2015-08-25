#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>

using namespace std;
void run(void* p);
static void* thread_fun(void* p);
int main() {
	int s;
	s = socket(AF_INET,SOCK_STREAM,0);
	run(&s);
	sleep(1);
	close(s);
	while(1) {
		sleep(3);
	}
	
}
	
void run(void* p) {
	pthread_t tid;
	pthread_create(&tid,0,thread_fun,p);	
	
	

}

void* thread_fun(void* p) {
	struct sockaddr_in peer;
	bzero(&peer,sizeof(peer));
	int socket = *((int*)p);
	char* ip = "165.243.31.37";
	short port = 16002;
	peer.sin_family=AF_INET;
	inet_pton(AF_INET,ip,&peer.sin_addr);
	peer.sin_port=htons(port);
	int rc = connect(socket,(sockaddr*)&peer,sizeof(peer));
	if(rc == 0) {
		cout<<"connection success"<<endl;
	}
	cout<<"start recv..."<<endl;
	while(1) {
		char buff[100];
		read(socket,buff,100);	
	}
	cout<<"terminate.."<<endl;
	
}



	




	




