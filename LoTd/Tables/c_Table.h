//This class is the table class, it holds the table rows and allows for their manipulation.

//Used for polymorphic referencing.
class c_Base_Table
{
public:
     
     virtual ~c_Base_Table(){};
     
     virtual void set_Table_ID(int p_Table_ID)=0;
     virtual void set_Table_Name(string p_Table_Name)=0;
     virtual string get_Table_Name()=0;
     virtual int get_Table_Dimension()=0;
     virtual int row_Is_Empty(int p_Row)=0;
     virtual int rowc_Is_Empty()=0;
     virtual int table_Is_Empty()=0;
     virtual string get_Table_Data_Type()=0;
     
     virtual void output_Table()=0;
     virtual void output_Table_Header()=0;
     virtual void output_Table_Verbose()=0;
     
     virtual void * get_Row_Reference(int p_Row)=0;
     virtual void * get_Cell_Reference(int p_Row, int p_Cell)=0;
     virtual void * get_Current_Cell_Reference(int p_Row)=0;
     
     virtual int get_Row_Cell_Count(int p_Row)=0;
     virtual int get_Current_Row_Cell_Count()=0;
     virtual int get_Row_Count()=0;
     virtual int get_Current_Row()=0;
     virtual int get_Current_Cell()=0;
     virtual int get_Current_Cell_In_Given_Row(int p_Row)=0;
     
     
     //Sets a given rows cell to the given data.
     virtual void set_string(   int p_Row, int p_Cell, string p_String)=0;
     virtual void set_int(      int p_Row, int p_Cell, int p_Int)=0;
     virtual void set_float(    int p_Row, int p_Cell, float p_Float)=0;
     virtual void set_reference(int p_Row, int p_Cell, void * p_Void)=0;
     virtual void set_bool(     int p_Row, int p_Cell, bool p_Bool)=0;
     
     //Pushes a data chunk onto the given row.
     virtual void push_string(   int p_Row, string p_String)=0;
     virtual void push_int(      int p_Row, int p_Int)=0;
     virtual void push_float(    int p_Row, float p_Float)=0;
     virtual void push_reference(int p_Row, void * p_Void)=0;
     virtual void push_bool(     int p_Row, bool p_Bool)=0;
     
     //Pushes a data chunk onto the current row.
     virtual void pushc_string(   string p_String)=0;
     virtual void pushc_int(      int p_Int)=0;
     virtual void pushc_float(    float p_Float)=0;
     virtual void pushc_reference(void * p_Void)=0;
     virtual void pushc_bool(     bool p_Bool)=0;
     
     //Pops the current row then pushes a data chunk onto the given row.
     virtual void pop_push_string(   string p_String)=0;
     virtual void pop_push_int(      int p_Int)=0;
     virtual void pop_push_float(    float p_Float)=0;
     virtual void pop_push_reference(void * p_Void)=0;
     virtual void pop_push_bool(     bool p_Bool)=0;
     
     //Pushes a data chunk onto the current row then pops it.
     virtual void push_pop_string(   string p_String)=0;
     virtual void push_pop_int(      int p_Int)=0;
     virtual void push_pop_float(    float p_Float)=0;
     virtual void push_pop_reference(void * p_Void)=0;
     virtual void push_pop_bool(     bool p_Bool)=0;
     
     //Gets a given cells data in a given row.
     virtual string get_string(   int p_Row, int p_Cell)=0;
     virtual int    get_int(      int p_Row, int p_Cell, int p_Index=0)=0;
     virtual float  get_float(    int p_Row, int p_Cell, int p_Index=0)=0;
     virtual void * get_reference(int p_Row, int p_Cell, int p_Index=0)=0;
     virtual bool   get_bool(     int p_Row, int p_Cell, int p_Index=0)=0;
     virtual u_Data get_data(     int p_Row, int p_Cell, int p_Index=0)=0;
     
     //Gets a given cells data in a given row.
     virtual string getc_string(   )=0;
     virtual int    getc_int(      )=0;
     virtual float  getc_float(    )=0;
     virtual void * getc_reference()=0;
     virtual bool   getc_bool(     )=0;
     virtual u_Data getc_data(     )=0;
     
     //Copies the data from the submitted cell into itself.
     virtual string copy_Cell(int p_Row, int p_Cell, void * p_Cell_To_Copy)=0;
     virtual string copy_Row(int p_Row, void * p_Cell_To_Copy)=0;
     virtual string copy_Row(void * p_Row_To_Overwrite, void * p_Row_To_Copy)=0;
     virtual string copy_Table(void * p_Table_To_Copy)=0;
     
     virtual string swap_Row_Reference(int p_Row_One, int p_Row_Two)=0;
     virtual string overlay_Row(int p_Row, int p_Index, void * p_Row_To_Copy)=0;
     virtual string shift_Column(int p_Column_To_Shift)=0;
     virtual string shift_All_Rows(int p_Index)=0;
     virtual string translate_Row(void * p_From_Table, int p_Row, int p_Column, int p_Offset)=0;
     virtual string translate_Column(void * p_From_Table, int p_Column, int p_Offset, int p_Depth, int p_Row)=0;
     virtual string rotate_Table()=0;
     
     virtual string convert_Table_To_String()=0;
     
     //Resets the table.
     virtual void reset_Table()=0;
     
     //Sorts the table.
     virtual void bubble_Sort()=0;
     virtual void bubble_Sort_String()=0;
     virtual void bubble_Sort_Int()=0;
     virtual void bubble_Sort_Float()=0;
     virtual void bubble_Sort_Raw_Int()=0;
     virtual void bubble_Sort_Raw_Float()=0;
     
};

class c_Table_1D: public c_Base_Table
{
protected:
     
     //The rows for the table.
     c_Row_1D ** Rows;
     
     //The counter for the rows.
     int Row_Count;
     
     //The current push index for the rows.
     int Push_Index;
     
     //The name of the table.
     string Table_Name;
     
     //The tables ID as an int.
     int Table_ID;
     
public:
     
     c_Table_1D(string p_Table_Name = "Generic_Table")
     {
          Table_Name = p_Table_Name;
          Rows = new c_Row_1D*[1];
          Rows[0] = new c_Row_1D;
          Row_Count = 1;
          Push_Index = 0;
          Table_ID = 0;
     }
     
     ~c_Table_1D()
     {
          for (int cou_Index=0;cou_Index<Row_Count;cou_Index++)
          {
               delete Rows[cou_Index];
               Rows[cou_Index] = NULL;
          }
          delete [] Rows;
          Rows = NULL;
     }
     
     //Sets the ID of the table.
     void set_Table_ID(int p_ID)
     {
          Table_ID = p_ID;
     }
     
     //Gets the ID of the table.
     int get_Table_ID()
     {
          return Table_ID;
     }
     
     //Gets whether or not the table is empty.
     int table_Is_Empty()
     {
          if (!Rows[0]->get_Given_Cells_Set_State(0) && Row_Count == 1 && Rows[0]->get_Cell_Count() == 1){ return 1; }
          return 0;
     }
     
     //Gets whether or not the table is empty.
     int row_Is_Empty(int p_Row)
     {
          if (!bounds(p_Row)){ return 1; }
          if (!Rows[p_Row]->get_Given_Cells_Set_State(0) && Rows[p_Row]->get_Cell_Count() == 1){ return 1; }
          return 0;
     }
     
     //Gets whether or not the table is empty.
     int rowc_Is_Empty()
     {
          if (!Rows[Push_Index]->get_Given_Cells_Set_State(0) && Rows[Push_Index]->get_Cell_Count() == 1){ return 1; }
          return 0;
     }
     
     //Gets the dimension of the table, hardcoded into the classes.
     int get_Table_Dimension(){ return 1; }
     
     //Sets the name of the table to the given name.
     void set_Table_Name(string p_Table_Name)
     {
          Table_Name = p_Table_Name;
     }
     
     //Gets the name of the table.
     string get_Table_Name()
     {
          return Table_Name;
     }
     
