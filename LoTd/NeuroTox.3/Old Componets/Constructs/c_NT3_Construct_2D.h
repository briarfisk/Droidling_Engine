#define DBGFHC2D /**/


//The construct class.
class c_NT3_Construct_2D: public c_NT3_Base_Construct
{
private:
     
     //The node network to use.
     c_NT3_Node_Network_2D Nodes;
     
     //The CAN used for building the memory trees.
     c_NT3_CAN_2D CAN;
     
     //The Charging_Buffers. One for each input.
     c_NT3_Buffer_2D Charging_Buffer;
     
     //The current input depth to charge using the settings_Charging_Depth
     int Input_Charging_Depth;
     
     //The current input.
     int Current_Input;
     
     //The current highest charge, used for gathing the vision.
     long long int Current_Highest_Charge;
     
     //The current input X depth for charging.
     int Current_Input_X;
     
     //The current input Y depth for charging.
     int Current_Input_Y;
     
     //The current Number of Tiers
     int Current_Number_Of_Tiers;
     
     
     
public:
     
     
     //The name of the Construct.
     string Name;
     
     //The bitmap input.
     //This will allow for easily interacting with the windows API.
     //img_Input is set to a given bmp and then read pixel by pixel into the three RGB inputs.
     //c_Image_Handler img_Input;
     
     //The bitmap output.
     //c_Image_Handler img_Output;
     
     //The input tables.
     c_Raw_Table_2D tbl_Input;
     
     //Output Tables.
     c_Raw_Table_2D tbl_Pattern_Output_C;
     c_Raw_Table_1D tbl_Charge_Output_C;
     c_Raw_Table_1D tbl_Position_Output_C;
     c_Raw_Table_1D tbl_RC_Output_C;
     c_Raw_Table_2D tbl_Treetops_Output_C;
     
     c_Raw_Table_1D tbl_Treetops;
     
     c_Raw_Table_2D tbl_Vision_Output;
     c_Raw_Table_1D tbl_Vision_Charge_Output;
     
     //--  Settings
     
     //Charging legs.
     bool setting_Charge_0;
     bool setting_Charge_1;
     bool setting_Charge_2;
     bool setting_Charge_3;
     
     //The tier to start charging.
     int settings_Tier_Start;
     
     //The maximum tier height to build or charge too, regardless of input depth.
     //If -1 then there is no limit.
     int settings_Tier_End;
     
     //The Action Potential Threshold as percentage of the base charge.
     double settings_Action_Potential_Threshold;
     
     //The base charge to apply from the CAN.
     int settings_Base_Charge;
     
     //The multiple to reduce each recursive charge by.
     double settings_Modifier_Charge;
     
     //Determines whether or not to stack outputs when discharging treetops from foreign constructs.
     int settings_Stack_Output_From_Foreign_Treetops;
     
     //The flag for using the charging spike in the buffer charging.
     int setting_Charge_Spike;
     
     //==--  SETTINGS FOR THE AUTOCALIBRATION
     
     //The type of auto to use, Numerical or Alphabetical for example.
     int sauto_Type;
     
     //Whether or not the RC is being evaluated.
     int sauto_Use_RC;
     
     c_NT3_Construct_2D()
     {
          u_Data_3 tmp_Name;
          tmp_Name.NR = this;
          Name = "C__" + int2str(tmp_Name.U);
          
          set_Name(Name);
          
          Current_Input = 0;
          Current_Highest_Charge = 0;
          Current_Input_X = 0;
          Current_Input_Y = 0;
          Current_Number_Of_Tiers = 0;
          
          settings_Tier_Start = 0;
          settings_Tier_End = -1;
          
          settings_Action_Potential_Threshold = .1;
          settings_Base_Charge = 10000;
          
          settings_Modifier_Charge = .5;
          
          settings_Stack_Output_From_Foreign_Treetops = 0;
          
          setting_Charge_Spike = 1;
          
          setting_Charge_0 = 1;
          setting_Charge_1 = 1;
          setting_Charge_2 = 1;
          setting_Charge_3 = 1;
          
          CAN.init(&Nodes);
          Input_Charging_Depth = 0;
          
          sauto_Use_RC = 0;
          sauto_Type = 0;
     }
     
     ~c_NT3_Construct_2D()
     {
          cout << "\n  ~c_NT3_Construct_2D " << Name << " " << this << ".........."; cout.flush();
          
          cout << "\n  ~~~c_NT3_Construct_2D " << Name << " " << this << ".........."; cout.flush();
     }
     
