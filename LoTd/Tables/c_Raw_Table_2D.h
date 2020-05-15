
//The raw table cell class for the 2d table.
class c_Raw_Table_Cell_2D
{
public:
     
     //The data held in the cell.
     u_Data_3 ** Data;
     
     //The bitmap data in the cell.
     HBITMAP * Data_IMG;
     
     //The bitmaps pixel array address.
     BYTE * Data_IMG_Pixel_Adress;
     
     //The number of indexes in a row, used to calculate pixel position.
     int Data_IMG_Row_Offset;
     
     //The Width of the current cell.
     int Width;
     
     //The max Width of the data in the current cell.
     int MAX_Width;
     
     //The Increment which to increase the max Width by.
     int MAX_Width_I;
     
     //The Width of the current cell.
     int Height;
     
     //The max Width of the data in the current cell.
     int MAX_Height;
     
     //The Increment which to increase the max Width by.
     int MAX_Height_I;
     
     c_Raw_Table_Cell_2D()
     {
          Data = NULL;
          Width = 0;
          MAX_Width = 0;
          MAX_Width_I = 10;
          Height = 0;
          MAX_Height = 0;
          MAX_Height_I = 10;
          
          Data_IMG = NULL;
          Data_IMG_Pixel_Adress = NULL;
          Data_IMG_Row_Offset = 0;
     }
     
     ~c_Raw_Table_Cell_2D()
     {
          //**--if (Data != NULL){ ga_Cell_Alloc--; }
          if (Data != NULL)
          {
               for (int cou_Index=0;cou_Index<Width;cou_Index++)
               {
                    if (Data[cou_Index] != NULL){ delete Data[cou_Index]; Data[cou_Index] = NULL; }
               }
               delete [] Data;
          }
          Data = NULL;
          if (Data_IMG != NULL){ delete Data_IMG; }
          Data_IMG = NULL;
          Data_IMG_Pixel_Adress = NULL;
          Data_IMG_Row_Offset = 0;
     }
     
     //Initializes the cells data.
     void initialize()
     {
          if (Data != NULL)
          {
               for (int cou_Index=0;cou_Index<Width;cou_Index++)
               {
                    if (Data[cou_Index] != NULL){ delete Data[cou_Index]; Data[cou_Index] = NULL; }
               }
               delete [] Data;
          }
          Data = NULL;
          Width = 0;
          MAX_Width = 0;
          MAX_Width_I = 10;
          Height = 0;
          MAX_Height = 0;
          MAX_Height_I = 10;
     }
     
     //Resets the cell.
     void reset()
     {
          if (Data != NULL)
          {
               for (int cou_Index=0;cou_Index<Width;cou_Index++)
               {
                    if (Data[cou_Index] != NULL){ delete Data[cou_Index]; Data[cou_Index] = NULL; }
               }
               delete [] Data;
          }
          Data = NULL;
          Width = 0;
          MAX_Width = 0;
          MAX_Width_I = 10;
          Height = 0;
          MAX_Height = 0;
          MAX_Height_I = 10;
     }
     
     //====----
     //==---     ACCEPTS A SUBMISSION AND SETS THE CELLS DATA ACCORDINGLY
     //====----
          
     //Takes a submission.
     void submit_String(string p_Submission, int p_X = 0, int p_Y = 0)
     {
          soft_Resize((p_Submission.size() + p_X), (p_Y + 1));
          
          for (unsigned int cou_C=p_X;cou_C<(p_Submission.size() + p_X);cou_C++)
          {
               Data[cou_C] [p_Y].C = p_Submission[cou_C];
          }
     }

	 //Takes a submission.
	 void submit_Int(long long int p_Submission, int p_X = 0, int p_Y = 0)
	 {
		 soft_Resize((p_X + 1), (p_Y + 1));

		 Data[p_X][p_Y].I = p_Submission;
	 }

	 //Takes a submission.
	 void submit_U_Int(unsigned long long int p_Submission, int p_X = 0, int p_Y = 0)
	 {
		 soft_Resize((p_X + 1), (p_Y + 1));

		 Data[p_X][p_Y].U = p_Submission;
	 }
     
     //Takes a submission.
     void submit_Float(double p_Submission, int p_X = 0, int p_Y = 0)
     {
          soft_Resize((p_X + 1), (p_Y + 1));
          
          Data[p_X] [p_Y].F = p_Submission;
     }
          
     //Takes a submission.
     void submit_NR(void * p_Submission, int p_X = 0, int p_Y = 0)
     {
          soft_Resize((p_X + 1), (p_Y + 1));
          
          Data[p_X] [p_Y].NR = p_Submission;
     }
     
     //====----
     //==---     ACCEPTS A SUBMISSION ADDS IT TO THE CELL
     //====----
          
     //Takes a submission.
     void add_String(string p_Submission)
     {
          soft_Resize((p_Submission.size() + 0), (0 + 1));
          
          for (unsigned int cou_C=0;cou_C<(p_Submission.size() + 0);cou_C++)
          {
               Data[cou_C] [0].C = p_Submission[cou_C];
          }
     }
     
     //Takes a submission.
     void add_Int(long long int p_Submission)
     {
          soft_Resize((0 + 1), (0 + 1));
          
          Data[0] [0].I = p_Submission;
     }
     
     //Takes a submission.
     void add_Float(double p_Submission)
     {
          soft_Resize((0 + 1), (0 + 1));
          
          Data[0] [0].F = p_Submission;
     }
          
     //Takes a submission.
     void add_NR(void * p_Submission)
     {
          soft_Resize((0 + 1), (0 + 1));
          
          Data[0] [0].NR = p_Submission;
     }
     
     
     //====----
     //==---     ACCEPTS AN ARRAY TO READ INTO THE CELL
     //====----
          
     //Takes an array of u_Data_3 to read into the cell.
     void set_IMG(u_Data_3 ** p_Data, int p_Width, int p_Height, int p_X = 0, int p_Y = 0)
     {
          resize_With_Reset(p_Width, p_Height);
          
          for (int cou_X=0;cou_X<p_Width;cou_X++)
          {
               for (int cou_Y=0;cou_Y<p_Height;cou_Y++)
               {
                    Data[cou_X] [cou_Y].NR = p_Data[cou_X] [cou_Y].NR;
               }
          }
     }
     
     //Takes an array of u_Data_3 to read into the cell.
     void set_IMG_Pixel(u_Data_3 p_Data, int p_X = 0, int p_Y = 0)
     {
          soft_Resize((p_X + 1), (p_Y + 1));
          
          Data[p_X] [p_Y].NR = p_Data.NR;
     }
     
     //Takes an array of u_Data_3 to read into the cell.
     void set_IMG_Pixel(double p_Data, int p_X = 0, int p_Y = 0)
     {
          soft_Resize((p_X + 1), (p_Y + 1));
          
          Data[p_X] [p_Y].F = p_Data;
     }
     
