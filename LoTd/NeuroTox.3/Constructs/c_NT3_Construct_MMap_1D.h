//The construct class.
class c_NT3_Construct_MMap_1D: public c_NT3_Base_Construct
{
private:
     
public: 

     //The node network to use.
     c_NT3_Node_Network_MMap_1D Nodes;
     
     //The CAN used for building the memory trees.
     c_NT3_CAN_MMap_1D CAN;

     //The Charging_Buffers. One for each input.
     c_NT3_Buffer_MMap * Charging_Buffers;
     
     //The Vision. One for each input.
     c_NT3_Vision_Buffer_MMap * Vision_Buffers;
     
     //The current input depth.
     int Input_Depth;
     
     //The current input depth to charge using the settings_Charging_Depth
     int Input_Charging_Depth;
     
     //The current input being evaluated.
     //--Replaced with current cell and row.
     //int Current_Input;
     
     //The current cell being evaluated
     int Current_Cell;
     
     //The current row being evaluated.
     int Current_Row;
     
     
     //The name of the Construct.
     std::string Name;
     
     //With the MMap the construct is tied to a directory.
     std::string Dir;
     std::string FName;
     
     c_NT3_Construct_MMap_1D()
     {
          u_Data_3 tmp_Name;
          tmp_Name.NR = this;
          Name = "C__" + int2str(int(tmp_Name.U));
          
          set_Name(Name);
          
          settings_Build_Method = 1;
          settings_Eval_Method = 0;
          
          setting_Charge_L = 0;
          setting_Charge_R = 1;
          
          settings_Tier = 0;
          settings_Tier_Is_Floating = 0;
          settings_Tier_Depth = 0;
          
          settings_Action_Potential_Threshold = 0.05;
          settings_Base_Charge = 10000;
          
          setting_Use_Input_Charges = 0;
          
          settings_Modifier_Charge = .9;
          
          settings_Bubble_Sort_Output = 0;
          
          settings_Build_Vision = 1;
          
          settings_Input_Charging_Depth = 1;
          
          settings_Cross_CAN_Charging = 0;
          
          settings_Stack_Output_From_Foreign_Treetops = 0;
          
          setting_Charge_Spike = 0;
          
          settings_Flat_Output = 0;
          
          CAN.init(&Nodes);
          Charging_Buffers = NULL;

          Vision_Buffers = NULL;
          Input_Depth = 0;
          Input_Charging_Depth = 0;
          
          settings_RC = 1;
          
          Current_Cell = 0;
          Current_Row = 0;
          
     }
     
     ~c_NT3_Construct_MMap_1D()
     {
          std::cout << "\n  ~c_NT3_Construct_MMap_1D " << Name << " " << this << ".........."; std::cout.flush();
          
          delete [] Charging_Buffers;
          Charging_Buffers = NULL;
          Input_Depth = 0;
          std::cout << "\n  ~~~c_NT3_Construct_MMap_1D " << Name << " " << this << ".........."; std::cout.flush();
     }
     
     //Sets the name of the construct and the tables.
     void set_Name(std::string p_Name)
     {
          Name = p_Name;
          
          //The input tables.
          tbl_Input.Name = Name + "___Input";
          
          tbl_Input_Aux.Name = Name + "___Input_Aux";
          tbl_Input_Charges.Name = Name + "___Input_Charges";
          tbl_Input_Position.Name = Name + "___Input_Position";
          tbl_Input_Depths.Name = Name + "___Input_Depths";
          
          //Output Tables.
          tbl_Pattern_Output.Name = Name + "___Pattern_Output";
          tbl_Salt_Output.Name = Name + "___Salt_Output";
          tbl_Charge_Output.Name = Name + "___Charge_Output";
          tbl_Position_Output.Name = Name + "___Position_Output";
          tbl_RC_Output.Name = Name + "___RC_Output";
          
          tbl_Treetops_Output.Name = Name + "___Treetops_Output";
          
          tbl_Vision_Output.Name = Name + "___Vision_Output";
          tbl_Vision_Charge_Output.Name = Name + "___Vision_Charge_Output";
     }
     
     void set_CID(unsigned long long int p_CID)
     {
          CID.U = p_CID;
     }
     
	 //This is done on the first run.
	 int first_Run(std::string p_Dir, std::string p_Name)
	 {
	     set_Name(p_Name);

		 Dir = p_Dir;
		 FName = p_Name;

		 Save(p_Dir, p_Name);

		 return Nodes.new_Network(p_Dir, p_Name);
	 }

	 //Init the node network.
	 int init(std::string p_Dir, std::string p_Name)
	 {
		 std::cout << "\n Initializing Construct:" << p_Name;
		 set_Name(p_Name);

		 Dir = p_Dir;
		 FName = p_Name;

		 return Nodes.init(p_Dir, p_Name);
	 }
	      
     c_Raw_Table_1D * get_Table_1D(std::string p_Table)
     {
          if (p_Table == "tbl_Input"){ return &tbl_Input; }
     
          if (p_Table == "tbl_Input_Aux"){ return &tbl_Input_Aux; }
          if (p_Table == "tbl_Input_Charges"){ return &tbl_Input_Charges; }
          if (p_Table == "tbl_Input_Position"){ return &tbl_Input_Position; }
          if (p_Table == "tbl_Input_Depths"){ return &tbl_Input_Depths; }
     
     
     
          //Output Tables.
          if (p_Table == "tbl_Pattern_Output"){ return &tbl_Pattern_Output; }
          if (p_Table == "tbl_Salt_Output"){ return &tbl_Salt_Output; }
          if (p_Table == "tbl_Charge_Output"){ return &tbl_Charge_Output; }
          if (p_Table == "tbl_Position_Output"){ return &tbl_Position_Output; }
          if (p_Table == "tbl_RC_Output"){ return &tbl_RC_Output; }
     
          if (p_Table == "tbl_Treetops_Output"){ return &tbl_Treetops_Output; }
     
          if (p_Table == "tbl_Vision_Output"){ return &tbl_Vision_Output; }
          if (p_Table == "tbl_Vision_Charge_Output"){ return &tbl_Vision_Charge_Output; }
          ostr(0, 12, "\n Table NOT found:"); std::cout << p_Table;
          return NULL;
     }
     
     
     