     //Sets the name of the construct and the tables.
     void set_Name(string p_Name)
     {
          Name = p_Name;
          
          //The input tables.
          tbl_Input.Name = Name + "___Input";
          
          tbl_Pattern_Output_C.Name = Name + "___Pattern_Output_C";
          tbl_Charge_Output_C.Name = Name + "___Charge_Output_C";
          tbl_Position_Output_C.Name = Name + "___Position_Output_C";
          tbl_RC_Output_C.Name = Name + "___RC_Output_C";
          tbl_Treetops_Output_C.Name = Name + "___Treetops_Output_C";
          
          tbl_Treetops.Name = Name + "___Treetops";
          
          tbl_Vision_Output.Name = Name + "___Vision_Output";
          tbl_Vision_Charge_Output.Name = Name + "___Vision_Charge_Output";
     }
     
     //Sets the maximum tier allowed.
     void set_MAX_Tier(int p_Tier)
     {
          settings_Tier_End = p_Tier;
     }
     
     ////==-----------------------+
     //==--   EVAL FUNCTIONS
     ////==-----------------------+
          
     //Eval with no RC.
     void Eval()
     {
          //cout << "\n c_NT3_Construct_2D::Eval()";
          
          reset_Outputs();
          Current_Input = 0;
          Current_Highest_Charge = 0;
          
          if (tbl_Input.Number_Of_Rows == 0){ return; }
          
          for (int cou_Cell=0;cou_Cell<tbl_Input.Rows[0]->Depth;cou_Cell++)
          {
               Current_Input = cou_Cell;
               
               //cout << "\n    Build_Input_Query()";
               Build_Input_Query(cou_Cell);
               
               //*cout << "\n " << Name << " settings_Tier->" << settings_Tier << " = Input_Depth->" << Input_Depth << " * " << settings_Tier_Depth;
               
               //output_CAN();
               
               //cout << "\n    init_Charging_Buffer_C()";
               init_Charging_Buffer_C();
               //cout << "\n    charge_Buffer_C()";
               charge_Buffer_C();
               //cout << "\n    discharge_Treetops_C()";
               discharge_Treetops_C();
               //cout << "\n    gather_Vision()";
               gather_Vision();
          }
     }
     
     //Eval with RC.
     void Eval_RC()
     {
          //cout << "\n c_NT3_Construct_2D::Eval_RC()";
          
          reset_Outputs();
          Current_Input = 0;
          Current_Highest_Charge = 0;
          
          if (tbl_Input.Number_Of_Rows == 0){ return; }
          
          for (int cou_Cell=0;cou_Cell<tbl_Input.Rows[0]->Depth;cou_Cell++)
          {
               Current_Input = cou_Cell;
               
               Build_Input_Query(cou_Cell);
               
               //*cout << "\n " << Name << " settings_Tier->" << settings_Tier << " = Input_Depth->" << Input_Depth << " * " << settings_Tier_Depth;
               
               init_Charging_Buffer_C();
               charge_Buffer_RC_C();
               discharge_Treetops_C();
          }
     }
     
     
     
     //Sets the charging buffers up for each input.
     void init_Charging_Buffer_C()
     {
          Charging_Buffer.Base_Charge = settings_Base_Charge;
          Charging_Buffer.Modifier_Charge = settings_Modifier_Charge;
          Charging_Buffer.Action_Potential_Threshold = settings_Action_Potential_Threshold;
          //Charging_Buffer.resize_Opp((Input_Depth - 0));
     }     
     