     //Sets the datatype for the current push index.
     void set_Data_Type_For_Current_Cell_In_Current_Row(int p_Data_Type)
     {
          Rows[Push_Index]->set_Data_Type_For_Current_Cell(p_Data_Type);
     }
     
     
     //--     PUSHING DATA BITS INTO THE CURRENT CELL ON THE CURRENT ROW
     
     
     //Pushes a data bit into the current cell in the current row.
     void push_Data_Bit_Into_Current_Cell_On_Current_Row_S (string p_Data) { Rows[Push_Index]->push_Data_Bit_Into_Current_Cell_C(p_Data.at(0));  }
     void push_Data_Bit_Into_Current_Cell_On_Current_Row_C (char p_Data)   { Rows[Push_Index]->push_Data_Bit_Into_Current_Cell_C(p_Data);        }
     void push_Data_Bit_Into_Current_Cell_On_Current_Row_I (int p_Data)    { Rows[Push_Index]->push_Data_Bit_Into_Current_Cell_I(p_Data);        }
     void push_Data_Bit_Into_Current_Cell_On_Current_Row_F (float p_Data)  { Rows[Push_Index]->push_Data_Bit_Into_Current_Cell_F(p_Data);        }
     void push_Data_Bit_Into_Current_Cell_On_Current_Row_NR(void * p_Data) { Rows[Push_Index]->push_Data_Bit_Into_Current_Cell_NR(p_Data);       }
     void push_Data_Bit_Into_Current_Cell_On_Current_Row_B (bool p_Data)   { Rows[Push_Index]->push_Data_Bit_Into_Current_Cell_B(p_Data);        }
     void push_Data_Bit_Into_Current_Cell_On_Current_Row   (u_Data p_Data, int p_Data_Type = 0){ Rows[Push_Index]->push_Data_Bit_Into_Current_Cell(p_Data, p_Data_Type); }
     
     //Pushes a bit of data into the current cell on the given row.
     void push_Data_Bit_Into_Current_Cell_On_Given_Row_S (int p_Row, string p_Data) { xTble(p_Row + 1); Rows[p_Row]->push_Data_Bit_Into_Current_Cell_C(p_Data.at(0));  }
     void push_Data_Bit_Into_Current_Cell_On_Given_Row_C (int p_Row, char p_Data)   { xTble(p_Row + 1); Rows[p_Row]->push_Data_Bit_Into_Current_Cell_C(p_Data);        }
     void push_Data_Bit_Into_Current_Cell_On_Given_Row_I (int p_Row, int p_Data)    { xTble(p_Row + 1); Rows[p_Row]->push_Data_Bit_Into_Current_Cell_I(p_Data);        }
     void push_Data_Bit_Into_Current_Cell_On_Given_Row_F (int p_Row, float p_Data)  { xTble(p_Row + 1); Rows[p_Row]->push_Data_Bit_Into_Current_Cell_F(p_Data);        }
     void push_Data_Bit_Into_Current_Cell_On_Given_Row_NR(int p_Row, void * p_Data) { xTble(p_Row + 1); Rows[p_Row]->push_Data_Bit_Into_Current_Cell_NR(p_Data);       }
     void push_Data_Bit_Into_Current_Cell_On_Given_Row_B (int p_Row, bool p_Data)   { xTble(p_Row + 1); Rows[p_Row]->push_Data_Bit_Into_Current_Cell_B(p_Data);        }
     void push_Data_Bit_Into_Current_Cell_On_Given_Row   (int p_Row, u_Data p_Data, int p_Data_Type = 0){ xTble(p_Row + 1); Rows[p_Row]->push_Data_Bit_Into_Current_Cell(p_Data, p_Data_Type); }
     
     //--     PUSHING DATA ONTO THE CURRENT ROW ONE PIECE AT A TIME
     
     //Pushes a string onto the current row.
     void push_Data_Bit_Onto_Current_Row_S(string p_Data)  { Rows[Push_Index]->push_Data_Bit_S(p_Data);  }
     void push_Data_Bit_Onto_Current_Row_I(int p_Data)     { Rows[Push_Index]->push_Data_Bit_I(p_Data);  }
     void push_Data_Bit_Onto_Current_Row_F(float p_Data)   { Rows[Push_Index]->push_Data_Bit_F(p_Data);  }
     void push_Data_Bit_Onto_Current_Row_NR(void * p_Data) { Rows[Push_Index]->push_Data_Bit_NR(p_Data); }
     void push_Data_Bit_Onto_Current_Row_B(bool p_Data)    { Rows[Push_Index]->push_Data_Bit_B(p_Data);  }
     
     
     //--     PUSHING DATASETS ONTO THE CURRENT ROW
     
     //Pushes a data set onto the current row.
     void push_Data_Chunk_Onto_Current_Row_S(string p_Data)                      { Rows[Push_Index]->push_Data_Chunk_S(p_Data); }
     void push_Data_Chunk_Onto_Current_Row_I(int p_Data[], int p_Data_Depth)     { Rows[Push_Index]->push_Data_Chunk_I(p_Data, p_Data_Depth); }
     void push_Data_Chunk_Onto_Current_Row_F(float p_Data[], int p_Data_Depth)   { Rows[Push_Index]->push_Data_Chunk_F(p_Data, p_Data_Depth); }
     void push_Data_Chunk_Onto_Current_Row_NR(void * p_Data[], int p_Data_Depth) { Rows[Push_Index]->push_Data_Chunk_NR(p_Data, p_Data_Depth); }
     void push_Data_Chunk_Onto_Current_Row_B(bool p_Data[], int p_Data_Depth)    { Rows[Push_Index]->push_Data_Chunk_B(p_Data, p_Data_Depth); }
     void push_Data_Chunk_Onto_Current_Row(u_Data p_Data[], int p_Data_Depth, int p_Data_Type){ Rows[Push_Index]->push_Data_Chunk(p_Data, p_Data_Depth, p_Data_Type); }
     
     //--     PUSHING DATA ONTO THE CURRENT ROW THEN POPPING THE ROW
     
     //Pushes a data set onto the current row and then pops it.
     void push_Data_Set_S(string p_Data)                     { Rows[Push_Index]->push_Data_Chunk_S(p_Data);                  pop_Row(); }
     void push_Data_Set_I(int p_Data[], int p_Data_Depth)    { Rows[Push_Index]->push_Data_Chunk_I(p_Data, p_Data_Depth);    pop_Row(); }
     void push_Data_Set_F(float p_Data[], int p_Data_Depth)  { Rows[Push_Index]->push_Data_Chunk_F(p_Data, p_Data_Depth);    pop_Row(); }
     void push_Data_Set_NR(void * p_Data[], int p_Data_Depth){ Rows[Push_Index]->push_Data_Chunk_NR(p_Data, p_Data_Depth);   pop_Row(); }
     void push_Data_Set_B(bool p_Data[], int p_Data_Depth)   { Rows[Push_Index]->push_Data_Chunk_B(p_Data, p_Data_Depth);    pop_Row(); }
     void push_Data_Set(u_Data p_Data[], int p_Data_Depth, int p_Data_Type){ Rows[Push_Index]->push_Data_Chunk(p_Data, p_Data_Depth, p_Data_Type); pop_Row(); }
     
     //--     SETTING A GIVEN CELLS DATA IN A GIVEN ROW, SINGLE VALUE
     
     //Sets a given cells data to a given value in a given row.
     void set_Data_Bit_For_Given_Cell_In_Given_Row_S (int p_Row, int p_Cell,  string p_Data){ xTble(p_Row + 1); Rows[p_Row]->set_Given_Cell_Data_Bit_S(p_Cell, p_Data);  }
     void set_Data_Bit_For_Given_Cell_In_Given_Row_C (int p_Row, int p_Cell, char p_Data)   { xTble(p_Row + 1); Rows[p_Row]->set_Given_Cell_Data_Bit_C(p_Cell, p_Data);  }
     void set_Data_Bit_For_Given_Cell_In_Given_Row_I (int p_Row, int p_Cell, int p_Data)    { xTble(p_Row + 1); Rows[p_Row]->set_Given_Cell_Data_Bit_I(p_Cell, p_Data);  }
     void set_Data_Bit_For_Given_Cell_In_Given_Row_F (int p_Row, int p_Cell, float p_Data)  { xTble(p_Row + 1); Rows[p_Row]->set_Given_Cell_Data_Bit_F(p_Cell, p_Data);  }
     void set_Data_Bit_For_Given_Cell_In_Given_Row_NR(int p_Row, int p_Cell, void * p_Data) { xTble(p_Row + 1); Rows[p_Row]->set_Given_Cell_Data_Bit_NR(p_Cell, p_Data); }
     void set_Data_Bit_For_Given_Cell_In_Given_Row_B (int p_Row, int p_Cell, bool p_Data)   { xTble(p_Row + 1); Rows[p_Row]->set_Given_Cell_Data_Bit_B(p_Cell, p_Data);  }
     void set_Data_Bit_For_Given_Cell_In_Given_Row   (int p_Row, int p_Cell, u_Data p_Data, int p_Data_Type){ xTble(p_Row + 1); Rows[p_Row]->set_Given_Cell_Data_Bit(p_Cell, p_Data, p_Data_Type); }
     
