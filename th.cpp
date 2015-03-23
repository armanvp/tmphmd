#include <iostream>
#include <cstdlib>
#include <termios.h>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <cstring>

using namespace std;

void parse_DHT(char *str) {
    
    char *token = strtok(str,",\t");
    while(token != NULL) {
        cout<<token<<endl;
        token = strtok(NULL,",\t");
    }
    
}

int main(int argc, char **argv) {

  int fd;
  size_t rdlen = 0;
  char buffer[255];
  struct termios opt;

  fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);
  if(fd == -1) {
    fprintf(stderr,"Could not connect to port");
    return EXIT_FAILURE;
  }

  fcntl(fd, F_SETFL, 0);

  tcgetattr(fd, &opt);
  cfsetispeed(&opt,B115200);
  cfsetospeed(&opt,B115200);
  opt.c_cflag |= (CLOCAL|CREAD);
  tcsetattr(fd,TCSANOW,&opt);

  rdlen = read(fd,buffer,255);
  while(rdlen != -1) {
    parse_DHT(buffer);
    bzero(buffer,255);
    rdlen = read(fd,buffer,255);
  }
  close(fd);
  return EXIT_SUCCESS;
}