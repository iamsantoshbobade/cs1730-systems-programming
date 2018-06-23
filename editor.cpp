#include <cstdlib>
#include <ncurses.h>
#include <unistd.h>
#include <fcntl.h>
#include <menu.h>
#include <cstring>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
//#define CTRLD 	4

const char * choices[] = {
  "Open",
  "Save",
  "Save As",
  "Exit",
  (char *)NULL,
};


const char * filename;

WINDOW *create_window(int height, int width, int starty, int startx)
{
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  return local_win;
}


WINDOW *create_bordered_window(int height, int width, int starty, int startx)
{
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0 , 0);

  return local_win;
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string)
{	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	mvwprintw(win, y, x, "%s", string);
	refresh();
}

int exitProgram()
{
  
  endwin();
  exit(0);

}


int main(int argc, const char * argv [])
 {
  WINDOW *main_window;
  WINDOW *border_window;
  WINDOW *padding_window;
  WINDOW *error_window;

  WINDOW *menu_window;
  ITEM **my_items;
  MENU *my_menu;

  char buffer [1200];
  char name[20];
  int fd = 0;
  int flag = 1;
  int argcValid = 1;
  int keep = 1;
  int c = 0; //count for debug nothing else
  int pressedKey = 0;
  int arrowKeyPressed = 0;
  int x = 0;
  int y = 0;
  int n_choices = 0;
  int i = 0;
  int editFlag = 1;

  initscr();
  start_color();
  init_pair(1, COLOR_RED, COLOR_WHITE);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3,COLOR_BLUE,COLOR_BLACK);
  refresh();

  n_choices = ARRAY_SIZE(choices);
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for(i = 0; i < n_choices; ++i)
                my_items[i] = new_item(choices[i], "");

  my_menu = new_menu((ITEM **)my_items);

  if(argc==2) 
   {
	filename= argv[1];
	fd = open (filename,O_RDWR);
	if(fd!=-1) filename= argv[1]; else filename = "Invalid filename supplied.";
   }
   else filename = "Filename not supplied.";

  main_window = create_window(LINES-5, COLS-4, 2, 3);
  border_window = create_bordered_window(LINES-3, COLS-2, 1, 2);
  padding_window = create_window(LINES-5,2,3,0);
  error_window =  create_window(9,59,(LINES/2)-5,(COLS/2)-20);
  //menu_window = create_window(10, 40, 4, 4);

  /*keypad (menu_window,TRUE);

  set_menu_win(my_menu, menu_window);
  set_menu_sub(my_menu, derwin (menu_window, 6, 38, 3, 1));
  refresh();
  //getch();
  wrefresh(main_window);
  box(menu_window, 0, 0);
  print_in_middle(menu_window, 1, 0, 40, "My Menu", COLOR_PAIR(1));
  mvaddch( 2, 0, ACS_LTEE);
  mvhline( 2, 1, ACS_HLINE, 38);
  mvaddch( 2, 39, ACS_RTEE);*/
  //mvprintw(LINES - 2, 0, "F1 to exit");

  refresh();
  wrefresh(border_window);
  //post_menu(my_menu);
  //wrefresh(menu_window);

  attron(COLOR_PAIR(1));
  wrefresh(border_window);
  wclear(error_window);
  wrefresh(error_window);
  wbkgd(border_window, COLOR_PAIR(2));
  wbkgd(padding_window,COLOR_PAIR(3));
  wrefresh(border_window);
  mvprintw(0,0,"F1: MENU");
  mvprintw(0,(COLS/2)-10,"CS1730 Text Editor!");
  mvprintw(LINES-1,0,filename);

  for(int i=1; i<=(LINES-3);i++)
  {    
	if(i<10)
	  {
	    wprintw(padding_window,"0%d",i);
	  }
	else
	  {
	    wprintw(padding_window,"%d",i);
	  }
	wrefresh(padding_window);
  } //   for loop for padding

  int n = 0;
  while (flag == 1)
  {
    if(argc==2)
      {
	wclear(main_window);
	if(fd!=-1)
	{
  		n = read(fd, buffer, 1000);
	  	buffer[n]='\0';
		mvwprintw(main_window,1,0,buffer);
		mvprintw(LINES-1,0,filename);
		flag = 0;
		editFlag = 0;
	}//file exists
 	else
	{
		 wmove(error_window, 0,0);
		 wprintw(error_window, "File does not exist.");
		 wmove(error_window, 1, 0);
		 //wprintw(error_window,"Input a valid filename: ");
		 mvprintw(LINES-1, (COLS-2)-45,"Input a valid filename: ");
		 wmove(error_window,1,30);
		 
		 wrefresh(main_window);
		 wrefresh(error_window);
		 getstr(name);
		 filename = name;
		 fd = open (filename,O_RDWR);
		 //mvwprintw(main_window,1,0,"File does not exist.");
		 clrtoeol();
		 wrefresh(main_window);
		flag = 1;
	}//file does not exist
	
  	wrefresh(main_window);
      }// if argc == 2

  else if(argc == 1){

	if(argcValid == 1)
		{
			wmove(error_window, 0,0);
			wprintw(error_window, "No command line arguments were passed.");
			wmove(error_window, 1, 0);
		}
	else
		{
			wmove(error_window, 0,0);
			wprintw(error_window, "File does not exist. Input filename again.");
			wmove(error_window, 1, 0);
		} 
	wclear(main_window);
//	mvwprintw(main_window,LINES-10,0,"Please input a filename: ");
	mvprintw(LINES-1, (COLS-2)-45,"Input a valid filename: ");
	
	wmove(main_window,LINES-20,50);
	wrefresh(main_window);
	wrefresh(error_window);
	getstr(name);
	filename = name;
	fd = open (filename,O_RDWR);
	clrtoeol();
	wrefresh(main_window);
	argcValid = 0;
	if(fd != -1)
	  {
		n = read(fd, buffer, 1000);
	  	buffer[n]='\0';
		mvwprintw(main_window,1,0,buffer);
		mvprintw(LINES-1,0,filename);
		wrefresh(main_window);
		wclear(error_window);
		wrefresh(error_window);
	    	flag = 0;
		editFlag = 0;
	  }
	else
	  {
	    flag = 1;
	  }
      } // if argc == 1
  } //while flag == 1 loop
 
  wrefresh(padding_window);
  scrollok(main_window,TRUE);
  wmove(main_window,0,0);
  refresh();
  wrefresh(main_window);
   while(keep == 1)
  {
    //noecho();
    c=0;
    arrowKeyPressed = 0;
    keypad(main_window,TRUE);
    pressedKey=wgetch(main_window);
	//DEBUG
	//getyx(main_window,y,x);
	//printw("%d : %d: %d",y,x,pressedKey);
	//refresh();
	//printw(pressed
	//waddch(main_window,'C');


  if(pressedKey == KEY_F(1)) //F1 Menu
  {
	n_choices = ARRAY_SIZE(choices);
  	my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
	for(i = 0; i < n_choices; ++i)
           my_items[i] = new_item(choices[i], "");

	my_menu = new_menu((ITEM **)my_items);
	menu_window = create_window(10, 40, 4, 4);
	keypad(menu_window,TRUE);

	set_menu_win(my_menu, menu_window);
	set_menu_sub(my_menu, derwin (menu_window, 6, 38, 3, 1));

	set_menu_format(my_menu, 5, 1);
	set_menu_mark(my_menu, "* ");
	box(menu_window, 0, 0);
	print_in_middle(menu_window, 1 , 0, 40, "My Menu");
	mvwaddch(menu_window, 2, 0, ACS_LTEE);
	mvwhline(menu_window, 2, 1, ACS_HLINE, 38);
	mvwaddch(menu_window, 2, 39, ACS_RTEE);
	refresh();
	  
	post_menu(my_menu);
	refresh();

	int menuKey = 0;
	int menuOptionSelected = 0;

	while ( (menuKey = wgetch(menu_window)) != KEY_F(1) )
	  {

	    switch (menuKey)
		{

		  case KEY_DOWN : menu_driver(my_menu, REQ_DOWN_ITEM);
		  break;
	
		  case KEY_UP : menu_driver(my_menu, REQ_UP_ITEM);
		  break;

		  case 10 : pos_menu_cursor(my_menu);
				   
				   if(item_name(current_item(my_menu))==choices[0])
				   {
				   	//open file here
					char openFileName [20];
				   	const char * openfile;
				   	int openfd = 0;
				   	int openFlag = 1;		   	
				   	while(openFlag == 1)
				   	{
				   	move(LINES-1,0);
				   	clrtoeol();
				   	refresh();
				   	mvprintw(LINES-1, (COLS-2)-45,"Input a filename to Open: "); refresh();
				   	getstr(openFileName);
					openfile=openFileName;
				   	openfd = open (openFileName, O_RDWR);
				   	if ( openfd != -1)
				   	{
				   	  // open success 
				   	  close(fd);
				   	  wclear(main_window);
				   	  clrtoeol();
				   	  wmove(main_window,LINES-20,50);
					  wrefresh(main_window);
				   	  n = read(openfd, buffer, 1000);
				   	  buffer[n]='\0';
				   	  mvwprintw(main_window,1,0,buffer);
				   	  mvprintw(LINES-1,0,openfile);
				   	  wrefresh(main_window);
					  wclear(error_window);
					  wrefresh(error_window);
					  refresh();
					  openFlag = 0;
					  editFlag = 0;
				   	  fd = openfd;
				   	} // if openfd = -1 block
				   	else
				   	{
				   	  //open fail 
				   	  wclear(main_window);clrtoeol();wrefresh(main_window);wclear(error_window); refresh();
					  wmove(error_window, 0,0);
				   	  wprintw(error_window, "File does not exist. Input filename again.");
				   	  wmove(error_window, 1, 0);
				   	  //wclear(main_window);
					  wrefresh(error_window);wrefresh(main_window);refresh();
				   	} // else of if openfd != -1 block
				   	
				   	} // while openFlag == 1 loop
				   } // if OPEN block
				   else if(item_name(current_item(my_menu))==choices[1])
				   {
				   	//save file
				   }
				   else if(item_name(current_item(my_menu))==choices[2])
				   {
				   	//save as
					int saveAsLoop = 1;
					int entryLoop = 1;
					int newfd = 0;
					while ( saveAsLoop == 1)
					{
					  char * saveAsFileName; 
					  if ( entryLoop == 1)
					  {  
					    
					    entryLoop = 0;
					    mvprintw(LINES-1, (COLS-2)-45,"Input a filename to Save As: "); refresh();
					    getstr(name);
					    saveAsFileName = name; 
					    newfd = open (saveAsFileName,O_RDWR);
					  } // if entryLoop == 1 block
					
					if (newfd != -1) // File exists with the same name
					{
					  close(newfd);
					  clrtoeol();
					  printw("File already exists. Replace the original file? (Enter Y/y or N/n)");
					  refresh();
					  char ans = getch();
					  if (ans == 'Y' || ans == 'y')
					  {
					    saveAsLoop = 0;
					    unlink(saveAsFileName);
					    FILE *newFile;
					    newFile = fopen(saveAsFileName,"w+");
					    fputs(buffer,newFile);
					    fclose(newFile);
					    move(LINES-1,0);clrtoeol();printw("Original file replaced."); refresh();getch();
					    editFlag = 0;
					    //move(LINES-1,0);clrtoeol();refresh();getch();

					  } // if ans == Y OR y block
					  else
					   {
					     //Do something here, user do not want to replace the already existing file.
					    move(LINES-1,0);clrtoeol();refresh();printw("Enter a new filename: "); refresh();
					    getstr(name);saveAsFileName = name;
					    //const char * saveAsNewFileName = name; 
					    newfd = open (saveAsFileName,O_RDWR);
					    /*if ( newfd != -1 )
					    {
					    	saveAsLoop = 1;
					    
					    }*/
					    /*else
					    {
					        FILE *newFile;
					        newFile = fopen(saveAsNewFileName,"w+");
					        fputs(buffer,newFile);
					        fclose(newFile);
					    
					    }*/
					  }// else of if ans == Y OR y if block					
					}// if newfd != -1 block
					else					
					{
					    FILE *newFile;
					    newFile = fopen(saveAsFileName,"w+");
					    fputs(buffer,newFile);
					    fclose(newFile);
					    saveAsLoop = 0;
					    editFlag = 0;
					} // else of if newfd != -1 block
					
					}//while saveAsLoop == 1
					
				   }
				   else if(item_name(current_item(my_menu))==choices[3])
				   {
				   	//exit program
				   	if ( editFlag == 0)
				   	{
				   	  //file not edited
					  exitProgram();
				   	}
				   	else
				   	{
				   	   mvprintw(LINES-1, (COLS-2)-60,"Save before exit? (Y/y or N/n): ");refresh();
				   	   char answer = getch();
				   	   if ( answer == 'Y' || answer == 'y')
				   	   {
					     close(fd);
					     unlink(filename);
					     FILE *newFile;
					     newFile = fopen(filename,"w+");
					     fputs(buffer,newFile);
					     fclose(newFile);
				   	     exitProgram();
				   	   
				   	   }
				   	   else
				   	    exitProgram();
				   	}
				   	
				   }

		  break; //CASE 10 i.e. KEY_ENTER (it is not detecting KEY_ENTER)
		}
	  wrefresh(menu_window);
	  }// while menuKey != F1 loop
  }
  else if(pressedKey == KEY_BACKSPACE) //backspace
      {
	//printw("LOL");
	//refresh();
	///////////getch(); It doesnt come here
	//wmove(main_window,1,1);
	//wrefresh(main_window);
	getyx(main_window,y,x);
        wmove(main_window,y,x);
	//wmove(main_window,y+2,x+2);
	wrefresh(main_window);
	waddch(main_window,' ');
	wmove(main_window,y,x );
	//refresh();
        wrefresh(main_window);
        arrowKeyPressed = 1;
	buffer[n-1]=' ';
        n--;
	editFlag = 1;
      } 
  else if (pressedKey == KEY_RIGHT)
      {
        getyx(main_window,y,x);
	wmove(main_window,y,x + 1);
	//wrefresh(main_window);
	arrowKeyPressed = 1;
	c++;
	//printw("RIGHT ARROW");
	wrefresh(main_window);
      }
  else if (pressedKey == KEY_LEFT)
      {
        getyx(main_window,y,x);
        wmove(main_window,y,x - 1);
        wrefresh(main_window);
        arrowKeyPressed = 1;
        c--;
      }
  else if (pressedKey == KEY_DOWN)
      {
        getyx(main_window,y,x);
        wmove(main_window,y+1,x);
        wrefresh(main_window);
        arrowKeyPressed = 1;
      }
  else if (pressedKey == KEY_UP)
      {
        getyx(main_window,y,x);
        wmove(main_window,y-1,x);
        wrefresh(main_window);
        arrowKeyPressed = 1;
      }

  } // while keep == 1 loop

  getch();
  endwin();
  return EXIT_SUCCESS;
 } // main