     ////==-----------------------+
     //==--   EVAL FUNCTIONS
     ////==-----------------------+
          
     //Eval with old style central charging.
     void Eval(int p_Discharge)
     {
          reset_Outputs();
          
          if (tbl_Input.Number_Of_Rows == 0){ return; }
          
          if (settings_Eval_Method == 0)
          {
			  //*std::cout << "\n settings_Eval_Method == 0";
               for (int cou_Row=0;cou_Row<tbl_Input.Number_Of_Rows;cou_Row++)
               {
                    Current_Row = cou_Row;
                    for (int cou_Cell=0;cou_Cell<tbl_Input.Rows[0]->Depth;cou_Cell++)
                    {
						//*std::cout << "\n Current_Cell = cou_Cell;";
                         Current_Cell = cou_Cell;
                         
						 //*std::cout << "\n Build_Input_Query(cou_Cell);";
                         Build_Input_Query();
                         
                         if (settings_Tier_Is_Floating)
                         {
                              settings_Tier = int (Input_Depth * settings_Tier_Depth);
                              //*std::cout << "\n " << Name << " settings_Tier->" << settings_Tier << " = Input_Depth->" << Input_Depth << " * " << settings_Tier_Depth;
                         }
                         
						 //*std::cout << "\n init_Charging_Buffer();";
                         init_Charging_Buffer();
						 //*std::cout << "\n charge_Buffer();";
                         if (settings_RC)
                         {
                              charge_Buffer_RC();
                         }
                         else
                         {
                              charge_Buffer();
                         }
                         
						 //*std::cout << "\n discharge_Treetops();";
                         discharge_Treetops(p_Discharge);
                    }
               }
          }
          
          //This has to be done separately because each cell has to have the corresponding charge input.
          if (settings_Eval_Method == 1)
          {
			  //*std::cout << "\n settings_Eval_Method == 1 " << tbl_Input.Number_Of_Rows;

               for (int cou_Row=0;cou_Row<tbl_Input.Number_Of_Rows;cou_Row++)
               {
                    tbl_Input.Rows[cou_Row]->flatten_Row();
                    
                    Current_Row = cou_Row;
                    
                    //Only one cell in the input.
					//*std::cout << "\n Current_Cell = 0;";
                    Current_Cell = 0;
                    
					//*std::cout << "\n Build_Input_Query(0);";
                    Build_Input_Query();
                    
                    if (settings_Tier_Is_Floating)
                    {
                         settings_Tier = int (Input_Depth * settings_Tier_Depth);
                         //*std::cout << "\n " << Name << " settings_Tier->" << settings_Tier << " = Input_Depth->" << Input_Depth << " * " << settings_Tier_Depth;
                    }
                    
					//*std::cout << "\n init_Charging_Buffer();";
                    init_Charging_Buffer();
					//*std::cout << "\n charge_Buffer();";
                    
                    //Aux refers to weighted input.
                    charge_Aux_Buffer();
                    
					//*std::cout << "\n discharge_Treetops();";
                    discharge_Treetops(p_Discharge);
               }
          }
     }
     
     //Sets the charging buffers up for each input.
     void init_Charging_Buffer()
     {
          delete [] Charging_Buffers;
          Charging_Buffers = NULL;
          Charging_Buffers = new c_NT3_Buffer_MMap[1];
          
          Charging_Buffers[0].Base_Charge = settings_Base_Charge;
          Charging_Buffers[0].Modifier_Charge = settings_Modifier_Charge;
          Charging_Buffers[0].Action_Potential_Threshold = settings_Action_Potential_Threshold;
          //Charging_Buffers[0].resize_Opp((Input_Depth - 0));
          Charging_Buffers[0].Nodes = &Nodes;
     }     
     
     //Charges a single buffer.
     void charge_Buffer()
     {
		 //*std::cout << "\n --charge_Buffer()";
          Charging_Buffers[0].Input_Position = 0;
		  //*Charging_Buffers[0].output_All_Buffers();
          
          for (int cou_T=settings_Tier;cou_T<Input_Depth;cou_T++)
          {    
               Charging_Buffers[0].charge_Outputs(setting_Charge_Spike);
               
               for (int cou_Index=0;cou_Index<(Input_Depth - cou_T);cou_Index++)
               {
                    if (CAN.CAN[cou_T] [cou_Index].U != 0)
                    {
                         Charging_Buffers[0].submit(CAN.CAN[cou_T] [cou_Index].U, (settings_Base_Charge));
                    }
               }
               
               Charging_Buffers[0].gather();

			   //===
			   //*Charging_Buffers[0].output_All_Buffers();

               if (!Charging_Buffers[0].flg_Not_Done){ break; }
               //-----*ostr(0, 7, "\n cou_T:"); std::cout << cou_T;
          }
          //-----*std::cout << "\n  overcharge flg_Not_Done->" << Charging_Buffers[0].flg_Not_Done;
          while(Charging_Buffers[0].flg_Not_Done)
          {
               Charging_Buffers[0].charge_Outputs(setting_Charge_Spike);
               
               Charging_Buffers[0].gather();
               //-----*ostr(0, 7, "\n   flg_Not_Done...");
          }
     }
     
     
     //Charges a single buffer.
     void charge_Buffer_RC()
     {
		 //*std::cout << "\n --charge_Buffer_RC()";
          Charging_Buffers[0].Input_Position = 0;
		  //*Charging_Buffers[0].output_All_Buffers();

          for (int cou_T=settings_Tier;cou_T<Input_Depth;cou_T++)
          {    
               Charging_Buffers[0].charge_Outputs_RC(setting_Charge_Spike);
               
               for (int cou_Index=0;cou_Index<(Input_Depth - cou_T);cou_Index++)
               {
                    if (CAN.CAN[cou_T] [cou_Index].U != 0)
                    {
                         Charging_Buffers[0].submit_RC(CAN.CAN[cou_T] [cou_Index].U, (settings_Base_Charge));
                    }
               }
               
               Charging_Buffers[0].gather();
               
               if (!Charging_Buffers[0].flg_Not_Done){ break; }
			   //*ostr(0, 7, "\n cou_T:"); std::cout << cou_T;
			   //*Charging_Buffers[0].output_All_Buffers();
          }
          //--std::cout << "\n  overcharge flg_Not_Done->" << Charging_Buffers[0].flg_Not_Done;
          
          while(Charging_Buffers[0].flg_Not_Done)
          {
               Charging_Buffers[0].charge_Outputs_RC(setting_Charge_Spike, setting_Charge_L, setting_Charge_R);
               
               Charging_Buffers[0].gather();
               //--ostr(0, 7, "\n   flg_Not_Done...");
          }
          //*---std::cout << "\n    Done Chargin RC";
     }
     