     //Takes an array of u_Data_3 to read into the cell.
     void set_IMG_Pixel(int p_Data, int p_X = 0, int p_Y = 0)
     {
          soft_Resize((p_X + 1), (p_Y + 1));
          
          Data[p_X] [p_Y].I = p_Data;
     }
     
     //Takes a cell and copies the data.
     void set_IMG_Cell_Data(c_Raw_Table_Cell_2D * p_Cell)
     {
          //**cout << "\n set_IMG_Cell_Data(" << p_Cell << ")";
          
          if (p_Cell == NULL){ return; }
          
          resize_With_Reset(p_Cell->Width, p_Cell->Height);
          
          //**cout << " X = " << Width << " Y = " << Height;
          
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               for (int cou_Y=0;cou_Y<Height;cou_Y++)
               {
                    Data[cou_X] [cou_Y] = p_Cell->Data[cou_X] [cou_Y];
               }
          }
          
          //**cout << "\n finished setting cell data...";
          //**system("PAUSE > NULL");
     }
     
     //Takes a cell and copies the data in the given block.
     void set_IMG_Cell_Data_Block(c_Raw_Table_Cell_2D * p_Cell, int p_X, int p_Y, int p_Size)
     {
          //**cout << "\n set_IMG_Cell_Data(" << p_Cell << ")";
          
          if (p_Cell == NULL){ return; }
          
          resize_With_Reset(p_Size, p_Size);
          
          //**cout << " X = " << Width << " Y = " << Height;
          
          for (int cou_X=0;cou_X<p_Size;cou_X++)
          {
               for (int cou_Y=0;cou_Y<p_Size;cou_Y++)
               {
                    Data[cou_X] [cou_Y] = p_Cell->Data[cou_X + p_X] [cou_Y + p_Y];
               }
          }
          
          //**cout << "\n finished setting cell data...";
          //**system("PAUSE > NULL");
     }
     
     
     
     //Takes an array of u_Data_3 to read into the cell.
     void add_IMG_Pixel_Data(double p_Data, int p_X = 0, int p_Y = 0)
     {
          soft_Resize((p_X + 1), (p_Y + 1));
          
          Data[p_X] [p_Y].F += p_Data;
     }
     
     //Takes an array of u_Data_3 to read into the cell.
     void add_IMG_Pixel_Data(int p_Data, int p_X = 0, int p_Y = 0)
     {
          soft_Resize((p_X + 1), (p_Y + 1));
          
          Data[p_X] [p_Y].I += p_Data;
     }
     
     //Takes an array of u_Data_3 to read into the cell.
     void add_IMG_Pixel_Data(long long unsigned int p_Data, int p_X = 0, int p_Y = 0)
     {
          soft_Resize((p_X + 1), (p_Y + 1));
          
          Data[p_X] [p_Y].U += p_Data;
     }
     
     //Resizes the row to hold a submission.
     void resize_With_Reset(int p_Width, int p_Height)
     {
          reset();
          resize(p_Width, p_Height);
     }
     
     
     
     //Resizes the row to hold a submission.
     void resize(int p_Width, int p_Height)
     {
          if (p_Width < MAX_Width)
          {
               if (p_Height < MAX_Height)
               {
                    for (int cou_X=0;cou_X<Width;cou_X++)
                    {
                         for (int cou_Y=0;cou_Y<Height;cou_Y++)
                         {
                              Data[cou_X] [cou_Y].NR = NULL;
                         }
                    }
                    
                    Width = p_Width;
                    Height = p_Height;
                    return;
               }
          }
          
          //**--if (Data != NULL){ ga_Cell_Alloc--; }
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               if (Data[cou_X] != NULL){ delete [] Data[cou_X]; Data[cou_X] = NULL; }
          }
          delete [] Data;
          Data = NULL;
          //**--ga_Cell_Alloc++;
          
          int tmp_MAX_Width = p_Width + MAX_Width_I;
          int tmp_MAX_Height = p_Height + MAX_Height_I;
          Data = new u_Data_3*[tmp_MAX_Width];
          for (int cou_X=0;cou_X<tmp_MAX_Width;cou_X++)
          {
               Data[cou_X] = new u_Data_3[tmp_MAX_Height];
               for (int cou_Y=0;cou_Y<tmp_MAX_Height;cou_Y++)
               {
                    Data[cou_X] [cou_Y].U = 0;
               }
          }
          Width = p_Width;
          MAX_Width = tmp_MAX_Width;
          Height = p_Height;
          MAX_Height = tmp_MAX_Height;
     }
     
     //Resizes the row to hold a submission.
     //No sense redeclaring these each time.
     bool flg_X;
     bool flg_Y;
     
     bool flg_X_L_MAX;
     bool flg_Y_L_MAX;
     
     int tmp_Old_Width;
     int tmp_Old_Height;
     
     int tmp_New_Width;
     int tmp_New_Height;
     
     u_Data_3 ** tmp_Data;
     
     void soft_Resize(int p_X, int p_Y)
     {
          flg_X = false;
          flg_Y = false;
          
          flg_X_L_MAX = false;
          flg_Y_L_MAX = false;
          
          tmp_Old_Width = Width;
          tmp_Old_Height = Height;
          
          tmp_New_Width = Width;
          tmp_New_Height = Height;
          
          if (p_X >= Width){ flg_X = true; tmp_New_Width = p_X; }
          if (p_Y >= Height){ flg_Y = true; tmp_New_Height = p_Y;}
          
          //If both X and Y fall under the old size then return;
          if (!flg_X && !flg_Y){ return; }
          
          if (p_X < MAX_Width){ flg_X_L_MAX = true; }
          if (p_X < MAX_Height){ flg_Y_L_MAX = true; }
          
          //If both X and Y fell withing the max bounds return.
          if (flg_X_L_MAX && flg_Y_L_MAX){ Width = tmp_New_Width; Height = tmp_New_Height; return; }
          
          /*
          cout << "\n                   soft_Resize(" << p_X << ", " << p_Y << ") " << this; cout.flush();
          cout << " flg_X " << flg_X; cout.flush();
          cout << " flg_Y " << flg_Y; cout.flush();
          cout << "  flg_X_L_MAX " << flg_X_L_MAX; cout.flush();
          cout << " flg_Y_L_MAX " << flg_Y_L_MAX; cout.flush();
          
          cout << "  MAX_Width " << MAX_Width; cout.flush();
          cout << " MAX_Height " << MAX_Height; cout.flush();
          
          cout << "  Width " << Width; cout.flush();
          cout << " Height " << Height; cout.flush();
          
          //cout << "  tmp_Old_Width " << tmp_Old_Width; cout.flush();
          //cout << " tmp_Old_Height " << tmp_Old_Height; cout.flush();
          
          cout << "  tmp_New_Width " << tmp_New_Width; cout.flush();
          cout << " tmp_New_Height " << tmp_New_Height; cout.flush();
          */
          
          tmp_Data = new u_Data_3*[tmp_Old_Width];
          
          for (int cou_X=0;cou_X<tmp_Old_Width;cou_X++)
          {
               tmp_Data[cou_X] = new u_Data_3[tmp_Old_Height];
               for (int cou_Y=0;cou_Y<tmp_Old_Height;cou_Y++)
               {
                    tmp_Data[cou_X] [cou_Y].NR = Data[cou_X] [cou_Y].NR;
               }
          }
          
          
          resize(tmp_New_Width, tmp_New_Height);
          
          for (int cou_X=0;cou_X<tmp_Old_Width;cou_X++)
          {
               for (int cou_Y=0;cou_Y<tmp_Old_Height;cou_Y++)
               {
                    Data[cou_X] [cou_Y].NR = tmp_Data[cou_X] [cou_Y].NR;
               }
               if (tmp_Data[cou_X] != NULL){ delete [] tmp_Data[cou_X]; tmp_Data[cou_X] = NULL; }
          }
          delete [] tmp_Data;
          tmp_Data = NULL;
     }
     
     //Outputs the cell as characters.
     void output_C_Force()
     {
          int tmp_X = get_Console_Cursor_X();
          int tmp_Y = get_Console_Cursor_X();
          
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               tmp_X = get_Console_Cursor_X();
               
               for (int cou_Y=0;cou_Y<Height;cou_Y++)
               {
                    char tmp_State = char (Data[cou_X] [cou_Y].I);
                    if (tmp_State == char (0)){ tmp_State = '?'; }
                    if (tmp_State == char (7)){ tmp_State = '?'; }
                    if (tmp_State == char (8)){ tmp_State = '?'; }
                    if (tmp_State == char (9)){ tmp_State = '?'; }
                    if (tmp_State == char (10)){ tmp_State = '?'; }
                    if (tmp_State == char (13)){ tmp_State = '?'; }
                    
                    xy(tmp_X, (tmp_Y + cou_Y));
                    cout << tmp_State;
               }
          }
     }
     
     //Outputs the cell as integer.
     void output_I()
     {
          int tmp_X = get_Console_Cursor_X();
          int tmp_Y = get_Console_Cursor_X();
          
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               tmp_X = get_Console_Cursor_X();
               
               for (int cou_Y=0;cou_Y<Height;cou_Y++)
               {
                    xy(tmp_X, (tmp_Y + cou_Y));
                    cout << " " << Data[cou_X] [cou_Y].I;
               }
          }
     }
     
     //Outputs the cell as integer.
     void output_U()
     {
          int tmp_X = get_Console_Cursor_X();
          int tmp_Y = get_Console_Cursor_X();
          
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               tmp_X = get_Console_Cursor_X();
               
               for (int cou_Y=0;cou_Y<Height;cou_Y++)
               {
                    xy(tmp_X, (tmp_Y + cou_Y));
                    cout << " " << Data[cou_X] [cou_Y].U;
               }
          }
     }
     
     //Outputs the cell as *void.
     void output_NR()
     {
          int tmp_X = get_Console_Cursor_X();
          int tmp_Y = get_Console_Cursor_X();
          
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               tmp_X = get_Console_Cursor_X();
               
               for (int cou_Y=0;cou_Y<Height;cou_Y++)
               {
                    xy(tmp_X, (tmp_Y + cou_Y));
                    cout << " " << Data[cou_X] [cou_Y].NR;
               }
          }
     }
     
     //Outputs the cell as float.
     void output_F()
     {
          int tmp_X = get_Console_Cursor_X();
          int tmp_Y = get_Console_Cursor_Y();
          
          //cout << "\n cou_X->" << Width;
          //cout << "\n cou_Y->" << Height;
          //system("PAUSE > NULL");
          
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               for (int cou_Y=0;cou_Y<Height;cou_Y++)
               {
                    xy((tmp_X + cou_X), (tmp_Y + cou_Y));
                    cout << Data[cou_X] [cou_Y].F;
               }
          }
          xy((tmp_X + Width), tmp_Y);
          //system("PAUSE > NULL");
     }
     
     //Outputs the cell as characters.
     void output_C()
     {
          for (int cou_Y=0;cou_Y<Height;cou_Y++)
          {
               cout << "\n";
               for (int cou_X=0;cou_X<Width;cou_X++)
               {
                    char tmp_State = char (Data[cou_X] [cou_Y].I);
                    if (tmp_State == char (0)){ tmp_State = '?'; }
                    if (tmp_State == char (7)){ tmp_State = '?'; }
                    if (tmp_State == char (8)){ tmp_State = '?'; }
                    if (tmp_State == char (9)){ tmp_State = '?'; }
                    if (tmp_State == char (10)){ tmp_State = '?'; }
                    if (tmp_State == char (13)){ tmp_State = '?'; }
                    
                    //xy(tmp_X, (tmp_Y + cou_Y));
                    cout << tmp_State;
               }
          }
     }
     
     
     
     
     //==--     SAVING TO FILE
     /*
     void save_C(ofstream * p_File)
     {
          for (int cou_Index=0;cou_Index<Width;cou_Index++)
          {
               if (Data[cou_Index].I == 0){ *p_File << char(254); if ((cou_Index + 1) < Width){ *p_File << " "; } continue; }
               *p_File << Data[cou_Index].C;
               if ((cou_Index + 1) < Width){ *p_File << " "; }
          }
     }
     
     void save_I(ofstream * p_File)
     {
          for (int cou_Index=0;cou_Index<Width;cou_Index++)
          {
               *p_File << Data[cou_Index].I;
               if ((cou_Index + 1) < Width){ *p_File << " "; }
          }
     }
     
     void save_F(ofstream * p_File)
     {
          for (int cou_Index=0;cou_Index<Width;cou_Index++)
          {
               *p_File << Data[cou_Index].F;
               if ((cou_Index + 1) < Width){ *p_File << " "; }
          }
     }
     
     void save_NR(ofstream * p_File)
     {
          for (int cou_Index=0;cou_Index<Width;cou_Index++)
          {
               *p_File << Data[cou_Index].NR;
               if ((cou_Index + 1) < Width){ *p_File << " "; }
          }
     }
     */
};


