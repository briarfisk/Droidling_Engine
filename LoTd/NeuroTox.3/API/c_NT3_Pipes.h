

//This is the IO class.
class c_NT3_IO: public c_NT3_Base
{
public:
     
     int Dimension;
     
     //Each table in the construct has a corresponding row here.
     c_Raw_Table_Row_1D * Input;
     c_Raw_Table_Row_1D * Input_Charges;
     
     c_Raw_Table_Row_1D * Pattern_Output;
     c_Raw_Table_Row_1D * Charge_Output;
     c_Raw_Table_Row_1D * RC_Output;
     
     c_Raw_Table_Row_1D * Treetops_Output;
     
     //This flag is used to track whether the row reference is locally created or a reference to a table.
     //If it references a table then deleting it may cause issues with dereferencing from other classes.
     bool flg_Input;
     bool flg_Input_Charges;
     
     bool flg_Pattern_Output;
     bool flg_Charge_Output;
     bool flg_RC_Output;
     
     bool flg_Treetops_Output;
     
     c_NT3_IO()
     {
          reset();
     }
     
     void set_Input_Data(string p_String)                                  { if (Dimension == 1){ Input->reset(); Input->set_String(0, p_String); } }
     void set_Input_Charges_Data(unsigned long long int p_Charge)          { if (Dimension == 1){ Input_Charges->reset(); Input_Charges->set_UInt(0, p_Charge); } }
     void set_Pattern_Output_Data(string p_String)                         { if (Dimension == 1){ Pattern_Output->reset(); Pattern_Output->set_String(0, p_String); } }
     void set_Charge_Output_Data(unsigned long long int p_Charge)          { if (Dimension == 1){ Charge_Output->reset(); Charge_Output->set_UInt(0, p_Charge); } }
     void set_RC_Output_Data(double p_RC)                                  { if (Dimension == 1){ RC_Output->reset(); RC_Output->set_Float(0, p_RC); } }
     void set_Treetops_Output_Data(unsigned long long int p_Treetop)       { if (Dimension == 1){ Treetops_Output->reset(); Treetops_Output->set_UInt(0, p_Treetop); } }
     
     void add_Input_Data(string p_String)                                  { if (Dimension == 1){ Input->reset(); Input->submit_String(p_String); } }
     void add_Input_Charges_Data(unsigned long long int p_Charge)          { if (Dimension == 1){ Input_Charges->reset(); Input_Charges->submit_UInt(p_Charge); } }
     void add_Pattern_Output_Data(string p_String)                         { if (Dimension == 1){ Pattern_Output->reset(); Pattern_Output->submit_String(p_String); } }
     void add_Charge_Output_Data(unsigned long long int p_Charge)          { if (Dimension == 1){ Charge_Output->reset(); Charge_Output->submit_UInt(p_Charge); } }
     void add_RC_Output_Data(double p_RC)                                  { if (Dimension == 1){ RC_Output->reset(); RC_Output->submit_Float(p_RC); } }
     void add_Treetops_Output_Data(unsigned long long int p_Treetop)       { if (Dimension == 1){ Treetops_Output->reset(); Treetops_Output->submit_UInt(p_Treetop); } }
     
     string get_Input_Data(int p_Cell = 0, int p_Index = 0)                          { if (Dimension == 1){ return  Input->Cells[p_Cell]->get_String(); } else { return ""; } }
     unsigned long long int get_Input_Charges_Data(int p_Cell = 0, int p_Index = 0)  { if (Dimension == 1){ return  Input_Charges->Cells[p_Cell]->Data[p_Index].U; } else { return 0; } }
     string get_Pattern_Output_Data(int p_Cell = 0, int p_Index = 0)                 { if (Dimension == 1){ return  Pattern_Output->Cells[p_Cell]->get_String(); } else { return ""; } }
     unsigned long long int get_Charge_Output_Data(int p_Cell = 0, int p_Index = 0)  { if (Dimension == 1){ return  Charge_Output->Cells[p_Cell]->Data[p_Index].U; } else { return 0; } }
     double get_RC_Output_Data(int p_Cell = 0, int p_Index = 0)                      { if (Dimension == 1){ return  RC_Output->Cells[p_Cell]->Data[p_Index].F; } else { return 0.0; } }
     unsigned long long int get_Treetops_Output_Data(int p_Cell = 0, int p_Index = 0){ if (Dimension == 1){ return  Treetops_Output->Cells[p_Cell]->Data[p_Index].U; } else { return 0; } }
     
     
     void set_Dimension(int p_Dimension)
     {
          Dimension = p_Dimension;
     }
     
     /*
     string get_String()
     {
          if (Dimension == 1)
          { 
               if (Row_1D == NULL){ return "NO_DATA"; }
               if (Row_1D->Depth <= 0){ return "NO_DATA"; }
               return Row_1D->Cells[0]->get_String(); 
          }
          if (Dimension == 2)
          { 
               if (Row_2D == NULL){ return "NO_DATA"; }
               if (Row_2D->Depth <= 0){ return "NO_DATA"; }
               return "2D_Cell";// Row_2D->Cells[0]->get_String(); 
          }
          return "NO_DATA";
     }*/
     