     //Charges a single buffer.
     //Steps through the input cells in the charge table applying the charge to each unit of data as it goes.
     //Always charges from tier 0, disregards settings_Tier.
     void charge_Aux_Buffer()
     {
          Charging_Buffers[0].Input_Position = 0;
          
          //Charge the lowest tier with the input charges.
          Charging_Buffers[0].charge_Outputs_RC(setting_Charge_Spike);
          
          for (int cou_Index=0;cou_Index<(Input_Depth - 0);cou_Index++)
          {
               if (CAN.CAN[0] [cou_Index].U != 0)
               {
                    if (settings_RC)
                    {
                         Charging_Buffers[0].submit_RC(CAN.CAN[0] [cou_Index].U, (tbl_Input_Charges.get_U(Current_Row, cou_Index, 0)));
                    }
                    else
                    {
                         Charging_Buffers[0].submit(CAN.CAN[0] [cou_Index].U, (tbl_Input_Charges.get_U(Current_Row, cou_Index, 0)));
                    }
               }
          }
          
          Charging_Buffers[0].gather();
               
               
          for (int cou_T=1;cou_T<Input_Depth;cou_T++)
          {    
               Charging_Buffers[0].charge_Outputs_RC(setting_Charge_Spike);
               
               for (int cou_Index=0;cou_Index<(Input_Depth - cou_T);cou_Index++)
               {
                    if (CAN.CAN[cou_T] [cou_Index].U != 0)
                    {
                         if (settings_RC)
                         {
                              Charging_Buffers[0].submit_RC(CAN.CAN[cou_T] [cou_Index].U, (settings_Base_Charge));
                         }
                         else
                         {
                              Charging_Buffers[0].submit(CAN.CAN[cou_T] [cou_Index].U, (settings_Base_Charge));
                         }
                    }
               }
               
               Charging_Buffers[0].gather();
               
               if (!Charging_Buffers[0].flg_Not_Done){ break; }
               //*---ostr(0, 7, "\n cou_T:"); std::cout << cou_T;
          }
          //--std::cout << "\n  overcharge flg_Not_Done->" << Charging_Buffers[0].flg_Not_Done;
          
          while(Charging_Buffers[0].flg_Not_Done)
          {
               if (settings_RC)
               {
                    Charging_Buffers[0].charge_Outputs_RC(setting_Charge_Spike);
               }
               else
               {
                    Charging_Buffers[0].charge_Outputs(setting_Charge_Spike);
               }
               Charging_Buffers[0].gather();
               //--ostr(0, 7, "\n   flg_Not_Done...");
          }
          //*---std::cout << "\n    Done Chargin RC";
     }
     
     ////==-----------------------+
     //==--   BUILDING FUNCTIONS
     ////==-----------------------+
     
     //Builds the query only, used for testing.
     void Query()
     {
          if (tbl_Input.Number_Of_Rows == 0){ return; }
          
          tbl_Treetops_Output.reset();
          
          //For each cell build separately and then put the treetop in the corresponding output cell.
          if (settings_Build_Method == 0)
          {
               for (int cou_Row=0;cou_Row<tbl_Input.Number_Of_Rows;cou_Row++)
               {
                    Current_Row = cou_Row;
                    for (int cou_Cell=0;cou_Cell<tbl_Input.Rows[cou_Row]->Depth;cou_Cell++)
                    {
                         Current_Cell = cou_Cell;
                         Build_Input_Query();
                    }
               }
          }
          if (settings_Build_Method == 1)
          {
               for (int cou_Row=0;cou_Row<tbl_Input.Number_Of_Rows;cou_Row++)
               {
                    Current_Row = cou_Row;
                    Current_Cell = 0;
                    
                    //Gather the cells into one input string.
                    tbl_Input.Rows[cou_Row]->flatten_Row();
                    
                    Build_Input_Query();
               }
          }
     }
     
     //Builds full CAN.
     void Build()
     {
          if (tbl_Input.Number_Of_Rows == 0){ return; }
          
          tbl_Treetops_Output.soft_Reset();
          tbl_Charge_Output.soft_Reset();
          
          Current_Cell = 0;
          Current_Row = 0;
          
          //For each cell build separately and then put the treetop in the corresponding output cell.
          if (settings_Build_Method == 0)
          {
               for (int cou_Row=0;cou_Row<tbl_Input.Number_Of_Rows;cou_Row++)
               {
                    Current_Row = cou_Row;
                    for (int cou_Cell=0;cou_Cell<tbl_Input.Rows[cou_Row]->Depth;cou_Cell++)
                    {
                         Current_Cell = cou_Cell;
                         if (settings_RC)
                         {
                              Build_Input_RC();
                         }
                         else
                         {
                              Build_Input();
                         }
                    }
               }
          }
          if (settings_Build_Method == 1)
          {
               for (int cou_Row=0;cou_Row<tbl_Input.Number_Of_Rows;cou_Row++)
               {
                    Current_Row = cou_Row;
                    Current_Cell = 0;
                    
                    //Gather the cells into one input string.
                    tbl_Input.Rows[cou_Row]->flatten_Row();
                    
                    if (settings_RC)
                    {
                         Build_Input_RC();
                    }
                    else
                    {
                         Build_Input();
                    }
               }
          }
          Nodes.update_Header(Dir, FName);
     }
     
