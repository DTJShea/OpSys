main(){
        while(1){
                char* command="";
                char filename[6];
		char* buffer[512];

		//reads in the file name
		for(int i = 0; i < 6; i++){
			filename[i] = command[i+5];
		} 

                syscall(0, "USER_SHELL:>");

                syscall(1, command);
		//Hello
                if(command[0] == 'h' && command[1] == 'e' && command[2] == 'l' && command[3] == 'l' && command[4] == 'o'){
			syscall(0, "HELLO NEW USER\r\n");
                }
		//Type
                if(command[0] == 't' && command[1] == 'y' && command[2] == 'p' && command[3] == 'e'){
			syscall(3, filename, buffer, 0);
			if(buffer){
				syscall(0, buffer);
			}else{
				syscall(0, "FILE NOT FOUND");
			}
		}
		//execute
                if(command[0] == 'e' && command[1] == 'x' && command[2] == 'e' && command[3] == 'c'){
			syscall(3, filename, buffer, 0);
			if(buffer){
				syscall(4, filename);
			}
			else{
				syscall(0, "FILE NOT FOUND");
			}
		
		}
                else{
                        syscall(0, "NOT A COMMAND\r\n");
                }
        }
}

