//The cell is the base unit for the table.

//The data types are; 0 = char, 1 = int, 2 = float, 3 = *void, 4 = bool
//These data types are mostly used for output.

class c_Cell_1D
{
protected:
     
     //The data in the cell. Is protected to avoid bounding errors.
     u_Data * Data;
     
     //The dimensions of the data in the cell.
     int Data_Depth;
     
     //The type of data containded within, used for output.
     int flg_Data_Type;
     
     //Whether or not the cell has been used.
     bool flg_Set;
     
     //The current position of the Push_Index.
     int Push_Index;
     
public:
     
     
     c_Cell_1D()
     {
          //Setup the array to use.
          Data = new u_Data[1];
          Data[0].I = 0;
          
          Push_Index = 0;
          Data_Depth = 1;
          
          //0 = character.
          flg_Data_Type = 1;
          
          flg_Set = 0;
     }
     
     ~c_Cell_1D()
     {
          delete [] Data;
          Data = NULL;
     }
     
     //--     MEMBERS FOR SETTING DATA
     
     
     //--     GETS A DATA BIT FROM THE CELL AT THE GIVEN INDEX
     
     //Character
     void set_Data_Bit_At_Given_Index_C(int p_Index, char p_Data)
     {
          //Resize the cell to hold the data.
          expand_Cell_Data(p_Index);
          
          Data[p_Index].I = int (p_Data);
     }
     
     //Integer
     void set_Data_Bit_At_Given_Index_I(int p_Index, int p_Data)
     {
          //Resize the cell to hold the data.
          expand_Cell_Data(p_Index);
          
          Data[p_Index].I = p_Data;
     }
     
     //Float
     void set_Data_Bit_At_Given_Index_F(int p_Index, float p_Data)
     {
          //Resize the cell to hold the data.
          expand_Cell_Data(p_Index);
          
          Data[p_Index].F = p_Data;
     }
     
     //Node Reference
     void set_Data_Bit_At_Given_Index_NR(int p_Index, void * p_Data)
     {
          //Resize the cell to hold the data.
          expand_Cell_Data(p_Index);
          
          Data[p_Index].NR = p_Data;
     }
     
     //Bool
     void set_Data_Bit_At_Given_Index_B(int p_Index, bool p_Data)
     {
          //Resize the cell to hold the data.
          expand_Cell_Data(p_Index);
          
          Data[p_Index].B = p_Data;
     }
     
     //u_Data
     void set_Data_Bit_At_Given_Index(int p_Index, u_Data p_Data)
     {
          //Resize the cell to hold the data.
          expand_Cell_Data(p_Index);
          
          Data[p_Index] = p_Data;
     }
     
     
     //==--   SETTING DATA CHUNK
     
     //Sets the data in the cell.
     void set_C(char p_Data[], int p_Data_Length)
     {
          //Setup the temporary array.
          u_Data * tmp_Data = new u_Data[p_Data_Length];
          
          //Copy the data into it.
          for (int cou_Index=0;cou_Index<p_Data_Length;cou_Index++)
          {
               tmp_Data[cou_Index].I = int (p_Data[cou_Index]);
          }
          
          //Ship the data to the set function now that it is converted to the appropriate datatype.
          set(tmp_Data, p_Data_Length, 0);
          
          //Delete the tmp_Data.
          delete [] tmp_Data;
     }
     
     //Sets the data in the cell.
     void set_S(string p_Data)
     {
          //Setup the temporary array.
          u_Data * tmp_Data = new u_Data[p_Data.size() + 1];
          
          //Copy the data into it.
          for (unsigned int cou_Index=0;cou_Index<(p_Data.size() + 1);cou_Index++)
          {
               tmp_Data[cou_Index].I = int (p_Data[cou_Index]);
          }
          
          //Ship the data to the set function now that it is converted to the appropriate datatype.
          //With .size() the +1 is not needed as we are not passing a NULL terminated sequence.
          set(tmp_Data, p_Data.size(), 0);
          
          //Delete the tmp_Data.
          delete [] tmp_Data;
     }
     
     //Sets the data in the cell.
     void set_I(int p_Data[], int p_Data_Length)
     {
          //Setup the temporary array.
          u_Data * tmp_Data = new u_Data[p_Data_Length];
          
          //Copy the data into it.
          for (int cou_Index=0;cou_Index<p_Data_Length;cou_Index++)
          {
               tmp_Data[cou_Index].I = p_Data[cou_Index];
          }
          
          //Ship the data to the set function now that it is converted to the appropriate datatype.
          set(tmp_Data, p_Data_Length, 1);
          
          //Delete the tmp_Data.
          delete [] tmp_Data;
     }
     