     //Builds for query.
     void Build_Query()
     {
          if (tbl_Input.Number_Of_Rows == 0){ return; }
          
          tbl_Treetops_Output.reset();
          
          //For each cell build separately and then put the treetop in the corresponding output cell.
          if (settings_Build_Method == 0)
          {
               for (int cou_Row=0;cou_Row<tbl_Input.Number_Of_Rows;cou_Row++)
               {
                    Current_Row = cou_Row;
                    for (int cou_Cell=0;cou_Cell<tbl_Input.Rows[cou_Row]->Depth;cou_Cell++)
                    {
                         Current_Cell = cou_Cell;
                         Build_Input_Query();
                    }
               }
          }
          if (settings_Build_Method == 1)
          {
               for (int cou_Row=0;cou_Row<tbl_Input.Number_Of_Rows;cou_Row++)
               {
                    Current_Row = cou_Row;
                    Current_Cell = 0;
                    
                    //Gather the cells into one input string.
                    tbl_Input.Rows[cou_Row]->flatten_Row();
                    
                    Build_Input_Query();
               }
          }
          
          Nodes.update_Header(Dir, FName);
     }
     
	 void add_Chrono_Input(u_Data_3 p_Node)
	 {
		 //Shift current inputs to the right, 0 will always be newest.
	 }

	 //Builds the Chrono if set.
	 void Build_Chrono()
	 {
		 //Add the current treetop to the Chrono Array.
		 add_Chrono_Input(CAN.Treetop);

		 //Build the Chrono
	 }

     //Builds full CAN.
     void Build_Input()
     {
          gather_CAN_Input();
          CAN.Full();
          if (settings_Build_Method == 0)
          {
               if (CAN.Treetop.U == 0){ tbl_Treetops_Output.add_Int(0); tbl_Charge_Output.add_Int(0); return; }
               //Why is it + 1?
               //tbl_Treetops_Output.add_Int(Current_Row, CAN.Treetop.U);
               //tbl_Treetops.add_Int(Current_Cell, (CAN.Treetop.U + 1));
               //In the other build methods to build across CAN.Treeto pwould be preceeded by Current_Row
               tbl_Treetops_Output.add_Int(Current_Row, u_Data_3_Set_Byte(CAN.Treetop.U, CID.U));
			   tbl_Charge_Output.add_Int(Current_Row, settings_Base_Charge);
               //-----*std::cout << "\n u_Data_3_Set_Byte(" << CAN.Treetop.U << ", " << CID.U << ")";
               //-----*std::cout << "\n   (" << u_Data_3_Set_Byte(CAN.Treetop.U, CID.U) << ")";
               //-----*std::cout << "\n   (" << u_Data_3_Get_Byte(u_Data_3_Set_Byte(CAN.Treetop.U, CID.U)) << ")";
               //tbl_Treetops_Output.add_Int((CAN.Treetop.U));
          }
          if (settings_Build_Method == 1)
          {
               if (CAN.Treetop.U == 0){ tbl_Treetops_Output.add_Int(0); tbl_Charge_Output.add_Int(0); return; }

			   tbl_Treetops_Output.add_Int(Current_Row, u_Data_3_Set_Byte(CAN.Treetop.U, CID.U));
			   tbl_Charge_Output.add_Int(Current_Row, settings_Base_Charge);
          }
     }
     
     //Builds RC CAN.
     void Build_Input_RC()
     {
          gather_CAN_Input();
          CAN.RC();
          if (settings_Build_Method == 0)
          {
               if (CAN.Treetop.U == 0){ tbl_Treetops_Output.add_Int(0); tbl_Charge_Output.add_Int(0); return; }
               
               tbl_Treetops_Output.add_Int(Current_Row, u_Data_3_Set_Byte(CAN.Treetop.U, CID.U));
			   tbl_Charge_Output.add_Int(Current_Row, settings_Base_Charge);
          }
          if (settings_Build_Method == 1)
          {
               if (CAN.Treetop.U == 0){ tbl_Treetops_Output.add_Int(0); tbl_Charge_Output.add_Int(0); return; }
               
               tbl_Treetops_Output.add_Int(Current_Row, u_Data_3_Set_Byte(CAN.Treetop.U, CID.U));
			   tbl_Charge_Output.add_Int(Current_Row, settings_Base_Charge);
          }
     }
     
     //Builds for query.
     void Build_Input_Query()
     {
          gather_CAN_Input();
          CAN.Query();
     }
     
     //Gathers the first cell of the input table to build. This is neccesary as the cells are dynamically
     //allocated and addresses may change.
     void gather_CAN_Input()
     {
          Input_Depth = 0;
          
          Input_Depth = tbl_Input.Rows[Current_Row]->Cells[Current_Cell]->Length;
          CAN.set_Input(tbl_Input.Rows[Current_Row]->Cells[Current_Cell]);
          
          
          Input_Charging_Depth = int (Input_Depth * settings_Input_Charging_Depth);
          if (Input_Charging_Depth < 1){ Input_Charging_Depth = 1; }
          //if (Input_Charging_Depth == 0 && Input_Depth == 1){ Input_Charging_Depth = 1; }
     }
     
     ////==--------------------------+
     //==--   DISCHARGING FUNCTIONS
     ////==--------------------------+
     
     //Discharges the treetops that have been passed from the higher tier constructs.
     void Discharge()
     {
          if (tbl_Treetops_Output.Number_Of_Rows == 0){ return; }
          
          tbl_Pattern_Output.reset();
          
          for (int cou_Row=0;cou_Row<tbl_Treetops_Output.Number_Of_Rows;cou_Row++)
          {
               if (tbl_Treetops_Output.Rows[cou_Row]->Depth == 0){ continue; }
               
               for (int cou_Cell=0;cou_Cell<tbl_Treetops_Output.Rows[cou_Row]->Depth;cou_Cell++)
               {
                    Nodes.bp(&tbl_Pattern_Output, settings_Flat_Output, cou_Row, tbl_Treetops_Output.Rows[cou_Row]->Cells[cou_Cell]->Data[0].U);
               }
          }
     }
     