     //Charges a single buffer.
     void charge_Buffer_C()
     {
          //cout << "\n c_NT3_Construct_2D::charge_Buffer_C()";
          
          Charging_Buffer.Input_Position = 0;
          Charging_Buffer.reset();
          
          /*
          cout << "\n\n\n\n\n\n\n"; 
          cout << "\n s_Tier_Start " << settings_Tier_Start;
          cout << "\n s_Tier_End " << settings_Tier_End;
          cout << "\n  ";
          cout << "\n Current_Input_X " << Current_Input_X;
          cout << "\n Current_Input_Y " << Current_Input_Y;
          */
          
          if (settings_Tier_End != -1)
          {
               //Charge until the tier below the top.
               for (int cou_T=settings_Tier_Start;cou_T<(settings_Tier_End - 1);cou_T++)
               {    
                    for (int cou_X=0;cou_X<(Current_Input_X - cou_T);cou_X++)
                    {
                         for (int cou_Y=0;cou_Y<(Current_Input_Y - cou_T);cou_Y++)
                         {
                              if (CAN.CAN[cou_T] [cou_X] [cou_Y] != NULL)
                              {
                                   Charging_Buffer.submit(CAN.CAN[cou_T] [cou_X] [cou_Y], (settings_Base_Charge));
                              }
                         }
                    }
                    Charging_Buffer.gather();
                    Charging_Buffer.charge_Outputs(setting_Charge_Spike);
                    
                    if (!Charging_Buffer.flg_Not_Done){ break; }
                    
               }
               
               //Charge the treetops level.
               for (int cou_X=0;cou_X<(Current_Input_X - (settings_Tier_End - 1));cou_X++)
               {
                    for (int cou_Y=0;cou_Y<(Current_Input_Y - (settings_Tier_End - 1));cou_Y++)
                    {
                         if (CAN.CAN[(settings_Tier_End - 1)] [cou_X] [cou_Y] != NULL)
                         {
                              Charging_Buffer.submit(CAN.CAN[(settings_Tier_End - 1)] [cou_X] [cou_Y], (settings_Base_Charge));
                         }
                    }
               }
               
               Charging_Buffer.gather();
               Charging_Buffer.charge_Outputs_Into_Treetops(setting_Charge_Spike);
               Current_Highest_Charge = Charging_Buffer.gather_Treetops();
               
               //Charging_Buffer.output_All_Buffers();
          }
          else
          {
               for (int cou_T=settings_Tier_Start;cou_T<Current_Number_Of_Tiers;cou_T++)
               {
                    for (int cou_X=0;cou_X<(Current_Input_X - cou_T);cou_X++)
                    {
                         for (int cou_Y=0;cou_Y<(Current_Input_Y - cou_T);cou_Y++)
                         {
                              if (CAN.CAN[cou_T] [cou_X] [cou_Y] != NULL)
                              {
                                   Charging_Buffer.submit(CAN.CAN[cou_T] [cou_X] [cou_Y], (settings_Base_Charge));
                              }
                         }
                    }
                    Charging_Buffer.gather();
                    Charging_Buffer.charge_Outputs(setting_Charge_Spike);
                    
                    if (!Charging_Buffer.flg_Not_Done){ break; }
                    
               }
               while(Charging_Buffer.flg_Not_Done)
               {
                    Charging_Buffer.gather();
                    Charging_Buffer.charge_Outputs(setting_Charge_Spike);
               }
          }
     }
     
     //Charges a single buffer.
     void charge_Buffer_RC_C()
     {
          //cout << "\n c_NT3_Construct_2D::charge_Buffer_RC_C()";
          
          Charging_Buffer.Input_Position = 0;
          
          
          
          if (settings_Tier_End != -1)
          {
               //Charge until the tier below the top.
               for (int cou_T=settings_Tier_Start;cou_T<(settings_Tier_End - 1);cou_T++)
               {    
                    for (int cou_X=0;cou_X<(Current_Input_X - cou_T);cou_X++)
                    {
                         for (int cou_Y=0;cou_Y<(Current_Input_Y - cou_T);cou_Y++)
                         {
                              if (CAN.CAN[cou_T] [cou_X] [cou_Y] != NULL)
                              {
                                   Charging_Buffer.submit_RC(CAN.CAN[cou_T] [cou_X] [cou_Y], (settings_Base_Charge));
                              }
                         }
                    }
                    Charging_Buffer.gather();
                    Charging_Buffer.charge_Outputs_RC(setting_Charge_Spike);
                    
                    if (!Charging_Buffer.flg_Not_Done){ break; }
                    
               }
               
               //Charge the treetops level.
               for (int cou_X=0;cou_X<(Current_Input_X - (settings_Tier_End - 1));cou_X++)
               {
                    for (int cou_Y=0;cou_Y<(Current_Input_Y - (settings_Tier_End - 1));cou_Y++)
                    {
                         if (CAN.CAN[(settings_Tier_End - 1)] [cou_X] [cou_Y] != NULL)
                         {
                              Charging_Buffer.submit_RC(CAN.CAN[(settings_Tier_End - 1)] [cou_X] [cou_Y], (settings_Base_Charge));
                         }
                    }
               }
               
               Charging_Buffer.gather();
               Charging_Buffer.charge_Outputs_Into_Treetops_RC(setting_Charge_Spike);
               Current_Highest_Charge = Charging_Buffer.gather_Treetops();
               
               Charging_Buffer.output_All_Buffers();
          }
          else
          {
               for (int cou_T=settings_Tier_Start;cou_T<Current_Number_Of_Tiers;cou_T++)
               {
                    for (int cou_X=0;cou_X<(Current_Input_X - cou_T);cou_X++)
                    {
                         for (int cou_Y=0;cou_Y<(Current_Input_Y - cou_T);cou_Y++)
                         {
                              if (CAN.CAN[cou_T] [cou_X] [cou_Y] != NULL)
                              {
                                   Charging_Buffer.submit_RC(CAN.CAN[cou_T] [cou_X] [cou_Y], (settings_Base_Charge));
                              }
                         }
                    }
                    Charging_Buffer.gather();
                    Charging_Buffer.charge_Outputs_RC(setting_Charge_Spike);
                    
                    if (!Charging_Buffer.flg_Not_Done){ break; }
                    
               }
               while(Charging_Buffer.flg_Not_Done)
               {
                    Charging_Buffer.gather();
                    Charging_Buffer.charge_Outputs_RC(setting_Charge_Spike);
               }
          }
     }
     
