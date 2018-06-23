#include<stdio.h>
#include<cstdlib>
#include<iostream>
#include<string.h>
#include<fstream>
#include<dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>


using namespace std;

vector <string> dirs;

void printRecursive(char *);

int main ( int argc, char *argv[] )
{

dirs.push_back(argv[1]);
printRecursive(argv[1]);


return EXIT_SUCCESS;
}

void printRecursive(char *tempPath)
{
cout<<"\nCurrent path: "<<tempPath<<endl<<flush;
//cout<<path<<endl;
 DIR *pDIR;
 struct dirent *entry;
 int ch = chdir(tempPath);

if(ch == -1) {cout<<"Tried ch to " << tempPath<<endl<<flush; perror("chdir");}
else { cout<<"Success chdir to " << tempPath<<flush; char * pwd = get_current_dir_name(); cout <<"   PWD:"<< pwd << endl<<flush;}

/*chdir("..");chdir("..");
char * pwd = get_current_dir_name();
 
      cout << pwd << endl;

*/
 if( pDIR=opendir(".") ){
                while(entry = readdir(pDIR)){
		     char *path = entry -> d_name;
                     if( strcmp(path, ".") != 0 && strcmp(path, "..") != 0 && strcmp ( path, ".\\recremtest") != 0) {
			struct stat temp;
			stat ( path, &temp);
			if(S_ISDIR(temp.st_mode)) {
					cout<<"This file: "<<path<<" is a directory. Trying to delete it now.."<<endl<<flush;
					if ( rmdir (path) == -1) printRecursive(path); // this is cuz dir may not be empty
					else // dir is empty
					 { cout<<"Dir "<<path<<" is empty and deleted."<<endl<<flush; /*chdir("..");*/
						//chdir(".."); if(rmdir (get_current_dir_name()) == -1 ) continue;

					} // else block of dir is empty

			} // if S_ISDIR temp to check if path is a directory
			else{
			  int fd = open (path, O_WRONLY);
				if ( fd != -1) { 
					cout<<"This file: "<<path<<" can be deleted easily."<<endl<<flush; //unlink code
					unlink(path);
				} // if block of file does have write permission
				else{
					cout<<"This file: "<<path<<" does not have write permission."<<endl<<flush; //prompt Y/N -> unlink code
					unlink(path);
				} // else block of file does not have write permission			

			}


		} // if strcmp path != . and ..

	}// while entry = readdir(pDIR)
chdir("..");
if( rmdir(tempPath) == -1) {cout<<"LOL: "<<tempPath<<" not deleted."<<endl<<flush; perror("tempPath");}
else cout<<"LOL: "<<tempPath<<" deleted successfully. "<<endl<<flush;
} // if  pDIR=opendir(".")

}