     //Discharges the treetops for a given input.
     void discharge_Treetops(int p_Backpropagate)
     {
          Charging_Buffers[0].gather_Treetops();
          c_NT3_Linked_List_MMap * tmp_LL = Charging_Buffers[0].Treetops.Root;
          
          if (p_Backpropagate)
          {
               while(tmp_LL != NULL)
               {
                    if (tmp_LL->NID.U == 0){ tmp_LL = tmp_LL->Next; continue; }
                    //*std::cout << "\n";
                    //*tmp_LL->NID->bp_O();
                    //*std::cout << " "; odbl(0, 12, (settings_Base_Charge * settings_Action_Potential_Threshold));
                    //*std::cout << " "; oint(0, 13, (tmp_LL->Charge.I));
                    if (tmp_LL->Charge.I > (settings_Base_Charge * settings_Action_Potential_Threshold)){  } else { tmp_LL = tmp_LL->Next; continue; }
                    
                    int tmp_Row = 0;
                    if (settings_Build_Method == 0){ tmp_Row = 0; }
                    if (settings_Build_Method == 1){ tmp_Row = 0; }
                    
                    if (tmp_LL->NID.U == 0)
                    {
                         tbl_Pattern_Output.new_Cell(tmp_Row);
                         tbl_Pattern_Output.add_String("NULL");
                         tbl_Position_Output.add_Int(tmp_Row, 0);
                         tbl_Charge_Output.add_Int(tmp_Row, tmp_LL->Charge.I);
                         tbl_RC_Output.add_Float(tmp_Row, 0);
                         tbl_Treetops_Output.add_Int(tmp_Row, 0);
                         tbl_Salt_Output.add_Int(tmp_Row, 0);
                         tmp_LL = tmp_LL->Next;
                         continue;
                    }
                    
                    Nodes.bp(&tbl_Pattern_Output, settings_Flat_Output, tmp_Row, tmp_LL->NID);
                    
                    
                    tbl_Position_Output.add_Int(tmp_Row, 0);
                    tbl_Charge_Output.add_Int(tmp_Row, tmp_LL->Charge.I);
                    tbl_RC_Output.add_Float(tmp_Row, Nodes.get_RC_Score(tmp_LL->NID));
                    tbl_Treetops_Output.add_Int(tmp_Row, tmp_LL->NID.U);
                    
                    tmp_LL = tmp_LL->Next;
               }
          }
          else
          {
               while(tmp_LL != NULL)
               {
                    if (tmp_LL->NID.U == 0){ tmp_LL = tmp_LL->Next; continue; }
                    //*std::cout << "\n";
                    //*tmp_LL->NID->bp_O();
                    //*std::cout << " "; odbl(0, 12, (settings_Base_Charge * settings_Action_Potential_Threshold));
                    //*std::cout << " "; oint(0, 13, (tmp_LL->Charge.I));
                    if (tmp_LL->Charge.I > (settings_Base_Charge * settings_Action_Potential_Threshold)){  } else { tmp_LL = tmp_LL->Next; continue; }
                    
                    int tmp_Row = 0;
                    if (settings_Build_Method == 0){ tmp_Row = 0; }
                    if (settings_Build_Method == 1){ tmp_Row = 0; }
                    
                    if (tmp_LL->NID.U == 0)
                    {
                         tbl_Charge_Output.add_Int(tmp_Row, tmp_LL->Charge.I);
                         tbl_RC_Output.add_Float(tmp_Row, 0);
                         tbl_Treetops_Output.add_Int(tmp_Row, 0);
                         tmp_LL = tmp_LL->Next;
                         continue;
                    }
                    
                    tbl_Charge_Output.add_Int(tmp_Row, tmp_LL->Charge.I);
                    tbl_RC_Output.add_Float(tmp_Row, Nodes.get_RC_Score(tmp_LL->NID));
                    tbl_Treetops_Output.add_Int(tmp_Row, tmp_LL->NID.U);
                    
                    tmp_LL = tmp_LL->Next;
               }

          }
          //As long as the input position corresponds to the output construct then using a salt of the CID is not neccesary.
          
          //Now go through and extract the salts.
          c_Raw_Table_Cell_1D * tmp_Cell = NULL;
          if (settings_Eval_Method == 1)
          {
               if (tbl_Pattern_Output.Number_Of_Rows <= 0){ return; }
               
               for (int cou_Cell=0;cou_Cell<tbl_Pattern_Output.Rows[Current_Row]->Depth;cou_Cell++)
               {
                    //std::cout << "\n Cell "; oindex(cou_Cell);
                    tmp_Cell = tbl_Pattern_Output.Rows[Current_Row]->Cells[cou_Cell];
                    
                    for (int cou_Data=0;cou_Data<tmp_Cell->Length;cou_Data++)
                    {
                         tbl_Salt_Output.set_UInt(Current_Row, cou_Cell, cou_Data, u_Data_3_Get_Byte(tmp_Cell->Data[cou_Data].U));
                         tbl_Pattern_Output.set_UInt(Current_Row, cou_Cell, cou_Data, u_Data_3_Get_Unsalted(tmp_Cell->Data[cou_Data].U));
                         //std::cout << "\n      ["; oint(0, 7, cou_Cell); std::cout << "] " << tmp_Cell->Data[cou_Data].U << " "; oint(0, 12, u_Data_3_Get_Byte(tmp_Cell->Data[cou_Data].U)); std::cout << " "; oint(0, 11, u_Data_3_Get_Byte(tmp_Cell->Data[cou_Data].U));
                    }
               }
          }
          
     }
     
     
     ////==---------------------+
     //==--   OUTPUT FUNCTIONS
     ////==---------------------+
     
     //Resets the input tables.
     void reset_Inputs()
     {
          tbl_Input.soft_Reset();
          
          tbl_Input_Aux.soft_Reset();
          tbl_Input_Charges.soft_Reset();
          tbl_Input_Depths.soft_Reset();
          tbl_Input_Position.soft_Reset();
     }
     
     //Resets the output tables.
     void reset_Outputs()
     {
          tbl_Pattern_Output.soft_Reset();
          tbl_Salt_Output.soft_Reset();
          tbl_Charge_Output.soft_Reset();
          tbl_Position_Output.soft_Reset();
          tbl_RC_Output.soft_Reset();
          tbl_Treetops_Output.soft_Reset();
     
          tbl_Vision_Output.soft_Reset();
          tbl_Vision_Charge_Output.soft_Reset();
          
     }
     
     //Resets all the IO tables.
     void reset_IO()
     {
          reset_Inputs();
          reset_Outputs();
     }
     
     
     ////==---------------------+
     //==--   OUTPUT FUNCTIONS
     ////==---------------------+
     
     //Outputs the node network.
     void output_Nodes()
     {
          std::cout << "\n\n " << Name;
          Nodes.output_Nodes();
     }
     