     void reset()
     {
          //We do not want to delete the row because if it is attached to a constructs table then the construct may attempt to dereference that row.
          //If no row is found we can create one.
          //if (Row_1D != NULL){ delete Row_1D; Row_1D = NULL; }
          //if (Row_2D != NULL){ delete Row_2D; Row_2D = NULL; }
          
               //overwrite_Row(p_From->Input, Input);
               //overwrite_Row(p_From->Input_Charges, Input_Charges);
               //overwrite_Row(p_From->Pattern_Output, Pattern_Output);
               //overwrite_Row(p_From->Charge_Output, Charge_Output);
               //overwrite_Row(p_From->RC_Output, RC_Output);
               //overwrite_Row(p_From->Treetops_Output, Treetops_Output);
          
          //cout << "\n Input: " << Input << " flg_Input: " <<  flg_Input;
          if (Input == NULL){ Input = new c_Raw_Table_Row_1D; flg_Input = 1; }
          if (Input != NULL){ Input->reset(); }
          
          //cout << " Input_Charges: " << Input_Charges << " flg: " <<  flg_Input_Charges;
          if (Input_Charges == NULL){ Input_Charges = new c_Raw_Table_Row_1D; flg_Input_Charges = 1; }
          if (Input_Charges != NULL){ Input_Charges->reset(); }
          
          //cout << " Pattern_Output: " << Pattern_Output << " flg: " <<  flg_Pattern_Output;
          if (Pattern_Output == NULL){ Pattern_Output = new c_Raw_Table_Row_1D; flg_Pattern_Output = 1; }
          if (Pattern_Output != NULL){ Pattern_Output->reset(); }
          
          //cout << " Charge_Output: " << Charge_Output << " flg: " <<  flg_Charge_Output;
          if (Charge_Output == NULL){ Charge_Output = new c_Raw_Table_Row_1D; flg_Charge_Output = 1; }
          if (Charge_Output != NULL){ Charge_Output->reset(); }
          
          //cout << " RC_Output: " << RC_Output << " flg: " <<  flg_RC_Output;
          if (RC_Output == NULL){ RC_Output = new c_Raw_Table_Row_1D; flg_RC_Output = 1; }
          if (RC_Output != NULL){ RC_Output->reset(); }
          
          //cout << " Treetops_Output: " << Treetops_Output << " flg: " <<  flg_Treetops_Output;
          if (Treetops_Output == NULL){ Treetops_Output = new c_Raw_Table_Row_1D; flg_Treetops_Output = 1; }
          if (Treetops_Output != NULL){ Treetops_Output->reset(); }
          
          //Row_2D->reset();
          Dimension = 1;
     }
     

     //Sets the Input
     void set_Input(c_Raw_Table_Row_1D * p_Row_1D)
     {
          if (Input != NULL)
          { 
               //If the row is locally created then it is safe to delete it. 
               if (flg_Input){ delete Input; flg_Input = 0; }
               Input = NULL; 
          }
          Input = p_Row_1D;
          Dimension = 1;
     }
     //Sets the Input_Charges
     void set_Input_Charges(c_Raw_Table_Row_1D * p_Row_1D)
     {
          if (Input_Charges != NULL)
          { 
               //If the row is locally created then it is safe to delete it. 
               if (flg_Input_Charges){ delete Input_Charges; flg_Input_Charges = 0; }
               Input_Charges = NULL; 
          }
          Input_Charges = p_Row_1D;
          Dimension = 1;
     }
     //Sets the Pattern_Output
     void set_Pattern_Output(c_Raw_Table_Row_1D * p_Row_1D)
     {
          if (Pattern_Output != NULL)
          { 
               //If the row is locally created then it is safe to delete it. 
               if (flg_Pattern_Output){ delete Pattern_Output; flg_Pattern_Output = 0; }
               Pattern_Output = NULL; 
          }
          Pattern_Output = p_Row_1D;
          Dimension = 1;
     }
     //Sets the Charge_Output
     void set_Charge_Output(c_Raw_Table_Row_1D * p_Row_1D)
     {
          if (Charge_Output != NULL)
          { 
               //If the row is locally created then it is safe to delete it. 
               if (flg_Charge_Output){ delete Charge_Output; flg_Charge_Output = 0; }
               Charge_Output = NULL; 
          }
          Charge_Output = p_Row_1D;
          Dimension = 1;
     }
     //Sets the RC_Output
     void set_RC_Output(c_Raw_Table_Row_1D * p_Row_1D)
     {
          if (RC_Output != NULL)
          { 
               //If the row is locally created then it is safe to delete it. 
               if (flg_RC_Output){ delete RC_Output; flg_RC_Output = 0; }
               RC_Output = NULL; 
          }
          RC_Output = p_Row_1D;
          Dimension = 1;
     }
     //Sets the Treetops_Output
     void set_Treetops_Output(c_Raw_Table_Row_1D * p_Row_1D)
     {
          if (Treetops_Output != NULL)
          { 
               //If the row is locally created then it is safe to delete it. 
               if (flg_Treetops_Output){ delete Treetops_Output; flg_Treetops_Output = 0; }
               Treetops_Output = NULL; 
          }
          Treetops_Output = p_Row_1D;
          Dimension = 1;
     }
     
