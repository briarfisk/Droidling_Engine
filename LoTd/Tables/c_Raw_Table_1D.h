
//The class encapsulating the union to ini it.
class c_Data_3
{
public:

     //The data.
     u_Data_3 D;
     
     c_Data_3()
     {
          D.I = 0;
     }
};

//The raw table cell class for the 1d table.
class c_Raw_Table_Cell_1D
{
public:
     
     //The data held in the cell.
     u_Data_3 * Data;
     
     //The length of the current cell.
     int Length;
     
     //The max length of the data in the current cell.
     int MAX_Length;
     
     //The Increment which to increase the max length by.
     int MAX_Length_I;
     
     c_Raw_Table_Cell_1D()
     {
          Data = NULL;
          Length = 0;
          MAX_Length = 0;
          MAX_Length_I = 10;
     }
     
     ~c_Raw_Table_Cell_1D()
     {
          //**--if (Data != NULL){ ga_Cell_Alloc--; }
          delete [] Data;
          Data = NULL;
     }
     
     //Initializes the cells data.
     void initialize()
     {
          Data = NULL;
          Length = 0;
          MAX_Length = 0;
          MAX_Length_I = 10;
     }
     
     //Resets the cell.
     void reset()
     {
          delete [] Data;
          Data = NULL;
          Length = 0;
          MAX_Length = 0;
          MAX_Length_I = 10;
     }
     
     //====----
     //==---     ACCEPTS A SUBMISSION AND SETS THE CELLS DATA ACCORDINGLY
     //====----
          
     //Takes a submission.
     void submit_String(string p_Submission)
     {
          resize(p_Submission.size());
          
          for (unsigned int cou_C=0;cou_C<p_Submission.size();cou_C++)
          {
               Data[cou_C].C = p_Submission[cou_C];
          }
     }
          
     //Takes a submission.
     void submit_UData_String(u_Data_3 * p_Submission, int p_Size)
     {
          resize(p_Size);
          
          for (int cou_C=0;cou_C<p_Size;cou_C++)
          {
               Data[cou_C] = p_Submission[cou_C];
          }
     }
     
     //Takes a submission.
     void submit_Int(long long int p_Submission)
     {
          resize(1);
          
          Data[0].I = p_Submission;
     }
     
     //Takes a submission.
     void submit_UInt(unsigned long long int p_Submission)
     {
          resize(1);
          
          Data[0].U = p_Submission;
     }
     
     //Takes a submission.
     void submit_Float(double p_Submission)
     {
          resize(1);
          
          Data[0].F = p_Submission;
     }
          
     //Takes a submission.
     void submit_NR(void * p_Submission)
     {
          resize(1);
          
          Data[0].NR = p_Submission;
     }
     
     
     
     //Takes a submission.
     void submit_Int(int p_Index, long long int p_Submission)
     {
          soft_resize(p_Index + 1);
          
          Data[p_Index].I = p_Submission;
     }
     
     //Takes a submission.
     void submit_UInt(int p_Index, unsigned long long int p_Submission)
     {
          soft_resize(p_Index + 1);
          
          Data[p_Index].U = p_Submission;
     }
     
     //Takes a submission.
     void submit_Float(int p_Index, double p_Submission)
     {
          soft_resize(p_Index + 1);
          
          Data[p_Index].F = p_Submission;
     }

	 //Takes a submission.
	 void submit_NR(int p_Index, void* p_Submission)
	 {
		 soft_resize(p_Index + 1);

		 Data[p_Index].NR = p_Submission;
	 }

	 //Takes a submission.
	 void submit_U_Data_3(int p_Index, u_Data_3 p_Submission)
	 {
		 soft_resize(p_Index + 1);

		 Data[p_Index] = p_Submission;
	 }
     
     
     
     //====----
     //==---     ADDS A BIT OF DATA TO CELLS DATA ARRAY
     //====----
     
     //Adds a bit to the current set.
     void add_Data(u_Data_3 p_Submission)
     {
          soft_resize(Length + 1);
          
          Data[Length - 1] = p_Submission;
     }
     
     //Adds a bit to the current set.
     void add_Data_Int(long long int p_Submission)
     {
          soft_resize(Length + 1);
          
          Data[Length - 1].I = p_Submission;
     }
          
     //Adds a bit to the current set.
     void add_Data_Float(double p_Submission)
     {
          soft_resize(Length + 1);
          
          Data[Length - 1].F = p_Submission;
     }
     