     //Outputs the input table.
     void output_Input_Table()
     {
          std::cout << "\n\n ---===+===---";
          std::cout << "\n " << Name << " Inputs Tables ->" << this;
          tbl_Input.output_C(1);
          
          std::cout << "\n --===+===--\n";
     }
     void output_Input_Table_I()
     {
          std::cout << "\n\n ---===+===---";
          std::cout << "\n " << Name << " Inputs Tables ->" << this;
          tbl_Input.output_I(1);
          
          std::cout << "\n --===+===--\n";
     }
     void output_Input_Table_F()
     {
          std::cout << "\n\n ---===+===---";
          std::cout << "\n " << Name << " Inputs Tables ->" << this;
          tbl_Input.output_F(1);
          
          std::cout << "\n --===+===--\n";
     }
     void output_Input_Table_NR()
     {
          std::cout << "\n\n ---===+===---";
          std::cout << "\n " << Name << " Inputs Tables ->" << this;
          tbl_Input.output_NR(1);
          
          std::cout << "\n --===+===--\n";
     }
     
     //Outputs the input tables.
     void output_Input_Tables()
     {
          std::cout << "\n\n ---===+===---";
          std::cout << "\n " << Name << " Inputs Tables ->" << this;
          tbl_Input.output_C(1);
          
          //tbl_Input_Aux.output_I(1);
          tbl_Input_Charges.output_I(1);
          tbl_Input_Position.output_I(1);
          tbl_Input_Depths.output_I(1);
          std::cout << "\n --===+===--\n";
     }

     
     //Returns the number of nodes in the network.
     long long int get_Node_Count()
     {
          return Nodes.get_Node_Count();
     }
     
     //Outputs the treetops tree in the node network.
     void output_Treetops()
     {
          Nodes.output_Treetops();
     }
     
     //Outputs the treetops tree in a compressed BP format.
     void output_Treetops_BP()
     {
          //Nodes.output_Treetops_BP();
     }
     
	 //Outputs the tables of the construct.
	 void output_Output_Tables()
	 {
		 std::cout << "\n\n\t ---===+===---";
		 std::cout << "\n\t " << Name << " [" << CID.U << "] Output Tables ->" << this;

		 tbl_Pattern_Output.output_C(2);
		 //tbl_Pattern_Output.output_U(2);
		 //tbl_Pattern_Output.output_F(2);
		 tbl_Salt_Output.output_U(2);
		 tbl_Charge_Output.output_I(2);
		 tbl_Position_Output.output_I(2);
		 tbl_RC_Output.output_F(2);
		 tbl_Treetops_Output.output_U(2);
		 std::cout << "\n\t ---===+===---\n";
	 }
	 
	 //Outputs the tables of the construct.
	 void output_Output_Tables(std::string p_Pat_Out)
	 {
		 std::cout << "\n\n\t ---===+===---";
		 std::cout << "\n\t " << Name << " [" << CID.U << "] Output Tables ->" << this;

		 //tbl_Pattern_Output.output_U(2);
		 if (p_Pat_Out == "C") { tbl_Pattern_Output.output_C(2); }
		 if (p_Pat_Out == "D") { tbl_Pattern_Output.output_F(2); }
		 if (p_Pat_Out == "I") { tbl_Pattern_Output.output_I(2); }
		 if (p_Pat_Out == "U") { tbl_Pattern_Output.output_U(2); }
		 tbl_Salt_Output.output_U(2);
		 tbl_Charge_Output.output_I(2);
		 tbl_Position_Output.output_I(2);
		 tbl_RC_Output.output_F(2);
		 tbl_Treetops_Output.output_U(2);
		 std::cout << "\n\t ---===+===---\n";
	 }
     
     //Outputs the tables of the construct with the patterns as int.
     void output_Output_Tables_I()
     {
          std::cout << "\n\n\t ---===+===---";
          std::cout << "\n\t " << Name << " Output_Tables_I ->" << this;
          
          tbl_Pattern_Output.output_I(2);
          tbl_Salt_Output.output_I(2);
          tbl_Charge_Output.output_I(2);
          tbl_Position_Output.output_I(2);
          tbl_RC_Output.output_F(2);
          tbl_Treetops_Output.output_I(2);
          std::cout << "\n\t ---===+===---\n";
     }
     
     //Outputs the tables of the construct with the patterns as int.
     void output_Output_Tables_F()
     {
          std::cout << "\n\n\t ---===+===---";
          std::cout << "\n\t " << Name << " Output_Tables_I ->" << this;
          
          tbl_Pattern_Output.output_F(2);
          tbl_Salt_Output.output_C(2);
          tbl_Charge_Output.output_F(2);
          tbl_Position_Output.output_F(2);
          tbl_RC_Output.output_F(2);
          tbl_Treetops_Output.output_F(2);
          std::cout << "\n\t ---===+===---\n";
     }

	 //Outputs the CANs.
	 void output_CAN()
	 {
		 CAN.output_CAN();
	 }

	 //Outputs the CANs.
	 void output_CAN(int p_X, int p_Y)
	 {
		 CAN.output_CAN(p_X, p_Y);
	 }
          
     //Outputs the CAN state.
     void output_CAN_State(int p_Output_Type = 0)
     {
          CAN.output_CAN_State(p_Output_Type);
     }
          
     //Outputs the CAN state.
     void output_CAN_RC()
     {
          CAN.output_CAN_RC();
     }
          
     
     //Outputs the nodes in the CAN in a compact format.
     void output_Nodes_In_CAN_Compact()
     {
          CAN.output_Nodes_In_CAN_Compact();
     }
     
     //Outputs the settings of the current Construct.
     void output_Settings()
     {
          std::cout << "\n\n --  Settings For penis enlargement ===D~~ "; ostr(0, 13, Name);
          
          //Use in the expanded form.
          //std::cout << "\n  setting_Charge_L.................... "; oint(0, 11, setting_Charge_L);
          //std::cout << "\n  setting_Charge_R.................... "; oint(0, 11, setting_Charge_R);
          //std::cout << "\n  settings_Tier....................... "; oint(0, 11, settings_Tier);
          //std::cout << "\n  settings_Tier_Is_Floating........... "; oint(0, 11, settings_Tier_Is_Floating);
          std::cout << "\n  settings_Tier_Depth................. "; odbl(0, 11, settings_Tier_Depth);
          std::cout << "\n  settings_Action_Potential_Threshold. "; odbl(0, 11, settings_Action_Potential_Threshold);
          std::cout << "\n  settings_Base_Charge................ "; oint(0, 11, settings_Base_Charge);
          std::cout << "\n  settings_Modifier_Charge............ "; odbl(0, 11, settings_Modifier_Charge);
     }
     
