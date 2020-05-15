//This class holds and array of cells for the table rows.

class c_Row_1D
{
     
protected:
     
     //The cell array to use.
     c_Cell_1D ** Cells;
     
     //The current push index.
     int Push_Index;
     
     //The current cell count.
     int Cell_Count;
     
     
     
public:
     
     c_Row_1D()
     {
          Cells = new c_Cell_1D*[1];
          Cells[0] = new c_Cell_1D;
          
          Cell_Count = 1;
          Push_Index = 0;
          
     }
     
     ~c_Row_1D()
     {
          for (int cou_Index=0;cou_Index<Cell_Count;cou_Index++)
          {
               delete Cells[cou_Index];
               Cells[cou_Index] = NULL;
          }
          delete [] Cells;
          Cells = NULL;
          
     }
     
     //--     DATA SETTING FUNCTIONS
     
     void set_Data_Type_For_Current_Cell(int p_Data_Type)
     {
          Cells[Push_Index]->set_flg_Data_Type(p_Data_Type);
     }
     
     //--     PUSHING A SINGLE BIT OF DATA INTO THE CURRENT CELL WITHOUT POPPING, USED FOR BUILDING A PATTERN DURING BACKPROPAGATION FOR EXAMPLE
     
     //Pushes a bit of data into the current push cell. Used when doing backpropagation.
     void push_Data_Bit_Into_Current_Cell_C(char p_Data){    Cells[Push_Index]->push_C(p_Data); }
     void push_Data_Bit_Into_Current_Cell_I(int p_Data){     Cells[Push_Index]->push_I(p_Data); }
     void push_Data_Bit_Into_Current_Cell_F(float p_Data){   Cells[Push_Index]->push_F(p_Data); }
     void push_Data_Bit_Into_Current_Cell_NR(void * p_Data){ Cells[Push_Index]->push_NR(p_Data); }
     void push_Data_Bit_Into_Current_Cell_B(bool p_Data){    Cells[Push_Index]->push_B(p_Data); }
     void push_Data_Bit_Into_Current_Cell(u_Data p_Data, int p_Data_Type){ Cells[Push_Index]->push(p_Data, p_Data_Type); }
     
     //--     PUSHING A SINGLE BIT OF DATA ONTO THE CURRENT CELL AFTER POPPING
     