     //Gets the Input
     c_Raw_Table_Row_1D * get_Input()
     {
          if (Input == NULL){ reset(); }
          return Input;
     }
     //Gets the Input_Charges
     c_Raw_Table_Row_1D * get_Input_Charges()
     {
          if (Input_Charges == NULL){ reset(); }
          return Input_Charges;
     }
     //Gets the Pattern_Output
     c_Raw_Table_Row_1D * get_Pattern_Output()
     {
          if (Pattern_Output == NULL){ reset(); }
          return Pattern_Output;
     }
     //Gets the Charge_Output
     c_Raw_Table_Row_1D * get_Charge_Output()
     {
          if (Charge_Output == NULL){ reset(); }
          return Charge_Output;
     }
     //Gets the RC_Output
     c_Raw_Table_Row_1D * get_RC_Output()
     {
          if (RC_Output == NULL){ reset(); }
          return RC_Output;
     }
     //Gets the Treetops_Output
     c_Raw_Table_Row_1D * get_Treetops_Output()
     {
          if (Treetops_Output == NULL){ reset(); }
          return Treetops_Output;
     }
     
     //Overwrites a single row.
     void overwrite_Row(c_Raw_Table_Row_1D * p_From, c_Raw_Table_Row_1D * p_To)
     {
          if ((p_From != NULL) && (p_To != NULL))
          {
               int tmp_Depth = p_From->Depth;
               for (int cou_Cell=0;cou_Cell<tmp_Depth;cou_Cell++)
               {
                    //---*cout << "\n"; oindex(cou_Cell, 13);
                    //---*cout << "\n\n " << p_From->Cells[cou_Cell]->get_String();
                    p_To->submit_UData_String(p_From->Cells[cou_Cell]->Data, p_From->Cells[cou_Cell]->Length);
               }
          }
     }
     
     //Appends a single row. Takes the cell data and transfers it to the given index in the the equivelent cell in the target row at the given index.
     //Works only on single units of data, only copies the first bit of data to the given index.
     void append_Row(c_Raw_Table_Row_1D * p_From, c_Raw_Table_Row_1D * p_To, int p_Index)
     {
          if ((p_From != NULL) && (p_To != NULL))
          {
			   //The depth of the row to be copied.
               int tmp_Depth = p_From->Depth;
			   //Where to start appending in the receiving table row.
			   int tmp_Offset = p_To->Depth;
               for (int cou_Cell=0;cou_Cell<tmp_Depth;cou_Cell++)
               {
                    //---*cout << "\n"; oindex(cou_Cell, 13);
                    //---*cout << "\n\n " << p_From->Cells[cou_Cell]->get_String();
                    p_To->set_UInt((cou_Cell + tmp_Offset), 0, p_From->Cells[cou_Cell]->Data[0].U);
               }
          }
     }
     
     //Overwrites the data in the rows with the given row.
     void overwrite(c_NT3_IO * p_From, int p_AnMoDe_Type, int p_Index)
     {
          /*---*/ostr(0, 8, "\n  overwrite("); cout << p_AnMoDe_Type << ", " << p_Index; ostr(0, 8, ")");
          
          reset();
          
          if (Dimension == 1)
          {
               overwrite_Row(p_From->Input, Input);
               overwrite_Row(p_From->Input_Charges, Input_Charges);
               overwrite_Row(p_From->Pattern_Output, Pattern_Output);
               overwrite_Row(p_From->Charge_Output, Charge_Output);
               overwrite_Row(p_From->RC_Output, RC_Output);
               overwrite_Row(p_From->Treetops_Output, Treetops_Output);
          }
     }
     
     //Appends the given row onto the end of the current row.
     void append(c_NT3_IO * p_From, int p_Index)
     {
          if (Dimension == 1)
          {
               append_Row(p_From->Input, Input, p_Index);
               append_Row(p_From->Input_Charges, Input_Charges, p_Index);
               append_Row(p_From->Pattern_Output, Pattern_Output, p_Index);
               append_Row(p_From->Charge_Output, Charge_Output, p_Index);
               append_Row(p_From->RC_Output, RC_Output, p_Index);
               append_Row(p_From->Treetops_Output, Treetops_Output, p_Index);
          }
     }
     
     //Writes form the lower construct to this higher one.
     void angel_Overwrite_Con(c_NT3_IO * p_Lower_Construct)
     {
          //*---ostr(0, 8, "\n  overwrite("); cout << p_From; ostr(0, 8, ")");
          
          reset();
          
          overwrite_Row(p_Lower_Construct->Treetops_Output, Input);
          overwrite_Row(p_Lower_Construct->Charge_Output, Input_Charges);
     }
     
     //Writes form the lower construct to this higher one.
     void angel_Append_Con(c_NT3_IO * p_Lower_Construct, int p_Index)
     {
          //*---ostr(0, 8, "\n  overwrite("); cout << p_From; ostr(0, 8, ")");
          
          append_Row(p_Lower_Construct->Treetops_Output, Input, p_Index);
          append_Row(p_Lower_Construct->Charge_Output, Input_Charges, p_Index);
     }
     
