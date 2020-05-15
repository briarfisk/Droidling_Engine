


//The construct class.
class c_NT3_Construct_1D
{
private:
     
     //The node network to use.
     c_NT3_Node_Network_1D Nodes;
     
     //The CAN used for building the memory trees.
     c_NT3_CAN_1D CAN;
     
     //The CAN used for charging the memory trees.
     c_NT3_Poly_CAN_1D * PCAN;
     
     //The input rows for the PCAN.
     c_Raw_Table_Row_1D * PCAN_Input_Rows;
     
     //The Charging_Buffers. One for each input.
     c_NT3_Buffer * Charging_Buffers;
     
     //The Vision. One for each input.
     c_NT3_Vision_Buffer * Vision_Buffers;
     
     //The current input depth.
     int Input_Depth;
     
     //The current input depth to charge using the settings_Charging_Depth
     int Input_Charging_Depth;
     
     //The current input being evaluated.
     int Current_Input;
     
public:
     
     
     //The name of the Construct.
     string Name;
     
     //The input tables.
     c_Raw_Table_1D tbl_Input;
     
     c_Raw_Table_1D tbl_Input_Aux_L;
     c_Raw_Table_1D tbl_Input_Charges_L;
     c_Raw_Table_1D tbl_Input_Position_L;
     c_Raw_Table_1D tbl_Input_Depths_L;
     
     c_Raw_Table_1D tbl_Input_Charges_R;
     c_Raw_Table_1D tbl_Input_Position_R;
     c_Raw_Table_1D tbl_Input_Aux_R;
     c_Raw_Table_1D tbl_Input_Depths_R;
     
     
     //Output Tables.
     c_Raw_Table_1D tbl_Pattern_Output_L;
     c_Raw_Table_1D tbl_Charge_Output_L;
     c_Raw_Table_1D tbl_Position_Output_L;
     c_Raw_Table_1D tbl_RC_Output_L;
     c_Raw_Table_1D tbl_Treetops_Output_L;
     
     c_Raw_Table_1D tbl_Pattern_Output_R;
     c_Raw_Table_1D tbl_Charge_Output_R;
     c_Raw_Table_1D tbl_Position_Output_R;
     c_Raw_Table_1D tbl_RC_Output_R;
     c_Raw_Table_1D tbl_Treetops_Output_R;
     
     c_Raw_Table_1D tbl_Treetops;
     
     c_Raw_Table_1D tbl_Vision_Output;
     c_Raw_Table_1D tbl_Vision_Charge_Output;
     
     c_Raw_Table_1D tbl_Depths_L;
     c_Raw_Table_1D tbl_Depths_R;
     
     //--  Settings
     
     //Charging legs.
     bool setting_Charge_L;
     bool setting_Charge_R;
     
     //The tier to start charging.
     int settings_Tier;
     
     //Fixed vrs floating tier depth. If floating then Tier = (Input_Depth * Tier_Depth)
     int settings_Tier_Is_Floating;
     
     //The floating tier modifier.
     double settings_Tier_Depth;
     
     //The Action Potential Threshold as percentage of the base charge.
     double settings_Action_Potential_Threshold;
     
     //The base charge to apply from the CAN.
     int settings_Base_Charge;
     
     //The multiple to reduce each recursive charge by.
     double settings_Modifier_Charge;
     
     //Whether or not to use the input charges for charging.
     bool setting_Use_Input_Charges;
     
     //Build a vision or do not build a vision.
     bool settings_Build_Vision;
     
     //Sort the tables by charge.
     bool settings_Bubble_Sort_Output;
     
     //The percentage of the input string to charge starting at index 0.
     double settings_Input_Charging_Depth;
     
     //Cross CAN Charging, when charging a leg all nodes are charged in the CAN to the (L||R) of the current,
     //rather than just charging a column of the CAN.
     // 0 = Normal, 1 = Normal + TNT, 2 = Full Charge
     int settings_Cross_CAN_Charging;
     
     //Determines whether or not to stack outputs when discharging treetops from foreign constructs.
     int settings_Stack_Output_From_Foreign_Treetops;
     
     //The flag for using the charging spike in the buffer charging.
     int setting_Charge_Spike;
     
     //==--  SETTINGS FOR THE AUTOCALIBRATION
     
     //The type of auto to use, Numerical or Alphabetical for example.
     int sauto_Type;
     
     //Whether or not the RC is being evaluated.
     int sauto_Use_RC;
     
     c_NT3_Construct_1D()
     {
          u_Data_3 tmp_Name;
          tmp_Name.NR = this;
          Name = "C__" + int2str(tmp_Name.U);
          
          set_Name(Name);
          
          Current_Input = 0;
          
          setting_Charge_L = 1;
          setting_Charge_R = 1;
          
          settings_Tier = 0;
          settings_Tier_Is_Floating = 0;
          settings_Tier_Depth = 0;
          
          settings_Action_Potential_Threshold = 0;
          settings_Base_Charge = 10000;
          
          setting_Use_Input_Charges = 0;
          
          settings_Modifier_Charge = .5;
          
          settings_Bubble_Sort_Output = 0;
          
          settings_Build_Vision = 1;
          
          settings_Input_Charging_Depth = 1;
          
          settings_Cross_CAN_Charging = 0;
          
          settings_Stack_Output_From_Foreign_Treetops = 0;
          
          setting_Charge_Spike = 1;
          
          CAN.init(&Nodes);
          Charging_Buffers = NULL;
          PCAN = NULL;
          PCAN_Input_Rows = NULL;
          Vision_Buffers = NULL;
          Input_Depth = 0;
          Input_Charging_Depth = 0;
          
          sauto_Use_RC = 0;
          sauto_Type = 0;
     }
     
     ~c_NT3_Construct_1D()
     {
          cout << "\n  ~c_NT3_Construct_1D " << Name << " " << this << ".........."; cout.flush();
          
          delete [] Charging_Buffers;
          Charging_Buffers = NULL;
          delete [] PCAN;
          PCAN = NULL;
          Input_Depth = 0;
          cout << "\n  ~~~c_NT3_Construct_1D " << Name << " " << this << ".........."; cout.flush();
     }
     
     //Sets the name of the construct and the tables.
     void set_Name(string p_Name)
     {
          Name = p_Name;
          
          //The input tables.
          tbl_Input.Name = Name + "___Input";
          
          tbl_Input_Aux_L.Name = Name + "___Input_Aux_L";
          tbl_Input_Charges_L.Name = Name + "___Input_Charges_L";
          tbl_Input_Position_L.Name = Name + "___Input_Position_L";
          tbl_Input_Depths_L.Name = Name + "___Input_Depths_L";
          
          tbl_Input_Aux_R.Name = Name + "___Input_Aux_R";
          tbl_Input_Charges_R.Name = Name + "___Input_Charges_R";
          tbl_Input_Position_R.Name = Name + "___Input_Position_R";
          tbl_Input_Depths_R.Name = Name + "___Input_Depths_R";
          
          //Output Tables.
          tbl_Pattern_Output_L.Name = Name + "___Pattern_Output_L";
          tbl_Charge_Output_L.Name = Name + "___Charge_Output_L";
          tbl_Position_Output_L.Name = Name + "___Position_Output_L";
          tbl_RC_Output_L.Name = Name + "___RC_Output_L";
          tbl_Treetops_Output_L.Name = Name + "___Treetops_Output_L";
          
          tbl_Pattern_Output_R.Name = Name + "___Pattern_Output_R";
          tbl_Charge_Output_R.Name = Name + "___Charge_Output_R";
          tbl_Position_Output_R.Name = Name + "___Position_Output_R";
          tbl_RC_Output_R.Name = Name + "___RC_Output_R";
          tbl_Treetops_Output_R.Name = Name + "___Treetops_Output_R";
          
          tbl_Treetops.Name = Name + "___Treetops";
          
          tbl_Vision_Output.Name = Name + "___Vision_Output";
          tbl_Vision_Charge_Output.Name = Name + "___Vision_Charge_Output";
          
          tbl_Depths_L.Name = Name + "___Depths_L";
          tbl_Depths_R.Name = Name + "___Depths_R";
     }
     
     ////==-----------------------+
     //==--   EVAL FUNCTIONS
     ////==-----------------------+
     
     //Eval with no RC.
     void Eval()
     {
          reset_Outputs();
          Current_Input = 0;
          
          if (tbl_Input.Number_Of_Rows == 0){ return; }
          
          for (int cou_Cell=0;cou_Cell<tbl_Input.Rows[0]->Depth;cou_Cell++)
          {
               Current_Input = cou_Cell;
               
               Build_Input_Query(cou_Cell);
               
               if (settings_Tier_Is_Floating)
               {
                    settings_Tier = (Input_Depth * settings_Tier_Depth);
                    //*cout << "\n " << Name << " settings_Tier->" << settings_Tier << " = Input_Depth->" << Input_Depth << " * " << settings_Tier_Depth;
               }
               
               if (setting_Charge_L)
               {
                    init_Charging_Buffers();
                    //charge_Buffers_L();
                    //discharge_All_Treetops_L();
               }
               
               if (setting_Charge_R)
               {
                    init_Charging_Buffers();
                    //charge_Buffers_R();
                    //discharge_All_Treetops_R();
               }
               
               
               //gather_Vision();
               
               /*
               if (tbl_Vision_Output.Rows != NULL)
               {
                    cout << "\n-<{";
                    if (tbl_Vision_Output.Rows[0]->Cells != NULL)
                    {
                         tbl_Vision_Output.Rows[0]->Cells[0]->output_C();
                    }
                    cout << "}>-";
               }
               */
               //tbl_Vision_Charge_Output.output_I();
               
          }
     }
     
     //Eval with RC.
     void Eval_RC()
     {
          reset_Outputs();
          Current_Input = 0;
          
          if (tbl_Input.Number_Of_Rows == 0){ return; }
          
          for (int cou_Cell=0;cou_Cell<tbl_Input.Rows[0]->Depth;cou_Cell++)
          {
               Current_Input = cou_Cell;
               
               Build_Input_Query(cou_Cell);
               
               if (settings_Tier_Is_Floating)
               {
                    settings_Tier = (Input_Depth * settings_Tier_Depth);
               }
               
               if (setting_Charge_L)
               {
                    init_Charging_Buffers();
                    charge_Buffers_RC_L();
                    discharge_All_Treetops_L();
               }
               
               if (setting_Charge_R)
               {
                    init_Charging_Buffers();
                    charge_Buffers_RC_R();
                    discharge_All_Treetops_R();
               }
               
               //gather_Vision();
               
               /*
               if (tbl_Vision_Output.Rows != NULL)
               {
                    cout << "\n-<{";
                    if (tbl_Vision_Output.Rows[0]->Cells != NULL)
                    {
                         tbl_Vision_Output.Rows[0]->Cells[0]->output_C();
                    }
                    cout << "}>-";
               }
               */
               
          }
     }
     
