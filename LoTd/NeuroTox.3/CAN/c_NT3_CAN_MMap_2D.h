
//This 2D CAN only accepts square inputs

//The node references have been converted to u_Data unions.
//The class for the 2D CAN.
class c_NT3_CAN_MMap_2D
{
private:
     
     
public:
     
     //Reference to the input table to use for building.
     c_Raw_Table_Cell_2D * Input;
     
     //Reference to the node network to use for the node handling.
     c_NT3_Node_Network_MMap_2D * Nodes;
     
     //The nodes that make up the CAN.
     u_Data_3 *** CAN;
     
     //The current number of tiers in the CAN.
     int Number_Of_Tiers;
     
     //The treetop node.
     u_Data_3 Treetop;
     
     c_NT3_CAN_MMap_2D()
     {
          Input = NULL;
          Nodes = NULL;
          CAN = NULL;
          Treetop.U = 0;
          Number_Of_Tiers = 0;
     }
     
     ~c_NT3_CAN_MMap_2D()
     {
          reset();
     }
     
     //Initializes the CAN with an input cell and the node netowrk.
     void init(c_NT3_Node_Network_MMap_2D * p_Nodes)
     {
          Nodes = p_Nodes;
     }
     
     //Sets the current input cell.
     void set_Input(c_Raw_Table_Cell_2D * p_Input)
     {
          Input = p_Input;
     }
     
     //Builds full.
     void Full()
     {
          resize();
          //*-----cout << "\n Resized for an input with " << Input->Length << " unit of data.";
          //*-----output_CAN();
          fill_State();
          //*-----cout << "\n Filled out the state with nodes in the input set.";
          //*-----output_CAN();
          build_Tiers_Full();
          //*-----cout << "\n Build the entire CAN.";
          //*-----output_CAN();
     }
     
     //Builds full with RC reinforcing.
     void RC()
     {
          resize();
          //*-----cout << "\n Resized for an input with " << Input->Length << " unit of data.";
          //*-----output_CAN();
          fill_State();
          //*-----cout << "\n Filled out the state with nodes in the input set.";
          //*-----output_CAN();
          build_Tiers_Full();
          //*-----cout << "\n Build the entire CAN.";
          //*-----output_CAN();
          reinforce();
     }
     
     //Builds for Query.
     void Query()
     {
          resize();
          query_State();
          build_Tiers_Query();
     }
     
     //Fills out the State tier.
     void fill_State()
     {
		 for (int cou_X = 0; cou_X < Number_Of_Tiers; cou_X++)
		 {
			 for (int cou_Y = 0; cou_Y < Number_Of_Tiers; cou_Y++)
			 {
				 CAN[0][cou_X] [cou_Y].U = Nodes->get_State_Node(Input->Data[cou_X][cou_Y]);
			 }
          }
     }
          
     //Fills out the State tier.
     void query_State()
     {
		 for (int cou_X = 0; cou_X < Number_Of_Tiers; cou_X++)
		 {
			 for (int cou_Y = 0; cou_Y < Number_Of_Tiers; cou_Y++)
			 {
				 CAN[0][cou_X][cou_Y].U = Nodes->get_State_Node_Query(Input->Data[cou_X][cou_Y]);
			 }
		 }
     }
     
     //Builds the Tiers full.
     void build_Tiers_Full()
     {
          //Init the tiers and setup the indexes NULLing them along the way.
          for (int cou_T=0;cou_T<(Number_Of_Tiers - 2);cou_T++)
          {
			  for (int cou_X = 0; cou_X < ((Number_Of_Tiers - cou_T) - 1); cou_X++)
			  {
				  for (int cou_Y = 0; cou_Y < ((Number_Of_Tiers - cou_T) - 1); cou_Y++)
				  {
					  //*-----ostr(0, 12, "\n["); cout << cou_T; ostr(0, 12, " \\ "); cout << Number_Of_Tiers; ostr(0, 12, "]"); 
					  //*-----ostr(0, 12, " ["); cout << cou_Index; ; ostr(0, 12, " \\ "); cout << (Number_Of_Tiers - cou_T); ostr(0, 12, "]");
					  CAN[cou_T + 1][cou_X][cou_Y].U = Nodes->get_Upper_Tier_Connection(cou_T, CAN[cou_T][cou_X][cou_Y].U, CAN[cou_T][cou_X][cou_Y + 1].U, CAN[cou_T][cou_X + 1][cou_Y].U, CAN[cou_T][cou_X + 1][cou_Y + 1].U);
				  }
			  }
          }
          
          //If there are not enough tiers to create a treetop connection then return NULL.
          if (Number_Of_Tiers < 1){ Treetop.U = 0; return; }
          if (Number_Of_Tiers == 1){ Nodes->convert_To_Treetop_Node(CAN[0] [0] [0].U); Treetop = CAN[0] [0] [0]; return; }
          
          //Gets the treetop node.
          CAN[Number_Of_Tiers - 1] [0] [0].U = Nodes->get_Treetop_Connection((Number_Of_Tiers - 2), CAN[Number_Of_Tiers - 2] [0] [0].U, CAN[Number_Of_Tiers - 2][0][1].U, CAN[Number_Of_Tiers - 2][1][0].U, CAN[Number_Of_Tiers - 2][1][1].U);
          
          //Gather treetop node.
          Treetop = CAN[Number_Of_Tiers - 1] [0] [0];
     }
     