     //--     SETTING A GIVEN DATA SET FOR A GIVEN CELL IN A GIVEN ROW
     
     void set_Data_Chunk_For_Given_Cell_In_Given_Row_S (int p_Row, int p_Cell, string p_Data){                      xTble(p_Row+1); Rows[p_Row]->set_Given_Cell_Data_Chunk_S (p_Cell, p_Data); }
     void set_Data_Chunk_For_Given_Cell_In_Given_Row_C (int p_Row, int p_Cell, char p_Data[],   int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->set_Given_Cell_Data_Chunk_C (p_Cell, p_Data, p_Data_Length); }
     void set_Data_Chunk_For_Given_Cell_In_Given_Row_I (int p_Row, int p_Cell, int p_Data[],    int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->set_Given_Cell_Data_Chunk_I (p_Cell, p_Data, p_Data_Length); }
     void set_Data_Chunk_For_Given_Cell_In_Given_Row_F (int p_Row, int p_Cell, float p_Data[],  int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->set_Given_Cell_Data_Chunk_F (p_Cell, p_Data, p_Data_Length); }
     void set_Data_Chunk_For_Given_Cell_In_Given_Row_NR(int p_Row, int p_Cell, void * p_Data[], int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->set_Given_Cell_Data_Chunk_NR(p_Cell, p_Data, p_Data_Length); }
     void set_Data_Chunk_For_Given_Cell_In_Given_Row_B (int p_Row, int p_Cell, bool p_Data[],   int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->set_Given_Cell_Data_Chunk_B (p_Cell, p_Data, p_Data_Length); }
     void set_Data_Chunk_For_Given_Cell_In_Given_Row   (int p_Row, int p_Cell, u_Data p_Data[], int p_Data_Length, int p_Data_Type){ xTble(p_Row+1); Rows[p_Row]->set_Given_Cell_Data_Chunk(p_Cell, p_Data, p_Data_Length, p_Data_Type); }
     
     //--     PUSHES A Chunk ONTO CURRENT ROW
     
     void push_Data_Chunk_Onto_Given_Row_S (int p_Row, string p_Data){                      xTble(p_Row+1); Rows[p_Row]->push_Data_Chunk_S (p_Data); }
     void push_Data_Chunk_Onto_Given_Row_I (int p_Row, int p_Data[],    int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->push_Data_Chunk_I (p_Data, p_Data_Length); }
     void push_Data_Chunk_Onto_Given_Row_F (int p_Row, float p_Data[],  int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->push_Data_Chunk_F (p_Data, p_Data_Length); }
     void push_Data_Chunk_Onto_Given_Row_NR(int p_Row, void * p_Data[], int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->push_Data_Chunk_NR(p_Data, p_Data_Length); }
     void push_Data_Chunk_Onto_Given_Row_B (int p_Row, bool p_Data[],   int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->push_Data_Chunk_B (p_Data, p_Data_Length); }
     void push_Data_Chunk_Onto_Given_Row   (int p_Row, u_Data p_Data[], int p_Data_Length, int p_Data_Type){ xTble(p_Row+1); Rows[p_Row]->push_Data_Chunk(p_Data, p_Data_Length, p_Data_Type); }
     
     
     //--     PUSHES A BIT ONTO CURRENT ROW
     
     void push_Data_Bit_Onto_Given_Row_S (int p_Row, string p_Data){ xTble(p_Row+1); Rows[p_Row]->push_Data_Bit_S (p_Data); }
     void push_Data_Bit_Onto_Given_Row_I (int p_Row, int p_Data)   { xTble(p_Row+1); Rows[p_Row]->push_Data_Bit_I (p_Data); }
     void push_Data_Bit_Onto_Given_Row_F (int p_Row, float p_Data) { xTble(p_Row+1); Rows[p_Row]->push_Data_Bit_F (p_Data); }
     void push_Data_Bit_Onto_Given_Row_NR(int p_Row, void * p_Data){ xTble(p_Row+1); Rows[p_Row]->push_Data_Bit_NR(p_Data); }
     void push_Data_Bit_Onto_Given_Row_B (int p_Row, bool p_Data)  { xTble(p_Row+1); Rows[p_Row]->push_Data_Bit_B (p_Data); }
     void push_Data_Bit_Onto_Given_Row   (int p_Row, u_Data p_Data, int p_Data_Type){ xTble(p_Row+1); Rows[p_Row]->push_Data_Bit(p_Data, p_Data_Type); }
     
     
     //--     GET FUNCTIONS
     
     //Gets the number of rows in the table.
     int get_Row_Count()
     {
          return Row_Count;
     }
     
     //Gets the current number of cells in a given row.
     int get_Row_Cell_Count(int p_Row)
     {
          if (p_Row >= Row_Count){ return 0; }
          
          return Rows[p_Row]->get_Cell_Count();
     }
     
     //Gets the current number of cells in the current row.
     int get_Current_Row_Cell_Count()
     {
          return get_Row_Cell_Count(Push_Index);
     }
     
     //Gets a reference to a row.
     void * get_Row_Reference(int p_Row)
     {
          expand_Table(p_Row + 1);
          
          return Rows[p_Row];
     }
     
     //Gets a reference to a cell.
     void * get_Cell_Reference(int p_Row, int p_Cell)
     {
          expand_Table(p_Row + 1);
          
          return Rows[p_Row]->get_Cell_Reference(p_Cell);
     }
     
     //Gets a reference to the current cell in the given row.
     void * get_Current_Cell_Reference(int p_Row)
     {
          return get_Cell_Reference(p_Row, get_Current_Cell_In_Given_Row(p_Row));
     }
     
     //Gets the current row.
     int get_Current_Row()
     {
          return Push_Index;
     }
     
     //Gets the current cells index in the current row.
     int get_Current_Cell()
     {
          return Rows[Push_Index]->get_Push_Index();
     }
     
     int get_Current_Cell_In_Given_Row(int p_Row)
     {
          if (!bounds(p_Row)){ return 0; }
          return Rows[p_Row]->get_Push_Index();
     }
     
     void * get_Current_Cell_Reference_In_Given_Row(int p_Row)
     {
          if (!bounds(p_Row)){ return NULL; }
          return get_Cell_Reference(p_Row, Rows[p_Row]->get_Push_Index());
     }
     
     //--    GETS THE DATA LENGTH
     
     //Gets the Data_Length for a given cell.
     int get_Data_Depth_For_Given_Cell_In_Given_Row(int p_Row, int p_Cell)
     {
          expand_Table(p_Row + 1);
          
          return Rows[p_Row]->get_Given_Cells_Data_Depth(p_Cell);
     }
     
     //Gets the data depth for the current cell in the given row.
     int get_Data_Depth_For_Current_Cell_In_Given_Row(int p_Row)
     {
          expand_Table(p_Row + 1);
          
          return Rows[p_Row]->get_Current_Cells_Data_Depth();
     }
     
     //Gets the data depth for the given cell in the current row.
     int get_Data_Depth_For_Given_Cell_In_Current_Row(int p_Cell)
     {
          return get_Data_Depth_For_Given_Cell_In_Given_Row(Push_Index, p_Cell);
     }
     
     //Gets the data depth for the current cell in the current row.
     int get_Data_Depth_For_Current_Cell_In_Current_Row()
     {
          return get_Data_Depth_For_Current_Cell_In_Given_Row(Push_Index);
     }     
     
     //--     GETS THE SET STATE FOR A CELL
     