     //Adds a bit to the current set.
     void add_Data_NR(void * p_Submission)
     {
          soft_resize(Length + 1);
          
          Data[Length - 1].NR = p_Submission;
     }
     
     
     
     
     
     
     
     //Gets teh contents of the cell as a string.
     string get_String()
     {
          string tmp_Out = "";
          
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               if (Data[cou_Index].I == 0){ tmp_Out += " "; continue; }
               tmp_Out += Data[cou_Index].C;
          }
          
          return tmp_Out;
     }
     
     //Resizes the row to hold a submission.
     void resize(int p_Size)
     {
          if (p_Size <= Length){ return; }
          
          if (p_Size < MAX_Length)
          {
               for (int cou_Index=0;cou_Index<Length;cou_Index++)
               {
                    Data[cou_Index].U = 0;
               }
               Length = p_Size;
               return;
          }
          
          //**--if (Data != NULL){ ga_Cell_Alloc--; }
          delete [] Data;
          Data = NULL;
          //**--ga_Cell_Alloc++;
          int tmp_MAX_Length = p_Size + MAX_Length_I;
          Data = new u_Data_3[tmp_MAX_Length];
          for (int cou_Index=0;cou_Index<tmp_MAX_Length;cou_Index++)
          {
               Data[cou_Index].U = 0;
          }
          Length = p_Size;
          MAX_Length = tmp_MAX_Length;
     }
     
     //Resizes the row to hold a submission.
     void soft_resize(int p_Size)
     {
          if (p_Size <= Length){ return; }
          
          if (p_Size < MAX_Length)
          {
               for (int cou_Index=Length;cou_Index<p_Size;cou_Index++)
               {
                    Data[cou_Index].U = 0;
               }
               Length = p_Size;
               return;
          }
          
          u_Data_3 * tmp_Data;
          tmp_Data = new u_Data_3[Length];
          
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               tmp_Data[cou_Index] = Data[cou_Index];
          }
          
          //**--if (Data != NULL){ ga_Cell_Alloc--; }
          delete [] Data;
          Data = NULL;
          //**--ga_Cell_Alloc++;
          int tmp_MAX_Length = p_Size + MAX_Length_I;
          Data = new u_Data_3[tmp_MAX_Length];
          
          for (int cou_Index=0;cou_Index<tmp_MAX_Length;cou_Index++)
          {
               Data[cou_Index].U = 0;
          }
          
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               Data[cou_Index] = tmp_Data[cou_Index];
          }
          
          delete [] tmp_Data;
          tmp_Data = NULL;
          
          Length = p_Size;
          MAX_Length = tmp_MAX_Length;
     }
     
     //Outputs the cell as characters.
     void output_C()
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               char tmp_State = char (Data[cou_Index].I);
               if (tmp_State == char (0)){ tmp_State = '?'; }
               if (tmp_State == char (7)){ tmp_State = '?'; }
               if (tmp_State == char (8)){ tmp_State = '?'; }
               if (tmp_State == char (9)){ tmp_State = '?'; }
               if (tmp_State == char (10)){ tmp_State = '?'; }
               if (tmp_State == char (13)){ tmp_State = '?'; }
               cout << tmp_State;
          }
     }
     
     //Outputs the cell as integer.
     void output_I()
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               ostr(0, 1, " ");
               oint64(0, 8, Data[cou_Index].I);
          }
     }
     
     //Outputs the cell as integer.
     void output_U()
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               ostr(0, 1, " ");
               oull(0, 8, Data[cou_Index].U);
          }
     }
     
     //Outputs the cell as *void.
     void output_NR()
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               ostr(0, 1, " ");
               opoi(0, 8, Data[cou_Index].NR);
          }
     }
     
     //Outputs the cell as float.
     void output_F()
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               ostr(0, 1, " ");
               odbl(0, 8, Data[cou_Index].F);
          }
     }
     
     
     
     
     
     //==--     SAVING TO FILE
     
     void save_C(ofstream * p_File)
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               if (Data[cou_Index].I == 0){ *p_File << char(254); if ((cou_Index + 1) < Length){ *p_File << " "; } continue; }
               *p_File << Data[cou_Index].C;
               if ((cou_Index + 1) < Length){ *p_File << " "; }
          }
     }
     
     void save_I(ofstream * p_File)
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               *p_File << Data[cou_Index].I;
               if ((cou_Index + 1) < Length){ *p_File << " "; }
          }
     }
     
     void save_F(ofstream * p_File)
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               *p_File << Data[cou_Index].F;
               if ((cou_Index + 1) < Length){ *p_File << " "; }
          }
     }
     
     void save_NR(ofstream * p_File)
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               *p_File << Data[cou_Index].NR;
               if ((cou_Index + 1) < Length){ *p_File << " "; }
          }
     }
};


