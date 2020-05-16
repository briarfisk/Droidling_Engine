/*****************************************************************  
**                                                              **
**                                                              **
**     c_Console                                                **
**                                                              **
**     This class is for the console handling functions         **
**                                                              **
**                                                              **
**     Functions:                                               **
**                                                              **
**          /set_Console_Color -This functions takes two        **
**               arguments, one for background color and        **
**               one for foreground color                       **
**                                                              **
**                                                              **
**                                                              **
**     Classes:                                                 **
**                                                              **
*****************************************************************/



//console color thingy
HANDLE  hConsole;
CONSOLE_SCREEN_BUFFER_INFO csbi;
int bg;//current background
int fg;//current foreground

void init_Console()
{
     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void set_Console_Title(std::string p_Title)
{
     SetConsoleTitleA(p_Title.c_str());
	 
}

void set_Console_Color(int p_bg=0, int p_fg=0)
{
     
     // sets color
     bg = p_bg;
     fg = p_fg;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}

void output_Console_Character_Colored(int p_bg, int p_fg, char p_Char)
{
     //hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     // sets color
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     std::cout << p_Char;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}


void output_Console_Int_Colored(int p_bg, int p_fg, int p_Int)
{
     //hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     // sets color
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     std::cout << p_Int;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
     set_Console_Color(bg, fg);
}

void output_Console_Float_Colored(int p_bg, int p_fg, float p_Float)
{
     //hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     // sets color
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     std::cout << p_Float;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
     set_Console_Color(bg, fg);
}

void output_Console_Float_Colored_With_Additional_Charge_For_Precision_Setting(int p_bg, int p_fg, float p_Float, float p_Precision = .0000001)
{
     //hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     // sets color
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     p_Float += p_Precision;
     std::cout << p_Float;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
     set_Console_Color(bg, fg);
}


void output_Console_Float_Colored_With_Precision_5(int p_bg, int p_fg, float p_Float)
{
     //hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     // sets color
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     int tmp = int (p_Float * 100000);
     p_Float = float (tmp * .000001);
     std::cout << p_Float;
     
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
     set_Console_Color(bg, fg);
}


void ostr(int p_bg, int p_fg, std::string p_String)
{
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     std::cout << p_String;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}

void ochr(int p_bg, int p_fg, char p_String)
{
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     std::cout << p_String;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}

void opchr(int p_bg, int p_fg, char p_String)
{
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     if (int (p_String) == 0 || int (p_String) == 7 || int (p_String) == 8 || int (p_String) == 9 || int (p_String) == 10 || int (p_String) == 13 || int (p_String) == 255)
     {
          std::cout << "?";
     }
     else
     {
          std::cout << p_String;
     }
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}

void oull(int p_bg, int p_fg, unsigned long long int p_Int)
{
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     std::cout << p_Int;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}

void oint(int p_bg, int p_fg, int p_Int)
{
	SetConsoleTextAttribute(hConsole, p_fg + (p_bg * 16));
	std::cout << p_Int;
	SetConsoleTextAttribute(hConsole, fg + (bg * 16));
}

void oint64(int p_bg, int p_fg, long long int p_Int)
{
	SetConsoleTextAttribute(hConsole, p_fg + (p_bg * 16));
	std::cout << p_Int;
	SetConsoleTextAttribute(hConsole, fg + (bg * 16));
}

void oflt(int p_bg, int p_fg, float p_Float)
{
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     std::cout << p_Float;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}

void odbl(int p_bg, int p_fg, double p_Double)
{
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     std::cout << p_Double;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}



void opoi(int p_bg, int p_fg, void * p_NR)
{
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     std::cout << p_NR;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}




void oindex(int p_Index, int p_Color = 13)
{
     std::cout << " ["; oint(0, p_Color, p_Index); std::cout << "] ";
}












void console_Move_Cursor_To_Given_X_And_Y( short int p_X, short int p_Y )
{
     //system("PAUSE > NULL");
    COORD tmp_Coordinate = { p_X, p_Y };
    SetConsoleCursorPosition( hConsole, tmp_Coordinate );
}

int get_Console_Cursor_X()
{
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	return csbi.dwCursorPosition.X;
}

int get_Console_Cursor_Y()
{
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	return csbi.dwCursorPosition.Y;
}

void xy(short int p_X, short int p_Y = -1)
{
     if (p_X == -1){ p_X = get_Console_Cursor_X(); }
     if (p_Y == -1){ p_Y = get_Console_Cursor_Y(); }
     console_Move_Cursor_To_Given_X_And_Y(p_X, p_Y);
}

void sx(short int p_X)
{
     short int tmp_Y = get_Console_Cursor_Y();
     
     xy(p_X, tmp_Y);
}

void sy(short int p_Y)
{
     short int tmp_X = get_Console_Cursor_X();
     
     xy(tmp_X, p_Y);
}


void cls(int X = 0, int Y = 0, int Width = 4, int Height = 100)
{
     console_Move_Cursor_To_Given_X_And_Y(X, Y);
     for (int cou_Y=Y;cou_Y<(Height + Y);cou_Y++)
     {
          console_Move_Cursor_To_Given_X_And_Y(X, cou_Y);
          for (int cou_Index=0;cou_Index<Width;cou_Index++)
          {
               std::cout << "                         ";
          }
     }
     console_Move_Cursor_To_Given_X_And_Y(X, Y);
}

//Outputs a line in the console window from the first set of coordinates to the second set of coordinates.
void console_Draw_Line(int p_X1, int p_Y1, int p_X2, int p_Y2, int p_Color = 1, int p_Char = 177, float p_Percentage = 1.0)
{
     //Bounds checking for the console window.
     if (p_X1 > 9998 || p_Y1 > 9998 || p_X2 > 9998 || p_Y2 > 9998){ return; }
     
     //Get the rise over run.
	 float tmp_Rise = float(p_Y2 - p_Y1);
	 float tmp_Run = float(p_X2 - p_X1);
     
     //The increment for rise and run.
     float tmp_Rise_Inc = 0.0;
     float tmp_Run_Inc = 0.0;
     
     //The temporary positions.
     float tmp_X = 0.0;
     float tmp_Y = 0.0;
     
     if (abs(tmp_Rise) >= abs(tmp_Run))
     {
          //If the tmp_Rise is larger then reduce by the rise.
          tmp_Rise_Inc = tmp_Rise / abs(tmp_Rise);
          tmp_Run_Inc = tmp_Run / abs(tmp_Rise);
          
//          std::cout << "\n ~Ri >= ~Ru " << tmp_Rise_Inc << "\\" << tmp_Run_Inc;
          
          for (int cou_Y=0;cou_Y<(int (abs(tmp_Rise) * p_Percentage));cou_Y++)
          {
               //Increase the current position.
               tmp_X += tmp_Run_Inc;
               tmp_Y += tmp_Rise_Inc;
               
               //Output the current position.
               console_Move_Cursor_To_Given_X_And_Y(tmp_X + p_X1, tmp_Y + p_Y1);
               output_Console_Character_Colored(0, p_Color, p_Char);
          }
     }
     if (abs(tmp_Rise) < abs(tmp_Run))
     {
          //If the tmp_Run is larger then reduce by the tmp_Run.
          tmp_Rise_Inc = tmp_Rise / abs(tmp_Run);
          tmp_Run_Inc = tmp_Run / abs(tmp_Run);
          
//          std::cout << "\n ~Ri < ~Ru " << tmp_Rise_Inc << "\\" << tmp_Run_Inc;
          
          for (int cou_X=0;cou_X<abs(int ((tmp_Run) * p_Percentage));cou_X++)
          {
               //Increase the current position.
               tmp_X += tmp_Run_Inc;
               tmp_Y += tmp_Rise_Inc;
               
               //Output the current position.
               console_Move_Cursor_To_Given_X_And_Y(int(tmp_X + p_X1), int(tmp_Y + p_Y1));
               output_Console_Character_Colored(0, p_Color, p_Char);
          }
     }
}






//Countdowns for the given amount of time displaying as it counts down.
void countdown(int p_Seconds)
{
     int tmp_X = get_Console_Cursor_X();
     int tmp_Y = get_Console_Cursor_Y();
     double tmp_Output = p_Seconds;
     
     for (int cou_Index=0;cou_Index<(p_Seconds * 10);cou_Index++)
     {
          xy(tmp_X, tmp_Y);
          std::cout << " <{ ";
          
          odbl(0, 12, tmp_Output);
          
          std::cout << "     ";
          
          Sleep(100);
          
          tmp_Output -= .1;
     }
}