     //Gets the Set_State for a given cell.
     int get_Set_State_For_Given_Cell_In_Given_Row(int p_Row, int p_Cell)
     {
          if (p_Row >= Row_Count){ return 0; }
          
          return Rows[p_Row]->get_Given_Cells_Set_State(p_Cell);
     }
     
     //Gets the Set_State for the current cell in the given row.
     int get_Set_State_For_Current_Cell_In_Given_Row(int p_Row)
     {
          if (p_Row >= Row_Count){ return 0; }
          
          return Rows[p_Row]->get_Current_Cells_Set_State();
     }
     
     //Gets the Set_State for the given cell in the current row.
     int get_Set_State_For_Given_Cell_In_Current_Row(int p_Cell)
     {
          return get_Set_State_For_Given_Cell_In_Given_Row(Push_Index, p_Cell);
     }
     
     //Gets the Set_State for the current cell in the current row.
     int get_Set_State_For_Current_Cell_In_Current_Row()
     {
          return get_Set_State_For_Current_Cell_In_Given_Row(Push_Index);
     }
     
     //Gets the Data_Type for a given cell.
     int get_Data_Type_For_Given_Cell_In_Given_Row(int p_Row, int p_Cell)
     {
          if (p_Row >= Row_Count){ return 0; }
          
          return Rows[p_Row]->get_Given_Cells_Data_Type(p_Cell);
     }
     
     //--     GETS DATA FROM A GIVEN CELL IN A GIVEN ROW
     //Data aquired through these functions is by passing and array reference to it and setting the data.
     
     //String
     void get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(int p_Row, int p_Cell, string * p_String)
     {
          if (p_Row >= Row_Count){ return; }
          
          Rows[p_Row]->get_Given_Cells_Data_Chunk_Passed_S(p_Cell, p_String);
     }
     
     //Character
     void get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_C(int p_Row, int p_Cell, char * &p_Passed_Data)
     {
          if (p_Row >= Row_Count){ return; }
          
          Rows[p_Row]->get_Given_Cells_Data_Chunk_Passed_C(p_Cell, p_Passed_Data);
     }
     
     //Integer
     void get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_I(int p_Row, int p_Cell, int * &p_Passed_Data)
     {
          if (p_Row >= Row_Count){ return; }
          
          Rows[p_Row]->get_Given_Cells_Data_Chunk_Passed_I(p_Cell, p_Passed_Data);
     }
     
     //Float
     void get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_F(int p_Row, int p_Cell, float * &p_Passed_Data)
     {
          if (p_Row >= Row_Count){ return; }
          
          Rows[p_Row]->get_Given_Cells_Data_Chunk_Passed_F(p_Cell, p_Passed_Data);
     }
     
     //Node Reference
     void get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_NR(int p_Row, int p_Cell, void ** &p_Passed_Data)
     {
          if (p_Row >= Row_Count){ return; }
          
          Rows[p_Row]->get_Given_Cells_Data_Chunk_Passed_NR(p_Cell, p_Passed_Data);
     }
     
     //Bool
     void get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_B(int p_Row, int p_Cell, bool * &p_Passed_Data)
     {
          if (p_Row >= Row_Count){ return; }
          
          Rows[p_Row]->get_Given_Cells_Data_Chunk_Passed_B(p_Cell, p_Passed_Data);
     }
     
     //--     GETS THE RAW u_Data FROM A GIVEN CELL IN A GIVEN ROW
     
     u_Data * get_Data_Chunk_From_Given_Cell_In_Given_Row(int p_Row, int p_Cell)
     {
          if (p_Row >= Row_Count){ return 0; }
          
          return Rows[p_Row]->get_Given_Cells_Data(p_Cell);
     }
     
     //--     GETS A DATA BIT FROM A GIVEN CELL IN A GIVEN ROW AT THE GIVEN INDEX
     
     //Char
     char get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_C(int p_Row, int p_Cell, int p_Index)
     {
          if (bounds(p_Row))
          {
               return Rows[p_Row]->get_Data_Bit_At_Given_Index_In_Given_Cell_C(p_Cell, p_Index);
          }
          return char (0);
     }
     
     //Int
     int get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(int p_Row, int p_Cell, int p_Index)
     {
          if (bounds(p_Row))
          {
               return Rows[p_Row]->get_Data_Bit_At_Given_Index_In_Given_Cell_I(p_Cell, p_Index);
          }
          return 0;
     }
     
     //Float
     float get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_F(int p_Row, int p_Cell, int p_Index)
     {
          if (bounds(p_Row))
          {
               return Rows[p_Row]->get_Data_Bit_At_Given_Index_In_Given_Cell_F(p_Cell, p_Index);
          }
          return 0.0;
     }
     
     //Node Reference
     void * get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_NR(int p_Row, int p_Cell, int p_Index)
     {
          if (bounds(p_Row))
          {
               return Rows[p_Row]->get_Data_Bit_At_Given_Index_In_Given_Cell_NR(p_Cell, p_Index);
          }
          return NULL;
     }
     
     //Bool
     bool get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_B(int p_Row, int p_Cell, int p_Index)
     {
          if (bounds(p_Row))
          {
               return Rows[p_Row]->get_Data_Bit_At_Given_Index_In_Given_Cell_B(p_Cell, p_Index);
          }
          return false;
     }
     
     //u_Data
     u_Data get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row(int p_Row, int p_Cell, int p_Index)
     {
          if (bounds(p_Row))
          {
               return Rows[p_Row]->get_Data_Bit_At_Given_Index_In_Given_Cell(p_Cell, p_Index);
          }
          u_Data tmp_Data;
          tmp_Data.I = 0;
          return tmp_Data;
     }
     
     
     //==--     BUBBLE SORT
     
     string get_Table_Data_Type()
     {
          //0 = int
          //1 = float
          //2 = string
          
          int tmp_Datatype = 0;
          int tmp_gDatatype = 0;
          string tmp_Data;
          
          for (int cou_Index=0;cou_Index<Row_Count;cou_Index++)
          {
               Rows[cou_Index]->get_Given_Cells_Data_Chunk_Passed_S(0, &tmp_Data);
               tmp_gDatatype = gdti(tmp_Data);
               
               if (tmp_gDatatype == 2){ return "string"; }
               if (tmp_Datatype < tmp_gDatatype)
               {
                    tmp_Datatype = tmp_gDatatype;
               }
          }
          
          if (tmp_Datatype == 0){ return "int"; }
          if (tmp_Datatype == 1){ return "float"; }
          
          return "string";
     }
     
     void bubble_Sort()
     {
          string tmp_Type = get_Table_Data_Type();
          
          if (tmp_Type == "string"){ bubble_Sort_String(); }
          if (tmp_Type == "int"){ bubble_Sort_Int(); }
          if (tmp_Type == "float"){ bubble_Sort_Float(); }
     }
     
     void bubble_Sort_String()
     {
          
          c_Row_1D tmp_Row;
          bool flg_Not_Done = 1;
          string Row_Low_Index;
          string Row_High_Index;
          
          while (flg_Not_Done)
          {
               flg_Not_Done = 0;
               
               for (int cou_Index=0;cou_Index<(Row_Count - 1);cou_Index++)
               {
                     Rows[cou_Index]->get_Given_Cells_Data_Chunk_Passed_S(0, &Row_Low_Index);
                     Rows[cou_Index + 1]->get_Given_Cells_Data_Chunk_Passed_S(0, &Row_High_Index);
                    
                    //Sorts High - Low
                    if (Row_Low_Index < Row_High_Index)
                    {
                         /*
                         copy_Row(&tmp_Row, Rows[cou_Index]);
                         
                         copy_Row(Rows[cou_Index], Rows[cou_Index + 1]);
                         copy_Row(Rows[cou_Index + 1], &tmp_Row);
                         */
                         
                         swap_Row_Reference(cou_Index, (cou_Index + 1));
                         
                         flg_Not_Done = 1;
                    }
               }
          }
     }
     void bubble_Sort_Int()
     {
          
          c_Row_1D tmp_Row;
          bool flg_Not_Done = 1;
          string Row_Low_Index;
          string Row_High_Index;
          
          while (flg_Not_Done)
          {
               flg_Not_Done = 0;
               
               for (int cou_Index=0;cou_Index<(Row_Count - 1);cou_Index++)
               {
                     Rows[cou_Index]->get_Given_Cells_Data_Chunk_Passed_S(0, &Row_Low_Index);
                     Rows[cou_Index + 1]->get_Given_Cells_Data_Chunk_Passed_S(0, &Row_High_Index);
                    
                    //Sorts High - Low
                    if (str2int(Row_Low_Index) < str2int(Row_High_Index))
                    {
                         /*
                         copy_Row(&tmp_Row, Rows[cou_Index]);
                         
                         copy_Row(Rows[cou_Index], Rows[cou_Index + 1]);
                         copy_Row(Rows[cou_Index + 1], &tmp_Row);
                         */
                         
                         swap_Row_Reference(cou_Index, (cou_Index + 1));
                         
                         flg_Not_Done = 1;
                    }
               }
          }
     }
     
