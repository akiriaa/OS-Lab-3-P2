#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void  Poor_Student_Process(int []);
void  Dear_Old_Dad_Process(int []);

int  main(int  argc, char *argv[])
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;

     ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     printf("Server has received a shared memory of four integers...\n");

     ShmPTR = (int *) shmat(ShmID, NULL, 0);
     if (*ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     printf("Server has attached the shared memory...\n");
     
     ShmPTR[0] = 0; //== BANKACCOUNT;
     ShmPTR[1] = 0; //== TURN;
  
     printf("Main has filled BANKACCOUNT = %d TURN = %d in shared memory...\n",
            ShmPTR[0], ShmPTR[1]);

     printf("Main is about to fork a child process...\n");
     pid = fork();
     if (pid < 0) {
          printf("*** fork error (Main) ***\n");
          exit(1);
     }
     else if (pid == 0) {
          Poor_Student_Process(ShmPTR);
          exit(0);
     }
     else{
          Dear_Old_Dad_Process(ShmPTR);
     }

     wait(&status);
     printf("Main has detected the completion of its child...\n");
     shmdt((void *) ShmPTR);
     printf("Main has detached its shared memory...\n");
     shmctl(ShmID, IPC_RMID, NULL);
     printf("Main has removed its shared memory...\n");
     printf("Main exits...\n");
     exit(0);
}

void  Poor_Student_Process(int  SharedMem[])
{
     int account;
     int balance;
     srand(getpid()); //seed rand num generator
      
     
     for(int i=0; i<25; i++){
       sleep(rand()%6);
       
       account = SharedMem[0];
       
       while(SharedMem[1] != 1) //loop do nothing
         ;
       //if(account<=100){
         //DepositMoney();
       balance = rand() % 51;
       printf("Poor Student needs $%d\n", balance);
       if(balance<=account){
         account -= balance;
         printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, account);
       }
       else{
         printf("Poor Student: Not Enough Cash ($%d)\n", account );
       }
       SharedMem[0] = account;
       SharedMem[1] = 0;
     
       
       
       printf("   Poor Student process started\n");
       printf("   Poor Student found %d %d in shared memory\n",
              SharedMem[0], SharedMem[1]);
       printf("   Poor Student is about to exit\n");
     }
     
     
}

void  Dear_Old_Dad_Process(int  SharedMem[])
{
     int account;
     int balance;
     

     srand(getpid()); //seed rand num generator
  
     for(int i=0; i<25; i++){
       sleep(rand()%6);
       
       account = SharedMem[0];
       
       while(SharedMem[1] != 0) //loop do nothing
         ;
       if(account<=100){
         //DepositMoney();
         balance = rand() % 101;
         if(balance%2==0){
           account += balance;
           printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, account);
         }
         else{
           printf("Dear old Dad: Doesn't have any money to give\n");
         }
         SharedMem[0] = account;
         SharedMem[1] = 1;
       }
       else{
         printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, account);
       }
       
       printf("   Dear Old Dad process started\n");
       printf("   Dear Old Dad found %d %d in shared memory\n",
              SharedMem[0], SharedMem[1]);
       printf("   Dear Old Dad is about to exit\n");
     }
     
     
}