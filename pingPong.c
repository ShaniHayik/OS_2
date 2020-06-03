#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


int fid, cid;
pid_t pID ;
int pfd[2];
int val = 0; // counter intil 5 

void parentSignal(int signum){ // parent sends messege to his child 
    
    sleep(0);
    read(pfd[0], &val, sizeof(val)); // read the number from the pipe
    if(val >= 5){ // check if we got to the maximum
      printf("Parent is going to be terminted\n"); 
      kill(pID, SIGKILL);
      exit(0); 
    }
    // if val<5 so we continue in the game 
    printf(" %d\n",  val);
	val++;   
     write(pfd[1], &val, sizeof(val));
     sleep(2);
     kill(pID,SIGUSR1);
}

void childSignal(int signum){ //child send messege to his father and read his messege 

    sleep(0);
    read(pfd[0], &val, sizeof(val)); // read the number from the pipe
    printf(" %d\n",  val);

    if( val >= 5 ){  // check if we got to the maximum
        printf("Child is going to be terminted\n");
        kill(getppid(),SIGUSR1);
    }
    // if val<5 so we continue in the game 
    val++;
    write(pfd[1], &val, sizeof(val));
    kill(getppid(),SIGUSR1);
    
  }


int main()
{
 
   signal(SIGUSR1,parentSignal);

   //Check whether the pipe opening was successful
   if (pipe(pfd) < 0){ printf("Pipe Failed"); }
   
   pID = fork(); //Creating a Child Process
  
   if(pID>0) { while(1){} } //num process bigger than 0 so its patent peocess
  
   else if (pID==0) {// its child process 
	
      signal(SIGUSR1,childSignal);
      write(pfd[1], &val, sizeof(val));
      kill(getppid(),SIGUSR1);
      while(1){}
    }

  else {  puts("Fork Failed !!"); } // the for did not sucsses 
  
 // closing the pipe so the processes coild not read and write
  close(pfd[0]);
  close(pfd[1]);
  
  return 0;
}