     //Sets the data in the cell.
     void set_F(float p_Data[], int p_Data_Length)
     {
          //Setup the temporary array.
          u_Data * tmp_Data = new u_Data[p_Data_Length];
          
          //Copy the data into it.
          for (int cou_Index=0;cou_Index<p_Data_Length;cou_Index++)
          {
               tmp_Data[cou_Index].F = p_Data[cou_Index];
          }
          
          //Ship the data to the set function now that it is converted to the appropriate datatype.
          set(tmp_Data, p_Data_Length, 2);
          
          //Delete the tmp_Data.
          delete [] tmp_Data;
     }
     
     //Sets the data in the cell.
     void set_NR(void * p_Data[], int p_Data_Length)
     {
          //Setup the temporary array.
          u_Data * tmp_Data = new u_Data[p_Data_Length];
          
          //Copy the data into it.
          for (int cou_Index=0;cou_Index<p_Data_Length;cou_Index++)
          {
               tmp_Data[cou_Index].NR = p_Data[cou_Index];
          }
          
          //Ship the data to the set function now that it is converted to the appropriate datatype.
          set(tmp_Data, p_Data_Length, 3);
          
          //Delete the tmp_Data.
          delete [] tmp_Data;
     }
     
     //Sets the data in the cell.
     void set_B(bool p_Data[], int p_Data_Length)
     {
          //Setup the temporary array.
          u_Data * tmp_Data = new u_Data[p_Data_Length];
          
          //Copy the data into it.
          for (int cou_Index=0;cou_Index<p_Data_Length;cou_Index++)
          {
               tmp_Data[cou_Index].B = p_Data[cou_Index];
          }
          
          //Ship the data to the set function now that it is converted to the appropriate datatype.
          set(tmp_Data, p_Data_Length, 4);
          
          //Delete the tmp_Data.
          delete [] tmp_Data;
     }
     
     //Sets the data in the cell.
     void set(u_Data p_Data[], int p_Data_Length, int p_Data_Type)
     {
          //cout << "\n\t\t\t set p_Data_Type->" << p_Data_Type;
          //cout << "\n\t\t\t  p_Data_Length->" << p_Data_Length;
          
          //Wipe the cell first.
          reset_Cell();
          
          //Resize the cell to hold the data.
          expand_Cell_Data(p_Data_Length);
          
          //Read the data into the cell.
          for (int cou_Index=0;cou_Index<p_Data_Length;cou_Index++)
          {
               Data[cou_Index].I = p_Data[cou_Index].I;
          }
          
          //Sets the data type for the cell.
          flg_Data_Type = p_Data_Type;
          
          //Sets the flag for being set.
          flg_Set = 1;
     }
     
     //--     MEMBERS FOR PUSHING
     
     //Pushes a character.
     void push_C(char p_Data)
     {
          u_Data tmp_Data;
          tmp_Data.I = int (p_Data);
          push(tmp_Data, 0);
     }
     
     //Pushes an int.
     void push_I(int p_Data)
     {
          u_Data tmp_Data;
          tmp_Data.I = p_Data;
          push(tmp_Data, 1);
     }
     
     //Pushes a float.
     void push_F(float p_Data)
     {
          u_Data tmp_Data;
          tmp_Data.F = p_Data;
          push(tmp_Data, 2);
     }
     
     //Pushes an address.
     void push_NR(void * p_Data)
     {
          u_Data tmp_Data;
          tmp_Data.NR = p_Data;
          push(tmp_Data, 3);
     }
     
     //Pushes an address.
     void push_B(bool p_Data)
     {
          u_Data tmp_Data;
          tmp_Data.B = p_Data;
          push(tmp_Data, 4);
     }
     
     //Pushes a bit of data into the cell.
     void push(u_Data p_Data, int p_Data_Type)
     {
          //If there is no room in the cell then increase the cells size.
          if (Push_Index >= Data_Depth)
          {
               expand_Cell_Data(Data_Depth + 1);
          }
          
          //Assign the given value to the current index.
          Data[Push_Index].I = p_Data.I;
          
          //Increment the Input Pointer.
          Push_Index++;
          
          //Sets the data type for the cell.
          flg_Data_Type = p_Data_Type;
          
          //Sets the flag for being set.
          flg_Set = 1;
     }
     
     //--     MEMBERS FOR RETRIEVING DATA
     
     //Gets the current data_Depth;
     int get_Data_Depth()
     {
          return Data_Depth;
     }
     
     //Gets the set flag.
     bool get_Set_State()
     {
          return flg_Set;
     }
     
     //Gets the current push index.
     int get_Push_Index()
     {
          return Push_Index;
     }
     
     //--     GET DATA CHUNK FROM CELL BY PASSING AN ARRAY REFERENCE
     
