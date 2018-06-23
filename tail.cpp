#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <vector>
#include <fstream>
using namespace std;

int main(int argc,  char * argv [])
{
	int a = 1, index =  0;
	char buffer [1]; 
	int chars = 0;  // keeps track of total number of characters
	int h = 10;	// value passed via command line paramaetr (-n argValue)
	int lines = 0;	// keeps track of total number of lines
	vector <vector<char>> fileContent ;
	int c = 10;	// value passed via command line paramaetr (-c argValue)
	char block = 'n';

	char *filename = argv[1];

	int atLeastOneArgPassed = 0;

	if ( argc > 3)
	{
	  filename = argv[3];
	  if ( strcmp (argv[1],"-n") == 0) // when launched like ./tail -n 8 filename
		{
			string s = argv[2];
			h = atoi(s.c_str());
			block = 'n';
		}
	  else if ( strcmp (argv[1],"-c") == 0) // when launched like ./tail -c 20 filename
		{
			string s = argv[2];
			c = atoi(s.c_str());
			block = 'c';
		}

	}
	else if (argc > 1)
	{	
	  atLeastOneArgPassed = ( strcmp (argv[1], "-n") == 0 || strcmp (argv[1], "-c") == 0 ) ? 1 : 0;
	  if (strcmp (argv[1], "-n") == 0 ) 
		{
			string s = argv[2];
			h = atoi(s.c_str());
			block = 'n'; 
		}
	  else if (strcmp (argv[1], "-c") == 0 )
		{
			string s = argv[2];
			c = atoi(s.c_str());
			block = 'c';
		}
	}
	char file[100][200];
	

	int rows = 0, cols = 0, i = 0, j = 0;
	int totalRows = 0, totalCols = 0;

	if ( argc == 1 || (argc == 3 && atLeastOneArgPassed == 1) ) // launched like ./tail OR ./tail -n 8 OR ./tail -c 20
	{

		while ( (a = read(STDIN_FILENO, buffer, 1)) > 0 ) 
		 {
		   file[rows][cols++]=buffer[0];
		   chars++;
		   if ( buffer [0] == '\n' ) {rows++, cols = 0; totalRows++; }
		 }

	}

	else
	{
		int fd = open (filename, O_RDONLY);

		while ( (a = read(fd, buffer, 1)) > 0 ) 
		{
			file[rows][cols++]=buffer[0];
			chars++;
			if ( buffer [0] == '\n' ) {rows++, cols = 0; totalRows++; }
		}

		close(fd);
	} 

	if ( block == 'n')
	{
	  int start = ( h > totalRows ) ? 0 : totalRows - h;

	  for ( i = start ; i < totalRows ; i++)
	  {
	    for( j = 0; file[i][j] != '\n' ; j++)
		{
		cout<<file[i][j]<<flush;
		} // for j loop
	    cout<<endl<<flush;
	  } // for i loop
	} // if block == n
	else if ( block == 'c')
	{
	  char charArray [1200];

	  for ( i = 0 ; i < totalRows ; i++)
		{
	  	for( j = 0; file[i][j] != '\n' ; j++)
		  {
		    charArray[index++]=file[i][j]; // copy entire 2D array into 1D array
		  } // for j loop
		 charArray[index++] = '\n';
		} // for i loop

	//cout<<"\n abt t print array\n"<<chars<<" "<<c<<" "<<index<<endl;

	i = ( c > chars ) ? 0 : chars - c;
	for ( ; i < chars; i++)
		cout << charArray[i]<<flush;


	} // else if block == c

return EXIT_SUCCESS;

}