//The raw table row class for the 1d table.
class c_Raw_Table_Row_1D
{
public:
     
     //The data held in the row.
     c_Raw_Table_Cell_1D ** Cells;
     
     //The depth of the current row.
     int Depth;
     
     //The max depth.
     int MAX_Depth;
     
     //The number to increase the MAX_Depth by whenever enlarging.
     int MAX_Depth_Inc;
     
     c_Raw_Table_Row_1D()
     {
          Cells = NULL;
          MAX_Depth = 0;
          MAX_Depth_Inc = 1000;
          Depth = 0;
     }
     
     ~c_Raw_Table_Row_1D()
     {
          reset();
     }
     
     //====----
     //==---     SETS A GIVEN CELLS DATA TO THE GIVEN SUBMISSION
     //====----
     
     //Takes a submission.
     void submit_String(string p_Submission)
     {
          //ostr(0, 12, "\n Depth_B:"); cout << Depth;
          resize(Depth + 1);
          Cells[Depth - 1]->submit_String(p_Submission);
          //ostr(0, 12, "\n Depth_A:"); cout << Depth;
     }
     
     //Takes a submission.
     void submit_UData_String(u_Data_3 * p_Submission, int p_Size)
     {
          //ostr(0, 12, "\n Depth_B:"); cout << Depth;
          resize(Depth + 1);
          Cells[Depth - 1]->submit_UData_String(p_Submission, p_Size);
          //ostr(0, 12, "\n Depth_A:"); cout << Depth;
     }
     
     //Takes a submission.
     void submit_Int(long long int p_Submission)
     {
          resize(Depth + 1);
          Cells[Depth - 1]->submit_Int(p_Submission);
     }
     
     //Takes a submission to a given cell.
     void submit_UInt(unsigned long long int p_Submission)
     {
          resize(Depth + 1);
          Cells[Depth - 1]->submit_UInt(p_Submission);
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
     void set_String(int p_Cell, string p_Submission)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->submit_String(p_Submission);
     }
     