     void get_Cell_Data_Chunk_Passed_S(string * p_String)
     {
          //Reset passed string.
          *p_String = "";
          
          //Read the data into the string as characters.
          for (int cou_Index=0;cou_Index<Data_Depth;cou_Index++)
          {
               *p_String += char (Data[cou_Index].I);
          }
     }
     
     void get_Cell_Data_Chunk_Passed_C(char * &p_Passed_Array)
     {
          //Reset passed array.
          delete [] p_Passed_Array;
          p_Passed_Array = new char[Data_Depth];
          
          //Read the data into the array.
          for (int cou_Index=0;cou_Index<Data_Depth;cou_Index++)
          {
               p_Passed_Array[cou_Index] = char (Data[cou_Index].I);
          }
     }
     
     void get_Cell_Data_Chunk_Passed_I(int * &p_Passed_Array)
     {
          //Reset passed array.
          delete [] p_Passed_Array;
          p_Passed_Array = new int[Data_Depth];
          
          //Read the data into the array.
          for (int cou_Index=0;cou_Index<Data_Depth;cou_Index++)
          {
               p_Passed_Array[cou_Index] = Data[cou_Index].I;
          }
     }
     
     void get_Cell_Data_Chunk_Passed_F(float * &p_Passed_Array)
     {
          //Reset passed array.
          delete [] p_Passed_Array;
          p_Passed_Array = new float[Data_Depth];
          
          //Read the data into the array.
          for (int cou_Index=0;cou_Index<Data_Depth;cou_Index++)
          {
               p_Passed_Array[cou_Index] = Data[cou_Index].F;
          }
     }
     
     void get_Cell_Data_Chunk_Passed_NR(void ** &p_Passed_Array)
     {
          //Reset passed array.
          delete [] p_Passed_Array;
          p_Passed_Array = new void*[Data_Depth];
          
          //Read the data into the array.
          for (int cou_Index=0;cou_Index<Data_Depth;cou_Index++)
          {
               p_Passed_Array[cou_Index] = Data[cou_Index].NR;
          }
     }
     
     void get_Cell_Data_Chunk_Passed_B(bool * &p_Passed_Array)
     {
          //Reset passed array.
          delete [] p_Passed_Array;
          p_Passed_Array = new bool[Data_Depth];
          
          //Read the data into the array.
          for (int cou_Index=0;cou_Index<Data_Depth;cou_Index++)
          {
               p_Passed_Array[cou_Index] = Data[cou_Index].B;
          }
     }
     
     
     u_Data * get_Cell_Data()
     {
          return Data;
     }
     
     //--     GETS A DATA BIT FROM THE CELL AT THE GIVEN INDEX
     
     //Character
     char get_Data_Bit_From_Given_Index_C(int p_Index)
     {
          if (p_Index >= Data_Depth){ return char (0); }
          return char (Data[p_Index].I);
     }
     
     //Integer
     int get_Data_Bit_From_Given_Index_I(int p_Index)
     {
          if (p_Index >= Data_Depth){ return 0; cout << "\n\t\t\t\t returning 0;"; }
          return Data[p_Index].I;
     }
     
     //Float
     float get_Data_Bit_From_Given_Index_F(int p_Index)
     {
          if (p_Index >= Data_Depth){ return 0.0; }
          return Data[p_Index].F;
     }
     
     //Node Reference
     void * get_Data_Bit_From_Given_Index_NR(int p_Index)
     {
          if (p_Index >= Data_Depth){ return NULL; }
          return Data[p_Index].NR;
     }
     
     //Bool
     bool get_Data_Bit_From_Given_Index_B(int p_Index)
     {
          if (p_Index >= Data_Depth){ return 0; }
          return Data[p_Index].B;
     }
     
     //u_Data
     u_Data get_Data_Bit_From_Given_Index(int p_Index)
     {
          if (p_Index < Data_Depth){ return Data[0]; }
          return Data[p_Index];
     }
     
     //Gets the datatype for the cell.
     int get_flg_Data_Type()
     {
          return flg_Data_Type;
     }
     
     //Sets the flg_Data_Type.
     void set_flg_Data_Type(int p_flg_Data_Type)
     {
          flg_Data_Type = p_flg_Data_Type;
     }
     
     
     
     //--      ARRAY MANIPULATION MEMBERS
     