//The raw table row class for the 1d table.
class c_Raw_Table_Row_2D
{
public:
     
     //The data held in the row.
     c_Raw_Table_Cell_2D ** Cells;
     
     //The depth of the current row.
     int Depth;
     
     //The max depth.
     int MAX_Depth;
     
     //The number to increase the MAX_Depth by whenever enlarging.
     int MAX_Depth_Inc;
     
     c_Raw_Table_Row_2D()
     {
          Cells = NULL;
          MAX_Depth = 0;
          MAX_Depth_Inc = 1000;
          Depth = 0;
     }
     
     ~c_Raw_Table_Row_2D()
     {
          reset();
     }
     
     //====----
     //==---     SETS A GIVEN CELLS DATA TO THE GIVEN SUBMISSION
     //====----
     
     //Takes a submission.
     void submit_String(string p_Submission)
     {
          resize(Depth + 1);
          Cells[Depth - 1]->submit_String(p_Submission);
     }
     
     //Takes a submission.
     void submit_Int(long long int p_Submission)
     {
          resize(Depth + 1);
          Cells[Depth - 1]->submit_Int(p_Submission);
     }
     
     //Takes a submission.
     void submit_Float(double p_Submission)
     {
          resize(Depth + 1);
          Cells[Depth - 1]->submit_Float(p_Submission);
     }
     