     void bubble_Sort_Raw_Int()
     {
          
          c_Row_1D tmp_Row;
          bool flg_Not_Done = 1;
          int Row_Low_Index;
          int Row_High_Index;
          
          while (flg_Not_Done)
          {
               flg_Not_Done = 0;
               
               for (int cou_Index=0;cou_Index<(Row_Count - 1);cou_Index++)
               {
                     Row_Low_Index = Rows[cou_Index]->get_Data_Bit_At_Given_Index_In_Given_Cell_I(0, 0);
                     Row_High_Index = Rows[cou_Index + 1]->get_Data_Bit_At_Given_Index_In_Given_Cell_I(0, 0);
                    
                    //Sorts High - Low
                    if (Row_Low_Index < Row_High_Index)
                    {
                         /*
                         copy_Row(&tmp_Row, Rows[cou_Index]);
                         
                         copy_Row(Rows[cou_Index], Rows[cou_Index + 1]);
                         copy_Row(Rows[cou_Index + 1], &tmp_Row);
                         */
                         
                         swap_Row_Reference(cou_Index, (cou_Index + 1));
                         
                         flg_Not_Done = 1;
                    }
               }
          }
     }
     
     void bubble_Sort_Float()
     {
          
          c_Row_1D tmp_Row;
          bool flg_Not_Done = 1;
          string Row_Low_Index;
          string Row_High_Index;
          
          while (flg_Not_Done)
          {
               flg_Not_Done = 0;
               
               for (int cou_Index=0;cou_Index<(Row_Count - 1);cou_Index++)
               {
                     Rows[cou_Index]->get_Given_Cells_Data_Chunk_Passed_S(0, &Row_Low_Index);
                     Rows[cou_Index + 1]->get_Given_Cells_Data_Chunk_Passed_S(0, &Row_High_Index);
                    
                    //Sorts High - Low
                    if (str2float(Row_Low_Index) < str2float(Row_High_Index))
                    {
                         /*
                         copy_Row(&tmp_Row, Rows[cou_Index]);
                         
                         copy_Row(Rows[cou_Index], Rows[cou_Index + 1]);
                         copy_Row(Rows[cou_Index + 1], &tmp_Row);
                         */
                         
                         swap_Row_Reference(cou_Index, (cou_Index + 1));
                         
                         
                         flg_Not_Done = 1;
                    }
               }
          }
     }
     
     void bubble_Sort_Raw_Float()
     {
          
          c_Row_1D tmp_Row;
          bool flg_Not_Done = 1;
          float Row_Low_Index;
          float Row_High_Index;
          
          while (flg_Not_Done)
          {
               flg_Not_Done = 0;
               
               for (int cou_Index=0;cou_Index<(Row_Count - 1);cou_Index++)
               {
                     Row_Low_Index = Rows[cou_Index]->get_Data_Bit_At_Given_Index_In_Given_Cell_F(0, 0);
                     Row_High_Index = Rows[cou_Index + 1]->get_Data_Bit_At_Given_Index_In_Given_Cell_F(0, 0);
                    
                    //Sorts High - Low
                    if (Row_Low_Index < Row_High_Index)
                    {
                         /*
                         copy_Row(&tmp_Row, Rows[cou_Index]);
                         
                         copy_Row(Rows[cou_Index], Rows[cou_Index + 1]);
                         copy_Row(Rows[cou_Index + 1], &tmp_Row);
                         */
                         
                         swap_Row_Reference(cou_Index, (cou_Index + 1));
                         
                         flg_Not_Done = 1;
                    }
               }
          }
     }
     
     

     
     //--    ARRAY MANIPULATION FUNCTIONS
     
     //Pops a cell in the given row.
     void pop_Cell(int p_Row)
     {
          if (p_Row >= Row_Count){ return; }
          
          Rows[p_Row]->pop_Cell();
     }
     
     //Pops a cell in the current push row.
     void pop_Cell_In_Current_Row()
     {
          Rows[Push_Index]->pop_Cell();
     }
     
     //Pops a row.
     void pop_Row()
     {
          Push_Index++;
          if (Push_Index >= Row_Count){ expand_Table(Push_Index + 1); }
     }
     
     void xTble()
     {
          expand_Table(Row_Count + 1);
     }
     
     void xTble(int p_Depth)
     {
          expand_Table(p_Depth);
     }
     
     //Resizes the table to the given dimension.
     void expand_Table()
     {
          expand_Table(Row_Count + 1);
     }
     
     //Resizes the table to the given dimension.
     void expand_Table(int p_Depth)
     {
          //If the given depth is less than the current depth then return as this would be a pointless operation.
          if (p_Depth <= Row_Count){ return; }
          
          //Setup the temporaray array to hold the rows.
          c_Row_1D ** tmp_Rows = new c_Row_1D*[Row_Count];
          
          //Copy the old row references into the temporary array.
          for (int cou_Index=0;cou_Index<Row_Count;cou_Index++)
          {
               tmp_Rows[cou_Index] = Rows[cou_Index];
               Rows[cou_Index] = NULL;
          }
          
          //Reallocate the rows array.
          delete [] Rows;
          Rows = new c_Row_1D*[p_Depth];
          
          //Copy the tmp into the reallocated row.
          for (int cou_Index=0;cou_Index<Row_Count;cou_Index++)
          {
               Rows[cou_Index] = tmp_Rows[cou_Index];
               tmp_Rows[cou_Index] = NULL;
          }
          
          //Ini the new rows.
          for (int cou_Index=Row_Count;cou_Index<p_Depth;cou_Index++)
          {
               Rows[cou_Index] = new c_Row_1D;
          }
          
          //Set the row count.
          Row_Count = p_Depth;
          
          //Cleanup
          delete [] tmp_Rows;
          
     }
     
     //resets the table.
     void reset_Table()
     {
          //cout << "\n\t\t Resetting Table->" << Table_Name;
          for (int cou_Index=0;cou_Index<Row_Count;cou_Index++)
          {
               delete Rows[cou_Index];
          }
          delete [] Rows;
          Rows = NULL;
          
          Rows = new c_Row_1D*[1];
          Rows[0] = new c_Row_1D;
          Row_Count = 1;
          Push_Index = 0;
          
          //cout << "    T_EOF->" << table_Is_Empty();
     }
     
     
     //--     FUNCTIONS FOR THE COMMAND PARSER TO USE
     
     
     //Pushes a data chunk onto the given row.
     void push_pop_string(   string p_String){ push_Data_Chunk_Onto_Given_Row_S(Push_Index, p_String); pop_Row(); }
     void push_pop_int(      int p_Int)      { push_Data_Bit_Onto_Given_Row_I(Push_Index, p_Int); pop_Row(); }
     void push_pop_float(    float p_Float)  { push_Data_Bit_Onto_Given_Row_F(Push_Index, p_Float); pop_Row(); }
     void push_pop_reference(void * p_Void)  { push_Data_Bit_Onto_Given_Row_NR(Push_Index, p_Void); pop_Row(); }
     void push_pop_bool(     bool p_Bool)    { push_Data_Bit_Onto_Given_Row_B(Push_Index, p_Bool); pop_Row(); }
     
     //Pushes a data chunk onto the given row.
     void pop_push_string(   string p_String){ pop_Row(); push_Data_Chunk_Onto_Given_Row_S(Push_Index, p_String); }
     void pop_push_int(      int p_Int)      { pop_Row(); push_Data_Bit_Onto_Given_Row_I(Push_Index, p_Int); }
     void pop_push_float(    float p_Float)  { pop_Row(); push_Data_Bit_Onto_Given_Row_F(Push_Index, p_Float); }
     void pop_push_reference(void * p_Void)  { pop_Row(); push_Data_Bit_Onto_Given_Row_NR(Push_Index, p_Void); }
     void pop_push_bool(     bool p_Bool)    { pop_Row(); push_Data_Bit_Onto_Given_Row_B(Push_Index, p_Bool); }
     
