#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <dhcp/iface_mgr.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

#include "BaseIPC.h"
namespace ipc{

BaseIPC::BaseIPC(){
	this->socketfd_ = -1;
}

int 
BaseIPC::openSocket(const std::string& socket_name,const int is_send){
	int fd = socket(AF_UNIX, SOCK_STREAM, 0);
		//use -1 indicate error
    if (fd < 0) {
                perror("socket create failed!"); 
		return -1;
	} 
    struct sockaddr_un server_address;
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, socket_name.c_str());
    server_address.sun_path[0] = 0;
    int len = strlen(socket_name.c_str()) + offsetof(struct sockaddr_un, sun_path);
	if(is_send == 1){
		  int result = connect(fd, (struct sockaddr*)&server_address, len);
		  if (result < 0) {
            		//failed to connect
			  std::cout<<"failed to connect to server at send function"<<std::endl;
			  return -1;
		  }
	}
	else{
		if(bind(fd, (struct sockaddr *)&server_address, len) == -1){
			perror("bind");
			exit(EXIT_FAILURE);
		}
		if(listen(fd,5)!=0)//is return value is not 0, error
			printf("error listening on socket\n");
	}
	socketfd_ = fd;
    //return fd if creation of socket if successful
    return fd;
}

int 
BaseIPC::send(const isc::util::OutputBuffer &buf){
	int count = write(socketfd_, buf.getData(), buf.getLength());
	return count;
}

isc::util::InputBuffer
BaseIPC::receive(){
	uint8_t buf[IfaceMgr::RCVBUFSIZE];
    int socket_client = accept(socketfd_, NULL, NULL);
   	int len = read(socket_client, buf, IfaceMgr::RCVBUFSIZE);
	isc::util::InputBuffer Input(buf,len);
	close(socket_client);
   	return Input;
}

int
BaseIPC::getsocket(){
	return socketfd_;
}

void 
BaseIPC::closeSocket(){
	if(socketfd_ != -1)
		close(socketfd_);
}

BaseIPC::~BaseIPC(){
	if(socketfd_ != -1)
		close(socketfd_);
}

}//namespace ipc