     //Takes a submission.
     void submit_NR(void * p_Submission)
     {
          resize(Depth + 1);
          Cells[Depth - 1]->submit_NR(p_Submission);
     }
     
     
     //====----
     //==---     SETS A GIVEN CELLS DATA TO THE GIVEN SUBMISSION
     //====----
     
     //Takes a submission to a given cell.
     void set_String(int p_Cell, string p_Submission, int p_X = 0, int p_Y = 0)
     {
          resize(p_Cell + 1);
          Cells[Depth - 1]->submit_String(p_Submission, p_X, p_Y);
     }
     
     

	 //Takes a submission to a given cell.
	 void set_Int(int p_Cell, long long int p_Submission, int p_X = 0, int p_Y = 0)
	 {
		 resize(p_Cell + 1);
		 Cells[p_Cell]->submit_Int(p_Submission, p_X, p_Y);
	 }

	 //Takes a submission to a given cell.
	 void set_U_Int(int p_Cell, unsigned long long int p_Submission, int p_X = 0, int p_Y = 0)
	 {
		 resize(p_Cell + 1);
		 Cells[p_Cell]->submit_U_Int(p_Submission, p_X, p_Y);
	 }
          
     //Takes a submission to a given cell.
     void set_Float(int p_Cell, double p_Submission, int p_X = 0, int p_Y = 0)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->submit_Float(p_Submission, p_X, p_Y);
     }
          
     //Takes a submission to a given cell.
     void set_NR(int p_Cell, void * p_Submission, int p_X = 0, int p_Y = 0)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->submit_NR(p_Submission, p_X, p_Y);
     }
     
     //Takes a cell and copies the data.
     void set_IMG_Cell_Data(int p_Cell, c_Raw_Table_Cell_2D * p_Cell_Data)
     {
          //If there are not enough rows then create them.
          if (Depth <= p_Cell){ resize(p_Cell + 1); }
          
          //Set the string.
          Cells[p_Cell]->set_IMG_Cell_Data(p_Cell_Data);
     }
     
     
     
     
     //====----
     //==---     ADDS THE GIVEN SUBMISSION TO THE DATA OF THE GIVEN CELL
     //====----
     
     //Takes a submission to a given cell.
     void add_String(int p_Cell, string p_Submission)
     {
          resize(p_Cell + 1);
          Cells[Depth - 1]->add_String(p_Submission);
     }
     
     
     
     //Takes a submission to a given cell.
     void add_Int(int p_Cell, long long int p_Submission)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->add_Int(p_Submission);
     }
          
     //Takes a submission to a given cell.
     void add_Float(int p_Cell, double p_Submission)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->add_Float(p_Submission);
     }
          
     //Takes a submission to a given cell.
     void add_NR(int p_Cell, void * p_Submission)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->add_NR(p_Submission);
     }
     
     
     
     //Takes an array of u_Data_3 to read into the cell.
     void set_IMG(int p_Cell, u_Data_3 ** p_Data, int p_Width, int p_Height, int p_X = 0, int p_Y = 0)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->set_IMG(p_Data, p_Width, p_Height, p_X, p_Y);
     }
     
     //Takes an array of u_Data_3 to read into the cell.
     void set_IMG_Pixel(int p_Cell, u_Data_3 p_Data, int p_X = 0, int p_Y = 0)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->set_IMG_Pixel(p_Data, p_X, p_Y);
     }
     
     //Resizes the row to hold a new submission.
     void resize(int p_Size)
     {
          //cout << "\n resize(" << p_Size << ") D->" << Depth << " MX->" << MAX_Depth; cout.flush();
          
          if (p_Size <= Depth){ return; }
          
          //When it hits (MAX_Depth - 1) it resizes the array.
          if (p_Size < MAX_Depth){ Depth = p_Size; return; }
          
          
          //Setup the tmp and transfer the old cells to it while reallocating the row and transfering the old cells back before destroying the tmp.
          c_Raw_Table_Cell_2D ** tmp_Row;
          //**--ga_Row_Alloc++;
          tmp_Row = new c_Raw_Table_Cell_2D*[MAX_Depth];
          int tmp_MAX_Depth = p_Size + MAX_Depth_Inc;
          
          for (int cou_Index=0;cou_Index<MAX_Depth;cou_Index++)
          {
               tmp_Row[cou_Index] = Cells[cou_Index];
               Cells[cou_Index] = NULL;
          }
          
          //**--if (Cells != NULL){ ga_Row_Alloc--; }
          delete [] Cells;
          Cells = NULL;
          //**--ga_Row_Alloc++;
          Cells = new c_Raw_Table_Cell_2D*[tmp_MAX_Depth];
          
          for (int cou_Index=0;cou_Index<MAX_Depth;cou_Index++)
          {
               Cells[cou_Index] = tmp_Row[cou_Index];
               tmp_Row[cou_Index] = NULL;
          }
          
          //**--if (tmp_Row != NULL){ ga_Row_Alloc--; }
          delete [] tmp_Row;
          tmp_Row = NULL;
          
          for (int cou_Index=MAX_Depth;cou_Index<tmp_MAX_Depth;cou_Index++)
          {
               //**--ga_Row_Alloc++;
               Cells[cou_Index] = new c_Raw_Table_Cell_2D;
          }
          
          Depth = p_Size;
          MAX_Depth = tmp_MAX_Depth;
          
          //cout << " ND->" << Depth << " NMX->" << MAX_Depth << " MX_I->" << tmp_MAX_Depth;
     }
     
     //Resets the cells back to nothing.
     void reset()
     {
          if (Cells == NULL){ return; }
          if (MAX_Depth > 0)
          {
               for (int cou_Index=0;cou_Index<MAX_Depth;cou_Index++)
               {
                    //**--if (Cells[cou_Index] != NULL){ ga_Row_Alloc--; }
                    delete Cells[cou_Index];
                    Cells[cou_Index] = NULL;
               }
               //**--if (Cells != NULL){ ga_Row_Alloc--; }
               delete [] Cells;
               Cells = NULL;
               
               Depth = 0;
               MAX_Depth = 0;
               return;
          }
          
          cout << "\n\n\n CELLS IN ROW " << this << " ARE NOT NULL YET DEPTH == 0, NOT GOOD!! WTF!!";
          //**--if (Cells != NULL){ ga_Row_Alloc--; }
          delete [] Cells;
          Cells = NULL;
          
          Depth = 0;
     }   
     
     //Resets the cells back to nothing.
     void reset_End()
     {
          cout << " reset_End() " << this; cout.flush();
          
          if (Cells == NULL){ return; }
          
          cout << " Depth->" << Depth; cout.flush();
          if (Depth != 0)
          {
               cout << " delete Cells->" << Cells; cout.flush();
               for (int cou_Index=0;cou_Index<Depth;cou_Index++)
               {
                    cout << " Cell[" << cou_Index << "]->" << Cells[cou_Index]; cout.flush();
                    delete Cells[cou_Index];
                    Cells[cou_Index] = NULL;
               }
               cout << " delete [] Cells->" << Cells; cout.flush();
               delete [] Cells;
               Cells = NULL;
          }
          
          //cout << " delete [] Cells->" << Cells; cout.flush();
          //delete [] Cells;
          //Cells = NULL;
          
          Depth = 0;
          cout << " Done(" << this << ")";
     }   
     
     //Outputs the row as characters.
     void output_C()
     {
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               cout << " {";
               oint(0, 12, cou_Index);
               cout << "} ";
               Cells[cou_Index]->output_C();
          }
     }
     
     //Outputs the row as integer.
     void output_I()
     {
          cout << " {";
          oint(0, 13, Depth);
          cout << "} ";
          
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               cout << " {";
               oint(0, 12, cou_Index);
               cout << "} ";
               Cells[cou_Index]->output_I();
          }
     }
     
     //Outputs the row as integer.
     void output_U()
     {
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               cout << " {";
               oint(0, 12, cou_Index);
               cout << "} ";
               Cells[cou_Index]->output_U();
          }
     }
     
     //Outputs the row as *void.
     void output_NR()
     {
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               cout << " {";
               oint(0, 12, cou_Index);
               cout << "} ";
               Cells[cou_Index]->output_NR();
          }
     }
     
     //Outputs the row as float.
     void output_F()
     {
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               cout << " {";
               oint(0, 12, cou_Index);
               cout << "} ";
               Cells[cou_Index]->output_F();
          }
     }
     
     
     
     
     
     //==--     SAVES TO GIVEN FILE
        
     /*
     //Outputs the row as characters.
     void save_C(ofstream * p_File)
     {
          *p_File << " " << Depth;
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               *p_File << " .. " << cou_Index << " ";
               Cells[cou_Index]->save_C(p_File);
          }
     }
     
     //Outputs the row as integer.
     void save_I(ofstream * p_File)
     {
          *p_File << " " << Depth;
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               *p_File << " .. " << cou_Index << " ";
               Cells[cou_Index]->save_I(p_File);
          }
     }
     
     //Outputs the row as *void.
     void save_NR(ofstream * p_File)
     {
          *p_File << " " << Depth;
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               *p_File << " .. " << cou_Index << " ";
               Cells[cou_Index]->save_NR(p_File);
          }
     }
     
     //Outputs the row as float.
     void save_F(ofstream * p_File)
     {
          *p_File << " " << Depth;
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               *p_File << " .. " << cou_Index << " ";
               Cells[cou_Index]->save_F(p_File);
          }
     }
      * */
};