     //Outputs the settings of the current Construct.
     void output_Settings_Full()
     {
          std::cout << "\n\n --  Settings For "; ostr(0, 13, Name);
          
          //Use in the expanded form.
          std::cout << "\n  Charge_L.................... "; oint(0, 11, setting_Charge_L);
          std::cout << "\n  Charge_R.................... "; oint(0, 11, setting_Charge_R);
          std::cout << "\n  Tier....................... "; oint(0, 11, settings_Tier);
          std::cout << "\n  Tier_Is_Floating........... "; oint(0, 11, settings_Tier_Is_Floating);
          std::cout << "\n  Tier_Depth................. "; odbl(0, 11, settings_Tier_Depth);
          std::cout << "\n  Action_Potential_Threshold. "; odbl(0, 11, settings_Action_Potential_Threshold);
          std::cout << "\n  Base_Charge................ "; oint(0, 11, settings_Base_Charge);
          std::cout << "\n  Modifier_Charge............ "; odbl(0, 11, settings_Modifier_Charge);
          std::cout << "\n  Use_Input_Charges............ "; odbl(0, 11, setting_Use_Input_Charges);
          std::cout << "\n  Build_Vision............ "; odbl(0, 11, settings_Build_Vision);
          std::cout << "\n  Bubble_Sort_Output............ "; odbl(0, 11, settings_Bubble_Sort_Output);
          std::cout << "\n  Input_Charging_Depth............ "; odbl(0, 11, settings_Input_Charging_Depth);
          std::cout << "\n  Cross_CAN_Charging............ "; odbl(0, 11, settings_Cross_CAN_Charging);
          std::cout << "\n  Stack_Output_From_Foreign_Treetops............ "; odbl(0, 11, settings_Stack_Output_From_Foreign_Treetops);
          std::cout << "\n  settings_RC.............. "; odbl(0, 11, settings_RC);
     }
     
     void output_Nodes_As_HTML(std::string p_Dir, std::string p_File, int p_X_Padd = 10, int p_Y_Padd = 40)
     {
          Nodes.output_Nodes_As_HTML(p_Dir, p_File, p_X_Padd, p_Y_Padd);
     }
     
     ////==------------------------+
     //==--   INTERFACE - TESTING
     ////==------------------------+
     
     //Backpropagates the current treetop node.
     void backpropagate_Current_Treetop()
     {
          if (CAN.Number_Of_Tiers != 0)
          {
               Nodes.bp_Output_Only(CAN.Treetop.U, 0);
          }
     }
     
     ////==--------------+
     //==--   INTERFACE
     ////==--------------+
     