     //Eval with no RC.
     void Eval_Aux_Inputs()
     {
          reset_Outputs();
          Current_Input = 0;
          
          //For each cell the input pattern will be gathered. This pattern will be distributed to the appropriate charging
          //buffers designated by the pattern input table. The state will be gathered from the node network and the resultant node
          //will be submitted to the charging buffer along with the charge from the charge table.
          if (setting_Charge_L && !(tbl_Input_Aux_L.Number_Of_Rows == 0))
          {
               //*cout << "\n LEFT -------===========--------";
               for (int cou_Cell=0;cou_Cell<tbl_Input_Aux_L.Rows[0]->Depth;cou_Cell++)
               {
                    Current_Input = cou_Cell;
                    //*cout << "\n\n cou_Cell->" << cou_Cell;
                    
                    Input_Depth = (tbl_Input_Depths_L.Rows[0]->Cells[cou_Cell]->Data[0]).U;
                    Input_Charging_Depth = Input_Depth;
                    
                    //*cout << "\n   init_Charging_Buffers()";
                    init_Charging_Buffers();
                    //*cout << "\n   gather_Aux_Input_L()";
                    gather_Aux_Input_L();
                    //*cout << "\n   charge_Buffers_L()";
                    charge_Aux_Buffers_L();
                    //*cout << "\n   discharge_All_Treetops_L()";
                    discharge_All_Treetops_L();
               }
               //*cout << "\n Discharge_Left.....Output_Tables";
               //output_Output_Tables();
               //*cout << "\n DONE ------------===========-------------";
          }
          
          if (setting_Charge_R && !(tbl_Input_Aux_R.Number_Of_Rows == 0))
          {
               //*cout << "\n RIGHT -------===========--------";
               for (int cou_Cell=0;cou_Cell<tbl_Input_Aux_R.Rows[0]->Depth;cou_Cell++)
               {
                    Current_Input = cou_Cell;
                    //*cout << "\n\n cou_Cell->" << cou_Cell;
                    
                    Input_Depth = (tbl_Input_Depths_R.Rows[0]->Cells[cou_Cell]->Data[0]).U;
                    Input_Charging_Depth = Input_Depth;
                    //*cout << "\n   Input_Charging_Depth->" << Input_Charging_Depth;
                    //*cout << "\n   init_Charging_Buffers()";
                    init_Charging_Buffers();
                    //*cout << "\n   gather_Aux_Input_R()";
                    gather_Aux_Input_R();
                    //*cout << "\n   charge_Buffers_R()";
                    charge_Aux_Buffers_R();
                    //*cout << "\n   discharge_All_Treetops_R()";
                    discharge_All_Treetops_R();
               }
               //*cout << "\n Discharge_Right.....Output_Tables";
               //*output_Output_Tables();
               //*cout << "\n DONE ------------===========-------------";
          }
          
     }
     
     
     //Eval with no RC.
     void Eval_Aux_Inputs_RC()
     {
          reset_Outputs();
          Current_Input = 0;
          
          //For each cell the input pattern will be gathered. This pattern will be distributed to the appropriate charging
          //buffers designated by the pattern input table. The state will be gathered from the node network and the resultant node
          //will be submitted to the charging buffer along with the charge from the charge table.
          if (setting_Charge_L && !(tbl_Input_Aux_L.Number_Of_Rows == 0))
          {
               //*cout << "\n LEFT -------===========--------";
               for (int cou_Cell=0;cou_Cell<tbl_Input_Aux_L.Rows[0]->Depth;cou_Cell++)
               {
                    Current_Input = cou_Cell;
                    //*cout << "\n\n cou_Cell->" << cou_Cell;
                    
                    Input_Depth = (tbl_Input_Depths_L.Rows[0]->Cells[cou_Cell]->Data[0]).U;
                    Input_Charging_Depth = Input_Depth;
                    
                    //*cout << "\n   init_Charging_Buffers()";
                    init_Charging_Buffers(Input_Depth);
                    //*cout << "\n   gather_Aux_Input_L()";
                    gather_Aux_Input_L();
                    //*cout << "\n   charge_Buffers_L()";
                    charge_Aux_Buffers_RC_L();
                    //*cout << "\n   discharge_All_Treetops_L()";
                    discharge_All_Treetops_L();
               }
               //*cout << "\n DONE ------------===========-------------";
          }
          
          if (setting_Charge_R && !(tbl_Input_Aux_R.Number_Of_Rows == 0))
          {
               //*cout << "\n RIGHT -------===========--------";
               for (int cou_Cell=0;cou_Cell<tbl_Input_Aux_R.Rows[0]->Depth;cou_Cell++)
               {
                    Current_Input = cou_Cell;
                    //*cout << "\n\n cou_Cell->" << cou_Cell;
                    
                    Input_Depth = (tbl_Input_Depths_R.Rows[0]->Cells[cou_Cell]->Data[0]).U;
                    Input_Charging_Depth = Input_Depth;
                    
                    //*cout << "\n   init_Charging_Buffers()";
                    init_Charging_Buffers(Input_Depth);
                    //*cout << "\n   gather_Aux_Input_L()";
                    gather_Aux_Input_R();
                    //*cout << "\n   charge_Buffers_L()";
                    charge_Aux_Buffers_RC_R();
                    //*cout << "\n   discharge_All_Treetops_L()";
                    discharge_All_Treetops_R();
               }
               //*cout << "\n DONE ------------===========-------------";
          }
          
     }
     
          
     //Eval with no RC.
     void Eval_Aux_Poly_Inputs(int p_Strip = 0)
     {
          reset_Outputs();
          Current_Input = 0;
          
          //For each cell the input pattern will be gathered. This pattern will be distributed to the appropriate charging
          //buffers designated by the pattern input table. The state will be gathered from the node network and the resultant node
          //will be submitted to the charging buffer along with the charge from the charge table.
          if (setting_Charge_L && !(tbl_Input_Aux_L.Number_Of_Rows == 0))
          {
               //*cout << "\n LEFT -------===========--------";
               for (int cou_Cell=0;cou_Cell<tbl_Input_Aux_L.Rows[0]->Depth;cou_Cell++)
               {
                    Current_Input = cou_Cell;
                    //*cout << "\n\n cou_Cell->" << cou_Cell;
                    
                    Input_Depth = (tbl_Input_Depths_L.Rows[0]->Cells[cou_Cell]->Data[0]).U;
                    Input_Charging_Depth = Input_Depth;
                    
                    //*cout << "\n   init_Charging_Buffers()";
                    init_Charging_Buffers();
                    
                    init_PCAN();
                    //*cout << "\n   gather_Aux_Input_L()";
                    gather_Aux_Input_Poly_L(p_Strip);
                    
                    /*
                    for (int cou_PCAN=0;cou_PCAN<Input_Depth;cou_PCAN++)
                    {
                         PCAN[cou_PCAN].output_CAN_Axons();
                    }
                    */
                    //*cout << "\n   charge_Buffers_L()";
                    charge_Poly_Aux_Buffers();
                    //*cout << "\n   discharge_All_Treetops_L()";
                    discharge_All_Treetops_L();
                    
               }
               //*cout << "\n Discharge_Left.....Output_Tables";
               //output_Output_Tables();
               //*cout << "\n DONE ------------===========-------------";
          }
          
          if (setting_Charge_R && !(tbl_Input_Aux_R.Number_Of_Rows == 0))
          {
               //*cout << "\n LEFT -------===========--------";
               for (int cou_Cell=0;cou_Cell<tbl_Input_Aux_R.Rows[0]->Depth;cou_Cell++)
               {
                    Current_Input = cou_Cell;
                    //*cout << "\n\n cou_Cell->" << cou_Cell;
                    
                    Input_Depth = (tbl_Input_Depths_R.Rows[0]->Cells[cou_Cell]->Data[0]).U;
                    Input_Charging_Depth = Input_Depth;
                    
                    //*cout << "\n   init_Charging_Buffers()";
                    init_Charging_Buffers();
                    
                    init_PCAN();
                    //*cout << "\n   gather_Aux_Input_R()";
                    gather_Aux_Input_Poly_R(p_Strip);
                    
                    /*
                    for (int cou_PCAN=0;cou_PCAN<Input_Depth;cou_PCAN++)
                    {
                         PCAN[cou_PCAN].output_CAN_Axons();
                    }
                    */
                    //*cout << "\n   charge_Buffers_R()";
                    charge_Poly_Aux_Buffers();
                    //*cout << "\n   discharge_All_Treetops_R()";
                    discharge_All_Treetops_R();
                    
               }
               //*cout << "\n Discharge_Left.....Output_Tables";
               //output_Output_Tables();
               //*cout << "\n DONE ------------===========-------------";
          }
          
     }
     
     //Eval with RC.
     void Eval_Aux_Poly_Inputs_RC(int p_Strip = 0)
     {
          reset_Outputs();
          Current_Input = 0;
          
          //For each cell the input pattern will be gathered. This pattern will be distributed to the appropriate charging
          //buffers designated by the pattern input table. The state will be gathered from the node network and the resultant node
          //will be submitted to the charging buffer along with the charge from the charge table.
          if (setting_Charge_L && !(tbl_Input_Aux_L.Number_Of_Rows == 0))
          {
               //cout << "\n LEFT -------===========--------";
               for (int cou_Cell=0;cou_Cell<tbl_Input_Aux_L.Rows[0]->Depth;cou_Cell++)
               {
                    Current_Input = cou_Cell;
                    //cout << "\n\n cou_Cell->" << cou_Cell;
                    
                    Input_Depth = (tbl_Input_Depths_L.Rows[0]->Cells[cou_Cell]->Data[0]).U;
                    Input_Charging_Depth = Input_Depth;
                    
                    //cout << "\n   init_Charging_Buffers()";
                    init_Charging_Buffers();
                    
                    init_PCAN();
                    //cout << "\n   gather_Aux_Input_L()";
                    gather_Aux_Input_Poly_L(p_Strip);
                    
                    /*
                    for (int cou_PCAN=0;cou_PCAN<Input_Depth;cou_PCAN++)
                    {
                         PCAN[cou_PCAN].output_CAN_Axons();
                    }
                    */
                    //cout << "\n   charge_Buffers_L()";
                    charge_Poly_Aux_Buffers_RC();
                    //cout << "\n   discharge_All_Treetops_L()";
                    discharge_All_Treetops_L();
                    
               }
               //cout << "\n Discharge_Left.....Output_Tables";
               //output_Output_Tables();
               //cout << "\n DONE ------------===========-------------";
          }
          
          if (setting_Charge_R && !(tbl_Input_Aux_R.Number_Of_Rows == 0))
          {
               //*cout << "\n LEFT -------===========--------";
               for (int cou_Cell=0;cou_Cell<tbl_Input_Aux_R.Rows[0]->Depth;cou_Cell++)
               {
                    Current_Input = cou_Cell;
                    //*cout << "\n\n cou_Cell->" << cou_Cell;
                    
                    Input_Depth = (tbl_Input_Depths_R.Rows[0]->Cells[cou_Cell]->Data[0]).U;
                    Input_Charging_Depth = Input_Depth;
                    
                    //*cout << "\n   init_Charging_Buffers()";
                    init_Charging_Buffers();
                    
                    init_PCAN();
                    //*cout << "\n   gather_Aux_Input_R()";
                    gather_Aux_Input_Poly_R(p_Strip);
                    
                    /*
                    for (int cou_PCAN=0;cou_PCAN<Input_Depth;cou_PCAN++)
                    {
                         PCAN[cou_PCAN].output_CAN_Axons();
                    }
                    */
                    //*cout << "\n   charge_Buffers_R()";
                    charge_Poly_Aux_Buffers_RC();
                    //*cout << "\n   discharge_All_Treetops_R()";
                    discharge_All_Treetops_R();
                    
               }
               //*cout << "\n Discharge_Left.....Output_Tables";
               //output_Output_Tables();
               //*cout << "\n DONE ------------===========-------------";
          }
     }
     
          
     //Eval with no RC.
     void Eval_Aux_Poly_Inputs_Inclusively(int p_Strip = 0)
     {
          reset_Outputs();
          Current_Input = 0;
          
          //For each cell the input pattern will be gathered. This pattern will be distributed to the appropriate charging
          //buffers designated by the pattern input table. The state will be gathered from the node network and the resultant node
          //will be submitted to the charging buffer along with the charge from the charge table.
          if (setting_Charge_L && !(tbl_Input_Aux_L.Number_Of_Rows == 0))
          {
               //*cout << "\n LEFT -------===========--------";
               for (int cou_Cell=0;cou_Cell<tbl_Input_Aux_L.Rows[0]->Depth;cou_Cell++)
               {
                    Current_Input = cou_Cell;
                    //*cout << "\n\n cou_Cell->" << cou_Cell;
                    
                    Input_Depth = (tbl_Input_Depths_L.Rows[0]->Cells[cou_Cell]->Data[0]).U;
                    Input_Charging_Depth = Input_Depth;
                    
                    //*cout << "\n   init_Charging_Buffers()";
                    init_Charging_Buffers();
                    
                    init_PCAN();
                    //*cout << "\n   gather_Aux_Input_L()";
                    gather_Aux_Input_Poly_Inclusive_L(p_Strip);
                    
                    /*
                    for (int cou_PCAN=0;cou_PCAN<Input_Depth;cou_PCAN++)
                    {
                         PCAN[cou_PCAN].output_CAN_Axons();
                    }
                    */
                    //*cout << "\n   charge_Buffers_L()";
                    charge_Poly_Aux_Buffers_Inclusively_L();
                    //*cout << "\n   discharge_All_Treetops_L()";
                    discharge_All_Treetops_L();
                    
               }
               //*cout << "\n Discharge_Left.....Output_Tables";
               //output_Output_Tables();
               //*cout << "\n DONE ------------===========-------------";
          }
          
          
          if (setting_Charge_R && !(tbl_Input_Aux_R.Number_Of_Rows == 0))
          {
          }
          
     }
     