//The class for the 1D raw table.
class c_Raw_Table_2D
{
public:
     
     //The rows of data in the table.
     c_Raw_Table_Row_2D ** Rows;
     
     //The number of rows in the table.
     int Number_Of_Rows;
     
     //Whether or not a row is hotlinked
     bool * Hotlinked;
     
     //The name of the table.
     string Name;
     
     c_Raw_Table_2D()
     {
          Rows = NULL;
          Hotlinked = NULL;
          Number_Of_Rows = 0;
          Name = "Table_1D";
     }
     
     ~c_Raw_Table_2D()
     {
          
          cout << "\n    ~c_Raw_Table_2D " << Name << " " << Number_Of_Rows << " "; cout.flush();
          reset_End();
          cout << "\n    ~~~c_Raw_Table_2D " << Name << " " << Number_Of_Rows << " "; cout.flush();
     }
     
     //====----
     //==---     CREATES A NEW ROW AND SUBMITS THE SUBMISSION TO THE FIRST CELL
     //====----
          
     //Makes a submission to the table adding a new row.
     void submit_String(string p_Submission)
     {
          //Make room for the new submission.
          resize(Number_Of_Rows + 1);
          
          //Submit it.
          Rows[Number_Of_Rows - 1]->submit_String(p_Submission);
     }
     
     //Makes a submission to the table adding a new cell.
     void submit_Int(long long int p_Submission)
     {
          //Make room for the new submission.
          resize(Number_Of_Rows + 1);
          
          //Submit it.
          Rows[Number_Of_Rows - 1]->submit_Int(p_Submission);
     }
          
     //Makes a submission to the table adding a new cell.
     void submit_Float(double p_Submission)
     {
          //Make room for the new submission.
          resize(Number_Of_Rows + 1);
          
          //Submit it.
          Rows[Number_Of_Rows - 1]->submit_Float(p_Submission);
     }
     
     //Makes a submission to the table adding a new cell.
     void submit_NR(void * p_Submission)
     {
          //Make room for the new submission.
          resize(Number_Of_Rows + 1);
          
          //Submit it.
          Rows[Number_Of_Rows - 1]->submit_NR(p_Submission);
     }
     
     
     
     //====----
     //==---     ADDS SUBMISSION TO NEW CELL IN CURRENT/GIVEN ROW
     //====----
          
     //Makes a submission to the table adding a new cell to the current row.
     void add_String(string p_Submission)
     {
          //If there are no rows then make the first one.
          if (Number_Of_Rows == 0){ resize(1); }
          
          //Submit it.
          Rows[Number_Of_Rows - 1]->submit_String(p_Submission);
     }
     
     //Makes a submission to the table adding a new cell to the current row.
     void add_Int(long long int p_Submission)
     {
          //If there are no rows then make the first one.
          if (Number_Of_Rows == 0){ resize(1); }
          
          //Submit it.
          Rows[Number_Of_Rows - 1]->submit_Int(p_Submission);
     }
     
     //Makes a submission to the table adding a new cell to the current row.
     void add_Float(double p_Submission)
     {
          //If there are no rows then make the first one.
          if (Number_Of_Rows == 0){ resize(1); }
          
          //Submit it.
          Rows[Number_Of_Rows - 1]->submit_Float(p_Submission);
     }
          
