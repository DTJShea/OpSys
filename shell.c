main(){
        while(1){
                char* command="";
                char filename[6];

                syscall(0, "USER_SHELL:>");

                syscall(1, command);
                if(command[0] == 'h' && command[1] == 'e' && command[2] == 'l' && command[3] == 'l' && command[4] == 'o'){
			syscall(0, "HELLO NEW USER\r\n");
                }
                else{
                        syscall(0, "NOT A COMMAND\r\n");
                }
        }
}