     //Takes a submission to a given cell.
     void set_Int(int p_Cell, long long int p_Submission)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->submit_Int(p_Submission);
     }
     
     //Takes a submission to a given cell.
     void set_UInt(int p_Cell, unsigned long long int p_Submission)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->submit_UInt(p_Submission);
     }
          
     //Takes a submission to a given cell.
     void set_Float(int p_Cell, double p_Submission)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->submit_Float(p_Submission);
     }

	 //Takes a submission to a given cell.
	 void set_NR(int p_Cell, void* p_Submission)
	 {
		 resize(p_Cell + 1);
		 Cells[p_Cell]->submit_NR(p_Submission);
	 }


	 //Takes a submission to a given cell.
	 void set_U_Data_3(int p_Cell, u_Data_3 p_Submission)
	 {
		 resize(p_Cell + 1);
		 Cells[p_Cell]->submit_UData_String(&p_Submission, 1);
	 }
     
     

	 //Takes a submission to a given cell.
	 void set_Int(int p_Cell, int p_Index, long long int p_Submission)
	 {
		 resize(p_Cell + 1);
		 Cells[p_Cell]->submit_Int(p_Index, p_Submission);
	 }

	 //Takes a submission to a given cell.
	 void set_U_Data_3(int p_Cell, int p_Index, u_Data_3 p_Submission)
	 {
		 resize(p_Cell + 1);
		 Cells[p_Cell]->submit_U_Data_3(p_Index, p_Submission);
	 }
     
     //Takes a submission to a given cell.
     void set_UInt(int p_Cell, int p_Index, unsigned long long int p_Submission)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->submit_UInt(p_Index, p_Submission);
     }
          
     //Takes a submission to a given cell.
     void set_Float(int p_Cell, int p_Index, double p_Submission)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->submit_Float(p_Index, p_Submission);
     }
          
     //Takes a submission to a given cell.
     void set_NR(int p_Cell, int p_Index, void * p_Submission)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->submit_NR(p_Index, p_Submission);
     }
     
     
     
     //====----
     //==---     ADDS A BIT OF DATA TO THE GIVEN/CURRENT CELL
     //====----
     
     //Adds a bit of data onto the current cell.
     void add_Data(u_Data_3 p_Data)
     {
          if (Depth == 0){ resize(1); }
          Cells[Depth - 1]->add_Data(p_Data);
     }
     
     //Adds a bit of data to the current cell in the current row.
     void add_Data_Int(long long int p_Data)
     {
          if (Depth == 0){ resize(1); }
          Cells[Depth - 1]->add_Data_Int(p_Data);
     }
          
     //Adds a bit of data to the current cell in the current row.
     void add_Data_Float(double p_Data)
     {
          if (Depth == 0){ resize(1); }
          Cells[Depth - 1]->add_Data_Float(p_Data);
     }
          
     //Adds a bit of data to the current cell in the current row.
     void add_Data_NR(void * p_Data)
     {
          if (Depth == 0){ resize(1); }
          Cells[Depth - 1]->add_Data_NR(p_Data);
     }
     
     //Adds a bit of data onto the current cell.
     void add_Data(int p_Cell, u_Data_3 p_Data)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->add_Data(p_Data);
     }
     
     //Adds a bit of data to the current cell in the current row.
     void add_Data_Int(int p_Cell, long long int p_Data)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->add_Data_Int(p_Data);
     }
          
     //Adds a bit of data to the current cell in the current row.
     void add_Data_Float(int p_Cell, double p_Data)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->add_Data_Float(p_Data);
     }
          
     //Adds a bit of data to the current cell in the current row.
     void add_Data_NR(int p_Cell, void * p_Data)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->add_Data_NR(p_Data);
     }
     
     //==--     RESIZE AND RESET
     
     
     //Flattens all the cells in the row into one.
     void flatten_Row()
     {
          c_Raw_Table_Cell_1D * tmp_Cell = new c_Raw_Table_Cell_1D;
          
          //Read each cell in the row into the first cell.
          for (int cou_Cell=0;cou_Cell<Depth;cou_Cell++)
          {
               for (int cou_Data=0;cou_Data<Cells[cou_Cell]->Length;cou_Data++)
               {
                    tmp_Cell->add_Data(Cells[cou_Cell]->Data[cou_Data]);
               }
          }
          
          resize_With_Reset(1);
          
          delete Cells[0];
          Cells[0] = NULL;
          Cells[0] = tmp_Cell;
     }
     
     
     //==--     RESIZE AND RESET
     
     
     //Resizes the row to hold a new submission.
     void resize_With_Reset(int p_Size)
     {
          reset();
          
          int tmp_MAX_Depth = p_Size + MAX_Depth_Inc;
          
          
          //**--if (Cells != NULL){ ga_Row_Alloc--; }
          delete [] Cells;
          Cells = NULL;
          //**--ga_Row_Alloc++;
          Cells = new c_Raw_Table_Cell_1D*[tmp_MAX_Depth];
          
          for (int cou_Index=0;cou_Index<tmp_MAX_Depth;cou_Index++)
          {
               //**--ga_Row_Alloc++;
               Cells[cou_Index] = new c_Raw_Table_Cell_1D;
          }
          
          Depth = p_Size;
          MAX_Depth = tmp_MAX_Depth;
          
          //cout << " ND->" << Depth << " NMX->" << MAX_Depth << " MX_I->" << tmp_MAX_Depth;
     }
     
     //Resizes the row to hold a new submission.
     void resize(int p_Size)
     {
          //cout << "\n resize(" << p_Size << ") D->" << Depth << " MX->" << MAX_Depth; cout.flush();
          
          if (p_Size <= Depth){ return; }
          
          //When it hits (MAX_Depth - 1) it resizes the array.
          if (p_Size < MAX_Depth){ Depth = p_Size; return; }
          
          
          //Setup the tmp and transfer the old cells to it while reallocating the row and transfering the old cells back before destroying the tmp.
          c_Raw_Table_Cell_1D ** tmp_Row;
          //**--ga_Row_Alloc++;
          tmp_Row = new c_Raw_Table_Cell_1D*[MAX_Depth];
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
          Cells = new c_Raw_Table_Cell_1D*[tmp_MAX_Depth];
          
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
               Cells[cou_Index] = new c_Raw_Table_Cell_1D;
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
     
     
     //==--     OUTPUTS THE DATA IN THE ROW
     
     
     //Outputs the row as characters.
     void output_C()
     {
          cout << " {";
          if (Depth != 0){ oint(0, 13, Depth); }
          cout << "} ";
          
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
          if (Depth != 0){ oint(0, 13, Depth); }
          cout << "} ";
          
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               cout << " {";
               oint(0, 12, cou_Index);
               cout << "}";
               Cells[cou_Index]->output_I();
          }
     }
     
     //Outputs the row as integer.
     void output_U()
     {
          cout << " {";
          if (Depth != 0){ oint(0, 13, Depth); }
          cout << "} ";
          
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               cout << " {";
               oint(0, 12, cou_Index);
               cout << "}";
               Cells[cou_Index]->output_U();
          }
     }
     
     //Outputs the row as *void.
     void output_NR()
     {
          cout << " {";
          if (Depth != 0){ oint(0, 13, Depth); }
          cout << "} ";
          
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               cout << " {";
               oint(0, 12, cou_Index);
               cout << "}";
               Cells[cou_Index]->output_NR();
          }
     }
     
     //Outputs the row as float.
     void output_F()
     {
          cout << " {";
          if (Depth != 0){ oint(0, 13, Depth); }
          cout << "} ";
          
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               cout << " {";
               oint(0, 12, cou_Index);
               cout << "}";
               Cells[cou_Index]->output_F();
          }
     }
     
     
     
     
     
     //==--     SAVES TO GIVEN FILE
        
     
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
};