     //Makes a submission to the table adding a new cell to the current row.
     void add_NR(void * p_Submission)
     {
          //If there are no rows then make the first one.
          if (Number_Of_Rows == 0){ resize(1); }
          
          //Submit it.
          Rows[Number_Of_Rows - 1]->submit_NR(p_Submission);
     }
          
     //Makes a submission to the table adding a new cell to the given row.
     void add_String(int p_Row, string p_Submission)
     {
          resize(p_Row + 1);
          
          //Submit it.
          Rows[p_Row]->submit_String(p_Submission);
     }
     
     //Makes a submission to the table adding a new cell to the given row.
     void add_Int(int p_Row, long long int p_Submission)
     {
          resize(p_Row + 1);
          
          //Submit it.
          Rows[p_Row]->submit_Int(p_Submission);
     }
     
     //Makes a submission to the table adding a new cell to the given row.
     void add_Float(int p_Row, double p_Submission)
     {
          resize(p_Row + 1);
          
          //Submit it.
          Rows[p_Row]->submit_Float(p_Submission);
     }
     
     //Makes a submission to the table adding a new cell to the given row.
     void add_NR(int p_Row, void * p_Submission)
     {
          resize(p_Row + 1);
          
          //Submit it.
          Rows[p_Row]->submit_NR(p_Submission);
     }
     
     
     
     //====----
     //==---     SETS A GIVEN CELLS DATA TO THE GIVEN SUBMISSION
     //====----
          


	 //Sets a given cell to the given integer.
	 void set_Int(int p_Row, int p_Cell, int p_Index, long long int p_Submission, int p_X = 0, int p_Y = 0)
	 {
		 //If there are not enough rows then create them.
		 if (Number_Of_Rows <= p_Row) { resize(p_Row + 1); }

		 //Set the string.
		 Rows[p_Row]->set_Int(p_Cell, p_Submission, p_X, p_Y);
	 }

	 //Sets a given cell to the given integer.
	 void set_U_Int(int p_Row, int p_Cell, int p_Index, unsigned long long int p_Submission, int p_X = 0, int p_Y = 0)
	 {
		 //If there are not enough rows then create them.
		 if (Number_Of_Rows <= p_Row) { resize(p_Row + 1); }

		 //Set the string.
		 Rows[p_Row]->set_Int(p_Cell, p_Submission, p_X, p_Y);
	 }
          