     //Expands the array in the given dimension by the given amount copying the data already held therein.
     void expand_Cell_Data(int p_Depth)
     {
          //If the cell already excedes the given depth then return.
          if (Data_Depth >= p_Depth){ return; }
          
          //Setup the temporary array.
          u_Data * tmp_Data = new u_Data[Data_Depth];
          
          //Copy the original data into the array.
          for (int cou_D=0;cou_D<Data_Depth;cou_D++)
          {
               //Copy the c_Data from Data to tmp_Data.
               tmp_Data[cou_D] = Data[cou_D];
          }
          
          //Reallocate the Data array.
          delete [] Data;
          Data = NULL;
          
          Data = new u_Data[p_Depth];
          
          //Copy the data back into the data array.
          for (int cou_Index=0;cou_Index<Data_Depth;cou_Index++)
          {
               Data[cou_Index] = tmp_Data[cou_Index];
          }
          
          //Destroy tmp_Array;
          delete [] tmp_Data;
          tmp_Data = NULL;
          
          //Now create the u_Data for the new cells.
          for (int cou_Index=Data_Depth;cou_Index<p_Depth;cou_Index++)
          {
               Data[cou_Index].I = 0;
          }
          
          //Resize the data tracker to the appropriate amount.
          Data_Depth = p_Depth;
     }
     
     //Resets all of the cells data members.
     void reset_Cell()
     {
          //Erase the old data.
          delete [] Data;
          Data = NULL;
          
          //Setup the array to use.
          Data = new u_Data[1];
          
          Push_Index = 0;
          Data_Depth = 1;
     }
     
     //Output the cell data.
     void convert_Cell_To_String()
     {
          //cout << " ^" << flg_Data_Type;
          if (flg_Data_Type == 0)
          {
               return;
          }
          
          if (flg_Data_Type == 1)
          {
               set_S(int2str(Data[0].I));
               flg_Data_Type = 0;
               return;
          }
          
          if (flg_Data_Type == 2)
          {
               set_S(float2str(Data[0].F));
               flg_Data_Type = 0;
               return;
          }
          
          if (flg_Data_Type == 3)
          { 
               set_S(int2str(Data[0].I));
               flg_Data_Type = 0;
               return;
          }
          
          if (flg_Data_Type == 4)
          {
               set_S(bool2str(Data[0].B));
               flg_Data_Type = 0;
               return;
          }
     }
     
     //--     OUTPUT FUNCTIONS
     
     //Output the cell data.
     void output_Cell_Data()
     {
          //cout << " ^" << flg_Data_Type;
          if (flg_Data_Type == 0)
          {
               ochr(0, 8, '[');
               for (int cou_Index=0;cou_Index<Data_Depth;cou_Index++)
               {
                    if (char (Data[cou_Index].I) == '_')
                    { 
                         ochr(0, 9, char (Data[cou_Index].I));
                         continue;
                    }
                    ochr(0, 11, char (Data[cou_Index].I));
               }
               ochr(0, 8, ']'); cout << " ";
          }
          
          if (flg_Data_Type == 1)
          {
               ochr(0, 8, '[');
               ostr(0, 14, int2str(Data[0].I));
               for (int cou_Index=1;cou_Index<Data_Depth;cou_Index++)
               {
                    cout << " ";
                    ostr(0, 14, int2str(Data[cou_Index].I));
               }
               ochr(0, 8, ']'); cout << " ";
          }
          
          if (flg_Data_Type == 2)
          {
               ochr(0, 8, '[');
               ostr(0, 13, float2str(Data[0].F));
               for (int cou_Index=1;cou_Index<Data_Depth;cou_Index++)
               {
                    cout << " ";
                    ostr(0, 13, float2str(Data[cou_Index].F));
               }
               ochr(0, 8, ']'); cout << " ";
          }
          
          if (flg_Data_Type == 3)
          {
               ochr(0, 8, '[');
               cout << Data[0].NR;
               for (int cou_Index=1;cou_Index<Data_Depth;cou_Index++)
               {
                    cout << " " << Data[cou_Index].NR;                    
               }
               ochr(0, 8, ']'); cout << " ";
          }
          
          if (flg_Data_Type == 4)
          {
               ochr(0, 8, '[');
               ostr(0, 15, bool2str(Data[0].B));
               for (int cou_Index=1;cou_Index<Data_Depth;cou_Index++)
               {
                    ostr(0, 15, bool2str(Data[cou_Index].B));
               }
               ochr(0, 8, ']'); cout << " ";
          }
     }
     
     //Outputs the cell.
     void output_Cell()
     {
          cout << "\n\t\t\t c_Cell_1D->" << this;
          cout << "\n\t\t\t   Data_Depth->" << Data_Depth;
          cout << "\n\t\t\t   flg_Data_Type->" << flg_Data_Type;
          cout << "\n\t\t\t   flg_Set->" << flg_Set;
          cout << "\n\t\t\t   Push_Index->" << Push_Index;
          cout << "\n\t\t\t   Data->";
          output_Cell_Data();
     }
};
