void printString(char*);
void printChar(char);
void readString(char*);
void readSector(char*, int);
void readFile(char*, char*, int*);
void executeProgram(char*);
void terminateProgram();
//void DeleteFile (char*);
void handleInterrupt21(int ax, int bx, int cx, int dx);
void writeSector(char*, int);
void writeFile(char*, char*, int);
void handleTimerInterrupt(int segment, int sp);

void main(){

	makeInterrupt21();	
	interrupt(0x21,8,"this is a test message","testmg",3);
	makeTimerInterrupt();
	interrupt(0x21, 4, "shell", 0, 0);

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
		readChar[count] = interrupt(0x16, 0, 0, 0, 0);
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
	char toPrint;
	while(1){
		toPrint = chars[counter];
		interrupt(0x10, 0xe*256 + toPrint, 0, 0, 0);
		counter++;
		if(toPrint==0x0){
			break;
		}
	}
}

void writeFile(char* buffer, char* filename, int numberOfSectors){
	char dir[512];
	char map[512];
	int i;
	int k;
	int j;
	int fileEntry;

	readSector(dir, 2);
	readSector(map, 1);

//	printChar('B');
	for(i=0; i<512; i+=32){
//		printChar('Q');
		if(dir[i] == '\0'){
//			printChar('X');
			dir[i]=filename[0];
			dir[i+1]=filename[1];
			dir[i+2]=filename[2];
			dir[i+3]=filename[3];
			dir[i+4]=filename[4];
			dir[i+5]=filename[5];
			for(fileEntry=6; fileEntry<32; fileEntry++){
				dir[i+fileEntry]=0;
//				printChar('Y');
			}
			for(k=0; k<numberOfSectors; k++){
//				printChar('F');
				for(j=3; j<512; j++){
					if(map[j] == '\0'){
						map[j]=0xFF;
						dir[i+6+k]=j;
						writeSector(buffer, j);
						writeSector(dir, 2);
						writeSector(map, 1);
//						printChar('Z');
						break;
					}
				}
			}
//			fileEntry=i;
//			printChar('A');
			
		}
//	printChar('R');
	
	}

	return;
}

void readFile(char* filename, char* buffer, int* numsec){
        char dir[512];
        int dirsect=2;
        int fileEntry;
        readSector(dir, dirsect);

        *numsec=0;

        for(fileEntry=0; fileEntry<512; fileEntry+=32){

                if(filename[0] == dir[fileEntry+0] && filename[1] == dir[fileEntry+1] && filename[2] == dir[fileEntry+2] && filename[3] == dir[fileEntry+3] && filename[4] == dir[fileEntry+4] && filename[5] == dir[fileEntry+5]){
                        int i;
                        for(i=0; i<26; i++){
                                if(dir[fileEntry+6+i]=='\0'){
                                        break;
                                }
                                readSector(buffer, dir[fileEntry+6+i]);
                                *numsec = *numsec + 1;
                                buffer+=512;
				//printChar('k');
                        }
                        break;
                }
			//printChar('L');
        }

}

void executeProgram(char* name){
	char buffer[13312];
	int sectorsRead;
	int count;
	readFile(name, buffer, &sectorsRead);
	
	if(sectorsRead == 0){
		return;
	}
	else{

		for(count = 0x0; count < sectorsRead*512; count++){
			putInMemory(0x2000, count, buffer[count]);
//                      printChar('X');
		}
	}
//      printChar('Y');
	launchProgram(0x2000);
	printChar('Z');

}

void terminateProgram(){
	char shellname[6];

	shellname[0]='s';
	shellname[1]='h';
	shellname[2]='e';
	shellname[3]='l';
	shellname[4]='l';
	shellname[5]='\0';

	executeProgram(shellname);
}

void writeSector(char* buffer, int sector){
	interrupt(0x13, 3*256+1, buffer,0*256+sector+1, 0*256+0x80);
}
/*
void DeleteFile(char* name){
	char map[512];
	char directory[512];
	char compareFile[7];
	int file,begin,end,k;
	int j;
	int x;
	
	// 1. Load the Directory and Map to 512 byte character arrays.
	readSector(map,1);
	readSector(directory,2);

	//2. Search through the directory and try to find the file name.
	for (file = 0; file < 16; file++) {
    		begin = file * 32;
    		end = begin + 6;
    			k = 0;
        			for (j = begin; j < end; j++) 
        			{
        			compareFile[k] = directory[j];
       	 		k++;
        }
        compareFile[6]='\0';    

	//3. Set the first byte of the file name to 0x00.
              if (Match(compareFile, name, 6)) {
              		directory[begin]=0x0;
*/

	/*4.Step through the sectors numbers listed as belonging to the file.  
	For each sector, set the corresponding Map byte to 0.  
	For example, if sector 7 belongs to the file, set the 7th Map byte to 0 
	[however you should set the 8th, since the Map starts at sector 0].*/
/*

              for (x = end ; directory[x] != 0x0; x++){ 
              		int correspondingMap = directory[x];
					map[correspondingMap] = 0x00;
              }
        }
    }
	//5. Write the character arrays holding the Directory and Map back to their
	//appropriate sectors.
	writeSector (directory, 2);
	writeSector (map,1);
}*/

void handleTimerInterrupt(int segment, int sp){
	printChar('T');
	printChar('i');
	printChar('c');
	printChar('\n');
	returnFromTimer(segment, sp);
}

void handleInterrupt21(int ax, int bx, int cx, int dx){
	
	if(ax == 0){ //Print String
		printString(bx);
	}
	if(ax == 1){ //Read String
		readString(bx);
	}
	if(ax == 2){ //Read Sector
		readSector(bx, cx);
	}
	if(ax == 3){ //Read File
		readFile(bx, cx, dx);
	}
	if(ax == 4){ //execute program
		executeProgram(bx);
	}
	if(ax == 5){ //Terminate Program
		terminateProgram();
	}
	if(ax == 6){ //Write Sector
		writeSector(bx, cx);
	}	
	/*if(ax == 7){ //Delete File
		DeleteFile(bx);
	}*/
	if(ax == 8){ //Write File
		writeFile(bx, cx, dx);
	}
}