     //Pushes a data chunk onto the current row.
     void pushc_string(   string p_String){ push_Data_Chunk_Onto_Given_Row_S(Push_Index, p_String); }
     void pushc_int(      int p_Int)      { push_Data_Bit_Onto_Given_Row_I(Push_Index, p_Int); }
     void pushc_float(    float p_Float)  { push_Data_Bit_Onto_Given_Row_F(Push_Index, p_Float); }
     void pushc_reference(void * p_Void)  { push_Data_Bit_Onto_Given_Row_NR(Push_Index, p_Void); }
     void pushc_bool(     bool p_Bool)    { push_Data_Bit_Onto_Given_Row_B(Push_Index, p_Bool); }
     
     //Pushes a data chunk onto the given row.
     void push_string(   int p_Row, string p_String){ push_Data_Chunk_Onto_Given_Row_S(p_Row, p_String); }
     void push_int(      int p_Row, int p_Int)      { push_Data_Bit_Onto_Given_Row_I(p_Row, p_Int); }
     void push_float(    int p_Row, float p_Float)  { push_Data_Bit_Onto_Given_Row_F(p_Row, p_Float); }
     void push_reference(int p_Row, void * p_Void)  { push_Data_Bit_Onto_Given_Row_NR(p_Row, p_Void); }
     void push_bool(     int p_Row, bool p_Bool)    { push_Data_Bit_Onto_Given_Row_B(p_Row, p_Bool); }
     
     //Sets a given rows cell to the given data.
     void set_string(   int p_Row, int p_Cell, string p_String){ set_Data_Chunk_For_Given_Cell_In_Given_Row_S(p_Row, p_Cell, p_String); }
     void set_int(      int p_Row, int p_Cell, int p_Int)      { set_Data_Bit_For_Given_Cell_In_Given_Row_I(p_Row, p_Cell, p_Int); }
     void set_float(    int p_Row, int p_Cell, float p_Float)  { set_Data_Bit_For_Given_Cell_In_Given_Row_F(p_Row, p_Cell, p_Float); }
     void set_reference(int p_Row, int p_Cell, void * p_Void)  { set_Data_Bit_For_Given_Cell_In_Given_Row_NR(p_Row, p_Cell, p_Void); }
     void set_bool(     int p_Row, int p_Cell, bool p_Bool)    { set_Data_Bit_For_Given_Cell_In_Given_Row_B(p_Row, p_Cell, p_Bool); }
     
     //Gets a given cells data in a given row.
     string get_string(   int p_Row, int p_Cell){ string tmp_String = ""; get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(p_Row, p_Cell, &tmp_String); return tmp_String; }
     int    get_int(      int p_Row, int p_Cell, int p_Index=0){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(p_Row, p_Cell, p_Index); }
     float  get_float(    int p_Row, int p_Cell, int p_Index=0){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_F(p_Row, p_Cell, p_Index); }
     void * get_reference(int p_Row, int p_Cell, int p_Index=0){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_NR(p_Row, p_Cell, p_Index); }
     bool   get_bool(     int p_Row, int p_Cell, int p_Index=0){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_B(p_Row, p_Cell, p_Index); }
     u_Data get_data(     int p_Row, int p_Cell, int p_Index=0){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row(p_Row, p_Cell, p_Index); }
     
     //Gets the current cells data in the current row.
     string getc_string(   ){ string tmp_String = ""; get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(Push_Index, Rows[Push_Index]->get_Push_Index(), &tmp_String); return tmp_String; }
     int    getc_int(      ){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(Push_Index, Rows[Push_Index]->get_Push_Index(), 0); }
     float  getc_float(    ){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_F(Push_Index, Rows[Push_Index]->get_Push_Index(), 0); }
     void * getc_reference(){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_NR(Push_Index, Rows[Push_Index]->get_Push_Index(), 0); }
     bool   getc_bool(     ){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_B(Push_Index, Rows[Push_Index]->get_Push_Index(), 0); }
     u_Data getc_data(     ){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row(Push_Index, Rows[Push_Index]->get_Push_Index(), 0); }
     
     //Copies the data from the submitted cell into itself.
     string copy_Cell(int p_Row, int p_Cell, void * p_Cell_To_Copy)
     {
          if (p_Cell_To_Copy == NULL){ cout << "\n\n\t\t ARG_ERR_NULL_POINTER->TABLE_COPY->p_Cell_To_Copy"; return "ARG_ERR_NULL_POINTER->TABLE_COPY->p_Cell_To_Copy"; }
          
          expand_Table(p_Row + 1);
          
          //Gets a reference to the receiving cell.
          c_Cell_1D * tmp_Cell = (c_Cell_1D*) Rows[p_Row]->get_Cell_Reference(p_Cell);
          
          //Copy the cells.
          tmp_Cell->set(((c_Cell_1D*) p_Cell_To_Copy)->get_Cell_Data(), ((c_Cell_1D*) p_Cell_To_Copy)->get_Data_Depth(), ((c_Cell_1D*) p_Cell_To_Copy)->get_flg_Data_Type());
          
          
          return "CODE_4";
     }
     
     //Copies the data from the submitted row into itself.
     string copy_Row(int p_Row, void * p_Row_To_Copy)
     {
          if (p_Row_To_Copy == NULL){ cout << "\n\n\t\t ARG_ERR_NULL_POINTER->TABLE_COPY->p_Row_To_Copy"; return "ARG_ERR_NULL_POINTER->TABLE_COPY->p_Row_To_Copy"; }
          
          expand_Table(p_Row + 1);
          
          //For readability cast the void to a row.
          c_Row_1D * tmp_Row_To_Copy = ((c_Row_1D*) p_Row_To_Copy);
          
          //Wipe the row that the data will be copied into.
          Rows[p_Row]->reset_Row();
          
          
          //For readability the data for the cell will be stored in the following temporary members.
          //void set_Given_Cell_Data_Chunk(int p_Cell, u_Data p_Data[], int p_Data_Length, int p_Data_Type)
          u_Data * tmp_Data;
          int tmp_Data_Depth;
          int tmp_flg_Data_Type;
               
          //Copy the cells.
          for (int cou_Index=0;cou_Index<tmp_Row_To_Copy->get_Cell_Count();cou_Index++)
          {
               //Gets the data from the current cell.
               tmp_Data = tmp_Row_To_Copy->get_Given_Cells_Data(cou_Index);
               tmp_Data_Depth = tmp_Row_To_Copy->get_Given_Cells_Data_Depth(cou_Index);
               tmp_flg_Data_Type = tmp_Row_To_Copy->get_Given_Cells_Data_Type(cou_Index);
               
               Rows[p_Row]->set_Given_Cell_Data_Chunk(cou_Index, tmp_Data, tmp_Data_Depth, tmp_flg_Data_Type);
          }
          
          
          
          return "CODE_4";
     }
     