//The class for the 1D raw table.
class c_Raw_Table_1D
{
public:
     
     //The rows of data in the table.
     c_Raw_Table_Row_1D ** Rows;
     
     //The number of rows in the table.
     int Number_Of_Rows;
     
     //Whether or not a row is hotlinked
     bool * Hotlinked;
     
     //The name of the table.
     string Name;
     
     c_Raw_Table_1D()
     {
          Rows = NULL;
          Hotlinked = NULL;
          Number_Of_Rows = 0;
          Name = "Table_1D";
     }
     
     ~c_Raw_Table_1D()
     {
          
          cout << "\n    ~c_Raw_Table_1D " << Name << " " << Number_Of_Rows << " "; cout.flush();
          reset_End();
          cout << "\n    ~~~c_Raw_Table_1D " << Name << " " << Number_Of_Rows << " "; cout.flush();
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
          
     //Sets a given cell cell to the given string.
     void set_String(int p_Row, int p_Cell, string p_Submission)
     {
          //IF there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_String(p_Cell, p_Submission);
     }
     
     //Sets a given cell to the given integer.
     void set_Int(int p_Row, int p_Cell, long long int p_Submission)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_Int(p_Cell, p_Submission);
     }
     
     //Sets a given cell to the given integer.
     void set_UInt(int p_Row, int p_Cell, unsigned long long int p_Submission)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_UInt(p_Cell, p_Submission);
     }
     
     //Sets a given cell to the given float.
     void set_Float(int p_Row, int p_Cell, double p_Submission)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_Float(p_Cell, p_Submission);
     }
     
     //Sets a given cell to the given float.
     void set_NR(int p_Row, int p_Cell, void * p_Submission)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_NR(p_Cell, p_Submission);
     }
     
     
     //Sets a given cell to the given float.
     void set_U_Data_3(int p_Row, int p_Cell, u_Data_3 p_Submission)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_U_Data_3(p_Cell, p_Submission);
     }
     
     
     
     //Sets a given cell to the given integer.
     void set_Int(int p_Row, int p_Cell, int p_Index, long long int p_Submission)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_Int(p_Cell, p_Index, p_Submission);
     }
     
     //Sets a given cell to the given integer.
     void set_UInt(int p_Row, int p_Cell, int p_Index, unsigned long long int p_Submission)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_UInt(p_Cell, p_Index, p_Submission);
     }
          
     //Sets a given cell to the given float.
     void set_Float(int p_Row, int p_Cell, int p_Index, double p_Submission)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_Float(p_Cell, p_Index, p_Submission);
     }

	 //Sets a given cell to the given float.
	 void set_NR(int p_Row, int p_Cell, int p_Index, void* p_Submission)
	 {
		 //If there are not enough rows then create them.
		 if (Number_Of_Rows <= p_Row) { resize(p_Row + 1); }

		 //Set the string.
		 Rows[p_Row]->set_NR(p_Cell, p_Index, p_Submission);
	 }

	 //Sets a given cell to the given float.
	 void set_U_Data_3(int p_Row, int p_Cell, int p_Index, u_Data_3 p_Submission)
	 {
		 //If there are not enough rows then create them.
		 if (Number_Of_Rows <= p_Row) { resize(p_Row + 1); }

		 //Set the string.
		 Rows[p_Row]->set_U_Data_3(p_Cell, p_Index, p_Submission);
	 }
     
     //Sets a row to the given row.
     void set_Row(int p_Row, c_Raw_Table_Row_1D * p_Row_Reference)
     {
          resize(p_Row + 1);
          
          //**--ga_Table_Alloc--;
          if (!Hotlinked[p_Row]){ delete Rows[p_Row]; }
          Rows[p_Row] = NULL;
          
          Rows[p_Row] = p_Row_Reference;
          Hotlinked[p_Row] = 1;
     }
     
     //====----
     //==---     ADDS A BIT OF DATA TO THE GIVEN/CURRENT CELL IN THE CURRENT/GIVEN ROW
     //====----
     
     //Adds a bit of data to the current cell in the current row.
     void add_Data(u_Data_3 p_Data)
     {
          if (Number_Of_Rows == 0){ resize(1); }
          Rows[Number_Of_Rows - 1]->add_Data(p_Data);
     }
     
     //Adds a bit of data to the current cell in the current row.
     void add_Data_Int(long long int p_Data)
     {
          if (Number_Of_Rows == 0){ resize(1); }
          Rows[Number_Of_Rows - 1]->add_Data_Int(p_Data);
     }
          
     //Adds a bit of data to the current cell in the current row.
     void add_Data_Float(double p_Data)
     {
          if (Number_Of_Rows == 0){ resize(1); }
          Rows[Number_Of_Rows - 1]->add_Data_Float(p_Data);
     }
          
     //Adds a bit of data to the current cell in the current row.
     void add_Data_NR(void * p_Data)
     {
          if (Number_Of_Rows == 0){ resize(1); }
          Rows[Number_Of_Rows - 1]->add_Data_NR(p_Data);
     }
     
     //Adds a bit of data to the current cell in the given row.
     void add_Data(int p_Row, u_Data_3 p_Data)
     {
          resize(p_Row + 1);
          Rows[p_Row]->add_Data(p_Data);
     }
     
     //Adds a bit of data to the current cell in the given row.
     void add_Data_Int(int p_Row, long long int p_Data)
     {
          resize(p_Row + 1);
          Rows[p_Row]->add_Data_Int(p_Data);
     }
          
     //Adds a bit of data to the current cell in the given row.
     void add_Data_Float(int p_Row, double p_Data)
     {
          resize(p_Row + 1);
          Rows[p_Row]->add_Data_Float(p_Data);
     }
     
     //Adds a bit of data to the current cell in the given row.
     void add_Data_NR(int p_Row, void * p_Data)
     {
          resize(p_Row + 1);
          Rows[p_Row]->add_Data_NR(p_Data);
     }
     
     //Adds a bit of data to the given cell in the given row.
     void add_Data(int p_Row, int p_Cell, u_Data_3 p_Data)
     {
          resize(p_Row + 1);
          Rows[p_Row]->add_Data(p_Cell, p_Data);
     }
     
     //Adds a bit of data to the given cell in the given row.
     void add_Data_Int(int p_Row, int p_Cell, long long int p_Data)
     {
          resize(p_Row + 1);
          Rows[p_Row]->add_Data_Int(p_Cell, p_Data);
     }
          
     //Adds a bit of data to the given cell in the given row.
     void add_Data_Float(int p_Row, int p_Cell, double p_Data)
     {
          resize(p_Row + 1);
          Rows[p_Row]->add_Data_Float(p_Cell, p_Data);
     }
     
     //Adds a bit of data to the given cell in the given row.
     void add_Data_NR(int p_Row, int p_Cell, void * p_Data)
     {
          resize(p_Row + 1);
          Rows[p_Row]->add_Data_NR(p_Cell, p_Data);
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
          c_Raw_Table_Row_1D ** tmp_Rows;
          bool * tmp_Hotlinked;
          
          //**--ga_Table_Alloc++;
          tmp_Rows = new c_Raw_Table_Row_1D*[Number_Of_Rows];
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
          Rows = new c_Raw_Table_Row_1D*[p_Size];
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
               Rows[cou_Index] = new c_Raw_Table_Row_1D;
               Hotlinked[cou_Index] = 0;
          }
          
          Number_Of_Rows = p_Size;
     }
     
     
     
     //====----
     //==---      GETS DATA FROM THE TABLE
     //====----
     
     // !!! NO BOUNDS CHECKING, ASSUMED DATA REQUESTED ALREADY EXISTS!
     
     //Gets a data bit from the given row, cell, position.
     u_Data_3 get(int p_Row, int p_Cell, int p_Position)
     {
          if (!bounds_C(p_Row, p_Cell)){ u_Data_3 tmp_Data; tmp_Data.U = 0; return tmp_Data; }
          
          return Rows[p_Row]->Cells[p_Cell]->Data[p_Position];
     }
     
     //Gets a data bit from the given row, cell, position.
     long long int get_I(int p_Row, int p_Cell, int p_Position)
     {
          if (!bounds_C(p_Row, p_Cell)){ return 0; }
          if (Rows[p_Row]->Cells[p_Cell]->Length <= p_Position){ return 0; }
          
          return (Rows[p_Row]->Cells[p_Cell]->Data[p_Position]).U;
     }
     
     //Gets a data bit from the given row, cell, position.
     char get_C(int p_Row, int p_Cell, int p_Position)
     {
          if (!bounds_C(p_Row, p_Cell)){ return char (0); }
          if (Rows[p_Row]->Cells[p_Cell]->Length <= p_Position){ return char (0); }
          
          return (Rows[p_Row]->Cells[p_Cell]->Data[p_Position]).C;
     }
     
     //Gets a data bit from the given row, cell, position.
     double get_F(int p_Row, int p_Cell, int p_Position)
     {
          if (!bounds_C(p_Row, p_Cell)){ return 0.0f; }
          if (Rows[p_Row]->Cells[p_Cell]->Length <= p_Position){ return 0.0f; }
          
          return (Rows[p_Row]->Cells[p_Cell]->Data[p_Position]).F;
     }
     
     //Gets a data bit from the given row, cell, position.
     unsigned long long int get_U(int p_Row, int p_Cell, int p_Position)
     {
          if (!bounds_C(p_Row, p_Cell)){ return 0.0f; }
          if (Rows[p_Row]->Cells[p_Cell]->Length <= p_Position){ return 0.0f; }
          
          return (Rows[p_Row]->Cells[p_Cell]->Data[p_Position]).U;
     }
     
     //Gets a data bit from the given row, cell, position.
     void * get_NR(int p_Row, int p_Cell, int p_Position)
     {
          if (!bounds_C(p_Row, p_Cell)){ return NULL; }
          if (Rows[p_Row]->Cells[p_Cell]->Length <= p_Position){ return NULL; }
          
          return (Rows[p_Row]->Cells[p_Cell]->Data[p_Position]).NR;
     }
     
     
     //Gets a cells reference.
     c_Raw_Table_Cell_1D * get_Cell_Ref(int p_Row, int p_Cell)
     {
          if (!bounds_C(p_Row, p_Cell)){ return NULL; }
          
          return Rows[p_Row]->Cells[p_Cell];
     }
     
     //Gets a row reference.
     c_Raw_Table_Row_1D * get_Row_Ref(int p_Row)
     {
          if (!bounds_R(p_Row)){ return NULL; }
          
          return Rows[p_Row];
     }
     
     //Gets a cells reference.
     c_Raw_Table_Cell_1D * get_Cell_Ref_Hard(int p_Row, int p_Cell)
     {
          if (!bounds_R(p_Row)){ resize(p_Row + 1); }
          if (!bounds_C(p_Row, p_Cell)){ Rows[p_Row]->resize(p_Cell + 1); }
          
          return Rows[p_Row]->Cells[p_Cell];
     }
     
     //Gets a row reference.
     c_Raw_Table_Row_1D * get_Row_Ref_Hard(int p_Row)
     {
          if (!bounds_R(p_Row)){ resize(p_Row + 1); }
          Rows[p_Row]->reset();
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
};