     ////==-----------------------+
     //==--   BUILDING FUNCTIONS
     ////==-----------------------+
     
     //Builds full CAN.
     void Build()
     {
          if (tbl_Input.Number_Of_Rows == 0){ return; }
          
          tbl_Treetops.reset();
          
          for (int cou_Cell=0;cou_Cell<tbl_Input.Rows[0]->Depth;cou_Cell++)
          {
               Build_Input(cou_Cell);
          }
     }
     
     //Builds RC CAN.
     void Build_RC()
     {
          if (tbl_Input.Number_Of_Rows == 0){ return; }
          
          tbl_Treetops.reset();
          
          for (int cou_Cell=0;cou_Cell<tbl_Input.Rows[0]->Depth;cou_Cell++)
          {
               Build_Input_RC(cou_Cell);
          }
     }
     
     //Builds for query.
     void Build_Query()
     {
          if (tbl_Input.Number_Of_Rows == 0){ return; }
          
          tbl_Treetops.reset();
          
          for (int cou_Cell=0;cou_Cell<tbl_Input.Rows[0]->Depth;cou_Cell++)
          {
               Build_Input_Query(cou_Cell);
          }
     }
     
     //Builds full CAN.
     void Build_Input(int p_Cell = 0)
     {
          gather_CAN_Input(0, p_Cell);
          CAN.Full();
          if (CAN.Treetop == NULL){ tbl_Treetops.add_Int(Current_Input, 0); return; }
          //**tbl_Treetops.add_Int(Current_Input, (CAN.Treetop->NID.U + 1));
     }
     
     //Builds RC CAN.
     void Build_Input_RC(int p_Cell = 0)
     {
          gather_CAN_Input(0, p_Cell);
          CAN.RC();
          if (CAN.Treetop == NULL){ tbl_Treetops.add_Int(Current_Input, 0); return; }
          //**tbl_Treetops.add_Int(Current_Input, (CAN.Treetop->NID.U + 1));
     }
     
     //Builds for query.
     void Build_Input_Query(int p_Cell = 0)
     {
          //**cout << "\n    Build_Input_Query(" << p_Cell << ")";
          gather_CAN_Input(0, p_Cell);
          //**cout << "\n    CAN.Query();";
          CAN.Query();
          //**cout << " End...";
     }
     
     /*
     //Creates a connection between two nodes.
     void create_Connection(long long int p_L, long long int p_R)
     {
          c_NT3_Base_Node_1D * tmp_L = Nodes.get_Node_By_NID(p_L);
          c_NT3_Base_Node_1D * tmp_R = Nodes.get_Node_By_NID(p_R);
          
          Nodes.get_Upper_Tier_Connection(tmp_L, tmp_R);
     }
      * */
     
     //Gathers the first cell of the input table to build. This is neccesary as the cells are dynamically
     //allocated and addresses may change.
     void gather_CAN_Input(int p_Row = 0, int p_Cell = 0)
     {
          //**cout << "\n gather_CAN_Input";
          
          /**/if (tbl_Input.Number_Of_Rows <= p_Row){ cout << "\n Aborted..."; }
          /**/if (tbl_Input.Rows[0]->Depth <= p_Cell){ cout << "\n Aborted..."; }
          
          if (tbl_Input.Number_Of_Rows <= p_Row){ return; }
          if (tbl_Input.Rows[0]->Depth <= p_Cell){ return; }
          
          Current_Input_X = tbl_Input.Rows[p_Row]->Cells[p_Cell]->Width;
          Current_Input_Y = tbl_Input.Rows[p_Row]->Cells[p_Cell]->Height;
          Current_Number_Of_Tiers = Current_Input_X;
          if (Current_Number_Of_Tiers > Current_Input_Y){ Current_Number_Of_Tiers = Current_Input_Y; }
          if ((Current_Number_Of_Tiers > settings_Tier_End) && (settings_Tier_End > 0))
          {
               CAN.set_Input(tbl_Input.Rows[p_Row]->Cells[p_Cell], settings_Tier_End);
          }
          else
          {
               CAN.set_Input(tbl_Input.Rows[p_Row]->Cells[p_Cell]);
          }
               
     }
     
