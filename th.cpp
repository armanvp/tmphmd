#include <iostream>
#include <cstdlib>
#include <termios.h>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <cstring>
#include <time.h>

using namespace std;

void parse_DHT(string str) {
    
    struct dht {
        char humid[5];
        char temp[5];
    } dhtdata;
    
    time_t timer;
    tm *timestmp;
    char timestr[51];
    
    int ok = 0;
    
    
    char *strc = (char *) str.c_str();
    char *token = std::strtok(strc,"|");
    while(token != NULL) {
        if(strncmp(token,"000",3) == 0) {
  
        time(&timer);
        timestmp = localtime(&timer);
            
        ok = 1;
        token = strtok(NULL,"|");
        strncpy(dhtdata.humid,token,4);
        dhtdata.humid[sizeof(dhtdata.humid)-1] = '\0';
            
        token = strtok(NULL,"|");
        strncpy(dhtdata.temp,token,4);
        dhtdata.temp[sizeof(dhtdata.temp)-1] = '\0';
            
        }else{
            break;
        }
    }
    
    if(ok == 1) {
        strftime(timestr,50,"%Y-%m-%d %H:%M:%S",timestmp);
        cout<<timestr<<" "<<dhtdata.humid<<" "<<dhtdata.temp<<endl;
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
