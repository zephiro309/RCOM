/*Non-Canonical Input Processing*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>

#include "logicallink.h"

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;

int connAttempts = 0;
typedef enum {CONNECTING, TRANSMITTING, CLOSING} connStage;
connStage currentConnStage = CONNECTING;

//frame creation
	

int establishConn(int fd) {
	
}

int main(int argc, char** argv)
{
	puts("Beginning.");
    int fd,c;
    struct termios oldtio,newtio;
    char buf[255];
    int i, sum = 0, speed = 0;
    
    if ( (argc < 2) || 
  	     (strcmp("/dev/ttyS4", argv[1])!=0)) {
      printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
      exit(1);
    }

	puts("Opening port...");

    fd = open(argv[1], O_RDWR | O_NOCTTY );
    if (fd <0) {perror(argv[1]); exit(-1); }

    if ( tcgetattr(fd,&oldtio) == -1) { /* save current port settings */
      perror("tcgetattr");
      exit(-1);
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = OPOST;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME]    = 3;   /* inter-character timer */
    newtio.c_cc[VMIN]     = 5;   /* blocking read until 5 chars received */

    tcflush(fd, TCIFLUSH);

    if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }

	while(currentConnStage == CONNECTING) {
		alarm(3);
	
		int res;

		//sending SET frame
		puts("Sending SET frame...");
    	res = write(fd,SET,5);
		sleep(5); 
		if(res != -1)
    		printf("%d bytes written\n", res);
	
		if(++connAttempts >= 4) {
			puts("Deactivating alarm.");
			alarm(0);
			}
	
		puts("Reading...");
		res = read(fd,UA,5);
		if(res != -1)
			printf("Established. Read %d bytes.", res);
		else {
			printf("Reading failed.");
		}
	
		if(UA[0] == F && UA[1] == A && UA[2] == 0x07 && UA[3] == (A ^ 0x07) && UA[4] == F) {
			puts("UA frame: valid");
			currentConnStage == TRANSMITTING;
		}
		else {
			puts("UA frame: invalid");
			return 1;
		}

		return 0;
		sleep(5);
	}
    
    if ( tcsetattr(fd,TCSANOW,&oldtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }
    close(fd);

    return 0;
}