     //Copies the data from the submitted row into itself.
     string copy_Row(void * p_Row_To_Overwrite, void * p_Row_To_Copy)
     {
          if (p_Row_To_Copy == NULL){ cout << "\n\n\t\t ARG_ERR_NULL_POINTER->TABLE_COPY->p_Row_To_Copy"; return "ARG_ERR_NULL_POINTER->TABLE_COPY->p_Row_To_Copy"; }
          if (p_Row_To_Overwrite == NULL){ cout << "\n\n\t\t ARG_ERR_NULL_POINTER->TABLE_COPY->p_Row_To_Overwrite"; return "ARG_ERR_NULL_POINTER->TABLE_COPY->p_Row_To_Overwrite"; }
          
          //For readability cast the void to a row.
          c_Row_1D * tmp_Row_To_Copy = ((c_Row_1D*) p_Row_To_Copy);
          c_Row_1D * tmp_Row_To_Overwrite = ((c_Row_1D*) p_Row_To_Overwrite);
          
          //Wipe the row that the data will be copied into.
          tmp_Row_To_Overwrite->reset_Row();
          
          
          //For readability the data for the cell will be stored in the following temporary members.
          //void set_Given_Cell_Data_Chunk(int p_Cell, u_Data p_Data[], int p_Data_Length, int p_Data_Type)
          u_Data * tmp_Data;
          int tmp_Data_Depth;
          int tmp_flg_Data_Type;
               
          //Copy the cells.
          for (int cou_Index=0;cou_Index<tmp_Row_To_Copy->get_Cell_Count();cou_Index++)
          {
               //Gets the data from the current cell.
               tmp_Data = tmp_Row_To_Copy->get_Given_Cells_Data(cou_Index);
               tmp_Data_Depth = tmp_Row_To_Copy->get_Given_Cells_Data_Depth(cou_Index);
               tmp_flg_Data_Type = tmp_Row_To_Copy->get_Given_Cells_Data_Type(cou_Index);
               
               tmp_Row_To_Overwrite->set_Given_Cell_Data_Chunk(cou_Index, tmp_Data, tmp_Data_Depth, tmp_flg_Data_Type);
          }
          
          
          
          return "CODE_4";
     }
     
     string swap_Row_Reference(int p_Row_One, int p_Row_Two)
     {
          c_Row_1D * tmp_Row;
          if (!bounds(p_Row_One) || !bounds(p_Row_Two)){ return "OOB_ERROR"; }
          
          tmp_Row = Rows[p_Row_One];
          Rows[p_Row_One] = Rows[p_Row_Two];
          Rows[p_Row_Two] = tmp_Row;
          tmp_Row = NULL;
          return "SWAP_ROW__1";
     }
     
     //Copies a set of rows from one table into another table.
     string copy_Row_Set(void * p_Table_To_Copy_From, int p_Start, int p_End, void * p_Table_To_Copy_To, int p_Offset)
     {
          void * tmp_Current_Row = NULL;
          //Gets the two tables.
          c_Table_1D * tmp_Table_To_Copy_From;
          tmp_Table_To_Copy_From = ((c_Table_1D*) p_Table_To_Copy_From);
          
          c_Table_1D * tmp_Table_To_Copy_To;
          tmp_Table_To_Copy_To = ((c_Table_1D*) p_Table_To_Copy_To);
          
          for (int cou_Row=0;cou_Row<=(p_End - p_Start);cou_Row++)
          {
               //Get the row we will be copying from.
               tmp_Current_Row = tmp_Table_To_Copy_From->get_Row_Reference(cou_Row + p_Start);
               
               
               //Copy it to the appropiate table with the appropriate offset.
               copy_Row(tmp_Table_To_Copy_To->get_Row_Reference(cou_Row + p_Offset), tmp_Current_Row);
               
          } 
          
          return "success";
     }
     
     //Copys a table into this table.
     virtual string copy_Table(void * p_Table_To_Copy)
     {
          //If now table then return.
          if (p_Table_To_Copy == NULL){ return "NULL_TABLE"; }
          
          //Create the temporary table pointer to use.
          c_Table_1D * tmp_Table = (c_Table_1D*) p_Table_To_Copy;
          
          //Reset this table.
          reset_Table();
          
          for (int cou_Row=0;cou_Row<tmp_Table->get_Row_Count();cou_Row++)
          {
               copy_Row(cou_Row, tmp_Table->get_Row_Reference(cou_Row));
          }
          
          return "1";
     }
     
     string translate_Row(void * p_From_Table, int p_Row, int p_Column, int p_Offset)
     {
          //The current cell to copy from.
          void * tmp_Cell = NULL;
          
          //The table to gather from.
          c_Table_1D * tmp_From_Table = ((c_Table_1D*) p_From_Table);
          
          for (int cou_Index=0;cou_Index<tmp_From_Table->get_Row_Cell_Count(p_Row);cou_Index++)
          {
               //Holds the current cell in the row.
               tmp_Cell = tmp_From_Table->get_Cell_Reference(p_Row, cou_Index);
               //cout << "\n\n\t Current_Cell->";
               
               copy_Cell((p_Offset + cou_Index), p_Column, tmp_Cell); 
               
          }
          
          return "translate_Row";
     }
     
     //Translates a column into a row using push(). Starts at the offset and continues down until either p_Depth or end of Table is hit.
     virtual string translate_Column(void * p_From_Table, int p_Column, int p_Offset, int p_Depth, int p_Row)
     {
          //Bounds checking.
          if (!bounds(p_Row)){ return "OOB_ERROR_translate_Column"; }
          
          //Get the table to draw the column from.
          c_Table_1D * tmp_From_Table = ((c_Table_1D*) p_From_Table);
          
          //Get the ending point.
          int tmp_Limit = (p_Depth + p_Offset);
          
          if (tmp_Limit > tmp_From_Table->get_Row_Count()){ tmp_Limit = tmp_From_Table->get_Row_Count(); }
          
          //Read the from table column into the given row.
          for (int cou_Index=p_Offset;cou_Index<tmp_Limit;cou_Index++)
          {
               copy_Cell(p_Row, get_Current_Cell_In_Given_Row(p_Row), tmp_From_Table->get_Cell_Reference(cou_Index, p_Column));
          }
          return "translate_Row";
     }
     
     //Copies the data from a row into another at a given bit index. The receiving row is not wiped.
     string overlay_Row(int p_Row, int p_Index, void * p_Row_To_Copy)
     {
          if (p_Row_To_Copy == NULL){ cout << "\n\n\t\t ARG_ERR_NULL_POINTER->TABLE_COPY->p_Row_To_Copy"; return "ARG_ERR_NULL_POINTER->TABLE_COPY->p_Row_To_Copy"; }
          
          expand_Table(p_Row + 1);
          
          //For readability cast the void to a row.
          c_Row_1D * tmp_Row_To_Copy = ((c_Row_1D*) p_Row_To_Copy);
          
          
          //For readability the data for the cell will be stored in the following temporary members.
          //void set_Given_Cell_Data_Chunk(int p_Cell, u_Data p_Data[], int p_Data_Length, int p_Data_Type)
          u_Data * tmp_Data;
               
          //Copy the cells.
          for (int cou_Index=0;cou_Index<tmp_Row_To_Copy->get_Cell_Count();cou_Index++)
          {
               //Gets the data from the current cell.
               tmp_Data = tmp_Row_To_Copy->get_Given_Cells_Data(cou_Index);
               
               Rows[p_Row]->set_Given_Cell_Data_Bit_At_Given_Index(cou_Index, p_Index, tmp_Data[0]);
          }
          
          
          
          return "CODE_4";
     }
     
     string shift_Column(int p_Column_To_Shift)
     {
          
          //The current cell to copy from.
          void * tmp_Cell = NULL;
          
          
          for (int cou_Index=0;cou_Index<Row_Count;cou_Index++)
          {
               //Holds the current cell in the row.
               tmp_Cell = Rows[cou_Index]->get_Cell_Reference(p_Column_To_Shift);
               
               //cout << "\n\n\t Current_Cell->";
               
               copy_Cell(cou_Index, (p_Column_To_Shift + 1), tmp_Cell); 
               
               set_string(cou_Index, p_Column_To_Shift, " ");
          }
          return "1";
     }
     
     string shift_All_Rows(int p_Index)
     {
          if (!bounds(p_Index)){ return "0"; }
          
          expand_Table();
          
          for (int cou_Index=(Row_Count - 1);cou_Index>p_Index;cou_Index--)
          {
               copy_Row(cou_Index, Rows[cou_Index-1]);
          }
          
          delete Rows[p_Index];
          Rows[p_Index] = new c_Row_1D;
          
          return "1";
     }
     
     //Rotates a tables data. Rows are translated to columns.
     string rotate_Table()
     {
          c_Table_1D tmp_Table;
          tmp_Table.reset_Table();
          
          for (int cou_Index=0;cou_Index<Row_Count;cou_Index++)
          {
               tmp_Table.translate_Row(this, cou_Index, cou_Index, 0);
          }
          
          reset_Table();
          
          copy_Table(&tmp_Table);
          
          cout << "\n\nRotated Table.";
          output_Table();
          
          return "Table_Rotated";
     }
     
