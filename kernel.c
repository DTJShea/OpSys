void printString(char*);
void printChar(char);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int ax, int bx, int cx, int dx);

void main(){

	char* test[512];
	readString(test);
	readSector(test, 30);
	printString(test);
	makeInterrupt21();
	interrupt(0x21,0,0,0,0);

	//Tests	
	char line[80];
	makeInterrupt21();
	interrupt(0x21,1,line,0,0);
	interrupt(0x21,0,line,0,0);


	while(1){}

}



void readSector(char* buffer, int sector){
	interrupt(0x13, 2*256+1, buffer,0*256+sector+1, 0*256+0x80);
}

void printChar(char c){
	interrupt(0x10, 0xe*256+c, 0, 0, 0);
}

void readString(char* readChar){
	int count = 0;
	while (1){
		readChar[count] = interrupt(0x16, 0xe*256+toPrint, 0, 0, 0);
		if(readChar[count] == 0xd){
			interrupt(0x10, 0xe*256+0xa,0,0,0);
			interrupt(0x10, 0xe*256+0xd,0,0,0);
			readChar[count+1] = 0xa;
			readChar[count+2] = 0xd;
			break;	
		}

		if(readChar[count] == 0x8){
			count--;
			interrupt(0x10, 0xe*256+0x8,0,0,0);
			interrupt(0x10, 0xe*256+' ',0,0,0);
			interrupt(0x10, 0xe*256+0x8,0,0,0);
		}else{
			interrupt(0x10, 0xe*256+readChar[count],0,0,0);
			count++;
			}	
		}
}

void printString(char* chars){
	int counter=0;
	while(1){
		char toPrint=chars[counter];
		interrupt(0x10, 0xe*256+toPrint, 0, 0, 0);
		counter++;
		if(toPrint==0x0){
			break;
		}
	}
}

void handleInterrupt21(int ax, int bx, int cx, int dx){
	printString("Handling Interrupt21");
	if(ax == 0){ //Print String
		printString(bx);
	}
	if(ax == 1){ //Read String
		readString(bx);
	}
	if(ax == 2){ //Read Sector
		readSector(bx, cx);
	}
	if(ax >= 3){ //Error
		printString("ERROR, AX IS 3 OR HIGHER")
	}
}