     //Writes from the higher construct to this lower one.
     void demon_Overwrite_Con(c_NT3_IO * p_Higher_Construct, int p_Index)
     {
          ostr(0, 12, "\n  demon_Overwrite_Con("); cout << p_Index; ostr(0, 8, ")");
          
          //From the Higher constructs pattern and charge output tables to this entities treetops and charge tables for backpropagation.
          
          //We do not reset because we are reading from this input to another.
          //reset();
          Treetops_Output->reset();
          
          if (Dimension == 1)
          {
               /*---*/cout << "\n   Pattern->Depth:"; oint(0, 12, p_Higher_Construct->Pattern_Output->Depth);
               if (Treetops_Output != NULL)
               {
                    for (int cou_Cell=0;cou_Cell<p_Higher_Construct->Pattern_Output->Depth;cou_Cell++)
                    {
                         Treetops_Output->set_UInt(cou_Cell, p_Higher_Construct->Pattern_Output->Cells[cou_Cell]->Data[p_Index].U);
                    }
                    cout << "\n\n\n\n\n\n\n p_Index: " << p_Index; 
                    cout << "\n -------p_Higher_Construct->Pattern_Output:";
                    p_Higher_Construct->Pattern_Output->output_U(); cout << "\n";
                    cout << "\n -------Treetops_Output:";
                    Treetops_Output->output_U(); cout << "\n";
               }
               overwrite_Row(p_Higher_Construct->Charge_Output, Charge_Output);
          }
     }
     
     void output_C()
     {
          if (Dimension == 1)
          {
               if (Input != NULL){ Input->output_C(); } else { ostr(0, 12, "NULL"); }
               if (Input_Charges != NULL){ Input_Charges->output_C(); } else { ostr(0, 12, "NULL"); }

               if (Pattern_Output != NULL){ Pattern_Output->output_C(); } else { ostr(0, 12, "NULL"); }
               if (Charge_Output != NULL){ Charge_Output->output_C(); } else { ostr(0, 12, "NULL"); }
               if (RC_Output != NULL){ RC_Output->output_C(); } else { ostr(0, 12, "NULL"); }

               if (Treetops_Output != NULL){ Treetops_Output->output_C(); } else { ostr(0, 12, "NULL"); }
          }
     }
};


class c_NT3_Pipe_Thread
{
public:
     
     bool flg_Treetops;
     bool flg_Charge;
     bool flg_Pattern;
     bool flg_RC;
     
     //The rows that the ends connect to.
     c_NT3_IO io_Connection;
     
     //The dimensions of the input and output.
     int Dimension;
     
     //The ends of the pipe.
     void * End;
     
     //The index that these pipes connects too
     int Index;
     
     //The types of object this end is.
     int Type;
     
     //The index of the objects in the registry.
     int Register;
     
     //Positions
     int X;
     int Y;
     
     
     c_NT3_Pipe_Thread()
     {
          Dimension = 1;
          
          End = NULL;
          
          Index = 0;
          
          Type = -1;
          
          X = 0;
          Y = 0;
          
          Register = 0;
     }
     
     //Accepts the 4 attachment points.
     void attach(c_Raw_Table_Row_1D * p_Row_1D, string p_Table)
     {
          if (p_Table == "Input"){ if (p_Row_1D != NULL){ io_Connection.set_Input(p_Row_1D); } }
          if (p_Table == "Input_Charges"){ if (p_Row_1D != NULL){ io_Connection.set_Input_Charges(p_Row_1D); } }
          if (p_Table == "Pattern_Output"){ if (p_Row_1D != NULL){ io_Connection.set_Pattern_Output(p_Row_1D); } }
          if (p_Table == "Charge_Output"){ if (p_Row_1D != NULL){ io_Connection.set_Charge_Output(p_Row_1D); } }
          if (p_Table == "RC_Output"){ if (p_Row_1D != NULL){ io_Connection.set_RC_Output(p_Row_1D); } }
          if (p_Table == "Treetops_Output"){ if (p_Row_1D != NULL){ io_Connection.set_Treetops_Output(p_Row_1D); } }
     }
     
     //Sets xy
     void set_XY(int p_X, int p_Y)
     {
          X = p_X;
          Y = p_Y;
     }
     
     //Overwrites the rows.
     void overwrite(c_NT3_Pipe_Thread * p_Pipe, int p_AnMoDe = -1, int p_Index = 0)
     {
          io_Connection.overwrite(&(p_Pipe->io_Connection), p_AnMoDe, p_Index);
     }
     
     //Overwrites the Con.
     void angel_Overwrite_Con(c_NT3_Pipe_Thread * p_Pipe, int p_Index = 0)
     {
          io_Connection.angel_Overwrite_Con(&(p_Pipe->io_Connection));
     }
     
     //Overwrites the Con.
     void angel_Append_Con(c_NT3_Pipe_Thread * p_Pipe, int p_Index = 0)
     {
          io_Connection.angel_Append_Con(&(p_Pipe->io_Connection), p_Index);
     }
     
     //Overwrites the Con in a Demonic fashion.
     void demon_Overwrite_Con(c_NT3_Pipe_Thread * p_Pipe, int p_Index = 0)
     {
          io_Connection.demon_Overwrite_Con(&(p_Pipe->io_Connection), p_Index);
     }
     
     //Overwrites the rows.
     void append(c_NT3_Pipe_Thread * p_Pipe, int p_Index = 0)
     {
          io_Connection.append(&(p_Pipe->io_Connection), p_Index);
     }
     
     //Overwrites the rows.
     void drain(int p_Index, c_NT3_Pipe_Thread * p_Pipe)
     {
          //io_Connection.drain(p_Index, p_Pipe->io_Connection.Charge, p_Pipe->io_Connection.Pattern);
     }
};

//This is the pipe class used to manage the connections between inputs, outputs, and constructs.
//Each pipe connects 2 things.
class c_NT3_Pipe
{
private:
     
     //The threads on the pipe that actually link the tables.
     c_NT3_Pipe_Thread From;
     c_NT3_Pipe_Thread To;
     
public:
     
     //The type of pipe. Determined by the two connections.
     int Pipe_Type;
     