     //Builds the Tiers full.
     void build_Tiers_Query()
     {
          //Init the tiers and setup the indexes NULLing them along the way.
          for (int cou_T=0;cou_T<(Number_Of_Tiers - 1);cou_T++)
          {
			  for (int cou_X = 0; cou_X < ((Number_Of_Tiers - cou_T) - 1); cou_X++)
			  {
				  for (int cou_Y = 0; cou_Y < ((Number_Of_Tiers - cou_T) - 1); cou_Y++)
				  {
					  CAN[cou_T + 1][cou_X][cou_Y].U = Nodes->does_Upper_Tier_Connection_Exist(CAN[cou_T][cou_X][cou_Y].U, CAN[cou_T][cou_X][cou_Y + 1].U, CAN[cou_T][cou_X + 1][cou_Y].U, CAN[cou_T][cou_X + 1][cou_Y + 1].U);
				  }
			  }
          }
     }
     
     //Reinforces the nodes in the CAN.
     void reinforce()
     {
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
			  for (int cou_X = 0; cou_X < (Number_Of_Tiers - cou_T); cou_X++)
			  {
				  for (int cou_Y = 0; cou_Y < (Number_Of_Tiers - cou_T); cou_Y++)
				  {
					  if (CAN[cou_T][cou_X][cou_Y].U == 0) { continue; }
					  Nodes->reinforce(CAN[cou_T][cou_X][cou_Y].U);
				  }
			  }
          }
     }
     
     //Resets the CAN.
     void reset()
     {
          if (Number_Of_Tiers != 0)
          {
               for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
               {
                    if (CAN[cou_T] == NULL){ continue; }
					for (int cou_X = 0; cou_X < (Number_Of_Tiers - cou_T); cou_X++)
					{
						for (int cou_Y = 0; cou_Y < (Number_Of_Tiers - cou_T); cou_Y++)
						{
							CAN[cou_T][cou_X][cou_Y].U = 0;
						}
						delete[] CAN[cou_T] [cou_X];
						CAN[cou_T] [cou_X]= NULL;
                    }
                    delete [] CAN[cou_T];
                    CAN[cou_T] = NULL;
               }
               delete [] CAN;
               CAN = NULL;
          }
          Number_Of_Tiers = 0;
          
     }
     
     //Resizes the CAN to build the current input set.
     void resize()
     {
          //Reset the CAN before we begin.
          reset();
          
          //Gather the number of tiers to build.
          Number_Of_Tiers = Input->Width;
          
          //Setup the tiers.
          CAN = new u_Data_3**[Number_Of_Tiers];
          
          //Init the tiers and setup the indexes NULLing them along the way.
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
			  CAN[cou_T] = new u_Data_3 * [(Number_Of_Tiers - cou_T)];

			   for (int cou_X = 0; cou_X < (Number_Of_Tiers - cou_T); cou_X++)
			   {
				   CAN[cou_T] [cou_X] = new u_Data_3[(Number_Of_Tiers - cou_T)];

				   for (int cou_Y = 0; cou_Y < (Number_Of_Tiers - cou_T); cou_Y++)
				   {
					   CAN[cou_T][cou_X][cou_Y].U = 0;
				   }
               }
          }
     }

	 //Outputs the CAN.
	 void output_CAN()
	 {
		 int tmp_CLR = 0;
		 char tmp_CHR = 0;
		 cout << "\n\n";
		 for (int cou_T = 0; cou_T < Number_Of_Tiers; cou_T++)
		 {
			 cout << "\n";// << cou_T << "->";
			 for (int cou_X = 0; cou_X < (Number_Of_Tiers - cou_T); cou_X++)
			 {
				 for (int cou_Y = 0; cou_Y < (Number_Of_Tiers - cou_T); cou_Y++)
				 {

					 tmp_CLR = int(Nodes->get_RC_Score(CAN[cou_T][cou_X][cou_Y]));
					 tmp_CHR = int(CAN[cou_T][cou_X][cou_Y].U - ((CAN[cou_T][cou_X][cou_Y].U * .001) * 100) + 14);

					 opchr(0, tmp_CLR, tmp_CHR);
					 //if (CAN[cou_T] [cou_X] [cou_Y].U != 0){ cout << " <_"; oull(0, tmp_CLR, CAN[cou_T] [cou_X] [cou_Y].U); cout << "_>"; }
					 if (CAN[cou_T][cou_X][cou_Y].U == 0) { cout << "-"; }
					 //if (CAN[cou_T] [cou_Index].U == 0){ cout << " <_NULL_>"; }
				 }
			 }
		 }
	 }

	 //Outputs the CAN.
	 void output_CAN(int p_X, int p_Y)
	 {
		 int tmp_CLR = 0;
		 char tmp_CHR = 0;
		 //cout << "\n\n";
		 for (int cou_T = 0; cou_T < Number_Of_Tiers; cou_T++)
		 {
			 //cout << "\n";// << cou_T << "->";
			 xy(p_X, (p_Y + cou_T));
			 for (int cou_X = 0; cou_X < (Number_Of_Tiers - cou_T); cou_X++)
			 {
				 for (int cou_Y = 0; cou_Y < (Number_Of_Tiers - cou_T); cou_Y++)
				 {
					 tmp_CLR = int(Nodes->get_RC_Score(CAN[cou_T][cou_X] [cou_Y]));
					 tmp_CHR = int(CAN[cou_T][cou_X] [cou_Y].U - ((CAN[cou_T][cou_X] [cou_Y].U * .001) * 100) + 14);

					 opchr(0, tmp_CLR, tmp_CHR);
					 //if (CAN[cou_T] [cou_X] [cou_Y].U != 0){ cout << " <_"; oull(0, tmp_CLR, CAN[cou_T] [cou_X] [cou_Y].U); cout << "_>"; }
					 if (CAN[cou_T][cou_X] [cou_Y].U == 0) { cout << "-"; }
					 //if (CAN[cou_T] [cou_X] [cou_Y].U == 0){ cout << " <_NULL_>"; }
				 }
			 }
		 }
	 }
          
     //Outputs the CAN.
     void output_CAN_RC()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
			   for (int cou_X = 0; cou_X < (Number_Of_Tiers - cou_T); cou_X++)
			   {
				   for (int cou_Y = 0; cou_Y < (Number_Of_Tiers - cou_T); cou_Y++)
				   {
					   cout << " {" << Nodes->get_RC_Score(CAN[cou_T][cou_X][cou_Y].U) << "}";
				   }
               }
          }
     }
          
     //Outputs the CAN.
     void output_CAN_State(int p_Output_Type)
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
			   for (int cou_Y = 0; cou_Y < (Number_Of_Tiers - cou_T); cou_Y++)
			   {
				   for (int cou_X = 0; cou_X < (Number_Of_Tiers - cou_T); cou_X++)
				   {
					   cout << " {";
					   Nodes->bp_Output_Only(CAN[cou_T][cou_X][cou_Y].U, p_Output_Type);
					   cout << "}";
				   }
				   cout << "\n";
               }
          }
     }
          
     //Outputs the CAN.
     void output_CAN_Type()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
			   for (int cou_Y = 0; cou_Y < (Number_Of_Tiers - cou_T); cou_Y++)
			   {
				   for (int cou_X = 0; cou_X < (Number_Of_Tiers - cou_T); cou_X++)
				   {
					   {
						   cout << " {" << (Nodes->get_Type(CAN[cou_T][cou_X][cou_Y].U)) << "}";
					   }
				   }
				   cout << "\n";
			   }
          }
     }
          
     //Outputs the CAN.
     void output_CAN_Axons()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    //cout << " {" << CAN[cou_T] [cou_Index] << " (" << (CAN[cou_T] [cou_Index]->Axon_Count_L) << ", " << (CAN[cou_T] [cou_Index]->Axon_Count_R) << ")}";
               }
          }
     }
     
     //Outputs the CAN nodes.
     void output_Nodes_In_CAN_Compact()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
			   for (int cou_Y = 0; cou_Y < (Number_Of_Tiers - cou_T); cou_Y++)
			   {
				   cout << "\n -" << cou_Y << "-->";
				   for (int cou_X = 0; cou_X < (Number_Of_Tiers - cou_T); cou_X++)
				   {
					   Nodes->output_Node_Compact(CAN[cou_T][cou_X][cou_Y].U);
				   }
			   }
          }
     }

};