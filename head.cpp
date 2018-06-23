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
	int a = 1;
	char buffer [1]; 
	int count = 0;  // keeps track of current new line characters
	int h = 10;	// value passed via command line paramaetr

	if ( (argc == 1) || ( argc == 2 && strcmp(argv[1],"-") == 0))
	{
			while ( (a = read(STDIN_FILENO, buffer, 1)) > 0 ) 
			{
				if ( buffer [0] == '\n') count++ ;
				write(STDOUT_FILENO,buffer,1);
				if ( count == h) return EXIT_SUCCESS;
			}

		return EXIT_SUCCESS;
	}
	if ( argc > 2 )
	{ 
		string s = argv[1];
		h = atoi((s.substr(1,2)).c_str());
	} 
	int fd = open (argv[2], O_RDONLY);
	if ( fd == -1 )
	{
		perror("");
		return EXIT_FAILURE;
	}
	while ( (a = read(fd, buffer, 1)) > 0 )
	{
		if(buffer [0] == '\n') count++ ;

		write(STDOUT_FILENO,buffer,1);
		if ( count == h) break;

	}

return EXIT_SUCCESS;

}