     //Whether the pipe is Angelic, Mortal, or Demonic.
     int Pipe_AnMoDe;
     
     c_NT3_Pipe()
     {
          Pipe_Type = -1;
          Pipe_AnMoDe = 0;
     }
     
     ~c_NT3_Pipe()
     {
     }
     
     //      NT3_Angel_In_Con 0
     //      NT3_Angel_Con_Con 1
     //      NT3_Angel_Out_Con 2

     //      NT3_Mortal_In_Out 10
     //      NT3_Mortal_In_Int 11
     //      NT3_Mortal_Con_Con 12
     //      NT3_Mortal_Out_Out 13
     //      NT3_Mortal_Out_In 14

     //      NT3_Demon_Con_In 20
     //      NT3_Demon_Con_Con 21
     //      NT3_Demon_Con_Out 22
     
     //Create a pipe connections.
     //The p_AnMoDe determines whether the constructs are Angelic, Mortal, or Demonic. 
     // 0: An, 1: Mo, 2: De
     void create_Connection(void * p_From, int p_From_Type, int p_From_Register, int p_From_Index, void * p_To, int p_To_Type, int p_To_Register, int p_To_Index, int p_AnMoDe = 0)
     {
          //Pipe[Data_Pipe_Count].create_Connection(tmp_From, Register_Type[p_From], p_From, p_From_Index, tmp_To, Register_Type[p_To], p_To, p_To_Index);
          From.End = p_From;
          From.Type = p_From_Type;
          From.Register = p_From_Register;
          From.Index = p_From_Index;
          
          To.End = p_To;
          To.Type = p_To_Type;
          To.Register = p_To_Register;
          To.Index = p_To_Index;
          
          Pipe_AnMoDe = p_AnMoDe;
          Pipe_Type = 0;
          
          ostr(0, 13, "\n\n From.End:"); cout << From.End;
          ostr(0, 14, " From.Type:"); cout << From.Type;
          ostr(0, 13, "\n To.End:"); cout << To.End;
          ostr(0, 14, " To.Type:"); cout << To.Type;
          ostr(0, 7, "\n     Connection Type: ");
          if (p_AnMoDe == 0){ ostr(0, 10, "Angelic"); }
          if (p_AnMoDe == 1){ ostr(0, 8, "Mortal"); }
          if (p_AnMoDe == 2){ ostr(0, 12, "Demonic"); }
          
          if (((p_From_Type == NT3INPUT1D) || (p_From_Type == NT3OUTPUT1D)) && ((To.Type == NT3INPUT1D) || (To.Type == NT3OUTPUT1D)))
          {
               Pipe_Type = NT3_Basic;
               
               //Attach both IO end together.
               From.attach(((c_NT3_IO*) p_From)->Input, "Input");
               From.attach(((c_NT3_IO*) p_From)->Input_Charges, "Input_Charges");
               From.attach(((c_NT3_IO*) p_From)->Pattern_Output, "Pattern_Output");
               From.attach(((c_NT3_IO*) p_From)->Charge_Output, "Charge_Output");
               From.attach(((c_NT3_IO*) p_From)->RC_Output, "RC_Output");
               From.attach(((c_NT3_IO*) p_From)->Treetops_Output, "Treetops_Output");
               
               To.attach(((c_NT3_IO*) p_To)->Input, "Input");
               To.attach(((c_NT3_IO*) p_To)->Input_Charges, "Input_Charges");
               To.attach(((c_NT3_IO*) p_To)->Pattern_Output, "Pattern_Output");
               To.attach(((c_NT3_IO*) p_To)->Charge_Output, "Charge_Output");
               To.attach(((c_NT3_IO*) p_To)->RC_Output, "RC_Output");
               To.attach(((c_NT3_IO*) p_To)->Treetops_Output, "Treetops_Output");
               
               //Link the XY
               From.set_XY(((c_NT3_IO*) p_From)->X, ((c_NT3_IO*) p_From)->Y);
               To.set_XY(((c_NT3_IO*) p_To)->X, ((c_NT3_IO*) p_To)->Y);
               
          }
               
          if ((From.Type == NT3CONSTRUCT1D) && ((To.Type == NT3INPUT1D) || (To.Type == NT3OUTPUT1D)))
          { 
               Pipe_Type = NT3_Basic; 
               Pipe_AnMoDe = 0;
               
               //Attach both IO end together.
               From.attach((((c_NT3_Base_Construct*) p_From)->get_Table_1D("tbl_Input"))->get_Row_Ref_Hard(0), "Input");
               From.attach((((c_NT3_Base_Construct*) p_From)->get_Table_1D("tbl_Input_Charges"))->get_Row_Ref_Hard(0), "Input_Charges");
               From.attach((((c_NT3_Base_Construct*) p_From)->get_Table_1D("tbl_Pattern_Output"))->get_Row_Ref_Hard(0), "Pattern_Output");
               From.attach((((c_NT3_Base_Construct*) p_From)->get_Table_1D("tbl_Charge_Output"))->get_Row_Ref_Hard(0), "Charge_Output");
               From.attach((((c_NT3_Base_Construct*) p_From)->get_Table_1D("tbl_RC_Output"))->get_Row_Ref_Hard(0), "RC_Output");
               From.attach((((c_NT3_Base_Construct*) p_From)->get_Table_1D("tbl_Treetops_Output"))->get_Row_Ref_Hard(0), "Treetops_Output");
               
               To.attach(((c_NT3_IO*) p_To)->Input, "Input");
               To.attach(((c_NT3_IO*) p_To)->Input_Charges, "Input_Charges");
               To.attach(((c_NT3_IO*) p_To)->Pattern_Output, "Pattern_Output");
               To.attach(((c_NT3_IO*) p_To)->Charge_Output, "Charge_Output");
               To.attach(((c_NT3_IO*) p_To)->RC_Output, "RC_Output");
               To.attach(((c_NT3_IO*) p_To)->Treetops_Output, "Treetops_Output");
               
               From.set_XY(((c_NT3_IO*) p_From)->X, ((c_NT3_IO*) p_From)->Y);
               To.set_XY(((c_NT3_Base_Construct*) p_To)->X, ((c_NT3_Base_Construct*) p_To)->Y);
               
               //Link From rows T[ ] C[C] P[P] RC[ ]
               //Link To rows   T[ ] C[C] P[P] RC[ ]
          }
          if (((From.Type == NT3INPUT1D) || (From.Type == NT3OUTPUT1D)) && (To.Type == NT3CONSTRUCT1D))
          { 
               Pipe_Type = NT3_Basic; 
               Pipe_AnMoDe = 0;
               
               //Attach both IO end together.
               From.attach(((c_NT3_IO*) p_From)->Input, "Input");
               From.attach(((c_NT3_IO*) p_From)->Input_Charges, "Input_Charges");
               From.attach(((c_NT3_IO*) p_From)->Pattern_Output, "Pattern_Output");
               From.attach(((c_NT3_IO*) p_From)->Charge_Output, "Charge_Output");
               From.attach(((c_NT3_IO*) p_From)->RC_Output, "RC_Output");
               From.attach(((c_NT3_IO*) p_From)->Treetops_Output, "Treetops_Output");
               
               To.attach((((c_NT3_Base_Construct*) p_To)->get_Table_1D("tbl_Input"))->get_Row_Ref_Hard(0), "Input");
               To.attach((((c_NT3_Base_Construct*) p_To)->get_Table_1D("tbl_Input_Charges"))->get_Row_Ref_Hard(0), "Input_Charges");
               To.attach((((c_NT3_Base_Construct*) p_To)->get_Table_1D("tbl_Pattern_Output"))->get_Row_Ref_Hard(0), "Pattern_Output");
               To.attach((((c_NT3_Base_Construct*) p_To)->get_Table_1D("tbl_Charge_Output"))->get_Row_Ref_Hard(0), "Charge_Output");
               To.attach((((c_NT3_Base_Construct*) p_To)->get_Table_1D("tbl_RC_Output"))->get_Row_Ref_Hard(0), "RC_Output");
               To.attach((((c_NT3_Base_Construct*) p_To)->get_Table_1D("tbl_Treetops_Output"))->get_Row_Ref_Hard(0), "Treetops_Output");
               
               From.set_XY(((c_NT3_IO*) p_From)->X, ((c_NT3_IO*) p_From)->Y);
               To.set_XY(((c_NT3_Base_Construct*) p_To)->X, ((c_NT3_Base_Construct*) p_To)->Y);
               
               //Link From rows T[ ] C[C] P[P] RC[ ]
               //Link To rows   T[ ] C[C] P[P] RC[ ]
          }
          if ((From.Type == NT3CONSTRUCT1D) && (To.Type == NT3CONSTRUCT1D) && (p_AnMoDe == 0))
          { 
               Pipe_Type = NT3_Construct; 
               Pipe_AnMoDe = 0;
               
               From.attach((((c_NT3_Base_Construct*) p_From)->get_Table_1D("tbl_Input"))->get_Row_Ref_Hard(0), "Input");
               From.attach((((c_NT3_Base_Construct*) p_From)->get_Table_1D("tbl_Input_Charges"))->get_Row_Ref_Hard(0), "Input_Charges");
               From.attach((((c_NT3_Base_Construct*) p_From)->get_Table_1D("tbl_Pattern_Output"))->get_Row_Ref_Hard(0), "Pattern_Output");
               From.attach((((c_NT3_Base_Construct*) p_From)->get_Table_1D("tbl_Charge_Output"))->get_Row_Ref_Hard(0), "Charge_Output");
               From.attach((((c_NT3_Base_Construct*) p_From)->get_Table_1D("tbl_RC_Output"))->get_Row_Ref_Hard(0), "RC_Output");
               From.attach((((c_NT3_Base_Construct*) p_From)->get_Table_1D("tbl_Treetops_Output"))->get_Row_Ref_Hard(0), "Treetops_Output");
               
               To.attach((((c_NT3_Base_Construct*) p_To)->get_Table_1D("tbl_Input"))->get_Row_Ref_Hard(0), "Input");
               To.attach((((c_NT3_Base_Construct*) p_To)->get_Table_1D("tbl_Input_Charges"))->get_Row_Ref_Hard(0), "Input_Charges");
               To.attach((((c_NT3_Base_Construct*) p_To)->get_Table_1D("tbl_Pattern_Output"))->get_Row_Ref_Hard(0), "Pattern_Output");
               To.attach((((c_NT3_Base_Construct*) p_To)->get_Table_1D("tbl_Charge_Output"))->get_Row_Ref_Hard(0), "Charge_Output");
               To.attach((((c_NT3_Base_Construct*) p_To)->get_Table_1D("tbl_RC_Output"))->get_Row_Ref_Hard(0), "RC_Output");
               To.attach((((c_NT3_Base_Construct*) p_To)->get_Table_1D("tbl_Treetops_Output"))->get_Row_Ref_Hard(0), "Treetops_Output");
                    
               From.set_XY(((c_NT3_Base_Construct*) p_From)->X, ((c_NT3_Base_Construct*) p_From)->Y);
               To.set_XY(((c_NT3_Base_Construct*) p_To)->X, ((c_NT3_Base_Construct*) p_To)->Y);
               
               //Link From rows T[P] C[C] P[ ] RC[ ]
               //Link To rows   T[ ] C[C] P[T] RC[ ]
          }
     }
     