     void push_Data_Bit_F(float p_Data){    if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->push_F(p_Data); }
     void push_Data_Bit_I(int p_Data){      if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->push_I(p_Data); }
     void push_Data_Bit_NR(void * p_Data){  if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->push_NR(p_Data); }     
     void push_Data_Bit_B(bool p_Data){     if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->push_B(p_Data); }
     void push_Data_Bit_S(string p_Data){   if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } push_Data_Chunk_S(p_Data); }
     void push_Data_Bit(u_Data p_Data, int p_Data_Type){ if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } push_Data_Chunk(&p_Data, 0, p_Data_Type); }
     
     
     //--     PUSHING A DATA CHUNK ONTO THE CURRENT CELL AFTER POPPING IT
     
     void push_Data_Chunk_S(string p_Data)                       { if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->set_S(p_Data); }
     void push_Data_Chunk_I(int p_Data[], int p_Data_Depth)      { if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->set_I(p_Data, p_Data_Depth); }
     void push_Data_Chunk_F(float p_Data[], int p_Data_Depth)    { if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->set_F(p_Data, p_Data_Depth); }
     void push_Data_Chunk_NR(void * p_Data[], int p_Data_Depth)  { if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->set_NR(p_Data, p_Data_Depth); }
     void push_Data_Chunk_B(bool p_Data[], int p_Data_Depth)     { if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->set_B(p_Data, p_Data_Depth); }
     void push_Data_Chunk(u_Data p_Data[], int p_Data_Depth, int p_Data_Type){ if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->set(p_Data, p_Data_Depth, p_Data_Type); }
     
     //--     SETTING A GIVEN CELLS DATA TO THE GIVEN DATA CHUNK
     
     //Sets a given index to the given data set.
     void set_Given_Cell_Data_Chunk_S (int p_Cell, string p_Data)                     { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_S(p_Data); }
     void set_Given_Cell_Data_Chunk_C (int p_Cell, char p_Data[], int p_Data_Length)  { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_C(p_Data, p_Data_Length); }
     void set_Given_Cell_Data_Chunk_I (int p_Cell, int p_Data[], int p_Data_Length)   { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_I(p_Data, p_Data_Length); }
     void set_Given_Cell_Data_Chunk_F (int p_Cell, float p_Data[], int p_Data_Length) { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_F(p_Data, p_Data_Length); }
     void set_Given_Cell_Data_Chunk_NR(int p_Cell, void * p_Data[], int p_Data_Length){ expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_NR(p_Data, p_Data_Length); }
     void set_Given_Cell_Data_Chunk_B (int p_Cell, bool p_Data[], int p_Data_Length)  { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_B(p_Data, p_Data_Length); }
     void set_Given_Cell_Data_Chunk   (int p_Cell, u_Data p_Data[], int p_Data_Length, int p_Data_Type){ expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set(p_Data, p_Data_Length, p_Data_Type); }
     
     
     //--     SETTING A GIVEN CELLS DATA TO THE GIVEN DATA BIT
     
     //Sets the given cells data to the given single value.
     void set_Given_Cell_Data_Bit_C (int p_Cell, char p_Data)  { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_C(&p_Data, 1);}
     void set_Given_Cell_Data_Bit_S (int p_Cell, string p_Data){ expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_S(p_Data);}
     void set_Given_Cell_Data_Bit_I (int p_Cell, int p_Data)   { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_I(&p_Data, 1);}
     void set_Given_Cell_Data_Bit_F (int p_Cell, float p_Data) { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_F(&p_Data, 1);}
     void set_Given_Cell_Data_Bit_NR(int p_Cell, void * p_Data){ expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_NR(&p_Data, 1);}
     void set_Given_Cell_Data_Bit_B (int p_Cell, bool p_Data)  { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_B(&p_Data, 1);}
     void set_Given_Cell_Data_Bit   (int p_Cell, u_Data p_Data, int p_Data_Type){ expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set(&p_Data, 1, p_Data_Type);}
     
     
     //--     SETTING A GIVEN CELLS DATA TO THE GIVEN DATA BIT AT THE GIVEN INDEX
     
     //Sets the given cells data to the given single value.
     void set_Given_Cell_Data_Bit_At_Given_Index_C (int p_Cell, int p_Index, char p_Data)  { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_Data_Bit_At_Given_Index_C(p_Index, p_Data); }
     void set_Given_Cell_Data_Bit_At_Given_Index_S (int p_Cell, int p_Index, string p_Data){ expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_Data_Bit_At_Given_Index_C(p_Index, p_Data[0]); }
     void set_Given_Cell_Data_Bit_At_Given_Index_I (int p_Cell, int p_Index, int p_Data)   { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_Data_Bit_At_Given_Index_I(p_Index, p_Data); }
     void set_Given_Cell_Data_Bit_At_Given_Index_F (int p_Cell, int p_Index, float p_Data) { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_Data_Bit_At_Given_Index_F(p_Index, p_Data); }
     void set_Given_Cell_Data_Bit_At_Given_Index_NR(int p_Cell, int p_Index, void * p_Data){ expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_Data_Bit_At_Given_Index_NR(p_Index, p_Data); }
     void set_Given_Cell_Data_Bit_At_Given_Index_B (int p_Cell, int p_Index, bool p_Data)  { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_Data_Bit_At_Given_Index_B(p_Index, p_Data); }
     void set_Given_Cell_Data_Bit_At_Given_Index   (int p_Cell, int p_Index, u_Data p_Data){ expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_Data_Bit_At_Given_Index(p_Index, p_Data); }
     
     //Sets the flg_Data_Type for a given cell.
     //From here up it will be abstracted as Data_Type.
     int set_Given_Cells_Data_Type(int p_Cell, int p_Data_Type)
     {
          if (p_Cell < Cell_Count)
          {
               Cells[p_Cell]->set_flg_Data_Type(p_Data_Type);
          }
          return 0;
     }
     
     //--     GET FUNCTIONS
     
     //Gets the current cell count.
     int get_Cell_Count()
     {
          return Cell_Count;
     }
     
     //Gets the Data_Depth for a given cell.
     int get_Given_Cells_Data_Depth(int p_Cell)
     {
          expand_Row_Depth(p_Cell + 1);
          return Cells[p_Cell]->get_Data_Depth();
     }
     
     //Gets the current cells data depth.
     int get_Current_Cells_Data_Depth()
     {
          return get_Given_Cells_Data_Depth(Push_Index);
     }
     
     //Gets the flg_Data_Type for a given cell.
     //From here up it will be abstracted as Data_Type.
     int get_Given_Cells_Data_Type(int p_Cell)
     {
          expand_Row_Depth(p_Cell + 1);
          return Cells[p_Cell]->get_flg_Data_Type();
     }
     
     //Gets the set flag for the given cell.
     bool get_Given_Cells_Set_State(int p_Cell)
     {
          expand_Row_Depth(p_Cell + 1);
          
          return Cells[p_Cell]->get_Set_State();
     }
          
     //Gets the set flag for the current cell.
     bool get_Current_Cells_Set_State()
     {
          return get_Given_Cells_Set_State(Push_Index);
     }
     
     //Gets the set flag for the given cell.
     bool get_Given_Cells_Push_Index(int p_Cell)
     {
          expand_Row_Depth(p_Cell + 1);
          
          return Cells[p_Cell]->get_Push_Index();
     }
     
     //Gets the reference to a given cell.
     void * get_Cell_Reference(int p_Cell)
     {
          expand_Row_Depth(p_Cell + 1);
          
          return Cells[p_Cell];
     }
     
     //Gets the current push index.
     int get_Push_Index()
     {
          return Push_Index;
     }
     
     //--     GETS A GIVEN CELLS DATA CHUNK USING AN ARRAY REFERENCE PASSED
     
     //String
     void get_Given_Cells_Data_Chunk_Passed_S(int p_Cell, string * p_String)
     {
          if (p_Cell < Cell_Count)
          {
               Cells[p_Cell]->get_Cell_Data_Chunk_Passed_S(p_String);
               return;
          }
          *p_String = "NULL";
     }
     
     //Character
     void get_Given_Cells_Data_Chunk_Passed_C(int p_Cell, char * &p_Passed_Data)
     {
          if (p_Cell < Cell_Count)
          {
               Cells[p_Cell]->get_Cell_Data_Chunk_Passed_C(p_Passed_Data);
          }
     }
     
     //Integer
     void get_Given_Cells_Data_Chunk_Passed_I(int p_Cell, int * &p_Passed_Data)
     {
          if (p_Cell < Cell_Count)
          {
               Cells[p_Cell]->get_Cell_Data_Chunk_Passed_I(p_Passed_Data);
          }
     }
     
     //Float
     void get_Given_Cells_Data_Chunk_Passed_F(int p_Cell, float * &p_Passed_Data)
     {
          if (p_Cell < Cell_Count)
          {
               Cells[p_Cell]->get_Cell_Data_Chunk_Passed_F(p_Passed_Data);
          }
     }
     
     //Node Reference
     void get_Given_Cells_Data_Chunk_Passed_NR(int p_Cell, void ** &p_Passed_Data)
     {
          if (p_Cell < Cell_Count)
          {
               Cells[p_Cell]->get_Cell_Data_Chunk_Passed_NR(p_Passed_Data);
          }
     }
     
     //Bool
     void get_Given_Cells_Data_Chunk_Passed_B(int p_Cell, bool * &p_Passed_Data)
     {
          if (p_Cell < Cell_Count)
          {
               Cells[p_Cell]->get_Cell_Data_Chunk_Passed_B(p_Passed_Data);
          }
     }
     
     //--     GETS THE GIVEN CELLS RAW DATA
     
     //Gets the data from the given cell assuming it is in the array.
     u_Data * get_Given_Cells_Data(int p_Cell)
     {
          if (p_Cell < Cell_Count)
          {
               return Cells[p_Cell]->get_Cell_Data();
          }
          return NULL;
     }
     
     //--    GETS THE GIVEN CELLS DATA FROM A GIVEN INDEX
     
     //Char
     char get_Data_Bit_At_Given_Index_In_Given_Cell_C(int p_Cell, int p_Index)
     {
          if (p_Cell < Cell_Count)
          {
               return Cells[p_Cell]->get_Data_Bit_From_Given_Index_C(p_Index);
          }
          return char (0);
     }
     
     //Int
     int get_Data_Bit_At_Given_Index_In_Given_Cell_I(int p_Cell, int p_Index)
     {
          if (p_Cell < Cell_Count)
          {
               return Cells[p_Cell]->get_Data_Bit_From_Given_Index_I(p_Index);
          }
          return 0;
     }
     
     //Float
     float get_Data_Bit_At_Given_Index_In_Given_Cell_F(int p_Cell, int p_Index)
     {
          if (p_Cell < Cell_Count)
          {
               return Cells[p_Cell]->get_Data_Bit_From_Given_Index_F(p_Index);
          }
          return 0.0;
     }
     
     //Node Reference
     void * get_Data_Bit_At_Given_Index_In_Given_Cell_NR(int p_Cell, int p_Index)
     {
          if (p_Cell < Cell_Count)
          {
               return Cells[p_Cell]->get_Data_Bit_From_Given_Index_NR(p_Index);
          }
          return NULL;
     }
     
     //Bool
     bool get_Data_Bit_At_Given_Index_In_Given_Cell_B(int p_Cell, int p_Index)
     {
          if (p_Cell < Cell_Count)
          {
               return Cells[p_Cell]->get_Data_Bit_From_Given_Index_B(p_Index);
          }
          return false;
     }
     
     //u_Data
     u_Data get_Data_Bit_At_Given_Index_In_Given_Cell(int p_Cell, int p_Index)
     {
          if (p_Cell < Cell_Count)
          {
               return Cells[p_Cell]->get_Data_Bit_From_Given_Index(p_Index);
          }
          u_Data tmp_Data;
          tmp_Data.I = -2;
          return tmp_Data;
     }
     
     
     //--     ARRAY MANIPULATIONS
     
     //Increases the push index.
     void pop_Cell()
     {
          Push_Index++;
          if (Push_Index >= Cell_Count){ expand_Row_Depth(Push_Index + 1); }
     }
     
     //Increases the row count to the given amount.
     void expand_Row_Depth(int p_Depth)
     {
          //If the given number is less than what already exists then return.
          if (p_Depth < Cell_Count){ return; }
          
          //Setup the temporary array.
          c_Cell_1D ** tmp_Cells = new c_Cell_1D*[Cell_Count];
          
          //Copy the old data into the temporary cells.
          for (int cou_Index=0;cou_Index<Cell_Count;cou_Index++)
          {
               tmp_Cells[cou_Index] = Cells[cou_Index];
               Cells[cou_Index] = NULL;
          }
          
          //Reallocate the old array.
          delete [] Cells;
          Cells = new c_Cell_1D*[p_Depth];
          
          //Copy the old cells back into the old array.
          for (int cou_Index=0;cou_Index<Cell_Count;cou_Index++)
          {
               Cells[cou_Index] = tmp_Cells[cou_Index];
               tmp_Cells[cou_Index] = NULL;
          }
          
          //Ini the new cells.
          for (int cou_Index=Cell_Count;cou_Index<p_Depth;cou_Index++)
          {
               Cells[cou_Index] = new c_Cell_1D;
          }
          
          //Set the current cell count.
          Cell_Count = p_Depth;
          
          //Cleanup.
          delete [] tmp_Cells;
          
     }
     
     //Converts each cell in the row into strings.
     string convert_Row_To_String()
     {
          for (int cou_Index=0;cou_Index<Cell_Count;cou_Index++)
          {
               Cells[cou_Index]->convert_Cell_To_String();
          }
          return "Converted";
     }
     
     //Resets the row.
     void reset_Row()
     {
          for (int cou_Index=0;cou_Index<Cell_Count;cou_Index++)
          {
               delete Cells[cou_Index];
               Cells[cou_Index] = NULL;
          }
          delete [] Cells;
          
          Cells = new c_Cell_1D*[1];
          Cells[0] = new c_Cell_1D;
          
          Cell_Count = 0;
          Push_Index = -1;
     }
     
     
     //--     OUTPUT MEMBERS
     
     //Outputs a single cells data.
     void output_Given_Cell(int p_Cell)
     {
          Cells[p_Cell]->output_Cell_Data();
     }
     
     //Outputs the row data.
     void output_Row_Data(int p_Limit)
     {
          //If p_Limit is -1 that means take the limits off and output everything.
          if (p_Limit == -1 || p_Limit > Cell_Count){ p_Limit = Cell_Count; }
          
          for (int cou_Index=0;cou_Index<p_Limit;cou_Index++)
          {
               cout << cou_Index;
               output_Given_Cell(cou_Index);
          }
     }
     
     //Outputs the row.
     void output_Row(int p_Limit)
     {
          cout << "\n\t ";
          ostr(0, 15, "Row");
          ostr(0, 7, "->");
          cout << this;
          ostr(0, 15, " Cell_Count");
          ostr(0, 7, "->");
          cout << Cell_Count;
          ostr(0, 7, " | ");
          output_Row_Data(p_Limit);
     }
     
     
     
     //Outputs a single cells data.
     void output_Given_Cell_Verbose(int p_Cell)
     {
          Cells[p_Cell]->output_Cell();
     }
     
     //Outputs the row data.
     void output_Row_Data_Verbose()
     {
          for (int cou_Index=0;cou_Index<Cell_Count;cou_Index++)
          {
               output_Given_Cell_Verbose(cou_Index);
          }
     }
     
     //Outputs the row.
     void output_Row_Verbose()
     {
          cout << "\n\t\t Row->" << this << " Cell_Count->" << Cell_Count << " | ";
          output_Row_Data_Verbose();
     }
     
};