     //Accepts an input to the inputs motherfucker.
     void in(int p_Row, int p_Cell, std::string p_Submission)
     {
          tbl_Input.set_string(p_Row, p_Cell, p_Submission);
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
	 void in(int p_Row, int p_Cell, void* p_Submission)
	 {
		 tbl_Input.set_NR(p_Row, p_Cell, p_Submission);
	 }


	 //Accepts an input to the inputs motherfucker.
	 void in(int p_Row, int p_Cell, u_Data_3 p_Submission)
	 {
		 tbl_Input.set_U_Data_3(p_Row, p_Cell, p_Submission);
	 }


	 //Accepts an input to the inputs motherfucker.
	 void in(int p_Row, int p_Cell, int p_Index, u_Data_3 p_Submission)
	 {
		 tbl_Input.set_U_Data_3(p_Row, p_Cell, p_Index, p_Submission);
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
	 
	 //Accepts an input to the inputs motherfucker.
	 void ina(int p_Row, int p_Cell, u_Data_3 p_Submission)
	 {
		 tbl_Input.add_Data(p_Row, p_Cell, p_Submission);
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
	 void ina_A(int p_Row, int p_Cell, void** p_Submission, int p_Length)
	 {
		 for (int cou_Index = 0; cou_Index < p_Length; cou_Index++)
		 {
			 ina(p_Row, p_Cell, p_Submission[cou_Index]);
		 }
	 }

	 //Accepts an array to be input into a row.
	 void ina_A(int p_Row, int p_Cell, u_Data_3 * p_Submission, int p_Length)
	 {
		 for (int cou_Index = 0; cou_Index < p_Length; cou_Index++)
		 {
			 ina(p_Row, p_Cell, p_Submission[cou_Index]);
		 }
	 }
     
	 //Gets the current treetop in the CAN.
	 uint64_t get_Treetop()
	 {
		 return CAN.Treetop.U;
	 }

	 //Gets the current treetop in the CAN.
	 u_Data_3 get_Treetop_Data_3()
	 {
		 return CAN.Treetop;
	 }
     
     
     ////==---------------------------------+
     //==--   SAVING AND LOADING FUNCTIONS
     ////==---------------------------------+
     
     //Saves the construct.
     void Save(std::string p_Dir, std::string p_Name)
     {
          if (p_Name == "NO_NAME"){ p_Name = Name; }
          
          //The save file.
          std::ofstream SF;
          
          std::string tmp_SF_FN = p_Dir + p_Name + ".Con.Header.dat";

		  std::cout << "\n     Saving Construct " << Name << " To \"" << tmp_SF_FN << "\"";

          SF.open(tmp_SF_FN, std::ios::trunc);
          
          if (!SF.is_open()){ std::cout << "\n\t SAVE FILE " << tmp_SF_FN << " CANNOT BE OPENED!!!"; return; }
          
          //-- Read the settings into the file.
          
          //The name of the construct.
          SF << "Name " << Name;
          // lol
          //The CID of the construct.
          SF << "\nCID.U " << CID.U;
          
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
          
          SF << "\nsettings_Build_Method " << settings_Build_Method;
          
          SF << "\nsettings_Eval_Method " << settings_Eval_Method;
          
          SF << "\nplacehold_For_Future_Setting " << 0;
          
          SF << "\nplacehold_For_Future_Setting " << 0;
          
          //Whether or not the RC is being used for the autocalibration.
          SF << "\nsettings_RC " << settings_RC;
          
          //Now that the settings are read in its time for the node network.
          //Nodes.save(&SF);
          
          SF.close();
     }
     
     //Loads a construct.     
     void Load(std::string p_Dir, std::string p_FName)
     {
          //The save file.
          std::ifstream LF;
          
          std::string tmp_Setting_Name = "";
          std::string tmp_Setting_Placeholder = "";
          
          std::string tmp_LF_FN = p_Dir + p_FName + ".Con.Header.dat";
          
          LF.open(tmp_LF_FN);
          
          if (!LF.is_open()){ std::cout << "\n\t SAVE FILE " << p_FName << " CANNOT BE OPENED!!!"; system("PAUSE > NULL"); return; }
          
          //-- Read the settings into the file.
          
          //The constructs name.
          LF >> tmp_Setting_Name;
          std::cout << "\nName " << tmp_Setting_Name;
          LF >> Name;
          std::cout << " " << Name;
          set_Name(Name);
          
          //CID of the construct.
          LF >> tmp_Setting_Name;
          std::cout << "\nCID " << tmp_Setting_Name;
          LF >> CID.U;
          std::cout << " " << CID.U;
          
          //Charging legs.
          LF >> tmp_Setting_Name;
          std::cout << "\nsetting_Charge_L " << tmp_Setting_Name;
          LF >> setting_Charge_L;
          std::cout << " " << setting_Charge_L;
          
          LF >> tmp_Setting_Name;
          std::cout << "\nsetting_Charge_R " << tmp_Setting_Name;
          LF >> setting_Charge_R;
          std::cout << " " << setting_Charge_R;
          
          //The tier to start charging.
          LF >> tmp_Setting_Name;
          std::cout << "\nsettings_Tier " << tmp_Setting_Name;
          LF >> settings_Tier;
          std::cout << " " << settings_Tier;
          
          //Floating vrs fixed tier depth
          LF >> tmp_Setting_Name;
          std::cout << "\nsettings_Tier_Is_Floating " << tmp_Setting_Name;
          LF >> settings_Tier_Is_Floating;
          std::cout << " " << settings_Tier_Is_Floating;
          
          //Floating tier modifier.
          LF >> tmp_Setting_Name;
          std::cout << "\nsettings_Tier_Depth " << tmp_Setting_Name;
          LF >> settings_Tier_Depth;
          std::cout << " " << settings_Tier_Depth;
          
          //The Action Potential Threshold as percentage of the base charge.
          LF >> tmp_Setting_Name;
          std::cout << "\nsettings_Action_Potential_Threshold " << tmp_Setting_Name;
          LF >> settings_Action_Potential_Threshold;
          std::cout << " " << settings_Action_Potential_Threshold;
          
          //The base charge to apply from the CAN.
          LF >> tmp_Setting_Name;
          std::cout << "\nsettings_Base_Charge " << tmp_Setting_Name;
          LF >> settings_Base_Charge;
          std::cout << " " << settings_Base_Charge;
          
          //The multiple to reduce each recursive charge by.
          LF >> tmp_Setting_Name;
          std::cout << "\nsettings_Modifier_Charge " << tmp_Setting_Name;
          LF >> settings_Modifier_Charge;
          std::cout << " " << settings_Modifier_Charge;
          
          //Whether or not to use the input charges for charging.
          LF >> tmp_Setting_Name;
          std::cout << "\nsetting_Use_Input_Charges " << tmp_Setting_Name;
          LF >> setting_Use_Input_Charges;
          std::cout << " " << setting_Use_Input_Charges;
          
          //Build a vision or do not build a vision.
          LF >> tmp_Setting_Name;
          std::cout << "\nsettings_Build_Vision " << tmp_Setting_Name;
          LF >> settings_Build_Vision;
          std::cout << " " << settings_Build_Vision;
          
          //Sort the tables by charge.
          LF >> tmp_Setting_Name;
          std::cout << "\nsettings_Bubble_Sort_Output " << tmp_Setting_Name;
          LF >> settings_Bubble_Sort_Output;
          std::cout << " " << settings_Bubble_Sort_Output;
          
          //settings_Input_Charging_Depth
          LF >> tmp_Setting_Name;
          std::cout << "\nsettings_Input_Charging_Depth " << tmp_Setting_Name;
          LF >> settings_Input_Charging_Depth;
          std::cout << " " << settings_Input_Charging_Depth;
          
          //Whether or not to stack outputs when discharging foreign nodes.
          LF >> tmp_Setting_Name;
          std::cout << "\nsettings_Stack_Output_From_Foreign_Treetops " << tmp_Setting_Name;
          LF >> settings_Stack_Output_From_Foreign_Treetops;
          std::cout << " " << settings_Stack_Output_From_Foreign_Treetops;
          
          //Placeholder For Future Settings
          LF >> tmp_Setting_Name;
          std::cout << "\nsettings_Build_Method " << tmp_Setting_Name;
          LF >> settings_Build_Method;
          std::cout << " " << settings_Build_Method;
          
          //Placeholder For Future Settings
          LF >> tmp_Setting_Name;
          std::cout << "\nsettings_Eval_Method " << tmp_Setting_Name;
          LF >> settings_Eval_Method;
          std::cout << " " << settings_Eval_Method;
          
          //Placeholder For Future Settings
		  LF >> tmp_Setting_Name;
		  std::cout << "\nsetting_Charge_Spike " << tmp_Setting_Name;
		  LF >> setting_Charge_Spike;
		  std::cout << " " << setting_Charge_Spike;
          
          //Placeholder For Future Settings
          LF >> tmp_Setting_Name;
          std::cout << "\nsettings_Flat_Output " << tmp_Setting_Name;
          LF >> settings_Flat_Output;
          std::cout << " " << settings_Flat_Output;
          
          //Placeholder For Future Settings
          LF >> tmp_Setting_Name;
          std::cout << "\nsettings_RC " << tmp_Setting_Name;
          LF >> settings_RC;
          std::cout << " " << settings_RC;
          
          //Now that the settings are read in its time for the node network.
          //Nodes.load(&LF);
          init(p_Dir, p_FName);
          
          LF.close();
     }
     
};