     //Gets XY for pipe From and To.
     int get_fX(){ return From.X; }
     int get_fY(){ return From.Y; }
     int get_tX(){ return To.X; }
     int get_tY(){ return To.Y; }
     
     void set_fX(int p_X){ From.X = p_X; }
     void set_fY(int p_Y){ From.Y = p_Y; }
     void set_tX(int p_X){ To.X = p_X; }
     void set_tY(int p_Y){ To.Y = p_Y; }
     
     int get_From_Register(){ return From.Register; }
     int get_To_Register(){ return To.Register; }
     
     int get_From_Index(){ return From.Index; }
     int get_To_Index(){ return To.Index; }
     
     //Resets the output index and copies the input index to it.
     void overwrite(int p_Index, int p_AnMoDe)
     {
          //The two types of pipes are those that contain constructs and those that don't.
          //When passing the second param:
          // 0 == Angel Basic, 1 == Demon Basic, 2 == Angel Construct, 3 == Demon Construct
          if ((Pipe_Type == NT3_Basic) && (p_AnMoDe == 0)){ To.overwrite(&From, p_Index); }
          if ((Pipe_Type == NT3_Construct) && (p_AnMoDe == 0)){ To.angel_Overwrite_Con(&From, p_Index); }
          
          if ((Pipe_Type == NT3_Basic) && (p_AnMoDe == 1)){ To.overwrite(&From, p_Index); }
          if ((Pipe_Type == NT3_Construct) && (p_AnMoDe == 1)){ To.angel_Overwrite_Con(&From, p_Index); }
          
          if ((Pipe_Type == NT3_Basic) && (p_AnMoDe == 2)){ From.overwrite(&To, p_Index); }
          if ((Pipe_Type == NT3_Construct) && (p_AnMoDe == 2)){ From.demon_Overwrite_Con(&To, p_Index); }
          
          //output_Pipe();
     }
     
