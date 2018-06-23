#include<stdio.h>
#include<cstdlib>
#include<iostream>
#include<string.h>
#include<fstream>
#include<dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

bool checkIfValidFilename ( char *path)
{

 for(unsigned int i = 0; i<strlen(path);i++)
   if ( path[i] == '/') return false;

 return true;
}

int main ( int argc, char *argv[] )
{

 if ( argc == 1 ) 
	{
	 cout<<"cp: missing file operand"<<endl<<flush;
	 return EXIT_FAILURE;
	 } // ./cp

 if ( argc == 2) 
	 {
	  cout<<"cp: missing destination file operand after "<<argv[1]<<endl<<flush; 
	  return EXIT_FAILURE;
	 } // ./cp arg1

 //this code ensures argc >= 3
 int c = 0;
 int rFlag = 0;
 int n = 0;
 //int cflags = O_RDWR| O_TRUNC;

 while (( c=getopt(argc,argv,"r"))!=-1 )
 {
     switch(c)
	{

    	 case 'r': rFlag = 1;
      		 break;

	 default:
      		 cout << "Incorrect input. Only -r is supported. " << endl;
      		 exit(EXIT_FAILURE);
      		 break;
	}//switch
 
 } // while getopt

 char *source = argv [optind];
 char *target = argv [optind+1];

 struct stat arg1; // holds info about source
 struct stat arg2; // holds info about target

 //this ensures that the source file exists
 if ( stat(source, &arg1) == -1)
	{
	  perror("cp: invalid source to copy. ");
	  return EXIT_FAILURE;
	} 

 if(S_ISDIR(arg1.st_mode))
 {
   //make sure you have rFlag ==1 then only proceed if it's not then error: omitting directory argv[1]
   if ( rFlag == 0)
	{ 
	  cout<<"cp: omitting directory "<<source<<endl<<flush;
	  return EXIT_FAILURE;
	} 

   int structId = stat(target, &arg2);
   int createTarget = 0;
   if ( structId == -1 ) createTarget = 1;
  
   if(S_ISREG(arg2.st_mode)) 
	{
	  cout<<"cp: cannot overwrite non-directory ‘"<<target<<"’ with directory ‘"<<source<< "’"<<endl<<flush;
	  return EXIT_FAILURE;
	}
  if ( ! (arg1.st_mode & S_IRUSR) )
	{
	  cout << "cp: cannot access '"<<source<<"': Permission denied"<<endl<<flush;
	  return EXIT_FAILURE;
	}
	



 } // if source is a directory
 else if(S_ISREG(arg1.st_mode))
 {
	char buffer [10000];
	int sourcefd = open ( source, O_RDONLY);
	if ( sourcefd == -1 )
	{
	  perror("cp: ");
	  return EXIT_FAILURE;
	}
	
	while ((n = read(sourcefd, buffer, 9990)) > 0) 
	    {
	      if ( n != -1 ) buffer[n]='\0';
	    }

	int structId = stat(target, &arg2);
	int createTarget = 0;
	if ( structId == -1 ) 
	{
	  createTarget = 1;
	  if(!checkIfValidFilename(target)) 
	      { 
		cout<<"cp: failed to access ‘"<<target<<"’: Not a directory"<<endl<<flush;
		return EXIT_FAILURE;
	      } 
	}// if structId == -1
	if ( createTarget == 0 ) //overWrite, do not create new file or directory
	{
	  if ( S_ISREG(arg2.st_mode)) // i.e. if both args are files
	  { 
	    
	    int targetfd = open ( target, O_RDWR);
	    if ( targetfd == -1 ) 
		{
		  perror ( "cp: target file is write protected. ");
		  return EXIT_FAILURE;
		} // target file does not have write permission

	    /*while ((n = read(sourcefd, buffer, 9990)) > 0) 
	    {
	      if ( n != -1 ) buffer[n]='\0';
	    }*/

	    ftruncate(targetfd,0);
	    lseek(targetfd,0,0);

	    n = write(targetfd,buffer,strlen(buffer));
	    if(n == -1) 
		{ 
		  perror("write to target file error LOL: "); 
		  return EXIT_FAILURE; 
		}
    	    close(sourcefd);
	    close(targetfd);
	    
	  } // if both args are files and both of them exists
	  else if ( S_ISDIR(arg2.st_mode))
	  {
	    char newfile[100];
	    unsigned int i = 0, j = 0;
	    for ( i = 0; i < strlen (target); i++)
	      newfile[i] = target[i];

	    if ( target[i-1] != '/' ) newfile[i++] = '/'; 

	    for (j = 0; j < strlen (source); i++, j++)
	      newfile[i] = source [j];

	    newfile[i] = '\0';
	    cout<<"Target would be: "<<newfile<<":"<<endl<<flush;
	    int newcreate = 0;
	    struct stat temp;
	    int stfd = stat ( newfile, &temp);
	    if ( stfd == -1 ) newcreate = 1;

	    if(newcreate == 1)
	    {
	      int targetfd = open ( newfile, O_CREAT | O_WRONLY, arg1.st_mode);
	      if ( targetfd == -1 ) { perror (" new line 157 "); return EXIT_FAILURE; }
	      /*int sourcefd = open ( source, O_RDONLY);
	      if ( sourcefd == -1 )
		{
	  	  perror("cp: ");
	  	  return EXIT_FAILURE;
		}
	    */ //cout<<"REACHED : " <<buffer<<endl<<flush;
 	    n = write(targetfd,buffer,strlen(buffer));
	    if(n == -1) 
		{ 
		  perror("write to target file error HERE WE GO AGAIN: "); 
		  return EXIT_FAILURE; 
		}

    	    close(sourcefd);
	    close(targetfd);
	    
	    	    
	    
	    } // if of newcreate == 1
	    else
	    {
	    
	    int targetfd = open ( newfile, O_RDWR);
	    if ( targetfd == -1 ) 
		{
		  perror ( "cp: target file is write protected. ");
		  return EXIT_FAILURE;
		} // target file does not have write permission

	    /*while ((n = read(sourcefd, buffer, 9990)) > 0) 
	    {
	      if ( n != -1 ) buffer[n]='\0';
	    }*/

	    ftruncate(targetfd,0);
	    lseek(targetfd,0,0);

	    n = write(targetfd,buffer,strlen(buffer));
	    if(n == -1) 
		{ 
		  perror("write to target file error LOL: "); 
		  return EXIT_FAILURE; 
		}
    	    close(sourcefd);
	    close(targetfd);	    
	    
	    
	    
	    
	    
	    
	    
	    } // else of newcreate == 1 if block i.e. do not create new file

	  }  // if source is a file, target is a directory and both of them exists
	}

	else // createTarget file and then write to it
	{
	  char buffer [10000];
	  /*int sourcefd = open ( source, O_RDONLY);
	  if ( sourcefd == -1 )
	  {
	    perror("cp: ");
	    return EXIT_FAILURE;
	  }*/
	  int targetfd = open ( target, O_CREAT | O_WRONLY, arg1.st_mode); 
	  //if ( targetfd == -1 ) {perror("write to new file: "); return EXIT_FAILURE; }
	    

	    while ((n = read(sourcefd, buffer, 9990)) > 0) 
	    {
	      if ( n != -1 ) buffer[n]='\0';
	    }
	    
	    lseek(targetfd,0,0); n=0;

	    n = write(targetfd,buffer,strlen(buffer));
	    if(n == -1) 
		{ 
		  perror("write to target file error HERE WE GO AGAIN: "); 
		  return EXIT_FAILURE; 
		}

    	    close(sourcefd);
	    close(targetfd);
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    
	    



	}

 } // else source is a reg file

 return EXIT_SUCCESS;

}