     ////==--------------------------+
     //==--   DISCHARGING FUNCTIONS
     ////==--------------------------+
     
     //Discharges the treetops for a given input.
     void discharge_Treetops_C()
     {
          //**cout << "\n    discharge_Treetops_C()"; cout.flush();
          Charging_Buffer.gather_Treetops();
          c_NT3_Linked_List_2D * tmp_LL = Charging_Buffer.Treetops.Root;
                    
          while(tmp_LL != NULL)
          {
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               //**cout << "\n";
               //**tmp_LL->NID->bp_O();
               //*cout << " "; odbl(0, 12, (settings_Base_Charge * settings_Action_Potential_Threshold));
               //*cout << " "; oint(0, 13, (tmp_LL->Charge.I));
               if (tmp_LL->Charge.I > (settings_Base_Charge * settings_Action_Potential_Threshold)){  } else { tmp_LL = tmp_LL->Next; continue; }
               
               if (tmp_LL->NID == NULL)
               {                    
                    tbl_Pattern_Output_C.new_Cell(Current_Input);
                    tbl_Pattern_Output_C.add_String("NULL");
                    tbl_Position_Output_C.add_Int(Current_Input, 0);
                    tbl_Charge_Output_C.add_Int(Current_Input, tmp_LL->Charge.I);
                    tbl_RC_Output_C.add_Float(Current_Input, 0);
                    tbl_Treetops_Output_C.add_Int(Current_Input, 0);
                    
                    tmp_LL = tmp_LL->Next;
                    continue;
               }
               
               //**cout << "\n       Nodes.bp(" << &tbl_Pattern_Output_C << ", Current_Input, Current_Input_X, Current_Input_Y, tmp_LL->NID);"; cout.flush();
               
               Nodes.bp(&tbl_Pattern_Output_C, Current_Input, Current_Input_X, Current_Input_Y, tmp_LL->NID);
               
               //**cout << "\n        tbl_Position_Output_C.add_Int(Current_Input, 0);"; cout.flush();
               tbl_Position_Output_C.add_Int(Current_Input, 0);
               //**cout << "\n         tbl_Charge_Output_C.add_Int(Current_Input, tmp_LL->Charge.I);"; cout.flush();
               tbl_Charge_Output_C.add_Int(Current_Input, tmp_LL->Charge.I);
               //**cout << "\n          tbl_RC_Output_C.add_Float(Current_Input, tmp_LL->NID->get_RC_Score());"; cout.flush();
               tbl_RC_Output_C.add_Float(Current_Input, tmp_LL->NID->get_RC_Score());
               //**cout << "\n           tbl_Treetops_Output_C.add_Int(Current_Input, tmp_LL->NID->NID.U + 1);"; cout.flush();
               tbl_Treetops_Output_C.add_Int(Current_Input, tmp_LL->NID->NID.U + 1);
               
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Gathers the vision for a cell.
     void gather_Vision_Cell(c_Raw_Table_Cell_2D * p_Cell, c_Raw_Table_Cell_2D * p_Vision_Cell)
     {
          if (p_Cell == NULL){ return; }
          
          for (int cou_X=0;cou_X<p_Cell->Width;cou_X++)
          {
               for (int cou_Y=0;cou_Y<p_Cell->Height;cou_Y++)
               {
                    p_Vision_Cell->add_IMG_Pixel_Data(p_Cell->Data[cou_X] [cou_Y].F, cou_X, cou_Y);
               }
          }
     }
     
     //Gathers the vision.
     void gather_Vision()
     {
          if (Current_Input >= tbl_Pattern_Output_C.Number_Of_Rows){ return; }
          
          c_Raw_Table_Cell_2D * tmp_Vision_Cell = tbl_Vision_Output.get_Cell_Ref_Hard(Current_Input, 0);
          
          
          //Loop through the current input row and gather the vision based on Current_Highest_Charge;
          for (int cou_Index=0;cou_Index<tbl_Pattern_Output_C.Rows[Current_Input]->Depth;cou_Index++)
          {
               gather_Vision_Cell(tbl_Pattern_Output_C.Rows[Current_Input]->Cells[cou_Index], tmp_Vision_Cell);
          }
          
          //This is for the charges to figure the weighted average.
          //tbl_Charge_Output_C
          
     }
     
     
     ////==---------------------+
     //==--   OUTPUT FUNCTIONS
     ////==---------------------+
     
     //Resets the input tables.
     void reset_Inputs()
     {
          tbl_Input.reset();
     }
     
     //Resets the output tables.
     void reset_Outputs()
     {
          tbl_Pattern_Output_C.reset();
          tbl_Charge_Output_C.reset();
          tbl_Position_Output_C.reset();
          tbl_RC_Output_C.reset();
          tbl_Treetops_Output_C.reset();
     
          tbl_Treetops.reset();
     
          tbl_Vision_Output.reset();
          tbl_Vision_Charge_Output.reset();
          
     }
     
     
     
     
     ////==---------------------+
     //==--   OUTPUT FUNCTIONS
     ////==---------------------+
     
     //Outputs the node network.
     void output_Nodes()
     {
          cout << "\n\n " << Name;
          Nodes.output_Nodes();
     }
     
     //Outputs the input table.
     void output_Input_Table()
     {
          cout << "\n\n ---===+===---";
          cout << "\n " << Name << " Inputs Tables ->" << this;
          tbl_Input.output_C(1);
          
          cout << "\n --===+===--\n";
     }
     void output_Input_Table_I()
     {
          cout << "\n\n ---===+===---";
          cout << "\n " << Name << " Inputs Tables ->" << this;
          tbl_Input.output_I(1);
          
          cout << "\n --===+===--\n";
     }
     void output_Input_Table_F()
     {
          cout << "\n\n ---===+===---";
          cout << "\n " << Name << " Inputs Tables ->" << this;
          tbl_Input.output_F(1);
          
          cout << "\n --===+===--\n";
     }
     void output_Input_Table_NR()
     {
          cout << "\n\n ---===+===---";
          cout << "\n " << Name << " Inputs Tables ->" << this;
          tbl_Input.output_NR(1);
          
          cout << "\n --===+===--\n";
     }
     
     //Outputs the input tables.
     void output_Input_Tables()
     {
          cout << "\n\n ---===+===---";
          cout << "\n " << Name << " Inputs Tables ->" << this;
          tbl_Input.output_C(1);
          cout << "\n --===+===--\n";
     }

     
     //Returns the number of nodes in the network.
     long long int get_Node_Count()
     {
          return Nodes.CNID.I;
     }
     
     //Outputs the treetops tree in the node network.
     void output_Treetops()
     {
          Nodes.output_Treetops();
     }
     
     //Outputs the treetops tree in a compressed BP format.
     void output_Treetops_BP()
     {
          Nodes.output_Treetops_BP();
     }
     
     //Outputs the tables of the construct.
     void output_Output_Tables()
     {
          cout << "\n\n\t ---===+===---";
          cout << "\n\t " << Name << " Output Tables ->" << this;
          
          tbl_Pattern_Output_C.output_F(2);
          
          tbl_Charge_Output_C.output_I(2);
          tbl_Position_Output_C.output_I(2);
          tbl_RC_Output_C.output_F(2);
          tbl_Treetops_Output_C.output_I(2);
          
          tbl_Treetops.output_I(2);
          cout << "\n\t ---===+===---\n";
     }
     
     //Outputs the tables of the construct with the patterns as int.
     void output_Output_Tables_I()
     {
          cout << "\n\n\t ---===+===---";
          cout << "\n\t " << Name << " Output_Tables_I ->" << this;
          
          tbl_Pattern_Output_C.output_I(2);
          tbl_Charge_Output_C.output_I(2);
          tbl_Position_Output_C.output_I(2);
          tbl_RC_Output_C.output_F(2);
          tbl_Treetops_Output_C.output_I(2);
          
          tbl_Treetops.output_I(2);
          cout << "\n\t ---===+===---\n";
     }
     
     //Outputs the tables of the construct with the patterns as int.
     void output_Output_Tables_F()
     {
          cout << "\n\n\t ---===+===---";
          cout << "\n\t " << Name << " Output_Tables_I ->" << this;
          
          tbl_Pattern_Output_C.output_F(2);
          tbl_Charge_Output_C.output_F(2);
          tbl_Position_Output_C.output_F(2);
          tbl_RC_Output_C.output_F(2);
          tbl_Treetops_Output_C.output_F(2);
          
          tbl_Treetops.output_F(2);
          cout << "\n\t ---===+===---\n";
     }
     
     //Outputs the CANs.
     void output_CAN()
     {
          CAN.output_CAN();
     }
          
     //Outputs the CAN state.
     void output_CAN_State()
     {
          CAN.output_CAN_State();
     }
          
     //Outputs the CAN state.
     void output_CAN_RC()
     {
          //CAN.output_CAN_RC();
     }
     
     //Outputs the settings of the current Construct.
     void output_Settings()
     {
          cout << "\n\n --  Settings For "; ostr(0, 13, Name);
          
          //Use in the expanded form.
          //cout << "\n  setting_Charge_L.................... "; oint(0, 11, setting_Charge_L);
          //cout << "\n  setting_Charge_R.................... "; oint(0, 11, setting_Charge_R);
          //cout << "\n  settings_Tier....................... "; oint(0, 11, settings_Tier);
          //cout << "\n  settings_Tier_Is_Floating........... "; oint(0, 11, settings_Tier_Is_Floating);
          cout << "\n  settings_Action_Potential_Threshold. "; odbl(0, 11, settings_Action_Potential_Threshold);
          cout << "\n  settings_Base_Charge................ "; oint(0, 11, settings_Base_Charge);
          cout << "\n  settings_Modifier_Charge............ "; odbl(0, 11, settings_Modifier_Charge);
     }
     
     //Outputs the settings of the current Construct.
     void output_Settings_Full()
     {
          cout << "\n\n --  Settings For "; ostr(0, 13, Name);
          
          //Use in the expanded form.
          cout << "\n  Charge_L.................... "; oint(0, 11, setting_Charge_0);
          cout << "\n  Charge_L.................... "; oint(0, 11, setting_Charge_1);
          cout << "\n  Charge_L.................... "; oint(0, 11, setting_Charge_2);
          cout << "\n  Charge_L.................... "; oint(0, 11, setting_Charge_3);
          cout << "\n  Tier_MAX....................... "; oint(0, 11, settings_Tier_End);
          cout << "\n  Tier_Start....................... "; oint(0, 11, settings_Tier_Start);
          cout << "\n  Tier_End....................... "; oint(0, 11, settings_Tier_End);
          cout << "\n  Action_Potential_Threshold. "; odbl(0, 11, settings_Action_Potential_Threshold);
          cout << "\n  Base_Charge................ "; oint(0, 11, settings_Base_Charge);
          cout << "\n  Modifier_Charge............ "; odbl(0, 11, settings_Modifier_Charge);
          cout << "\n  Stack_Output_From_Foreign_Treetops............ "; odbl(0, 11, settings_Stack_Output_From_Foreign_Treetops);
          cout << "\n  sauto_Type............ "; odbl(0, 11, sauto_Type);
          cout << "\n  sauto_Use_RC............ "; odbl(0, 11, sauto_Use_RC);
     }
     
     
     
     ////==--------------+
     //==--   INTERFACE
     ////==--------------+
     
     
     //Accepts an input to the inputs motherfucker.
     void inpxl(int p_Row, int p_Cell, u_Data_3 p_Submission, int p_X, int p_Y)
     {
          tbl_Input.set_IMG_Pixel(p_Row, p_Cell, p_Submission, p_X, p_Y);
     }
     
     //Accepts an input to the inputs motherfucker.
     void inIMG(int p_Row, int p_Cell, u_Data_3 ** p_Submission, int p_Width, int p_Height, int p_X = 0, int p_Y = 0)
     {
          tbl_Input.set_IMG(p_Row, p_Cell, p_Submission, p_Width, p_Height, p_X, p_Y);
     }
     
     //Acceps a cell to copy into an input cell.
     void in_IMG_Cell(int p_Row, int p_Cell, c_Raw_Table_Cell_2D * p_Cell_Data)
     {
          tbl_Input.set_IMG_Cell_Data(p_Row, p_Cell, p_Cell_Data);
     }
     
     
     
     ////==---------------------------------+
     //==--   SAVING AND LOADING FUNCTIONS
     ////==---------------------------------+
     
     //Saves the construct.
     void Save(string p_Name = "NO_NAME", string p_Dir = "NT3.Save/")
     {
          if (p_Name == "NO_NAME"){ p_Name = Name; }
          
          //The save file.
          ofstream SF;
          
          string tmp_SF_FN = p_Dir + p_Name;
          
          SF.open(tmp_SF_FN, ios::trunc);
          
          if (!SF.is_open()){ cout << "\n\t SAVE FILE " << p_Name << " CANNOT BE OPENED!!!"; return; }
          
          //-- Read the settings into the file.
          
          //The name of the construct.
          SF << "Name " << Name;
          
          //Charging legs.
          SF << "\nsetting_Charge_L " << setting_Charge_0;
          SF << "\nsetting_Charge_L " << setting_Charge_1;
          SF << "\nsetting_Charge_L " << setting_Charge_2;
          SF << "\nsetting_Charge_L " << setting_Charge_3;
          
          //The tier to start charging.
          SF << "\nsettings_Tier_Start " << settings_Tier_Start;
          SF << "\nsettings_Tier_End " << settings_Tier_End;
          
          //The Action Potential Threshold as percentage of the base charge.
          SF << "\nsettings_Action_Potential_Threshold " << settings_Action_Potential_Threshold;
          
          //The base charge to apply from the CAN.
          SF << "\nsettings_Base_Charge " << settings_Base_Charge;
          
          //The multiple to reduce each recursive charge by.
          SF << "\nsettings_Modifier_Charge " << settings_Modifier_Charge;
          
          //Whether or not to stack outputs.
          SF << "\nsettings_Stack_Output_From_Foreign_Treetops " << settings_Stack_Output_From_Foreign_Treetops;
          
          SF << "\nplaceholder_For_Future_Settings " << 0;
          
          SF << "\nplaceholder_For_Future_Settings " << 0;
          
          SF << "\nplaceholder_For_Future_Settings " << 0;
          
          SF << "\nsauto_Type " << sauto_Type;
          
          //Whether or not the RC is being used for the autocalibration.
          SF << "\nsauto_Use_RC " << sauto_Use_RC;
          
          //Now that the settings are read in its time for the node network.
          Nodes.save(&SF);
          
          SF.close();
     }
     
     //Loads a construct.     
     void Load(string p_Name, string p_Dir = "NT3.Save/")
     {
          //The save file.
          ifstream LF;
          
          string tmp_Setting_Name = "";
          string tmp_Setting_Placeholder = "";
          
          string tmp_LF_FN = p_Dir + p_Name;
          
          LF.open(tmp_LF_FN);
          
          if (!LF.is_open()){ cout << "\n\t SAVE FILE " << p_Name << " CANNOT BE OPENED!!!"; system("PAUSE > NULL"); return; }
          
          //-- Read the settings into the file.
          
          //The constructs name.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> Name;
          cout << " " << Name;
          set_Name(Name);
          
          //== Charging_Legs
          
          //Charging legs.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> setting_Charge_0;
          cout << " " << setting_Charge_0;
          
          //Charging legs.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> setting_Charge_1;
          cout << " " << setting_Charge_1;
          
          //Charging legs.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> setting_Charge_2;
          cout << " " << setting_Charge_2;
          
          //Charging legs.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> setting_Charge_3;
          cout << " " << setting_Charge_3;
          
          
          //== Settings
          
          //The tier to start charging.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Tier_Start;
          cout << " " << settings_Tier_Start;
          
          //The tier to start charging.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Tier_End;
          cout << " " << settings_Tier_End;
          
          //The Action Potential Threshold as percentage of the base charge.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Action_Potential_Threshold;
          cout << " " << settings_Action_Potential_Threshold;
          
          //The base charge to apply from the CAN.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Base_Charge;
          cout << " " << settings_Base_Charge;
          
          //The multiple to reduce each recursive charge by.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Modifier_Charge;
          cout << " " << settings_Modifier_Charge;
          
          //Whether or not to stack outputs when discharging foreign nodes.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Stack_Output_From_Foreign_Treetops;
          cout << " " << settings_Stack_Output_From_Foreign_Treetops;
          
          //Placeholder For Future Settings
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> tmp_Setting_Placeholder;
          cout << " " << tmp_Setting_Placeholder;
          
          //Placeholder For Future Settings
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> tmp_Setting_Placeholder;
          cout << " " << tmp_Setting_Placeholder;
          
          //Placeholder For Future Settings
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> tmp_Setting_Placeholder;
          cout << " " << tmp_Setting_Placeholder;
          
          //Placeholder For Future Settings
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> sauto_Type;
          cout << " " << sauto_Type;
          
          //Placeholder For Future Settings
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> sauto_Use_RC;
          cout << " " << sauto_Use_RC;
          
          //Now that the settings are read in its time for the node network.
          Nodes.load(&LF);
          
          LF.close();
     }
     
};