     //Appends the data in each cell, not the cells onto the rows. Sets the data according to the index given.
     void append(int p_Index = 0)
     {
          //The two types of pipes are those that contain constructs and those that don't.
          //When passing the second param:
          // 0 == Angel Basic, 1 == Demon Basic, 2 == Angel Construct, 3 == Demon Construct
          if (Pipe_Type == NT3_Basic){ To.append(&From, p_Index); }
          if (Pipe_Type == NT3_Construct){ To.angel_Append_Con(&From, p_Index); }
          
          //No demonic functions for append right now.
          
          //output_Pipe();
     }
     
     //Being demonic this will always be from a source to the local source.
     //This member is for the specific case of draining an MSC into a lower construct.
     //Takes a pattern row to exract the given index of data from each cell and place them in the Treetop.
     void drain(int p_Index)
     {
          To.drain(p_Index, &From);
     }
     
     //Creates a hotlink.
     void hotlink()
     {
     }
     
     //Outputs the two ends of a pipe.
     void output_Pipe()
     {
          if (Pipe_AnMoDe == 0){ ostr(0, 10, "\n    Angelic:"); }
          if (Pipe_AnMoDe == 1){ ostr(0, 7, "\n    Mortal:"); }
          if (Pipe_AnMoDe == 2){ ostr(0, 12, "\n    Demonic:"); }
          
          ostr(0, 7, "\n       From.Input__________:"); if (From.io_Connection.Input != NULL){ From.io_Connection.Input->output_U(); } else { ostr(0, 12, "NULL"); }
          ostr(0, 7, "\n       From.Input_Charges__:"); if (From.io_Connection.Input_Charges != NULL){ From.io_Connection.Input_Charges->output_U(); } else { ostr(0, 12, "NULL"); }
          ostr(0, 7, "\n       From.Pattern_Output_:"); if (From.io_Connection.Pattern_Output != NULL){ From.io_Connection.Pattern_Output->output_U(); } else { ostr(0, 12, "NULL"); }
          ostr(0, 7, "\n       From.Charge_Output__:"); if (From.io_Connection.Charge_Output != NULL){ From.io_Connection.Charge_Output->output_U(); } else { ostr(0, 12, "NULL"); }
          ostr(0, 7, "\n       From.RC_Output______:"); if (From.io_Connection.RC_Output != NULL){ From.io_Connection.RC_Output->output_U(); } else { ostr(0, 12, "NULL"); }
          ostr(0, 7, "\n       From.TreeTops_Output:"); if (From.io_Connection.Treetops_Output != NULL){ From.io_Connection.Treetops_Output->output_U(); } else { ostr(0, 12, "NULL"); }
          cout << "\n\n";
          ostr(0, 7, "\n       To.Input____________:"); if (To.io_Connection.Input != NULL){ To.io_Connection.Input->output_U(); } else { ostr(0, 12, "NULL"); }
          ostr(0, 7, "\n       To.Input_Charges____:"); if (To.io_Connection.Input_Charges != NULL){ To.io_Connection.Input_Charges->output_U(); } else { ostr(0, 12, "NULL"); }
          ostr(0, 7, "\n       To.Pattern_Output___:"); if (To.io_Connection.Pattern_Output != NULL){ To.io_Connection.Pattern_Output->output_U(); } else { ostr(0, 12, "NULL"); }
          ostr(0, 7, "\n       To.Charge_Output____:"); if (To.io_Connection.Charge_Output != NULL){ To.io_Connection.Charge_Output->output_U(); } else { ostr(0, 12, "NULL"); }
          ostr(0, 7, "\n       To.RC_Output________:"); if (To.io_Connection.RC_Output != NULL){ To.io_Connection.RC_Output->output_U(); } else { ostr(0, 12, "NULL"); }
          ostr(0, 7, "\n       To.Treetops_Output__:"); if (To.io_Connection.Treetops_Output != NULL){ To.io_Connection.Treetops_Output->output_U(); } else { ostr(0, 12, "NULL"); }
     }
};


