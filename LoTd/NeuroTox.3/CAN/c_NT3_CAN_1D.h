

//The class for the 1D CAN.
class c_NT3_CAN_1D
{
private:
     
     
public:
     
     //Reference to the input table to use for building.
     c_Raw_Table_Cell_1D * Input;
     
     //Reference to the node network to use for the node handling.
     c_NT3_Node_Network_1D * Nodes;
     
     //The nodes that make up the CAN.
     c_NT3_Base_Node_1D *** CAN;
     
     //The current number of tiers in the CAN.
     int Number_Of_Tiers;
     
     //The treetop node.
     c_NT3_Base_Node_1D * Treetop;
     
     c_NT3_CAN_1D()
     {
          Input = NULL;
          Nodes = NULL;
          CAN = NULL;
          Treetop = NULL;
          Number_Of_Tiers = 0;
     }
     
     ~c_NT3_CAN_1D()
     {
          reset();
     }
     
     //Initializes the CAN with an input cell and the node netowrk.
     void init(c_NT3_Node_Network_1D * p_Nodes)
     {
          Nodes = p_Nodes;
     }
     
     //Sets the current input cell.
     void set_Input(c_Raw_Table_Cell_1D * p_Input)
     {
          Input = p_Input;
     }
     
     //Builds full.
     void Full()
     {
          resize();
          fill_State();
          build_Tiers_Full();
     }
     
     //Builds full with RC reinforcing.
     void RC()
     {
          resize();
          fill_State();
          build_Tiers_Full();
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
          for (int cou_Index=0;cou_Index<Number_Of_Tiers;cou_Index++)
          {
               CAN[0] [cou_Index] = Nodes->get_State_Node(Input->Data[cou_Index]);
          }
     }
          
     //Fills out the State tier.
     void query_State()
     {
          for (int cou_Index=0;cou_Index<Number_Of_Tiers;cou_Index++)
          {
               CAN[0] [cou_Index] = Nodes->get_State_Node_Query(Input->Data[cou_Index]);
          }
     }
     
     //Builds the Tiers full.
     void build_Tiers_Full()
     {
          //Init the tiers and setup the indexes NULLing them along the way.
          for (int cou_T=0;cou_T<(Number_Of_Tiers - 2);cou_T++)
          {
               for (int cou_Index=0;cou_Index<((Number_Of_Tiers - cou_T) - 1);cou_Index++)
               {
                    CAN[cou_T + 1] [cou_Index] = Nodes->get_Upper_Tier_Connection(CAN[cou_T] [cou_Index], CAN[cou_T] [cou_Index + 1]);
               }
          }
          
          //If there are not enough tiers to create a treetop connection then return NULL.
          if (Number_Of_Tiers < 1){ Treetop = NULL; return; }
          if (Number_Of_Tiers == 1){ Nodes->convert_To_Treetop_Node(CAN[0] [0]); Treetop = CAN[0] [0]; return; }
          
          //Gets the treetop node.
          CAN[Number_Of_Tiers - 1] [0] = Nodes->get_Treetop_Connection(CAN[Number_Of_Tiers - 2] [0], CAN[Number_Of_Tiers - 2] [1]);
          
          //Gather treetop node.
          Treetop = CAN[Number_Of_Tiers - 1] [0];
     }
     
     //Builds the Tiers full.
     void build_Tiers_Query()
     {
          //Init the tiers and setup the indexes NULLing them along the way.
          for (int cou_T=0;cou_T<(Number_Of_Tiers - 1);cou_T++)
          {
               for (int cou_Index=0;cou_Index<((Number_Of_Tiers - cou_T) - 1);cou_Index++)
               {
                    CAN[cou_T + 1] [cou_Index] = Nodes->does_Upper_Tier_Connection_Exist(CAN[cou_T] [cou_Index], CAN[cou_T] [cou_Index + 1]);
               }
          }
     }
     
     //Reinforces the nodes in the CAN.
     void reinforce()
     {
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    if (CAN[cou_T] [cou_Index] == NULL){ continue; }
                    CAN[cou_T] [cou_Index]->reinforce();
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
                    for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
                    {
                         CAN[cou_T] [cou_Index] = NULL;
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
          Number_Of_Tiers = Input->Length;
          
          //Setup the tiers.
          CAN = new c_NT3_Base_Node_1D**[Number_Of_Tiers];
          
          //Init the tiers and setup the indexes NULLing them along the way.
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               CAN[cou_T] = new c_NT3_Base_Node_1D*[(Number_Of_Tiers - cou_T)];
               
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    CAN[cou_T] [cou_Index] = NULL;
               }
          }
     }
     
     //Outputs the CAN.
     void output_CAN()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    cout << " {" << CAN[cou_T] [cou_Index] << "}";
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
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    cout << " {" << CAN[cou_T] [cou_Index]->get_RC_Score() << "}";
               }
          }
     }
          
     //Outputs the CAN.
     void output_CAN_State()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    cout << " {";
                    Nodes->bp_Output_Only(CAN[cou_T] [cou_Index]);
                    cout << "}";
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
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    cout << " {" << (CAN[cou_T] [cou_Index]->get_Type()) << "}";
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
                    cout << " {" << CAN[cou_T] [cou_Index] << " (" << (CAN[cou_T] [cou_Index]->Axon_Count_L) << ", " << (CAN[cou_T] [cou_Index]->Axon_Count_R) << ")}";
               }
          }
     }

};