#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <cstdio>
#include <cerrno>
#include <cstring>

using namespace std;

int main(int argc,  char * argv [])
{
	int a = 1, c= 0;
	int cFlag = 0, lFlag = 0, wFlag = 0, mFlag = 0;
	int bytes = 0, lines = 0, words = 0, chars = 0;
	int atLeastOneFlag = 0;
	char buffer [1];

	while((c=getopt(argc,argv,"lcmw"))!=-1){
    
	 switch(c){
	     case 'l': 
	       lFlag = 1; atLeastOneFlag = 1;
	       break;
	     case 'm':
	        mFlag = 1; atLeastOneFlag = 1;
	       break;
	     case 'w':
	       wFlag = 1; atLeastOneFlag = 1;
	       break;
	     case 'c':
	        cFlag = 1; atLeastOneFlag = 1;
	       break;
       	
     case '?':
       if(optopt== 'm'){
           cout << "-Uncognized argument" << endl;
       }
       break;
     case':':
	 cout<< "m requires arguments" << endl;
       break;
     default:
       cout << "Incorrect input" << endl;
       exit(EXIT_FAILURE);
       break;
      } // switch

  } // while loop

	//launched like ./wc OR ./wc - OR ./wc -cmlw OR ./wc -l -
	int otherCondition =  ( argc == 3 && (strcmp (argv[2], "-") == 0) &&  atLeastOneFlag == 1)  ? 1 : 0;
	
	if ( argc == 1  || (argc == 2 && strcmp (argv[1],"-") == 0) || (argc == 2 && atLeastOneFlag == 1) || otherCondition == 1 )
	{
		while ( (a = read(STDIN_FILENO, buffer, 1)) > 0 ) 
			{
				chars++;
				bytes++;
				if ( buffer [0] == '\n') 	{lines++ ;}
				if ( buffer [0] == ' ')  words++ ;
				//write(STDOUT_FILENO,buffer,1);
			}

	if ( words == 0 && chars > 0) words = 1;

	cout<<"\t"<<lines<<"\t"<<words<<"\t"<<bytes<<"\t"<<endl<<flush;
	
	if (argc == 2) cout<<"\t-"<<endl<<flush;

	return EXIT_SUCCESS;
	} // if argc == 1


//cout<<"\n C W L M : " <<cFlag<<"\t"<<wFlag<<"\t"<<lFlag<<"\t"<<mFlag<<endl;

	if ( argc == 2 && cFlag == 0 && mFlag == 0 && lFlag == 0 && wFlag == 0) // launched like ./wc filename
	{

		int fd = open ( argv[1], O_RDONLY);
		while ( (a = read(fd, buffer, 1)) > 0 ) 
			{
				chars++;
				bytes++;
				if ( buffer [0] == '\n') 	{lines++ ; words++;}
				if ( buffer [0] == ' ')  words++ ;
				//write(STDOUT_FILENO,buffer,1);
			}
	close(fd);

	cout<<lines<<" "<<words<<" "<<bytes<<" "<<argv[1]<<endl<<flush;


	}

return EXIT_SUCCESS;
}
