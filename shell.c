main(){
        while(1){
                char* command="";
                char filename[6];
		char* buffer[512];
		int i;
		int k;

                syscall(0, "USER_SHELL:>");

                syscall(1, command);

		//reads in the file name
		for(i=0; i<6; i++){
			filename[i] = command[i+5];
		}
		//Hello
                if(command[0] == 'h' && command[1] == 'e' && command[2] == 'l' && command[3] == 'l' && command[4] == 'o'){
			syscall(0, "HELLO NEW USER\r\n");
                }
		//Type
                else if(command[0] == 't' && command[1] == 'y' && command[2] == 'p' && command[3] == 'e'){
			syscall(3, filename, buffer, 0);
			if(buffer){
				syscall(0, buffer);
			}else{
				syscall(0, "FILE NOT FOUND");
			}
		}
		//execute
                else if(command[0] == 'e' && command[1] == 'x' && command[2] == 'e' && command[3] == 'c'){
			syscall(3, filename, buffer, 0);
			if(buffer){
				syscall(4, filename);
			}
			else{
				syscall(0, "FILE NOT FOUND");
			}
		
		}
		//dir
		else if(command[0]=='d' && command[1]=='i' && command[2]=='r'){
			for(i=0; i<1000; i = i + 32){
				syscall(2, buffer, 2);
				if(buffer[0] != '\0'){
					for(k=0; k<6; k++){
						filename[k] = buffer[k];
					}
					syscall(0, buffer);
				}
			}

		}
		//copy
               /* else if(command[0] == 'c' && command[1] == 'o' && command[2] == 'p' && command[3] == 'y'){

		}
                else{
                        syscall(0, "NOT A COMMAND\r\n");
                }*/
        }
}