     //Eval with RC.
     void Eval_Aux_Poly_Inputs_Inclusively_RC(int p_Strip = 0)
     {
          reset_Outputs();
          Current_Input = 0;
          
          //For each cell the input pattern will be gathered. This pattern will be distributed to the appropriate charging
          //buffers designated by the pattern input table. The state will be gathered from the node network and the resultant node
          //will be submitted to the charging buffer along with the charge from the charge table.
          if (setting_Charge_L && !(tbl_Input_Aux_L.Number_Of_Rows == 0))
          {
               //*cout << "\n LEFT -------===========--------";
               for (int cou_Cell=0;cou_Cell<tbl_Input_Aux_L.Rows[0]->Depth;cou_Cell++)
               {
                    Current_Input = cou_Cell;
                    //*cout << "\n\n cou_Cell->" << cou_Cell;
                    
                    Input_Depth = (tbl_Input_Depths_L.Rows[0]->Cells[cou_Cell]->Data[0]).U;
                    Input_Charging_Depth = Input_Depth;
                    
                    //*cout << "\n   init_Charging_Buffers()";
                    init_Charging_Buffers();
                    
                    init_PCAN();
                    //*cout << "\n   gather_Aux_Input_L()";
                    gather_Aux_Input_Poly_Inclusive_L(p_Strip);
                    
                    /*
                    for (int cou_PCAN=0;cou_PCAN<Input_Depth;cou_PCAN++)
                    {
                         PCAN[cou_PCAN].output_CAN_Axons();
                    }
                    */
                    //*cout << "\n   charge_Buffers_L()";
                    charge_Poly_Aux_Buffers_Inclusively_L();
                    //*cout << "\n   discharge_All_Treetops_L()";
                    discharge_All_Treetops_L();
                    
               }
               //*cout << "\n Discharge_Left.....Output_Tables";
               //output_Output_Tables();
               //*cout << "\n DONE ------------===========-------------";
          }
          
          
          if (setting_Charge_R && !(tbl_Input_Aux_R.Number_Of_Rows == 0))
          {
          }
          
     }
     
     
     //Sets the charging buffers up for each input.
     void init_Charging_Buffers()
     {
          delete [] Charging_Buffers;
          Charging_Buffers = NULL;
          Charging_Buffers = new c_NT3_Buffer[Input_Depth];
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               Charging_Buffers[cou_Index].Base_Charge = settings_Base_Charge;
               Charging_Buffers[cou_Index].Modifier_Charge = settings_Modifier_Charge;
               Charging_Buffers[cou_Index].Action_Potential_Threshold = settings_Action_Potential_Threshold;
               //Charging_Buffers[cou_Index].resize_Opp((Input_Depth - cou_Index));
          }
     }     
     
     //Sets the charging buffers up for each input.
     void init_Charging_Buffers(int p_Input_Depth)
     {
          Input_Depth = p_Input_Depth;
          Input_Charging_Depth = Input_Depth;
          
          delete [] Charging_Buffers;
          Charging_Buffers = NULL;
          Charging_Buffers = new c_NT3_Buffer[Input_Depth];
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               Charging_Buffers[cou_Index].Base_Charge = settings_Base_Charge;
               Charging_Buffers[cou_Index].Modifier_Charge = settings_Modifier_Charge;
               Charging_Buffers[cou_Index].Action_Potential_Threshold = settings_Action_Potential_Threshold;
          }
     }
     
     //Sets the charging buffers up for each input.
     void init_PCAN()
     {
          delete [] PCAN;
          PCAN = NULL;
          PCAN = new c_NT3_Poly_CAN_1D[Input_Depth];
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               PCAN[cou_Index].reset();
               PCAN[cou_Index].init(&Nodes);
          }
     }
     
     //Charges a single buffer.
     void charge_Aux_Buffer_L(int p_Input)
     {
          //*cout << "\n\t     charge_Aux_Buffer_L(" << p_Input <<  ")";
          Charging_Buffers[p_Input].Input_Position = p_Input;
          Charging_Buffers[p_Input].gather();
          
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               //*Charging_Buffers[p_Input].output_Buffer();
               
               Charging_Buffers[p_Input].charge_Outputs_L(setting_Charge_Spike);
               
               //*Charging_Buffers[p_Input].output_Buffer();
               
               Charging_Buffers[p_Input].gather();
               
               //*Charging_Buffers[p_Input].output_Buffer();
          }
     }
     
     //Charges a single buffer.
     void charge_Aux_Buffer_R(int p_Input)
     {
          //*cout << "\n\t     charge_Aux_Buffer_R(" << p_Input <<  ")";
          Charging_Buffers[p_Input].Input_Position = p_Input;
          Charging_Buffers[p_Input].gather();
          
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               //*Charging_Buffers[p_Input].output_Buffer();
               
               Charging_Buffers[p_Input].charge_Outputs_R(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].gather();
          }
     }
     
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Aux_Buffers_L()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               charge_Aux_Buffer_L(cou_Index);
          }
     }
     
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Aux_Buffers_R()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               charge_Aux_Buffer_R(cou_Index);
          }
     }
     
     //RC CHARGING FOR AUX INPUTS
     //Charges a single buffer.
     void charge_Aux_Buffer_RC_L(int p_Input)
     {
          Charging_Buffers[p_Input].Input_Position = p_Input;
          Charging_Buffers[p_Input].gather();
          
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               Charging_Buffers[p_Input].charge_Outputs_RC_L(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].gather();
          }
     }
     
     //Charges a single buffer.
     void charge_Aux_Buffer_RC_R(int p_Input)
     {
          Charging_Buffers[p_Input].Input_Position = p_Input;
          Charging_Buffers[p_Input].gather();
          
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               Charging_Buffers[p_Input].charge_Outputs_RC_R(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].gather();
          }
     }
     
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Aux_Buffers_RC_L()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               charge_Aux_Buffer_RC_L(cou_Index);
          }
     }
     
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Aux_Buffers_RC_R()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               charge_Aux_Buffer_RC_R(cou_Index);
          }
     }
     
     //Charges a single buffer.
     void charge_Buffer_L(int p_Input)
     {
          Charging_Buffers[p_Input].Input_Position = p_Input;
          
          //Cross CAN Charging.
          if (settings_Cross_CAN_Charging == 1 && settings_Tier == 0)
          {
               for (int cou_Index=p_Input;cou_Index<Input_Depth;cou_Index++)
               {
                    if ((CAN.CAN[0] [cou_Index] != NULL)){ Charging_Buffers[p_Input].submit(CAN.CAN[0] [cou_Index], settings_Base_Charge); }
               }
          }
          
          for (int cou_T=settings_Tier;cou_T<Input_Depth;cou_T++)
          {    
               Charging_Buffers[p_Input].charge_Outputs_L(setting_Charge_Spike);
               
               if (((Input_Depth - cou_T) - p_Input) > 0 && (CAN.CAN[cou_T] [p_Input] != NULL)){ Charging_Buffers[p_Input].submit(CAN.CAN[cou_T] [p_Input], settings_Base_Charge); }
               
               Charging_Buffers[p_Input].gather();
               
               if (!Charging_Buffers[p_Input].flg_Not_Done){ break; }
               
          }
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               Charging_Buffers[p_Input].charge_Outputs_L(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].gather();
          }
     }
     
     //Charges a single buffer.
     void charge_Buffer_R(int p_Input)
     {
          Charging_Buffers[p_Input].Input_Position = p_Input;
          
          
          //Cross CAN Charging.
          if (settings_Cross_CAN_Charging == 1 && settings_Tier == 0)
          {
               for (int cou_Index=0;cou_Index<=p_Input;cou_Index++)
               {
                    if ((CAN.CAN[0] [cou_Index] != NULL)){ Charging_Buffers[p_Input].submit(CAN.CAN[0] [cou_Index], settings_Base_Charge); }
               }
          }
          
          for (int cou_T=settings_Tier;cou_T<Input_Depth;cou_T++)
          {
               Charging_Buffers[p_Input].charge_Outputs_R(setting_Charge_Spike);
               
               if ((p_Input - cou_T) >= 0 && (CAN.CAN[cou_T] [p_Input - cou_T] != NULL)){ Charging_Buffers[p_Input].submit(CAN.CAN[cou_T] [p_Input - cou_T], settings_Base_Charge); }
               
               Charging_Buffers[p_Input].gather();
               
               if (!Charging_Buffers[p_Input].flg_Not_Done){ break; }
               
               //cout << "\n\t   Tier[" << cou_T << "] ->" << Charging_Buffers[p_Input].Current_Highest_Charge;
          }
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               Charging_Buffers[p_Input].charge_Outputs_R(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].gather();
          }
     }
     
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Buffers_L()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Charging_Depth;cou_Index++)
          {
               charge_Buffer_L(cou_Index);
          }
     }
     
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Buffers_R()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Charging_Depth;cou_Index++)
          {
               charge_Buffer_R(cou_Index);
          }
     }
     
     //Charges a single buffer.
     void charge_Buffer_RC_L(int p_Input)
     {
          Charging_Buffers[p_Input].Input_Position = p_Input;
          
          for (int cou_T=settings_Tier;cou_T<Input_Depth;cou_T++)
          {
               Charging_Buffers[p_Input].charge_Outputs_RC_L(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].destroy_Output_Buffers();
               
               if (((Input_Depth - cou_T) - p_Input) > 0 && (CAN.CAN[cou_T] [p_Input] != NULL)){ Charging_Buffers[p_Input].submit_RC(CAN.CAN[cou_T] [p_Input]); }
               
               Charging_Buffers[p_Input].gather();
               
               if (!Charging_Buffers[p_Input].flg_Not_Done){ break; }
          }
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               Charging_Buffers[p_Input].charge_Outputs_RC_L(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].destroy_Output_Buffers();
               
               Charging_Buffers[p_Input].gather();
          }
     }
     
     //Charges a single buffer.
     void charge_Buffer_RC_R(int p_Input)
     {
          Charging_Buffers[p_Input].Input_Position = p_Input;
          
          for (int cou_T=settings_Tier;cou_T<Input_Depth;cou_T++)
          {
               Charging_Buffers[p_Input].charge_Outputs_R(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].destroy_Output_Buffers();
               
               if ((p_Input - cou_T) >= 0 && (CAN.CAN[cou_T] [p_Input - cou_T] != NULL)){ Charging_Buffers[p_Input].submit_RC(CAN.CAN[cou_T] [p_Input - cou_T]); }
               
               Charging_Buffers[p_Input].gather();
               
               if (!Charging_Buffers[p_Input].flg_Not_Done){ break; }
          }
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               Charging_Buffers[p_Input].charge_Outputs_R(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].destroy_Output_Buffers();
               
               Charging_Buffers[p_Input].gather();
          }
     }
     
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Buffers_RC_L()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Charging_Depth;cou_Index++)
          {
               charge_Buffer_RC_L(cou_Index);
          }
     }
     
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Buffers_RC_R()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Charging_Depth;cou_Index++)
          {
               charge_Buffer_RC_R(cou_Index);
          }
     }
     

     //--  BUFFER CHARGING FOR THE POLY
     //CHARGING FOR AUX INPUTS
     //Charges a single buffer.
     //PCAN & Buffer use the p_Input index.
     void charge_Poly_Aux_Buffer(int p_Input)
     {
          int tmp_CAN_Tier_Position_Depth = 0;
          int tmp_Tier_Count = PCAN[p_Input].Number_Of_Tiers;
          int tmp_Node_Count = 0;
          
          //*cout << "\n\t     charge_Poly_Aux_Buffer_L(" << p_Input <<  ")";
          Charging_Buffers[p_Input].Input_Position = p_Input;
          //Charging_Buffers[p_Input].gather();
          
          //Gathers and charges the base tier.
          Charging_Buffers[p_Input].gather();
          Charging_Buffers[p_Input].charge_Outputs(setting_Charge_Spike);
          
          for (int cou_T=1;cou_T<tmp_Tier_Count;cou_T++)
          {
               //*cout << "\n #T->" << tmp_Tier_Count;
               Charging_Buffers[p_Input].charge_Outputs(setting_Charge_Spike);
               
               //Why was I destroying the old output buffers???
               Charging_Buffers[p_Input].destroy_Output_Buffers();
               
               tmp_CAN_Tier_Position_Depth = ((tmp_Tier_Count - cou_T));
               //*cout << "\n cou_T->"; oint(0, 7, cou_T);
               for (int cou_P=0;cou_P<tmp_CAN_Tier_Position_Depth;cou_P++)
               {
                    tmp_Node_Count = PCAN[p_Input].Number_Of_Nodes[cou_T] [cou_P];
                    //*cout << "  {"; oint(0, 13, cou_P); cout << " #N->"; oint(0, 7, tmp_Node_Count); cout << "}";
                    for (int cou_Node=0;cou_Node<tmp_Node_Count;cou_Node++)
                    {
                         Charging_Buffers[p_Input].submit(PCAN[p_Input].CAN[cou_T] [cou_P] [cou_Node], settings_Base_Charge); 
                         //*cout << " " << PCAN[p_Input].CAN[cou_T] [cou_P] [cou_Node];
                    }
               }
               Charging_Buffers[p_Input].gather();
               
               if (!Charging_Buffers[p_Input].flg_Not_Done){ break; }
          }
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               //*Charging_Buffers[p_Input].output_Buffer();
               
               Charging_Buffers[p_Input].charge_Outputs(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].gather();
               
               //*Charging_Buffers[p_Input].output_Buffer();
               
          }
     }
     
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Poly_Aux_Buffers()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               charge_Poly_Aux_Buffer(cou_Index);
          }
     }
     
     
     
     
     
     
     //--  BUFFER CHARGING FOR THE POLY
     //CHARGING FOR AUX INPUTS
     //Charges a single buffer.
     //PCAN & Buffer use the p_Input index.
     void charge_Poly_Aux_Buffer_RC(int p_Input)
     {
          //system("CLS");
          
          int tmp_CAN_Tier_Position_Depth = 0;
          int tmp_Tier_Count = PCAN[p_Input].Number_Of_Tiers;
          int tmp_Node_Count = 0;
          
          //*-*/cout << "\n\t     charge_Poly_Aux_Buffer_L(" << p_Input <<  ")"; cout.flush();
          Charging_Buffers[p_Input].Input_Position = p_Input;
          //Charging_Buffers[p_Input].gather();
          
          //Gathers and charges the base tier.
          Charging_Buffers[p_Input].gather();
          Charging_Buffers[p_Input].charge_Outputs_RC(setting_Charge_Spike);
          
          for (int cou_T=1;cou_T<tmp_Tier_Count;cou_T++)
          {
               //*-*/cout << "\n #T->" << tmp_Tier_Count; cout.flush();
               Charging_Buffers[p_Input].charge_Outputs_RC(setting_Charge_Spike);
               
               //Why was I destroying the old output buffers???
               Charging_Buffers[p_Input].destroy_Output_Buffers();
               
               tmp_CAN_Tier_Position_Depth = ((tmp_Tier_Count - cou_T));
               //*-*/cout << "\n cou_T->"; oint(0, 7, cou_T); cout.flush();
               for (int cou_P=0;cou_P<tmp_CAN_Tier_Position_Depth;cou_P++)
               {
                    tmp_Node_Count = PCAN[p_Input].Number_Of_Nodes[cou_T] [cou_P];
                    //*-*/cout << "  {"; oint(0, 13, cou_P); cout << " #N->"; oint(0, 7, tmp_Node_Count); cout << "}"; cout.flush();
                    for (int cou_Node=0;cou_Node<tmp_Node_Count;cou_Node++)
                    {
                         Charging_Buffers[p_Input].submit(PCAN[p_Input].CAN[cou_T] [cou_P] [cou_Node], settings_Base_Charge); 
                         //*-*/cout << " " << PCAN[p_Input].CAN[cou_T] [cou_P] [cou_Node]; cout.flush();
                    }
               }
               Charging_Buffers[p_Input].gather();
               
               
               if (!Charging_Buffers[p_Input].flg_Not_Done){ break; }
          }
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               Charging_Buffers[p_Input].charge_Outputs_RC(setting_Charge_Spike);
               Charging_Buffers[p_Input].gather();
          }
          //system("PAUSE > NULL");
     }
          
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Poly_Aux_Buffers_RC()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               charge_Poly_Aux_Buffer_RC(cou_Index);
          }
     }
     

     //Charges the poly input using the new incluse method of charging utilizeing the left and right buffers.
     void charge_Poly_Aux_Buffer_Inclusively_L(int p_Input)
     {
          int tmp_CAN_Tier_Position_Depth = 0;
          int tmp_Tier_Count = PCAN[p_Input].Number_Of_Tiers;
          int tmp_Node_Count = 0;
          
          //*cout << "\n\t     charge_Poly_Aux_Buffer_L(" << p_Input <<  ")";
          Charging_Buffers[p_Input].Input_Position = p_Input;
          //Charging_Buffers[p_Input].gather();
          
          //Gathers and charges the base tier.
          Charging_Buffers[p_Input].gather();
          Charging_Buffers[p_Input].charge_Outputs_L(setting_Charge_Spike);
          
          //Flush the Charging Buffers [1] Opp
          Charging_Buffers[p_Input].flush(1);
          
          for (int cou_T=1;cou_T<tmp_Tier_Count;cou_T++)
          {
               //*cout << "\n #T->" << tmp_Tier_Count;
               Charging_Buffers[p_Input].charge_Outputs_L(setting_Charge_Spike);
               
               //Why was I destroying the old output buffers???
               Charging_Buffers[p_Input].destroy_Output_Buffers();
               
               tmp_CAN_Tier_Position_Depth = ((tmp_Tier_Count - cou_T));
               //*cout << "\n cou_T->"; oint(0, 7, cou_T);
               
               //Read in the normal charging nodes.
               tmp_Node_Count = PCAN[p_Input].Number_Of_Nodes[cou_T] [0];
               //*cout << "  {"; oint(0, 13, cou_P); cout << " #N->"; oint(0, 7, tmp_Node_Count); cout << "}";
               for (int cou_Node=0;cou_Node<tmp_Node_Count;cou_Node++)
               {
                    //Look like i may already be charging all of the nodes.
                    Charging_Buffers[p_Input].submit(PCAN[p_Input].CAN[cou_T] [0] [cou_Node], settings_Base_Charge); 
                    //*cout << " " << PCAN[p_Input].CAN[cou_T] [cou_P] [cou_Node];
               }
               
               //Read in the Opp nodes. Because of the shifting effect the nodes are read into the index + tier.
               
               //Left
               // (0)
               // (0) (4)
               // (0) (3) (4)
               // (0) (2) (3) (4)
               // (0) (1) (2) (3) (4)
               
               //Right
               //                 (0)
               //             (4) (0)
               //         (4) (3) (0)
               //     (4) (3) (2) (0)
               // (4) (3) (2) (1) (0)
               
               for (int cou_P=1;cou_P<tmp_CAN_Tier_Position_Depth;cou_P++)
               {
                    tmp_Node_Count = PCAN[p_Input].Number_Of_Nodes[cou_T] [cou_P];
                    //*cout << "  {"; oint(0, 13, cou_P); cout << " #N->"; oint(0, 7, tmp_Node_Count); cout << "}";
                    for (int cou_Node=0;cou_Node<tmp_Node_Count;cou_Node++)
                    {
                         //Look like i may already be charging all of the nodes.
                         //The Opp submission must be (-1) for the index shift.
                         Charging_Buffers[p_Input].submit_Opp(((cou_P + cou_T) - 1), PCAN[p_Input].CAN[cou_T] [cou_P] [cou_Node], settings_Base_Charge); 
                         //*cout << " " << PCAN[p_Input].CAN[cou_T] [cou_P] [cou_Node];
                    }
               }
               Charging_Buffers[p_Input].gather();
               
               Charging_Buffers[p_Input].flush(cou_T + 1);
               
               //*cout << "\n Done->" << Charging_Buffers[p_Input].flg_Not_Done;
               //*Charging_Buffers[p_Input].output_Charge_LL();
               //*Charging_Buffers[p_Input].output_Output_LL();
               //*Charging_Buffers[p_Input].output_Treetops();
               //Charging_Buffers[p_Input].output_All_Buffers();
               
               if (!Charging_Buffers[p_Input].flg_Not_Done){ break; }
          }
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               //*Charging_Buffers[p_Input].output_Buffer();
               
               Charging_Buffers[p_Input].charge_Outputs_L(setting_Charge_Spike);
               
               //*Charging_Buffers[p_Input].output_Buffer();
               
               Charging_Buffers[p_Input].gather();
               
               //*Charging_Buffers[p_Input].output_Buffer();
               
          }
     }
          
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Poly_Aux_Buffers_Inclusively_L()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               charge_Poly_Aux_Buffer_Inclusively_L(cou_Index);
          }
     }
     
     
     //Charges the poly input using the new incluse method of charging utilizeing the left and right buffers.
     void charge_Poly_Aux_Buffer_Inclusively_RC_L(int p_Input)
     {
          int tmp_CAN_Tier_Position_Depth = 0;
          int tmp_Tier_Count = PCAN[p_Input].Number_Of_Tiers;
          int tmp_Node_Count = 0;
          
          //*cout << "\n\t     charge_Poly_Aux_Buffer_L(" << p_Input <<  ")";
          Charging_Buffers[p_Input].Input_Position = p_Input;
          //Charging_Buffers[p_Input].gather();
          
          //Gathers and charges the base tier.
          Charging_Buffers[p_Input].gather();
          Charging_Buffers[p_Input].charge_Outputs_RC_L(setting_Charge_Spike);
          
          //Flush the Charging Buffers [1] Opp
          Charging_Buffers[p_Input].flush(1);
          
          for (int cou_T=1;cou_T<tmp_Tier_Count;cou_T++)
          {
               //*cout << "\n #T->" << tmp_Tier_Count;
               Charging_Buffers[p_Input].charge_Outputs_RC_L(setting_Charge_Spike);
               
               //Why was I destroying the old output buffers???
               Charging_Buffers[p_Input].destroy_Output_Buffers();
               
               tmp_CAN_Tier_Position_Depth = ((tmp_Tier_Count - cou_T));
               //*cout << "\n cou_T->"; oint(0, 7, cou_T);
               
               //Read in the normal charging nodes.
               tmp_Node_Count = PCAN[p_Input].Number_Of_Nodes[cou_T] [0];
               //*cout << "  {"; oint(0, 13, cou_P); cout << " #N->"; oint(0, 7, tmp_Node_Count); cout << "}";
               for (int cou_Node=0;cou_Node<tmp_Node_Count;cou_Node++)
               {
                    //Look like i may already be charging all of the nodes.
                    Charging_Buffers[p_Input].submit_RC(PCAN[p_Input].CAN[cou_T] [0] [cou_Node], settings_Base_Charge); 
                    //*cout << " " << PCAN[p_Input].CAN[cou_T] [cou_P] [cou_Node];
               }
               
               //Read in the Opp nodes. Because of the shifting effect the nodes are read into the index + tier.
               
               //Left
               // (0)
               // (0) (4)
               // (0) (3) (4)
               // (0) (2) (3) (4)
               // (0) (1) (2) (3) (4)
               
               //Right
               //                 (0)
               //             (4) (0)
               //         (4) (3) (0)
               //     (4) (3) (2) (0)
               // (4) (3) (2) (1) (0)
               
               for (int cou_P=1;cou_P<tmp_CAN_Tier_Position_Depth;cou_P++)
               {
                    tmp_Node_Count = PCAN[p_Input].Number_Of_Nodes[cou_T] [cou_P];
                    //*cout << "  {"; oint(0, 13, cou_P); cout << " #N->"; oint(0, 7, tmp_Node_Count); cout << "}";
                    for (int cou_Node=0;cou_Node<tmp_Node_Count;cou_Node++)
                    {
                         //Look like i may already be charging all of the nodes.
                         //The Opp submission must be (-1) for the index shift.
                         Charging_Buffers[p_Input].submit_RC_Opp(((cou_P + cou_T) - 1), PCAN[p_Input].CAN[cou_T] [cou_P] [cou_Node], settings_Base_Charge); 
                         //*cout << " " << PCAN[p_Input].CAN[cou_T] [cou_P] [cou_Node];
                    }
               }
               Charging_Buffers[p_Input].gather();
               
               Charging_Buffers[p_Input].flush(cou_T + 1);
               
               //*cout << "\n Done->" << Charging_Buffers[p_Input].flg_Not_Done;
               //*Charging_Buffers[p_Input].output_Charge_LL();
               //*Charging_Buffers[p_Input].output_Output_LL();
               //*Charging_Buffers[p_Input].output_Treetops();
               //Charging_Buffers[p_Input].output_All_Buffers();
               
               if (!Charging_Buffers[p_Input].flg_Not_Done){ break; }
          }
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               //*Charging_Buffers[p_Input].output_Buffer();
               
               Charging_Buffers[p_Input].charge_Outputs_RC_L(setting_Charge_Spike);
               
               //*Charging_Buffers[p_Input].output_Buffer();
               
               Charging_Buffers[p_Input].gather();
               
               //*Charging_Buffers[p_Input].output_Buffer();
               
          }
     }
          
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Poly_Aux_Buffers_Inclusively_RC_L()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               charge_Poly_Aux_Buffer_Inclusively_RC_L(cou_Index);
          }
     }
     
     
     //Gathers the output for the left vision table.
     void gather_Vision_L()
     {
          //The position.
          int tmp_Position = 0;
          u_Data_3 tmp_Charge;
          int tmp_Length = 0;
          c_Raw_Table_Row_1D * tmp_Row = NULL;
          
          //*cout << "\n\n\nGV__L #R " << tbl_Pattern_Output_L.Number_Of_Rows;
          
          for (int cou_R=0;cou_R<tbl_Pattern_Output_L.Number_Of_Rows;cou_R++)
          {
               //*cout << "\n\n Row[" << cou_R << "]\t";
               //*tbl_Pattern_Output_L.Rows[cou_R]->output_C();
               //*cout << "\n";
               
               tmp_Row = tbl_Pattern_Output_L.Rows[cou_R];
               tmp_Position = tmp_Row->Cells[0]->Data[0].I;
               tmp_Charge.I = tmp_Row->Cells[1]->Data[0].I;
               tmp_Length = tmp_Row->Cells[2]->Length;
               
               //*cout << "\t P " << tmp_Position << "  C " << tmp_Charge.I << "  L " << tmp_Length << "\n";
               
               for (int cou_D=0;cou_D<tmp_Length;cou_D++)
               {
                    //*cout << "\n\t     cou_D->" << cou_D << " tfp->" << (cou_D + tmp_Position);
                    if ((cou_D + tmp_Position) >= Input_Depth){ continue; }
                    //*cout << "  [" << cou_D << "]->" << tmp_Row->Cells[2]->Data[cou_D].C << " $" << tmp_Charge.I << " p" << (cou_D + tmp_Position);
                    Vision_Buffers[(cou_D + tmp_Position)].submit(tmp_Row->Cells[2]->Data[cou_D], tmp_Charge.I);
               }
               //*cout << "\n\n";
          }
     }

     //Gathers the output for the left vision table.
     void gather_Vision_R()
     {
          //The position.
          int tmp_Position = 0;
          u_Data_3 tmp_Charge;
          int tmp_Length = 0;
          c_Raw_Table_Row_1D * tmp_Row = NULL;
          
          //*cout << "\n\n\nGV__R #R " << tbl_Pattern_Output_R.Number_Of_Rows;
          
          for (int cou_R=0;cou_R<tbl_Pattern_Output_R.Number_Of_Rows;cou_R++)
          {
               //*cout << "\n\n Row[" << cou_R << "]\t";
               //*tbl_Pattern_Output_R.Rows[cou_R]->output_C();
               //*cout << "\n";
               
               tmp_Row = tbl_Pattern_Output_R.Rows[cou_R];
               tmp_Position = tmp_Row->Cells[0]->Data[0].I;
               tmp_Charge.I = tmp_Row->Cells[1]->Data[0].I;
               tmp_Length = tmp_Row->Cells[2]->Length;
               
               //*cout << "\t P " << tmp_Position << "  C " << tmp_Charge.I << "  L " << tmp_Length << "\n";
               
               for (int cou_Data=0;cou_Data<tmp_Length;cou_Data++)
               {
                    //*cout << "\n\t     cou_Data->" << cou_Data << " tfp->" << (cou_Data + tmp_Position - (tmp_Length - 1));
                    if ((cou_Data + tmp_Position - (tmp_Length - 1)) < 0){ continue; }
                    //*cout << "  [" << cou_Data << "]->{" << tmp_Row->Cells[2]->Data[cou_Data].C << "} $" << tmp_Charge.I << " p" << (cou_Data + tmp_Position - (tmp_Length - 1));
                    
                    Vision_Buffers[(cou_Data + tmp_Position - (tmp_Length - 1))].submit(tmp_Row->Cells[2]->Data[cou_Data], tmp_Charge.I);
               }
          }
     }
     
     //Gathers the outputs into the vision table.
     void gather_Vision()
     {
          //If the settings say no then return;
          if (!settings_Build_Vision){ return; }
          
          //Setup the vision buffers.
          delete [] Vision_Buffers;
          Vision_Buffers = NULL;
          Vision_Buffers = new c_NT3_Vision_Buffer[Input_Depth];
          
          //The two cells that will hold the vision output.
          //c_Raw_Table_Cell_1D * tmp_V;
          //c_Raw_Table_Cell_1D * tmp_VC;
          
          //Read the left side into the vision buffer.
          if (setting_Charge_L)
          {
               //*cout << "\n\n Gather_Vision_L";
               gather_Vision_L();
          }
          
          //Read the right side into the vision buffer.
          if (setting_Charge_R)
          {
               //*cout << "\n\n Gather_Vision_R";
               gather_Vision_R();
          }
          
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               Vision_Buffers[cou_Index].gather();
               //*cout << "\n\t V[" << cou_Index << "] {" << Vision_Buffers[cou_Index].SState.C << "} " << Vision_Buffers[cou_Index].SCharge.I;
               tbl_Vision_Output.add_Data(Vision_Buffers[cou_Index].SState);
               tbl_Vision_Charge_Output.add_Data_Int(Vision_Buffers[cou_Index].SCharge.I);
          }
          
          //*system("PAUSE > NULL");
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
          tbl_Treetops.add_Int(Current_Input, (CAN.Treetop->NID.U + 1));
     }
     
     //Builds RC CAN.
     void Build_Input_RC(int p_Cell = 0)
     {
          gather_CAN_Input(0, p_Cell);
          CAN.RC();
          if (CAN.Treetop == NULL){ tbl_Treetops.add_Int(Current_Input, 0); return; }
          tbl_Treetops.add_Int(Current_Input, (CAN.Treetop->NID.U + 1));
     }
     
     //Builds for query.
     void Build_Input_Query(int p_Cell = 0)
     {
          gather_CAN_Input(0, p_Cell);
          CAN.Query();
     }
     
     
     
     //Gathers the first cell of the input table to build. This is neccesary as the cells are dynamically
     //allocated and addresses may change.
     void gather_CAN_Input(int p_Row = 0, int p_Cell = 0)
     {
          Input_Depth = 0;
          
          if (tbl_Input.Number_Of_Rows <= p_Row){ return; }
          if (tbl_Input.Rows[0]->Depth <= p_Cell){ return; }
          
          Input_Depth = tbl_Input.Rows[p_Row]->Cells[p_Cell]->Length;
          CAN.set_Input(tbl_Input.Rows[p_Row]->Cells[p_Cell]);
          
          
          Input_Charging_Depth = (Input_Depth * settings_Input_Charging_Depth);
          if (Input_Charging_Depth < 1){ Input_Charging_Depth = 1; }
          //if (Input_Charging_Depth == 0 && Input_Depth == 1){ Input_Charging_Depth = 1; }
     }
     
     //Gathers the aux input into the buffers.
     //For each cell the input pattern will be gathered. This pattern will be distributed to the appropriate charging
     //buffers designated by the pattern input table. The state will be gathered from the node network and the resultant node
     //will be submitted to the charging buffer along with the charge from the charge table.
     void gather_Aux_Input_L()
     {
          u_Data_3 tmp_State;
          tmp_State.U = 0;
          c_NT3_Base_Node_1D * tmp_Node = 0;
          long long int tmp_Charge = 0;
          int tmp_Position = 0;
          
          for (int cou_Data=0;cou_Data<tbl_Input_Aux_L.Rows[0]->Cells[Current_Input]->Length;cou_Data++)
          {
               tmp_State.U = (tbl_Input_Aux_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Node = Nodes.get_State_Node(tmp_State);
               tmp_Position = (tbl_Input_Position_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Charge = (tbl_Input_Charges_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               Charging_Buffers[tmp_Position].submit(tmp_Node, tmp_Charge);
          }
     }
     
     //Gathers the aux input into the buffers.
     void gather_Aux_Input_R()
     {
          u_Data_3 tmp_State;
          tmp_State.U = 0;
          c_NT3_Base_Node_1D * tmp_Node = 0;
          long long int tmp_Charge = 0;
          int tmp_Position = 0;
          
          for (int cou_Data=0;cou_Data<tbl_Input_Aux_R.Rows[0]->Cells[Current_Input]->Length;cou_Data++)
          {
               tmp_State.U = (tbl_Input_Aux_R.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Node = Nodes.get_State_Node(tmp_State);
               tmp_Position = (tbl_Input_Position_R.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Charge = (tbl_Input_Charges_R.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               Charging_Buffers[tmp_Position].submit(tmp_Node, tmp_Charge);
          }
     }
     
     void gather_Aux_Input_RC_L()
     {
          u_Data_3 tmp_State;
          tmp_State.U = 0;
          c_NT3_Base_Node_1D * tmp_Node = 0;
          long long int tmp_Charge = 0;
          int tmp_Position = 0;
          
          for (int cou_Data=0;cou_Data<tbl_Input_Aux_L.Rows[0]->Cells[Current_Input]->Length;cou_Data++)
          {
               tmp_State.U = (tbl_Input_Aux_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Node = Nodes.get_State_Node(tmp_State);
               tmp_Position = (tbl_Input_Position_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Charge = (tbl_Input_Charges_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               Charging_Buffers[tmp_Position].submit_RC(tmp_Node, tmp_Charge);
          }
     }
     
     //Gathers the aux input into the buffers.
     void gather_Aux_Input_RC_R()
     {
          u_Data_3 tmp_State;
          tmp_State.U = 0;
          c_NT3_Base_Node_1D * tmp_Node = 0;
          long long int tmp_Charge = 0;
          int tmp_Position = 0;
          
          for (int cou_Data=0;cou_Data<tbl_Input_Aux_R.Rows[0]->Cells[Current_Input]->Length;cou_Data++)
          {
               tmp_State.U = (tbl_Input_Aux_R.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Node = Nodes.get_State_Node(tmp_State);
               tmp_Position = (tbl_Input_Position_R.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Charge = (tbl_Input_Charges_R.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               Charging_Buffers[tmp_Position].submit_RC(tmp_Node, tmp_Charge);
          }
     }
     
     
     //Gathers the aux data into a row to be read into the poly CAN.
     void gather_Aux_Input_Poly_L(int p_Strip = 0)
     {
          u_Data_3 tmp_State;
          tmp_State.U = 0;
          c_NT3_Base_Node_1D * tmp_Node = 0;
          long long int tmp_Charge = 0;
          int tmp_Position = 0;
          u_Data_3 tmp_NID; tmp_NID.U = 0;
          u_Data_3 tmp_CID; tmp_CID.U = 0;
          delete [] PCAN_Input_Rows;
          PCAN_Input_Rows = NULL;
          PCAN_Input_Rows = new c_Raw_Table_Row_1D[Input_Depth];
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               PCAN_Input_Rows[cou_Index].reset();
          }
          
          //For each data bit in the current aux input cell gather the state, CID, NID, and position.
          //The position determines which buffer and PCAN to read into.
          //The CID determines which position block the data is read into, NOT the tmp_Position.
          //The SSCC will need to append a faux CID to the nodes when submitting them to the input.
          for (int cou_Data=0;cou_Data<tbl_Input_Aux_L.Rows[0]->Cells[Current_Input]->Length;cou_Data++)
          {
               tmp_State.U = (tbl_Input_Aux_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               
               tmp_NID.U = tmp_State.U * .01;
               tmp_CID.U = tmp_State.U - (tmp_NID.U * 100);
               
               //This is for when you want to strip the CID for example in a network that builds from multiple evaluation of a single construct.
               if (p_Strip)
               {
                    tmp_State.U = tmp_NID.U;
               } 
               
               tmp_Node = Nodes.get_State_Node(tmp_State);
               tmp_Position = (tbl_Input_Position_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Charge = (tbl_Input_Charges_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               
               Charging_Buffers[tmp_Position].submit(tmp_Node, tmp_Charge);
               
               PCAN_Input_Rows[tmp_Position].add_Data(tmp_CID.U, tmp_State);
          }
          
          //Each index is to be evaluated independently of the others.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               PCAN[cou_Index].set_Input(&PCAN_Input_Rows[cou_Index]);
               PCAN[cou_Index].Query();
               //*PCAN[cou_Index].output_CAN_State();
               //*cout << "\n PCAN_IR->" << cou_Index << " "; PCAN_Input_Rows[cou_Index].output_I();
          }
          
     }
     
     
     //Gathers the aux data into a row to be read into the poly CAN.
     void gather_Aux_Input_Poly_R(int p_Strip = 0)
     {
          u_Data_3 tmp_State;
          tmp_State.U = 0;
          c_NT3_Base_Node_1D * tmp_Node = 0;
          long long int tmp_Charge = 0;
          int tmp_Position = 0;
          u_Data_3 tmp_NID; tmp_NID.U = 0;
          u_Data_3 tmp_CID; tmp_CID.U = 0;
          delete [] PCAN_Input_Rows;
          PCAN_Input_Rows = NULL;
          PCAN_Input_Rows = new c_Raw_Table_Row_1D[Input_Depth];
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               PCAN_Input_Rows[cou_Index].reset();
          }
          
          //For each data bit in the current aux input cell gather the state, CID, NID, and position.
          //The position determines which buffer and PCAN to read into.
          //The CID determines which position block the data is read into, NOT the tmp_Position.
          //The SSCC will need to append a faux CID to the nodes when submitting them to the input.
          for (int cou_Data=0;cou_Data<tbl_Input_Aux_R.Rows[0]->Cells[Current_Input]->Length;cou_Data++)
          {
               tmp_State.U = (tbl_Input_Aux_R.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               
               tmp_NID.U = tmp_State.U * .01;
               tmp_CID.U = tmp_State.U - (tmp_NID.U * 100);
               
               //This is for when you want to strip the CID for example in a network that builds from multiple evaluation of a single construct.
               if (p_Strip)
               {
                    tmp_State.U = tmp_NID.U;
               } 
               
               tmp_Node = Nodes.get_State_Node(tmp_State);
               tmp_Position = (tbl_Input_Position_R.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Charge = (tbl_Input_Charges_R.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               
               Charging_Buffers[tmp_Position].submit(tmp_Node, tmp_Charge);
               
               PCAN_Input_Rows[tmp_Position].add_Data(tmp_CID.U, tmp_State);
          }
          
          //Each index is to be evaluated independently of the others.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               PCAN[cou_Index].set_Input(&PCAN_Input_Rows[cou_Index]);
               PCAN[cou_Index].Query();
               //PCAN[cou_Index].output_CAN_State();
               //cout << "\n PCAN_IR->" << cou_Index << " "; PCAN_Input_Rows[cou_Index].output_I();
          }
          
     }
     
     
     //Gathers the aux inputs for the inclusive poly eval.
     //Gathers the aux data into a row to be read into the poly CAN.
     void gather_Aux_Input_Poly_Inclusive_L(int p_Strip = 0)
     {
          u_Data_3 tmp_State;
          tmp_State.U = 0;
          c_NT3_Base_Node_1D * tmp_Node = 0;
          long long int tmp_Charge = 0;
          int tmp_Position = 0;
          u_Data_3 tmp_NID; tmp_NID.U = 0;
          u_Data_3 tmp_CID; tmp_CID.U = 0;
          u_Data_3 tmp_Highest_CID; tmp_Highest_CID.U = 0;
          delete [] PCAN_Input_Rows;
          PCAN_Input_Rows = NULL;
          PCAN_Input_Rows = new c_Raw_Table_Row_1D[Input_Depth];
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               PCAN_Input_Rows[cou_Index].reset();
          }
          
          //For initializing the _Opp in the charging buffers the Highest CID (Input) will be gathered.
          for (int cou_Data=0;cou_Data<tbl_Input_Aux_L.Rows[0]->Cells[Current_Input]->Length;cou_Data++)
          {
               tmp_State.U = (tbl_Input_Aux_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               
               tmp_CID.U = tmp_State.U - (tmp_NID.U * 100);
               
               if (tmp_CID.U > tmp_Highest_CID.U){ tmp_Highest_CID.U = tmp_CID.U; }
          }
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               Charging_Buffers[cou_Index].resize_Opp(tmp_Highest_CID.U);
          }
          
          
          //For each data bit in the current aux input cell gather the state, CID, NID, and position.
          //The position determines which buffer and PCAN to read into.
          //The CID determines which position block the data is read into, NOT the tmp_Position.
          //The SSCC will need to append a faux CID to the nodes when submitting them to the input.
          for (int cou_Data=0;cou_Data<tbl_Input_Aux_L.Rows[0]->Cells[Current_Input]->Length;cou_Data++)
          {
               tmp_State.U = (tbl_Input_Aux_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               
               tmp_NID.U = tmp_State.U * .01;
               tmp_CID.U = tmp_State.U - (tmp_NID.U * 100);
               
               //This is for when you want to strip the CID for example in a network that builds from multiple evaluation of a single construct.
               if (p_Strip)
               {
                    tmp_State.U = tmp_NID.U;
               } 
               
               tmp_Node = Nodes.get_State_Node(tmp_State);
               tmp_Position = (tbl_Input_Position_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Charge = (tbl_Input_Charges_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               
               if (cou_Data == 0){ Charging_Buffers[tmp_Position].submit(tmp_Node, tmp_Charge); }
               if (cou_Data > 0){ Charging_Buffers[tmp_Position].submit_Opp((cou_Data - 1), tmp_Node, tmp_Charge); }
               
               PCAN_Input_Rows[tmp_Position].add_Data(tmp_CID.U, tmp_State);
          }
          
          //Each index is to be evaluated independently of the others.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               PCAN[cou_Index].set_Input(&PCAN_Input_Rows[cou_Index]);
               PCAN[cou_Index].Query();
               //*PCAN[cou_Index].output_CAN_State();
               //*cout << "\n PCAN_IR->" << cou_Index << " "; PCAN_Input_Rows[cou_Index].output_I();
          }
          
     }
     
     
     
     ////==--------------------------+
     //==--   DISCHARGING FUNCTIONS
     ////==--------------------------+
     
     //Discharges the treetops for a given input.
     void discharge_Treetops_L(int p_Input)
     {
          Charging_Buffers[p_Input].gather_Treetops();
          c_NT3_Linked_List * tmp_LL = Charging_Buffers[p_Input].Treetops.Root;
                    
          while(tmp_LL != NULL)
          {
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               if (tmp_LL->Charge.I > (settings_Base_Charge * settings_Action_Potential_Threshold)){} else { tmp_LL = tmp_LL->Next; continue; }
               
               if (tmp_LL->NID == NULL)
               {                    
                    tbl_Pattern_Output_L.new_Cell(Current_Input);
                    tbl_Pattern_Output_L.add_String("NULL");
                    tbl_Position_Output_L.add_Int(Current_Input, p_Input);
                    tbl_Charge_Output_L.add_Int(Current_Input, tmp_LL->Charge.I);
                    tbl_RC_Output_L.add_Float(Current_Input, 0);
                    tbl_Treetops_Output_L.add_Int(Current_Input, 0);
                    
                    tmp_LL = tmp_LL->Next;
                    continue;
               }
               
               Nodes.bp(&tbl_Pattern_Output_L, Current_Input, tmp_LL->NID);
               
               tbl_Position_Output_L.add_Int(Current_Input, p_Input);
               tbl_Charge_Output_L.add_Int(Current_Input, tmp_LL->Charge.I);
               tbl_RC_Output_L.add_Float(Current_Input, tmp_LL->NID->get_RC_Score());
               tbl_Treetops_Output_L.add_Int(Current_Input, tmp_LL->NID->NID.U + 1);
               
               tmp_LL = tmp_LL->Next;
          }
     }
          
     //Discharges the treetops for a given input.
     void discharge_Treetops_R(int p_Input)
     {
          Charging_Buffers[p_Input].gather_Treetops();
          c_NT3_Linked_List * tmp_LL = Charging_Buffers[p_Input].Treetops.Root;
          
          while(tmp_LL != NULL)
          {
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               if (tmp_LL->Charge.I > (settings_Base_Charge * settings_Action_Potential_Threshold)){} else { tmp_LL = tmp_LL->Next; continue; }
               
               if (tmp_LL->NID == NULL)
               {                    
                    tbl_Pattern_Output_R.new_Cell(Current_Input);
                    tbl_Pattern_Output_R.add_String("NULL");
                    tbl_Position_Output_R.add_Int(Current_Input, p_Input);
                    tbl_Charge_Output_R.add_Int(Current_Input, tmp_LL->Charge.I);
                    tbl_RC_Output_R.add_Float(Current_Input, 0);
                    tbl_Treetops_Output_R.add_Int(Current_Input, 0);
                    
                    tmp_LL = tmp_LL->Next;
                    continue;
               }
               
               Nodes.bp(&tbl_Pattern_Output_R, Current_Input, tmp_LL->NID);
               
               tbl_Position_Output_R.add_Int(Current_Input, p_Input);
               tbl_Charge_Output_R.add_Int(Current_Input, tmp_LL->Charge.I);
               tbl_RC_Output_R.add_Float(Current_Input, tmp_LL->NID->get_RC_Score());
               tbl_Treetops_Output_R.add_Int(Current_Input, tmp_LL->NID->NID.U + 1);
               
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Discharges the treetops in the L buffer.
     void discharge_All_Treetops_L()
     {          
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Charging_Depth;cou_Index++)
          {
               discharge_Treetops_L(cou_Index);
          }
          tbl_Depths_L.add_Int(Current_Input, Input_Charging_Depth);
     }
     
     //Discharges the treetops in the L buffer.
     void discharge_All_Treetops_R()
     {          
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Charging_Depth;cou_Index++)
          {
               discharge_Treetops_R(cou_Index);
          }
          tbl_Depths_R.add_Int(Current_Input, Input_Charging_Depth);
     }
     
     //Discharges a given treetop.
     void discharge_Treetop_By_NID_L(int p_Position, u_Data_3 p_NID, u_Data_3 p_Charge, int p_Current_Input)
     {
          //cout << "\n (Pos " << p_Position << ", p_NID " << p_NID.U << ", p_Charge " << p_Charge.I << ", p_Current_Input " << p_Current_Input << ")";
                
          c_NT3_Base_Node_1D * tmp_Node = NULL;
          
          if (p_NID.U == 0)
          { 
               return;
               tbl_Pattern_Output_L.new_Cell(Current_Input);
               tbl_Pattern_Output_L.add_String("NULL");
               tbl_Position_Output_L.add_Int(p_Current_Input, p_Position);
               tbl_Charge_Output_L.add_Int(p_Current_Input, p_Charge.I);
               tbl_RC_Output_L.add_Float(p_Current_Input, 0);
               tbl_Treetops_Output_L.add_Int(p_Current_Input, 0);
               return; 
          }
          
          //Because it is incremented before submission.
          p_NID.U--;
          
          tmp_Node = Nodes.bp_Treetop(&tbl_Pattern_Output_L, p_Current_Input, p_NID);
          
          tbl_Position_Output_L.add_Int(p_Current_Input, p_Position);
          tbl_Charge_Output_L.add_Int(p_Current_Input, p_Charge.I);
          tbl_RC_Output_L.add_Float(p_Current_Input, tmp_Node->get_RC_Score());
          tbl_Treetops_Output_L.add_Int(p_Current_Input, tmp_Node->NID.U);
          
          if (tbl_Pattern_Output_L.Rows[p_Current_Input]->Depth != tbl_Charge_Output_L.Rows[p_Current_Input]->Depth)
          { 
               cout << "\n      //==-- po->" << tbl_Pattern_Output_L.Rows[p_Current_Input]->Depth << " != co->" << tbl_Charge_Output_L.Rows[p_Current_Input]->Depth;
               cout << "\n discharge_Treetop_By_NID_L(int p_Position " << p_Position << ", u_Data_3 p_NID " << p_NID.U << ", u_Data_3 p_Charge " << p_Charge.I << ", int p_Current_Input " << p_Current_Input << ")";
               cout << "\n po->";
               tbl_Pattern_Output_L.output_I();
               cout << "\n co->";
               tbl_Charge_Output_L.output_I();
               system("PAUSE > NULL");
          }
     }
          
     //Discharges a given treetop.
     void discharge_Treetop_By_NID_R(int p_Position, u_Data_3 p_NID, u_Data_3 p_Charge, int p_Current_Input)
     {
          c_NT3_Base_Node_1D * tmp_Node = NULL;
          
          if (p_NID.U == 0)
          { 
               tbl_Pattern_Output_R.new_Cell(Current_Input);
               tbl_Pattern_Output_R.add_String("NULL");
               tbl_Position_Output_R.add_Int(p_Current_Input, p_Position);
               tbl_Charge_Output_R.add_Int(p_Current_Input, p_Charge.I);
               tbl_RC_Output_R.add_Float(p_Current_Input, 0);
               tbl_Treetops_Output_R.add_Int(p_Current_Input, 0);
               return; 
          }
          
          //Because it is incremented before submission.
          p_NID.U--;
          
          tmp_Node = Nodes.bp_Treetop(&tbl_Pattern_Output_R, p_Current_Input, p_NID);
          
          tbl_Position_Output_R.add_Int(p_Current_Input, p_Position);
          tbl_Charge_Output_R.add_Int(p_Current_Input, p_Charge.I);
          tbl_RC_Output_R.add_Float(p_Current_Input, tmp_Node->get_RC_Score());
          tbl_Treetops_Output_R.add_Int(p_Current_Input, tmp_Node->NID.U);
          
     }
     
     //Discharges a given treetop into the given tables.
     void discharge_Treetop_By_NID_Into_Given_Tables_L(int p_Position, u_Data_3 p_NID, u_Data_3 p_Charge, int p_Current_Input, c_Raw_Table_1D * p_PTbl, c_Raw_Table_1D * p_PosTbl, c_Raw_Table_1D * p_CTbl, c_Raw_Table_1D * p_RCTbl, c_Raw_Table_1D * p_TtTbl)
     {
          //cout << "\n (Pos " << p_Position << ", p_NID " << p_NID.U << ", p_Charge " << p_Charge.I << ", p_Current_Input " << p_Current_Input << ")";
                
          c_NT3_Base_Node_1D * tmp_Node = NULL;
          
          if (p_NID.U == 0)
          { 
               return;
               p_PTbl->new_Cell(Current_Input);
               p_PTbl->add_String("NULL");
               p_PosTbl->add_Int(p_Current_Input, p_Position);
               p_CTbl->add_Int(p_Current_Input, p_Charge.I);
               p_RCTbl->add_Float(p_Current_Input, 0);
               p_TtTbl->add_Int(p_Current_Input, 0);
               return; 
          }
          
          //Because it is incremented before submission.
          p_NID.U--;
          
          tmp_Node = Nodes.bp_Treetop(p_PTbl, p_Current_Input, p_NID);
          
          p_PosTbl->add_Int(p_Current_Input, p_Position);
          p_CTbl->add_Int(p_Current_Input, p_Charge.I);
          p_RCTbl->add_Float(p_Current_Input, tmp_Node->get_RC_Score());
          p_TtTbl->add_Int(p_Current_Input, tmp_Node->NID.U);
          
          if (p_PTbl->Rows[p_Current_Input]->Depth != p_CTbl->Rows[p_Current_Input]->Depth)
          { 
               cout << "\n      //==-- po->" << p_PTbl->Rows[p_Current_Input]->Depth << " != co->" << p_CTbl->Rows[p_Current_Input]->Depth;
               cout << "\n discharge_Treetop_By_NID_L(int p_Position " << p_Position << ", u_Data_3 p_NID " << p_NID.U << ", u_Data_3 p_Charge " << p_Charge.I << ", int p_Current_Input " << p_Current_Input << ")";
               cout << "\n po->";
               p_PTbl->output_I();
               cout << "\n co->";
               p_CTbl->output_I();
               system("PAUSE > NULL");
          }
     }
     
     ////==---------------------+
     //==--   OUTPUT FUNCTIONS
     ////==---------------------+
     
     //Resets the input tables.
     void reset_Inputs()
     {
          tbl_Input.reset();
          
          tbl_Input_Aux_L.reset();
          tbl_Input_Charges_L.reset();
          tbl_Input_Depths_L.reset();
          tbl_Input_Position_L.reset();
          
          tbl_Input_Aux_R.reset();
          tbl_Input_Charges_R.reset();
          tbl_Input_Depths_R.reset();
          tbl_Input_Position_R.reset();
     }
     
     //Resets the output tables.
     void reset_Outputs()
     {
          tbl_Pattern_Output_L.reset();
          tbl_Charge_Output_L.reset();
          tbl_Position_Output_L.reset();
          tbl_RC_Output_L.reset();
          tbl_Treetops_Output_L.reset();
     
          tbl_Pattern_Output_R.reset();
          tbl_Charge_Output_R.reset();
          tbl_Position_Output_R.reset();
          tbl_RC_Output_R.reset();
          tbl_Treetops_Output_R.reset();
     
          tbl_Treetops.reset();
     
          tbl_Vision_Output.reset();
          tbl_Vision_Charge_Output.reset();
     
          tbl_Depths_L.reset();
          tbl_Depths_R.reset();
          
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
          
          tbl_Input_Aux_L.output_I(1);
          tbl_Input_Charges_L.output_I(1);
          tbl_Input_Position_L.output_I(1);
          tbl_Input_Depths_L.output_I(1);
          
          tbl_Input_Aux_R.output_I(1);
          tbl_Input_Charges_R.output_I(1);
          tbl_Input_Position_R.output_I(1);
          tbl_Input_Depths_R.output_I(1);
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
          tbl_Pattern_Output_L.output_C(2);
          tbl_Charge_Output_L.output_I(2);
          tbl_Position_Output_L.output_I(2);
          tbl_RC_Output_L.output_F(2);
          tbl_Treetops_Output_L.output_I(2);
          tbl_Depths_L.output_I(2);
     
          
          tbl_Pattern_Output_R.output_C(2);
          tbl_Charge_Output_R.output_I(2);
          tbl_Position_Output_R.output_I(2);
          tbl_RC_Output_R.output_F(2);
          tbl_Treetops_Output_R.output_I(2);
          tbl_Depths_R.output_I(2);
          
          tbl_Treetops.output_I(2);
          cout << "\n\t ---===+===---\n";
     }
     
     //Outputs the tables of the construct with the patterns as int.
     void output_Output_Tables_I()
     {
          cout << "\n\n\t ---===+===---";
          cout << "\n\t " << Name << " Output_Tables_I ->" << this;
          tbl_Pattern_Output_L.output_I(2);
          tbl_Charge_Output_L.output_I(2);
          tbl_Position_Output_L.output_I(2);
          tbl_RC_Output_L.output_F(2);
          tbl_Treetops_Output_L.output_I(2);
          tbl_Depths_L.output_I(2);
     
          
          tbl_Pattern_Output_R.output_I(2);
          tbl_Charge_Output_R.output_I(2);
          tbl_Position_Output_R.output_I(2);
          tbl_RC_Output_R.output_F(2);
          tbl_Treetops_Output_R.output_I(2);
          tbl_Depths_R.output_I(2);
          
          tbl_Treetops.output_I(2);
          cout << "\n\t ---===+===---\n";
     }
     
     //Outputs the tables of the construct with the patterns as int.
     void output_Output_Tables_F()
     {
          cout << "\n\n\t ---===+===---";
          cout << "\n\t " << Name << " Output_Tables_I ->" << this;
          tbl_Pattern_Output_L.output_F(2);
          tbl_Charge_Output_L.output_F(2);
          tbl_Position_Output_L.output_F(2);
          tbl_RC_Output_L.output_F(2);
          tbl_Treetops_Output_L.output_F(2);
          tbl_Depths_L.output_F(2);
     
          
          tbl_Pattern_Output_R.output_F(2);
          tbl_Charge_Output_R.output_F(2);
          tbl_Position_Output_R.output_F(2);
          tbl_RC_Output_R.output_F(2);
          tbl_Treetops_Output_R.output_F(2);
          tbl_Depths_R.output_F(2);
          
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
          CAN.output_CAN_RC();
     }
          
     //Outputs the PCANs.
     void output_PCAN()
     {
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               PCAN[cou_Index].output_CAN();
          }
     }
          
     //Outputs the PCAN state.
     void output_PCAN_State()
     {
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               PCAN[cou_Index].output_CAN_State();
          }
     }
     
     //Outputs the PCAN state.
     void output_PCAN_State_I()
     {
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               PCAN[cou_Index].output_CAN_State_I();
          }
     }
     
     //Outputs the PCAN state.
     void output_PCAN_RC()
     {
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               PCAN[cou_Index].output_CAN_RC();
          }
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
          cout << "\n  settings_Tier_Depth................. "; odbl(0, 11, settings_Tier_Depth);
          cout << "\n  settings_Action_Potential_Threshold. "; odbl(0, 11, settings_Action_Potential_Threshold);
          cout << "\n  settings_Base_Charge................ "; oint(0, 11, settings_Base_Charge);
          cout << "\n  settings_Modifier_Charge............ "; odbl(0, 11, settings_Modifier_Charge);
     }
     
     //Outputs the settings of the current Construct.
     void output_Settings_Full()
     {
          cout << "\n\n --  Settings For "; ostr(0, 13, Name);
          
          //Use in the expanded form.
          cout << "\n  Charge_L.................... "; oint(0, 11, setting_Charge_L);
          cout << "\n  Charge_R.................... "; oint(0, 11, setting_Charge_R);
          cout << "\n  Tier....................... "; oint(0, 11, settings_Tier);
          cout << "\n  Tier_Is_Floating........... "; oint(0, 11, settings_Tier_Is_Floating);
          cout << "\n  Tier_Depth................. "; odbl(0, 11, settings_Tier_Depth);
          cout << "\n  Action_Potential_Threshold. "; odbl(0, 11, settings_Action_Potential_Threshold);
          cout << "\n  Base_Charge................ "; oint(0, 11, settings_Base_Charge);
          cout << "\n  Modifier_Charge............ "; odbl(0, 11, settings_Modifier_Charge);
          cout << "\n  Use_Input_Charges............ "; odbl(0, 11, setting_Use_Input_Charges);
          cout << "\n  Build_Vision............ "; odbl(0, 11, settings_Build_Vision);
          cout << "\n  Bubble_Sort_Output............ "; odbl(0, 11, settings_Bubble_Sort_Output);
          cout << "\n  Input_Charging_Depth............ "; odbl(0, 11, settings_Input_Charging_Depth);
          cout << "\n  Cross_CAN_Charging............ "; odbl(0, 11, settings_Cross_CAN_Charging);
          cout << "\n  Stack_Output_From_Foreign_Treetops............ "; odbl(0, 11, settings_Stack_Output_From_Foreign_Treetops);
          cout << "\n  sauto_Type............ "; odbl(0, 11, sauto_Type);
          cout << "\n  sauto_Use_RC............ "; odbl(0, 11, sauto_Use_RC);
     }
     
     
     
     ////==--------------+
     //==--   INTERFACE
     ////==--------------+
     
     //Accepts an input to the inputs motherfucker.
     void in(int p_Row, int p_Cell, string p_Submission)
     {
          tbl_Input.set_String(p_Row, p_Cell, p_Submission);
     }
     
     //Accepts an input to the inputs motherfucker.
     void in(int p_Row, int p_Cell, int p_Submission)
     {
          tbl_Input.set_Int(p_Row, p_Cell, p_Submission);
     }
     
     //Accepts an input to the inputs motherfucker.
     void in(int p_Row, int p_Cell, double p_Submission)
     {
          tbl_Input.set_Float(p_Row, p_Cell, p_Submission);
     }
     
     //Accepts an input to the inputs motherfucker.
     void in(int p_Row, int p_Cell, void * p_Submission)
     {
          tbl_Input.set_NR(p_Row, p_Cell, p_Submission);
     }
     
     
     //==--   Adds the submission to the given cell.
     //Accepts an input to the inputs motherfucker.
     void ina(int p_Row, int p_Cell, int p_Submission)
     {
          tbl_Input.add_Data_Int(p_Row, p_Cell, p_Submission);
     }
     
     //Accepts an input to the inputs motherfucker.
     void ina(int p_Row, int p_Cell, double p_Submission)
     {
          tbl_Input.add_Data_Float(p_Row, p_Cell, p_Submission);
     }
     
     //Accepts an input to the inputs motherfucker.
     void ina(int p_Row, int p_Cell, void * p_Submission)
     {
          tbl_Input.add_Data_NR(p_Row, p_Cell, p_Submission);
     }
     
     
     
     
     //Accepts an array to be input into a row.
     void ina_A(int p_Row, int p_Cell, int * p_Submission, int p_Length)
     {
          for (int cou_Index=0;cou_Index<p_Length;cou_Index++)
          {
               ina(p_Row, p_Cell, p_Submission[cou_Index]);
          }
     }
     
     //Accepts an array to be input into a row.
     void ina_A(int p_Row, int p_Cell, double * p_Submission, int p_Length)
     {
          for (int cou_Index=0;cou_Index<p_Length;cou_Index++)
          {
               ina(p_Row, p_Cell, p_Submission[cou_Index]);
          }
     }
     
     //Accepts an array to be input into a row.
     void ina_A(int p_Row, int p_Cell, void ** p_Submission, int p_Length)
     {
          for (int cou_Index=0;cou_Index<p_Length;cou_Index++)
          {
               ina(p_Row, p_Cell, p_Submission[cou_Index]);
          }
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
          SF << "\nsetting_Charge_L " << setting_Charge_L;
          SF << "\nsetting_Charge_R " << setting_Charge_R;
          
          //The tier to start charging.
          SF << "\nsettings_Tier " << settings_Tier;
          
          //The tier to start charging.
          SF << "\nsettings_Tier_Is_Floating " << settings_Tier_Is_Floating;
          
          //The tier to start charging.
          SF << "\nsettings_Tier_Depth " << settings_Tier_Depth;
          
          //The Action Potential Threshold as percentage of the base charge.
          SF << "\nsettings_Action_Potential_Threshold " << settings_Action_Potential_Threshold;
          
          //The base charge to apply from the CAN.
          SF << "\nsettings_Base_Charge " << settings_Base_Charge;
          
          //The multiple to reduce each recursive charge by.
          SF << "\nsettings_Modifier_Charge " << settings_Modifier_Charge;
          
          //Whether or not to use the input charges for charging.
          SF << "\nsetting_Use_Input_Charges " << setting_Use_Input_Charges;
          
          //Build a vision or do not build a vision.
          SF << "\nsettings_Build_Vision " << settings_Build_Vision;
          
          //Sort the tables by charge.
          SF << "\nsettings_Bubble_Sort_Output " << settings_Bubble_Sort_Output;
          
          //The percentage depth of the input to charge starting at index 0.
          SF << "\nsettings_Input_Charging_Depth " << settings_Input_Charging_Depth;
          
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
          
          //Charging legs.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> setting_Charge_L;
          cout << " " << setting_Charge_L;
          
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> setting_Charge_R;
          cout << " " << setting_Charge_R;
          
          //The tier to start charging.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Tier;
          cout << " " << settings_Tier;
          
          //Floating vrs fixed tier depth
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Tier_Is_Floating;
          cout << " " << settings_Tier_Is_Floating;
          
          //Floating tier modifier.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Tier_Depth;
          cout << " " << settings_Tier_Depth;
          
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
          
          //Whether or not to use the input charges for charging.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> setting_Use_Input_Charges;
          cout << " " << setting_Use_Input_Charges;
          
          //Build a vision or do not build a vision.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Build_Vision;
          cout << " " << settings_Build_Vision;
          
          //Sort the tables by charge.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Bubble_Sort_Output;
          cout << " " << settings_Bubble_Sort_Output;
          
          //settings_Input_Charging_Depth
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Input_Charging_Depth;
          cout << " " << settings_Input_Charging_Depth;
          
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