     //Converts all indexes in a table to strings.
     string convert_Table_To_String()
     {
          for (int cou_Index=0;cou_Index<Row_Count;cou_Index++)
          {
               Rows[cou_Index]->convert_Row_To_String();
          }
          return "Converted";
     }
     
     //Bounds checking.
     bool bounds(int p_Row)
     {
          if (p_Row >= Row_Count || p_Row < 0){ return 0; }
          return 1;
     }
     
     //--     OUTPUT FUNCTIONS
     
     //Outputs a given row.
     void output_Row(int p_Row, int p_Limit)
     {
          if (Rows[p_Row] == NULL){ return; }
          if (row_Is_Empty(p_Row)){ cout << " Row Is Empty..."; return; }
          Rows[p_Row]->output_Row_Data(p_Limit);
     }
     
     //Outputs all the rows in the table.
     void output_All_Rows(int p_Limit = 100)
     {
          for (int cou_Index=0;cou_Index<Row_Count - 1;cou_Index++)
          {
               cout << "\n\t    ";
               ochr(0, 8, char (195));
               ostr(0, 7, "<");
               cout << cou_Index;
               ostr(0, 7, ">-");
               output_Row(cou_Index, p_Limit);
          }
          
          cout << "\n\t    ";
          ochr(0, 8, char (192));
          ostr(0, 7, "<");
          cout << Row_Count - 1;
          ostr(0, 7, ">-");
          
          output_Row(Row_Count - 1, p_Limit);
     }
     
     //Outputs a row verbose.
     void output_Row_Verbose(int p_Row)
     {
          Rows[p_Row]->output_Row_Verbose();
     }
     
     //Output all the rows in verbose mode.
     void output_All_Rows_Verbose()
     {
          for (int cou_Index=0;cou_Index<Row_Count - 1;cou_Index++)
          {
               cout << "\n\t    " << char (195) << "<" << cou_Index << ">-";
               output_Row_Verbose(cou_Index);
          }
          cout << "\n\t    " << char (192) << "<" << (Row_Count - 1) << ">-";
          output_Row_Verbose(Row_Count - 1);
     }
     
     //Outputs the table.
     void output_Table()
     {
          cout << "\n\n\t ";
          ostr(0, 15, "Table");
          ostr(0, 7, "[");
          cout << Table_ID;
          ostr(0, 7, "]->");
          ostr(0, 15, Table_Name);
          cout << "\n\t   ";
          if (table_Is_Empty()){ cout << " Table Is Empty..."; return; }
          cout << Row_Count;
          ostr(0, 7, "->");
          cout << Row_Count;
          output_All_Rows();
     }
     
     //Outputs the table in verbose mode.
     void output_Table_Verbose()
     {
          cout << "\n\n\t Table->" << Table_Name;
          cout << "\n\t   Table_ID->" << Table_ID;
          cout << "\n\t   Row_Count->" << Row_Count;
          cout << "\n\t   Push_Index->" << Push_Index;
          output_All_Rows_Verbose();
     }
     
     //Outputs the information for the table without its contained data.
     void output_Table_Header()
     {
          cout << " #Rows" << Row_Count << " " << Table_Name;
     }
     
     //Saves a table int the given mode in the given file, otherwise defaults to current table ID.Table_Name.svc;
     void save_Table(string p_Mode = "csv", string p_Name = "Default")
     {
		 cout << "\n Saving Table: " << p_Name << "." << p_Mode;
          if (p_Name == "Default")
          {
               p_Name = "Tables\\" + int2str(Table_ID) + "." + Table_Name + "." + p_Mode;
          }

          output_Table();
          
          //Open the file to write to.
          ofstream SF;
          SF.open(p_Name.c_str());
          
          //Save the table.
          if (p_Mode == "ssv")
          {
               for (int cou_Row=0;cou_Row<Row_Count;cou_Row++)
               {
                    for (int cou_Index=0;cou_Index<Rows[cou_Row]->get_Cell_Count();cou_Index++)
                    {
                         switch (get_Data_Type_For_Given_Cell_In_Given_Row(cou_Row, cou_Index))
                         {
                              //String
                              case 0:
                                   SF << get_string(cou_Row, cou_Index) << " ";
                                   break;
                                   
                              //Int
                              case 1:
                                   SF << get_int(cou_Row, cou_Index) << " ";
                                   break;
                                   
                              //Float
                              case 2:
                                   SF << get_float(cou_Row, cou_Index) << " ";
                                   break;
                         }
                         
                    }
                    SF << "\n";
               }
          }
          
          if (p_Mode == "csv")
          {
               for (int cou_Row=0;cou_Row<Row_Count;cou_Row++)
               {
                    for (int cou_Index=0;cou_Index<Rows[cou_Row]->get_Cell_Count();cou_Index++)
                    {
                         if (cou_Index > 0){ SF << ", "; }
                         switch (get_Data_Type_For_Given_Cell_In_Given_Row(cou_Row, cou_Index))
                         {
                              //String
                              case 0:
                                   SF << get_string(cou_Row, cou_Index);
                                   break;
                                   
                              //Int
                              case 1:
                                   SF << get_int(cou_Row, cou_Index);
                                   break;
                                   
                              //Float
                              case 2:
                                   SF << get_float(cou_Row, cou_Index);
                                   break;
                         }
                         
                    }
                    SF << ";\n";
               }
          }
          
          //Close the file.
          SF.close();
          
     }

     //Loads a table into the current table wiping out any existing data.
     bool load_Table(string p_Filename, int p_Type = 0)
     {
          //Setup the tmp variables.
          int tmp_Int;
          //float tmp_Float;
          string tmp_String;
		  string tmp_Data;
          string tmp_File_Type;
          
          p_Filename = "Tables\\" + p_Filename;
          cout << "\n\t load_Table: " << p_Filename;
          
          //Open the file to write to.
          ifstream SF;
          SF.open(p_Filename.c_str());
          
          if (!SF.is_open())
          {
			  cout << "\n Failed To Load Table";
               return 0;
          }
          
          //p_Types
          //0: Read checking each entry for type.
          //1: String
          //2: Int
          //3: Float
          //4: NR
          //5: Bool
          
          tmp_Int = p_Filename.size();
          tmp_File_Type = "";
          for (int cou_Index=(tmp_Int-3);cou_Index<(tmp_Int);cou_Index++)
          {
               tmp_File_Type += p_Filename[cou_Index];
          }

		  ifstream tmp_SF;
		  tmp_SF.open(p_Filename);
          
		  if (!tmp_SF.is_open())
		  {
			  cout << "\n File Failed To Open: " << p_Filename;
		  }

          if (tmp_File_Type == "csv")
          {
			  cout << "\n Comma Separated Volume Found...";
			  return 1;
          }
          
		  if (tmp_File_Type == "ssv")
		  {
			  cout << "\n Space Separated Volume Found...";

			  while (!tmp_SF.eof())
			  {
				  tmp_String = "";
				  getline(tmp_SF, tmp_String);
				  cout << "\n ->" << tmp_String << "<-";
				  if ((!table_Is_Empty()) && (tmp_String != "")) { pop_Row(); }

				  int flg_Var_Pos = 0;
				  int flg_String = 0;

				  //Loop through the command text finding variable identifiers and extracting them.
				  for (unsigned int cou_Index = 0; cou_Index < tmp_String.size(); cou_Index++)
				  {
					  //If a space is encountered then start on the next token.
					  if (tmp_String[cou_Index] != ' ')
					  {
						  tmp_Data += tmp_String[cou_Index];
						  continue;
					  }
					  if (tmp_String[cou_Index] == ' ')
					  {
						  if (tmp_Data != "")
						  {
							  pushc_string(tmp_Data);
							  tmp_Data = "";
						  }
					  }

				  }
			  }
			  return 1;
		  }

		  cout << "\n Unrecognized Filetype: " << p_Filename;


          /*
                         switch (get_Data_Type_For_Given_Cell_In_Given_Row(cou_Row, cou_Index))
                         {
                              //String
                              case 0:
                                   SF << get_string(cou_Row, cou_Index);
                                   break;
                                   
                              //Int
                              case 1:
                                   SF << get_int(cou_Row, cou_Index);
                                   break;
                                   
                              //Float
                              case 2:
                                   SF << get_float(cou_Row, cou_Index);
                                   break;
                         }
          */
		  return 1;
     }
};