class c_NT3_Pipe_Handler
{
private:
     
     
     //The current number of data pipes.
     int Data_Pipe_Count;
     
     //-- Tracks the pipe connections and their indexes for each construct.
     
     //Number of current pipe connections each construct has.
     int Construct_Connection_Count[10000];
     
     //The index that the pipe connects to.
     int Pipe_Construct_Index[10000];
     
public:
     
     //The data pipes.
     //Make this dynamic in future.
     //OUTDATED: There are 3 pipes for each connection, an Angelic, Mortal, and Demonic.
     //Now we are back to one pipe, this pipe can handle all the transfer types.
     c_NT3_Pipe Pipe[10000];
     
     c_NT3_Pipe_Handler()
     {
          Data_Pipe_Count = 0;
          
          for (int cou_Index=0;cou_Index<10000;cou_Index++)
          {
               Construct_Connection_Count[cou_Index] = 0;
               Pipe_Construct_Index[cou_Index] = 0;
          }
     }
     
     //Gets the datapipe count.
     int get_Data_Pipe_Count()
     {
          return Data_Pipe_Count;
     }
     
     //Creates a connection using a pipe.
     //If a construct is passed use the CID to determine where it goes.
     //Otherwise it is neccesary to track which constructs are connected to which constructs and the indexes that they connect to.
     int create_Connection(void * p_From_V, int p_From_Type, int p_From, int p_From_Index, void * p_To_V, int p_To_Type, int p_To, int p_To_Index, unsigned long long int p_CID = 0)
     {
          //Create the connections.
          Pipe[Data_Pipe_Count].create_Connection(p_From_V, p_From_Type, p_From, p_From_Index, p_To_V, p_To_Type, p_To, p_To_Index, 0);
          
          //Sets the pipe to the current open index in the construct. Then iterates the Construct_Connection_Count to account for the increase.
          if (p_To_Type == NT3CONSTRUCT1D)
          {
               Pipe_Construct_Index[Data_Pipe_Count] = Construct_Connection_Count[p_CID];
               Construct_Connection_Count[Data_Pipe_Count]++;
          }
          Data_Pipe_Count++;
          
          return 1;
     }
     
     //Appends cells from the first index to the row in the second index.
     void Append(int p_Pipe)
     {
          //ostr(0, 7, "\n void Append"); ostr(0, 12, "("); cout << p_Pipe; ostr(0, 12, ")");
          //Check for pipe.
          if (p_Pipe >= Data_Pipe_Count){ return; }
          
          //Append won't be called for demonic purposes, only mortal or angelic.
          //The type of pipe handles the way that it transfers between constructs.
          Pipe[p_Pipe].append(Pipe_Construct_Index[p_Pipe]);
     }
     
     //Resets the output index and copies the input index to it.
     void Overwrite(int p_Pipe, int p_AnMoDe = 0)
     {
          //ostr(0, 7, "\n void Overwrite"); ostr(0, 12, "("); cout << p_Pipe; ostr(0, 12, ")");
          //*---cout << "\n void Overwrite(" << p_Pipe << ")";
          
          //Check for pipe.
          if (p_Pipe >= Data_Pipe_Count){ return; }
          
          Pipe[p_Pipe].overwrite(Pipe_Construct_Index[p_Pipe], p_AnMoDe);
     }
     
     //Resets the output index and copies the input index to it.
     void Drain(int p_Pipe)
     {
          //ostr(0, 7, "\n void Overwrite"); ostr(0, 12, "("); cout << p_Pipe; ostr(0, 12, ")");
          //*---cout << "\n void Overwrite(" << p_Pipe << ")";
          
          //Check for pipe.
          if (p_Pipe >= Data_Pipe_Count){ return; }
          
          //Pipe[p_Pipe].drain(Pipe_Construct_Index[p_Pipe]);
     }
     
     //Resets the output index and copies the input index to it.
     void Hotlink(int p_Pipe)
     {
          cout << "\n void pipe_Hotlink(" << p_Pipe << ")";
          //*---cout << "\n void pipe_Overwrite(" << p_Pipe << ")";
          
          //Check for pipe.
          if (p_Pipe >= Data_Pipe_Count){ return; }
          
          //Pipe[p_Pipe].hotlink();
     }
};