     //Sets a given cell to the given float.
     void set_Float(int p_Row, int p_Cell, int p_Index, double p_Submission, int p_X = 0, int p_Y = 0)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_Float(p_Cell, p_Submission, p_X, p_Y);
     }
     
     //Sets a given cell to the given float.
     void set_NR(int p_Row, int p_Cell, int p_Index, void * p_Submission, int p_X = 0, int p_Y = 0)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_NR(p_Cell, p_Submission, p_X, p_Y);
     }
     
     //Sets a row to the given row.
     void set_Row(int p_Row, c_Raw_Table_Row_2D * p_Row_Reference)
     {
          resize(p_Row + 1);
          
          //**--ga_Table_Alloc--;
          if (!Hotlinked[p_Row]){ delete Rows[p_Row]; }
          Rows[p_Row] = NULL;
          
          Rows[p_Row] = p_Row_Reference;
          Hotlinked[p_Row] = 1;
     }
     
     
     //Takes an array of u_Data_3 to read into the cell.
     void set_IMG(int p_Row, int p_Cell, u_Data_3 ** p_Data, int p_Width, int p_Height, int p_X = 0, int p_Y = 0)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_IMG(p_Cell, p_Data, p_Width, p_Height, p_X, p_Y);
     }
     
     //Takes an array of u_Data_3 to read into the cell.
     void set_IMG_Pixel(int p_Row, int p_Cell, u_Data_3 p_Data, int p_X, int p_Y)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_IMG_Pixel(p_Cell, p_Data, p_X, p_Y);
     }
     
     
     //Takes a cell and copies the data.
     void set_IMG_Cell_Data(int p_Row, int p_Cell, c_Raw_Table_Cell_2D * p_Cell_Data)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_IMG_Cell_Data(p_Cell, p_Cell_Data);
     }
     
     //====----
     //==---      CREATES NEW CELLS AND ROWS
     //====----
     
     //Creates a new cell in the current row to submit data to, return the Cell ID.
     int new_Cell()
     {
          resize(1);
          Rows[Number_Of_Rows - 1]->resize(Rows[Number_Of_Rows - 1]->Depth + 1);
          
          //Return the new cells ID.
          return Rows[Number_Of_Rows - 1]->Depth - 1;
     }
     
     //Creates a new cell in the given row to submit data to.
     int new_Cell(int p_Row)
     {
          resize(p_Row + 1);
          Rows[p_Row]->resize(Rows[p_Row]->Depth + 1);
          
          //Return the new cells ID.
          return Rows[p_Row]->Depth - 1;
     }
     
     int new_Row()
     {
          //IF there are not enough rows then create them.
          resize(Number_Of_Rows + 1);
          
          //Return the new Rows ID.
          return Number_Of_Rows - 1;
     }
     
     void new_Row(int p_Row)
     {
          resize(p_Row + 1);
     }
          
     //Resets the table.
     void reset()
     {
          if (Number_Of_Rows != 0)
          {
               for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
               {
                    //**--if (Rows[cou_Index] != NULL){ ga_Table_Alloc--; }
                    if (Hotlinked[cou_Index]){ Rows[cou_Index] = NULL; continue; }
                    delete Rows[cou_Index];
                    Rows[cou_Index] = NULL;
               }
               //**--if (Rows != NULL){ ga_Table_Alloc--; }
               delete [] Rows;
               Rows = NULL;
          }
          Number_Of_Rows = 0;
     }
          
     //Resets the table.
     void reset_End()
     {
          cout << "\n    " << this << " Rows->" << Rows << "  ";
          if (Number_Of_Rows != 0)
          {
               cout << " (Number_Of_Rows->" << Number_Of_Rows << " != 0)"; cout.flush();
               for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
               {
                    cout << " delete Rows[" << cou_Index << "]->" << Rows[cou_Index]; cout.flush();
                    if (Hotlinked[cou_Index]){ Rows[cou_Index] = NULL; cout << "REEEEE"; continue; }
                    delete Rows[cou_Index];
                    
                    Rows[cou_Index] = NULL;
                    cout << " Roe"; cout.flush();
               }
               
               cout << " delete [] Rows->" << Rows; cout.flush();
               
               delete [] Rows;
               Rows = NULL;
          }
          Number_Of_Rows = 0;
          cout << " Done"; cout.flush();
     }
     
     //Resets the data in the rows, however, the row itself is not destroyed.
     void soft_Reset()
     {
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               if (Rows[cou_Index] == NULL){ continue; }
               Rows[cou_Index]->reset();
          }
     }
     
     //Resizes the table to hold submissions.
     void resize(int p_Size)
     {
          if (Number_Of_Rows >= p_Size){ return; }
          
          //Setup the tmp rows array.
          c_Raw_Table_Row_2D ** tmp_Rows;
          bool * tmp_Hotlinked;
          
          //**--ga_Table_Alloc++;
          tmp_Rows = new c_Raw_Table_Row_2D*[Number_Of_Rows];
          tmp_Hotlinked = new bool[Number_Of_Rows];
          
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               tmp_Hotlinked[cou_Index] = Hotlinked[cou_Index];
               tmp_Rows[cou_Index] = Rows[cou_Index];
               Rows[cou_Index] = NULL;
          }
          
          //**--if (Rows != NULL){ ga_Table_Alloc--; }
          delete [] Rows;
          Rows = NULL;
          delete Hotlinked;
          Hotlinked = NULL;
          
          //**--ga_Table_Alloc++;
          Rows = new c_Raw_Table_Row_2D*[p_Size];
          Hotlinked = new bool[p_Size];
          
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               Hotlinked[cou_Index] = tmp_Hotlinked[cou_Index];
               Rows[cou_Index] = tmp_Rows[cou_Index];
               tmp_Rows[cou_Index] = NULL;
          }
          
          //**--if (tmp_Rows != NULL){ ga_Table_Alloc--; }
          delete [] tmp_Rows;
          tmp_Rows = NULL;
          delete [] tmp_Hotlinked;
          tmp_Hotlinked = NULL;
          
          for (int cou_Index=Number_Of_Rows;cou_Index<p_Size;cou_Index++)
          {
               Rows[cou_Index] = NULL;
               //**--ga_Table_Alloc++;
               Rows[cou_Index] = new c_Raw_Table_Row_2D;
               Hotlinked[cou_Index] = 0;
          }
          
          Number_Of_Rows = p_Size;
     }
     
     
     
     //====----
     //==---      GETS DATA FROM THE TABLE
     //====----
     
     // !!! NO BOUNDS CHECKING, ASSUMED DATA REQUESTED ALREADY EXISTS!
     
     //Gets a data bit from the given row, cell, position.
     u_Data_3 get(int p_Row, int p_Cell, int p_X = 0, int p_Y = 0)
     {
          if (!bounds_C(p_Row, p_Cell)){ u_Data_3 tmp_Data; tmp_Data.U = 0; return tmp_Data; }
          if (Rows[p_Row]->Cells[p_Cell]->Width <= p_X){ u_Data_3 tmp_Data; tmp_Data.U = 0; return tmp_Data; }
          if (Rows[p_Row]->Cells[p_Cell]->Height <= p_Y){ u_Data_3 tmp_Data; tmp_Data.U = 0; return tmp_Data; }
          
          return Rows[p_Row]->Cells[p_Cell]->Data[p_X] [p_Y];
     }
     
     //Gets a data bit from the given row, cell, position.
     long long int get_I(int p_Row, int p_Cell, int p_X = 0, int p_Y = 0)
     {
          if (!bounds_C(p_Row, p_Cell)){ return 0; }
          if (Rows[p_Row]->Cells[p_Cell]->Width <= p_X){ return 0; }
          if (Rows[p_Row]->Cells[p_Cell]->Height <= p_Y){ return 0; }
          
          return (Rows[p_Row]->Cells[p_Cell]->Data[p_X] [p_Y]).U;
     }
     
     //Gets a data bit from the given row, cell, position.
     char get_C(int p_Row, int p_Cell, int p_X = 0, int p_Y = 0)
     {
          if (!bounds_C(p_Row, p_Cell)){ return char (0); }
          if (Rows[p_Row]->Cells[p_Cell]->Width <= p_X){ return char (0); }
          if (Rows[p_Row]->Cells[p_Cell]->Height <= p_Y){ return char (0); }
          
          return (Rows[p_Row]->Cells[p_Cell]->Data[p_X] [p_Y]).C;
     }
     
     //Gets a data bit from the given row, cell, position.
     double get_F(int p_Row, int p_Cell, int p_X = 0, int p_Y = 0)
     {
          if (!bounds_C(p_Row, p_Cell)){ return 0.0f; }
          if (Rows[p_Row]->Cells[p_Cell]->Width <= p_X){ return 0.0f; }
          if (Rows[p_Row]->Cells[p_Cell]->Height <= p_Y){ return 0.0f; }
          
          return (Rows[p_Row]->Cells[p_Cell]->Data[p_X] [p_Y]).F;
     }
     
     //Gets a data bit from the given row, cell, position.
     unsigned long long int get_U(int p_Row, int p_Cell, int p_X = 0, int p_Y = 0)
     {
          if (!bounds_C(p_Row, p_Cell)){ return 0.0f; }
          if (Rows[p_Row]->Cells[p_Cell]->Width <= p_X){ return 0.0f; }
          if (Rows[p_Row]->Cells[p_Cell]->Height <= p_Y){ return 0.0f; }
          
          return (Rows[p_Row]->Cells[p_Cell]->Data[p_X] [p_Y]).U;
     }
     
     //Gets a data bit from the given row, cell, position.
     void * get_NR(int p_Row, int p_Cell, int p_X = 0, int p_Y = 0)
     {
          if (!bounds_C(p_Row, p_Cell)){ return NULL; }
          if (Rows[p_Row]->Cells[p_Cell]->Width <= p_X){ return NULL; }
          if (Rows[p_Row]->Cells[p_Cell]->Height <= p_Y){ return NULL; }
          
          return (Rows[p_Row]->Cells[p_Cell]->Data[p_X] [p_Y]).NR;
     }
     
     
     //Gets a cells reference.
     c_Raw_Table_Cell_2D * get_Cell_Ref(int p_Row, int p_Cell)
     {
          if (!bounds_C(p_Row, p_Cell)){ return NULL; }
          
          return Rows[p_Row]->Cells[p_Cell];
     }
     
     //Gets a row reference.
     c_Raw_Table_Row_2D * get_Row_Ref(int p_Row)
     {
          if (!bounds_R(p_Row)){ return NULL; }
          
          return Rows[p_Row];
     }
     
     //Gets a cells reference.
     c_Raw_Table_Cell_2D * get_Cell_Ref_Hard(int p_Row, int p_Cell)
     {
          if (!bounds_R(p_Row)){ resize(p_Row + 1); }
          if (!bounds_C(p_Row, p_Cell)){ Rows[p_Row]->resize(p_Cell + 1); }
          
          return Rows[p_Row]->Cells[p_Cell];
     }
     
     //Gets a row reference.
     c_Raw_Table_Row_2D * get_Row_Ref_Hard(int p_Row)
     {
          if (!bounds_R(p_Row)){ resize(p_Row + 1); }
          
          return Rows[p_Row];
     }
     
     //Bounds checking.
     bool bounds_R(int p_Row)
     {
          if (Number_Of_Rows <= p_Row){ return 0; }
          
          return 1;
     }
     
     //Bounds for a cell
     bool bounds_C(int p_Row, int p_Cell)
     {
          if (!bounds_R(p_Row)){ return 0; }
          
          if (Rows[p_Row]->Depth <= p_Cell){ return 0; }
          
          return 1;
     }
     
     //====----
     //==---      OUTPUTS
     //====----
     
     
     void output_C(int p_Tab = 0)
     {
          string tmp_Tabl = "";
          for (int cou_Index=0;cou_Index<p_Tab;cou_Index++){ tmp_Tabl += "\t"; }
          
          cout << "\n\n" << tmp_Tabl << " --== ";
          ostr(0, 8, Name);
          cout << " ==--";
          
          cout << "\n" << tmp_Tabl << "  Number_Of_Rows->";
          oint(0, 7, Number_Of_Rows);
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               cout << "\n" << tmp_Tabl << "  --[";
               oint(0, 15, cou_Index);
               cout << "]";
               Rows[cou_Index]->output_C();
          }
     }
     
     void output_I(int p_Tab = 0)
     {
          string tmp_Tabl = "";
          for (int cou_Index=0;cou_Index<p_Tab;cou_Index++){ tmp_Tabl += "\t"; }
          
          cout << "\n\n" << tmp_Tabl << " --== ";
          ostr(0, 8, Name);
          cout << " ==--";
          
          cout << "\n" << tmp_Tabl << "  Number_Of_Rows->";
          oint(0, 7, Number_Of_Rows);
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               cout << "\n" << tmp_Tabl << "  --[";
               oint(0, 15, cou_Index);
               cout << "]";
               Rows[cou_Index]->output_I();
          }
     }
     
     void output_U(int p_Tab = 0)
     {
          string tmp_Tabl = "";
          for (int cou_Index=0;cou_Index<p_Tab;cou_Index++){ tmp_Tabl += "\t"; }
          
          cout << "\n\n" << tmp_Tabl << " --== ";
          ostr(0, 8, Name);
          cout << " ==--";
          
          cout << "\n" << tmp_Tabl << "  Number_Of_Rows->";
          oint(0, 7, Number_Of_Rows);
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               cout << "\n" << tmp_Tabl << "  --[";
               oint(0, 15, cou_Index);
               cout << "]";
               Rows[cou_Index]->output_U();
          }
     }
     
     void output_NR(int p_Tab = 0)
     {
          string tmp_Tabl = "";
          for (int cou_Index=0;cou_Index<p_Tab;cou_Index++){ tmp_Tabl += "\t"; }
          
          cout << "\n\n" << tmp_Tabl << " --== ";
          ostr(0, 8, Name);
          cout << " ==--";
          
          cout << "\n" << tmp_Tabl << "  Number_Of_Rows->";
          oint(0, 7, Number_Of_Rows);
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               cout << "\n" << tmp_Tabl << "  --[";
               oint(0, 15, cou_Index);
               cout << "]";
               Rows[cou_Index]->output_NR();
          }
     }
     
     void output_F(int p_Tab = 0)
     {
          string tmp_Tabl = "";
          for (int cou_Index=0;cou_Index<p_Tab;cou_Index++){ tmp_Tabl += "\t"; }
          
          cout << "\n\n" << tmp_Tabl << " --== ";
          ostr(0, 8, Name);
          cout << " ==--";
          
          cout << "\n" << tmp_Tabl << "  Number_Of_Rows->";
          oint(0, 7, Number_Of_Rows);
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               cout << "\n" << tmp_Tabl << "  --[";
               oint(0, 15, cou_Index);
               cout << "]";
               Rows[cou_Index]->output_F();
          }
     }
     
     
     
     
     
     
     /*
     //====----
     //==---      SAVES
     //====----
     
     
     void save_C(ofstream * SF)
     {
          *SF << "\n\nR " << Number_Of_Rows;
          
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               *SF << "\n" << cou_Index << " ";
               Rows[cou_Index]->save_C(SF);
          }
     }
     
     void save_I(ofstream * SF)
     {
          *SF << "\n\nR " << Number_Of_Rows;
          
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               *SF << "\n" << cou_Index << " ";
               Rows[cou_Index]->save_I(SF);
          }
     }
     
     void save_NR(ofstream * SF)
     {
          *SF << "\n\nR " << Number_Of_Rows;
          
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               *SF << "\n" << cou_Index << " ";
               Rows[cou_Index]->save_NR(SF);
          }
     }
     
     void save_F(ofstream * SF)
     {
          *SF << "\n\nR " << Number_Of_Rows;
          
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               *SF << "\n" << cou_Index << " ";
               Rows[cou_Index]->save_F(SF);
          }
     }
     
     
     
     
     void save_C(string p_File, string p_DIR = "NO_DIR")
     {
          ofstream SF;
          string tmp_File;
          if (p_DIR != "NO_DIR"){ tmp_File = p_DIR + "\\" + p_File; } else { tmp_File = p_File; }
          
          SF.open(tmp_File);
          if (!SF.is_open()){ cout << "\n FILE " << tmp_File << " COULD NOT BE OPENED!"; return; }
          
          save_C(&SF);
          
          SF.close();
     }
     
     void save_I(string p_File, string p_DIR = "NO_DIR")
     {
          ofstream SF;
          string tmp_File;
          if (p_DIR != "NO_DIR"){ tmp_File = p_DIR + "\\" + p_File; } else { tmp_File = p_File; }
          
          SF.open(tmp_File);
          if (!SF.is_open()){ cout << "\n FILE " << tmp_File << " COULD NOT BE OPENED!"; return; }
          
          save_I(&SF);
          
          SF.close();
     }
     
     void save_NR(string p_File, string p_DIR = "NO_DIR")
     {
          ofstream SF;
          string tmp_File;
          if (p_DIR != "NO_DIR"){ tmp_File = p_DIR + "\\" + p_File; } else { tmp_File = p_File; }
          
          SF.open(tmp_File);
          if (!SF.is_open()){ cout << "\n FILE " << tmp_File << " COULD NOT BE OPENED!"; return; }
          
          save_NR(&SF);
          
          SF.close();
     }
     
     void save_F(string p_File, string p_DIR = "NO_DIR")
     {
          ofstream SF;
          string tmp_File;
          if (p_DIR != "NO_DIR"){ tmp_File = p_DIR + "\\" + p_File; } else { tmp_File = p_File; }
          
          SF.open(tmp_File);
          if (!SF.is_open()){ cout << "\n FILE " << tmp_File << " COULD NOT BE OPENED!"; return; }
          
          save_F(&SF);
          
          SF.close();
     }
     */
};

