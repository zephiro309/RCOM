#include "logicallink.h"

void set() {
	puts("Trying again...");
	retry_count++;
	connecting;
}

int llopen() {
	//frame creation
	unsigned char SET[5], UA[5];
	SET[0] = F;
	SET[1] = A;
	SET[2] = C;
	SET[3] = SET[1] ^ SET[2];
	SET[4] = F;

	puts("Setting alarm signal...");
	signal(SIGALRM, set); //install set routine
	puts("Alarm signal set.");

	while(retry_count <= 3) {
		if(connecting) {
			alarm(3);
	
			int res;

			//sending SET frame
			puts("Sending SET frame...");
    		res = write(fd,SET,5);
			sleep(5); 
			if(res != -1)
    			printf("%d bytes written\n", res);
			else {
				puts("Connection establishment failed.");
				return 1;
			}
	
			if(retry_count >= 4) {
				puts("Deactivating alarm.");
				alarm(0);
			}
			/*
			//receiving UA frame
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
			*/
			sleep(5);
			return 0;
		}
	}
}
