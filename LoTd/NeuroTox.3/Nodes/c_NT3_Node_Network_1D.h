

//The node network that handles the nodes.
class c_NT3_Node_Network_1D
{
private:
     
public:
     
     //The one that started it all.
     c_NT3_Base_Node_1D * Root;
     
     //The current node to attach to.
     c_NT3_Base_Node_1D ** Current_Node;
     
     //The Fractional tree for handling state queries.
     c_NT3_Fractal_State_Tree State_Tree;
     
     //The tree to hold the Treetops for backpropagation.
     c_NT3_Fractal_State_Tree Treetops_Tree;
     
     //This tracks the current Nodes ID to be entered into the node tracking tree for saving and loading.
     u_Data_3 CNID;
     
     //Tree to track the nodes and their IDs.
     //Only used during saving and loading, then it is destroyed to save memory.
     c_NT3_Fractal_State_Tree NID_Tree;
     
     c_NT3_Node_Network_1D()
     {
          CNID.I = 0;
          Root = NULL;
          Current_Node = &Root;
     }
     
     ~c_NT3_Node_Network_1D()
     {
          cout << "\n ~c_NT3_Node_Network_1D " << CNID.I << " " << this << ".........."; cout.flush();
          //delete Root;
          
          c_NT3_Base_Node_1D * tmp_LL = Root;
          Root = NULL;
          c_NT3_Base_Node_1D * tmp_LL_Next = NULL;
          
          while (tmp_LL != NULL)
          {
               tmp_LL_Next = tmp_LL->Next;
               tmp_LL->Next = NULL;
               delete tmp_LL;
               tmp_LL = NULL;
               tmp_LL = tmp_LL_Next;
          }
          
          delete Root;
          Root = NULL;
          
          cout << "\n ~~~c_NT3_Node_Network_1D " << CNID.I << " " << this << ".........."; cout.flush();
     }
     
     //Resets the tree.
     void reset()
     {
          delete Root;
     }
     
     
     ////==------------------+
     //==--   NODE CREATION
     ////==------------------+
     
     //Creates a new state node.
     c_NT3_Base_Node_1D * new_State_Node(u_Data_3 p_State, int p_A_L = 0, int p_A_R = 0)
     {
          //Create the state node.
          *Current_Node = new c_NT3_State_Node_1D;
          
          //Sets the new nodes ID.
          (*Current_Node)->NID.I = CNID.I;
          
          //Increment the node tracker.
          CNID.I++;
          
          //If axon couts are given expand the axon arrays.
          if (p_A_L){ (*Current_Node)->expand_Axon_L(p_A_L); }
          if (p_A_R){ (*Current_Node)->expand_Axon_R(p_A_R); }
          
          //Add it to the state tree, assuming that the current state has already been queried.
          State_Tree.set_Current_Node_NID(*Current_Node);
          
          //Set the current nodes state.
          (*Current_Node)->set_State(p_State);
          
          //Set the Current node to the next one in the chain.
          Current_Node = &(*Current_Node)->Next;
                    
          //Return the created state node using the state tree as Current_Node is changed.
          return State_Tree.get_Current_Node_NID();
     }
     
     //Creates a new node.
     c_NT3_Base_Node_1D * new_Node(int p_A_L = 0, int p_A_R = 0)
     {
          //A tmp Node because we iterate the Current_Node.
          c_NT3_Base_Node_1D * tmp_Return_Node;
          
          //Create the state node.
          *Current_Node = new c_NT3_Node_1D;
          tmp_Return_Node = *Current_Node;
          
          //Sets the new nodes ID.
          (*Current_Node)->NID.I = CNID.I;
          
          //Increment the node tracker.
          CNID.I++;
          
          //If axon couts are given expand the axon arrays.
          if (p_A_L){ (*Current_Node)->expand_Axon_L(p_A_L); }
          if (p_A_R){ (*Current_Node)->expand_Axon_R(p_A_R); }
          
          //Set the Current node to the next one in the chain.
          Current_Node = &(*Current_Node)->Next;
          
          //Return the node that was created.
          return tmp_Return_Node;
     }
     
     //Creates a treetop state node for cases where input has only one tier.
     void convert_To_Treetop_Node(c_NT3_Base_Node_1D * p_Node)
     {
          
          if (p_Node->get_Type() == 0){ p_Node->set_Type(2); } //If the node is a state node then set it to a state treetop node.
          if (p_Node->get_Type() == 1){ p_Node->set_Type(3); } //IF the node is a normal node then set the type to treetop.
          
          //Search for the node in the treetop tree.
          Treetops_Tree.search(p_Node->NID);
          
          //If the node has not been found then add it to the tree.
          if (Treetops_Tree.get_Current_Node_NID() == NULL)
          {
               Treetops_Tree.set_Current_Node_NID(p_Node);
          }
     }
     
     
     ////==------------------------------------+
     //==--   CONNECTION TESTING AND CREATION
     ////==------------------------------------+
     
     //Creates a connection from p_From to p_To.
     void create_Connection_L(c_NT3_Base_Node_1D * p_From, c_NT3_Base_Node_1D * p_To)
     {
          p_From->add_Axon_L(p_To);
          p_To->set_Dendrite_L(p_From);
     }
     
     //Creates a connection from p_From to p_To.
     void create_Connection_R(c_NT3_Base_Node_1D * p_From, c_NT3_Base_Node_1D * p_To)
     {
          p_From->add_Axon_R(p_To);
          p_To->set_Dendrite_R(p_From);
     }
     
     //Creates a connection from p_From to p_To.
     void create_Predefined_Connection_L(c_NT3_Base_Node_1D * p_From, c_NT3_Base_Node_1D * p_To)
     {
          p_From->add_Predefined_Axon_L(p_To);
          p_To->set_Dendrite_L(p_From);
     }
     
     //Creates a connection from p_From to p_To.
     void create_Predefined_Connection_R(c_NT3_Base_Node_1D * p_From, c_NT3_Base_Node_1D * p_To)
     {
          p_From->add_Predefined_Axon_R(p_To);
          p_To->set_Dendrite_R(p_From);
     }
     
     //Checks for an upper tier connection.
     c_NT3_Base_Node_1D * does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_1D * p_L, c_NT3_Base_Node_1D * p_R)
     {
          if (p_L == NULL || p_R == NULL){ return NULL; }
          return p_L->does_Upper_Tier_Connection_Exist(p_R);
     }
     
     
     
     ////==---------------------+
     //==--   RETRIEVING NODES
     ////==---------------------+
     
     //Assigns a given node to a state, used for loading.
     c_NT3_Base_Node_1D * assign_State_Node(u_Data_3 p_State, c_NT3_Base_Node_1D * p_Node, int p_A_L = 0, int p_A_R = 0)
     {
          //Search for the node.
          State_Tree.search(p_State);
          
          //If the node has not been found then create it.
          if (State_Tree.get_Current_Node_NID() == NULL)
          {
               new_State_Node(p_State, p_A_L, p_A_R);
          }
          
          //Return the current node NID.
          return State_Tree.get_Current_Node_NID();
     }
     
     //Get a state node.
     c_NT3_Base_Node_1D * get_State_Node(u_Data_3 p_State, int p_A_L = 0, int p_A_R = 0)
     {
          //Search for the node.
          State_Tree.search(p_State);
          
          //If the node has not been found then create it.
          if (State_Tree.get_Current_Node_NID() == NULL)
          {
               new_State_Node(p_State, p_A_L, p_A_R);
          }
          
          //Return the current node NID.
          return State_Tree.get_Current_Node_NID();
     }
     
     //Get a state node for a query, if none exist return NULL.
     c_NT3_Base_Node_1D * get_State_Node_Query(u_Data_3 p_State)
     {
          //Search for the node.
          State_Tree.search(p_State);
          
          //Return the current node NID.
          return State_Tree.get_Current_Node_NID();
     }
     
     //Gets an upper tier connection even if one has to be created.
     c_NT3_Base_Node_1D * get_Upper_Tier_Connection(c_NT3_Base_Node_1D * p_L, c_NT3_Base_Node_1D * p_R)
     {
          //If either submitted node is NULL then return NULL.
          if (p_L == NULL || p_R == NULL){ return NULL; }
          
          //A tmp var to hold the upper tier node for returning.
          c_NT3_Base_Node_1D * tmp_Node = NULL;
          
          //Check for an upper tier connection already, if one exists then return it.
          tmp_Node = p_L->does_Upper_Tier_Connection_Exist(p_R);
          if (tmp_Node != NULL)
          {
               return tmp_Node;
          }
          
          //If one does not exist then create it.
          tmp_Node = new_Node();
          
          tmp_Node->set_Dendrite_L(p_L);
          tmp_Node->set_Dendrite_R(p_R);
          
          p_L->add_Axon_L(tmp_Node);
          p_R->add_Axon_R(tmp_Node);
          
          return tmp_Node;
     }
     
     //Gets a treetop connection.
     c_NT3_Base_Node_1D * get_Treetop_Connection(c_NT3_Base_Node_1D * p_L, c_NT3_Base_Node_1D * p_R)
     {
          c_NT3_Base_Node_1D * tmp_Node = get_Upper_Tier_Connection(p_L, p_R);
          
          convert_To_Treetop_Node(tmp_Node);
          
          return tmp_Node;
     }
     
     
     ////==----------------------------+
     //==--   BACKPROPAGATION STATION
     ////==----------------------------+
     
     //Starts the Backprop procedures for output only.
     void bp_Output_Only(c_NT3_Base_Node_1D * p_Node, int p_Type = 0)
     {
          //For when directly output the dendrites of a nodes on the base tier.
          if (p_Node == NULL){ return; }
          ochr(0, 12, '<');
          bp_Output_Only_L(p_Node, p_Type);
          ochr(0, 12, '>');
     }
     
     //bp_Output the left node.
     void bp_Output_Only_L(c_NT3_Base_Node_1D * p_Node, int p_Type)
     {
          char tmp_State = ' ';
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (p_Node->Dendrite_L != NULL)
          {
               bp_Output_Only_L(p_Node->Dendrite_L, p_Type);
               bp_Output_Only_R(p_Node->Dendrite_R, p_Type);
          }
          else
          {
               if (p_Type == 0)
               {
                    tmp_State = char (p_Node->get_State());
                    if (tmp_State == char (0)){ tmp_State = '0'; }
                    if (tmp_State == char (7)){ tmp_State = '7'; }
                    if (tmp_State == char (8)){ tmp_State = '8'; }
                    if (tmp_State == char (9)){ tmp_State = '9'; }
                    if (tmp_State == char (10)){ tmp_State = 'a'; }
                    if (tmp_State == char (13)){ tmp_State = 'd'; }
                    cout << tmp_State;
               }
               if (p_Type == 1)
               {
                    cout << p_Node->get_State() << " ";
               }
          }
     }
     
     //bp_Output the right node.
     void bp_Output_Only_R(c_NT3_Base_Node_1D * p_Node, int p_Type)
     {
          char tmp_State = ' ';
          //If a right leg exists then initiate a backpropagation.
          if (p_Node->Dendrite_R != NULL)
          {
               bp_Output_Only_R(p_Node->Dendrite_R, p_Type);
          }
          else
          {
               if (p_Type == 0)
               {
                    tmp_State = char (p_Node->get_State());
                    if (tmp_State == char (0)){ tmp_State = '0'; }
                    if (tmp_State == char (7)){ tmp_State = '7'; }
                    if (tmp_State == char (8)){ tmp_State = '8'; }
                    if (tmp_State == char (9)){ tmp_State = '9'; }
                    if (tmp_State == char (10)){ tmp_State = 'a'; }
                    if (tmp_State == char (13)){ tmp_State = 'd'; }
                    cout << tmp_State;
               }
               if (p_Type == 1)
               {
                    cout << p_Node->get_State();
               }
          }
     }
     
     
     //Starts the Backprop procedures for output only.
     void bp(c_Raw_Table_1D * p_Pattern_Output, int p_Flat_Output, int p_Input, c_NT3_Base_Node_1D * p_Node)
     {
          //For when directly output the dendrites of a nodes on the base tier.
          if (p_Node == NULL){ return; }
          
          if (p_Flat_Output)
          {
               p_Pattern_Output->new_Cell(p_Input);
          }
          else
          {
               p_Pattern_Output->new_Cell(0);
          }
          bp_L(p_Pattern_Output, p_Input, p_Node);
     }
     
     //bp_Output the left node.
     void bp_L(c_Raw_Table_1D * p_Pattern_Output, int p_Input, c_NT3_Base_Node_1D * p_Node)
     {
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (p_Node->Dendrite_L != NULL)
          {
               bp_L(p_Pattern_Output, p_Input, p_Node->Dendrite_L);
               bp_R(p_Pattern_Output, p_Input, p_Node->Dendrite_R);
          }
          else
          {
               p_Pattern_Output->add_Data_Int(p_Input, p_Node->get_State());
          }
     }
     
     //bp_Output the right node.
     void bp_R(c_Raw_Table_1D * p_Pattern_Output, int p_Input, c_NT3_Base_Node_1D * p_Node)
     {
          //If a right leg exists then initiate a backpropagation.
          if (p_Node->Dendrite_R != NULL)
          {
               bp_R(p_Pattern_Output, p_Input, p_Node->Dendrite_R);
          }
          else
          {
               p_Pattern_Output->add_Data_Int(p_Input, p_Node->get_State());
          }
     }
     
     
     //Backpropagates a given Treetop. Returns the treetops address.
     c_NT3_Base_Node_1D * bp_Treetop(c_Raw_Table_1D * p_Pattern_Output,int p_Flat_Output, int p_Input, u_Data_3 p_NID)
     {
          c_NT3_Base_Node_1D * tmp_Node = NULL;
          
          //Search for the node.
          Treetops_Tree.search(p_NID);
          
          //Return the current node NID.
          tmp_Node = Treetops_Tree.get_Current_Node_NID();
          
          //Backpropagate the treetop node. It is assumed this will always be a valid treetop, however, the bp() will catch a NULL with no error.
          bp(p_Pattern_Output, p_Flat_Output, p_Input, tmp_Node);
          
          return tmp_Node;
     }
     
     ////==---------------------------------+
     //==--   SAVING AND LOADING FUNCTIONS
     ////==---------------------------------+
     
     //Saves the node network.
     void save(ofstream * p_SF)
     {
          //Create the file.
          cout << "\n\n Node Network Saving..........";
          if (p_SF->is_open()){ cout << "Authorized"; } else { cout << "Denied"; return; }
          
          //Save the nodes of the nodes.
          cout << "\n Saving Nodes.........." << CNID.U << ".Found.....";
          
          *p_SF << "\nNode_Count " << CNID.U;
          
          c_NT3_Base_Node_1D * tmp_Node = Root;
          
          if ((tmp_Node == NULL) && (CNID.U > 0)){ ostr(0, 12, "\n\n ERROR IN SAVING, NODES FOUND BUT ROOT IS NULL!!!"); }
          
          while(tmp_Node != NULL)
          {
               if (tmp_Node->Type == 0)
               {
                    *p_SF << "\nSN " << tmp_Node->get_State();
                    *p_SF << " " << tmp_Node->NID.I;
                    *p_SF << " " << tmp_Node->Axon_Count_L;
                    *p_SF << " " << tmp_Node->Axon_Count_R;
               }
               if (tmp_Node->Type == 1)
               {
                    *p_SF << "\nN ";
                    *p_SF << " " << tmp_Node->NID.I;
                    *p_SF << " " << tmp_Node->Axon_Count_L;
                    *p_SF << " " << tmp_Node->Axon_Count_R;
                    *p_SF << " " << tmp_Node->Dendrite_L->NID.I;
                    *p_SF << " " << tmp_Node->Dendrite_R->NID.I;
               }
               if (tmp_Node->Type == 2)
               {
                    *p_SF << "\nSTN " << tmp_Node->get_State();
                    *p_SF << " " << tmp_Node->NID.I;
                    *p_SF << " " << tmp_Node->Axon_Count_L;
                    *p_SF << " " << tmp_Node->Axon_Count_R;
               }
               if (tmp_Node->Type == 3)
               {
                    *p_SF << "\nTN ";
                    *p_SF << " " << tmp_Node->NID.I;
                    *p_SF << " " << tmp_Node->Axon_Count_L;
                    *p_SF << " " << tmp_Node->Axon_Count_R;
                    *p_SF << " " << tmp_Node->Dendrite_L->NID.I;
                    *p_SF << " " << tmp_Node->Dendrite_R->NID.I;
               }
               *p_SF << " " << tmp_Node->RC_Lvl;
               
               tmp_Node = tmp_Node->Next;
          }
          
          cout << "Complete";
     }
     
     //Loads the node network.
     void load_O(ifstream * p_LF)
     {
          //Create the file.
          cout << "\n\n Node Network Loading..........";
          if (p_LF->is_open()){ cout << "Authorized"; } else { cout << "Denied"; return; }
          
          u_Data_3 Node_Count;
          Node_Count.U = 0;
          
          c_NT3_Fractal_State_Tree tmp_Scaffold;
          
          c_NT3_Base_Node_1D * tmp_Node = NULL;
          c_NT3_Base_Node_1D * tmp_D_L = NULL;
          c_NT3_Base_Node_1D * tmp_D_R = NULL;
          
          string tmp_Node_Type = "";
          u_Data_3 tmp_State;
          tmp_State.I = 0;
          u_Data_3 tmp_NID;
          tmp_NID.I = 0;
          u_Data_3 tmp_L;
          tmp_L.I = 0;
          u_Data_3 tmp_R;
          tmp_R.I = 0;
          u_Data_3 tmp_RC_Lvl;
          tmp_RC_Lvl.F = 0.0f;
          
          
          *p_LF >> tmp_Node_Type;
          tmp_Node_Type = "";
          *p_LF >> Node_Count.U;
          cout << "\n " << Node_Count.U << " Nodes Found..........";
          int tmp_X = get_Console_Cursor_X();
          int tmp_Y = get_Console_Cursor_Y();
          u_Data_3 Node_Counter;
          Node_Counter.U = 1;
          int tmp_Div = int(Node_Count.I) / 1000;
          if (tmp_Div == 0){ tmp_Div = 1; }
          
          while(!(*p_LF).eof())
          {
               if (!(Node_Counter.U % tmp_Div))
               { 
                    xy(tmp_X, tmp_Y); 
                    cout << ((double (Node_Counter.U) / double (Node_Count.U)) * 100) << "%      "; 
               }
               Node_Counter.U++;
               
               *p_LF >> tmp_Node_Type;
               //*cout << "\n\n" << tmp_Node_Type;
               if (tmp_Node_Type == "SN")
               {
                    *p_LF >> tmp_State.I;
                    //*cout << " St " << tmp_State.I;
                    
                    *p_LF >> tmp_NID.I;
                    //*cout << " NID " << tmp_NID.I;
                    
                    tmp_Node = get_State_Node(tmp_State);
                    
                    tmp_Scaffold.search(tmp_NID);
                    tmp_Scaffold.set_Current_Node_NID(tmp_Node);
                    
               }
               if (tmp_Node_Type == "N")
               {
                    *p_LF >> tmp_NID.I;
                    //*cout << " NID " << tmp_NID.I;
                    
                    *p_LF >> tmp_L.I;
                    //*cout << " L " << tmp_L.I;
                    
                    *p_LF >> tmp_R.I;
                    //*cout << " R " << tmp_R.I;
                    
                    //Create the node and add it to the state tree with the NID as an identifier.
                    tmp_Node = new_Node();
                    tmp_Scaffold.search(tmp_NID);
                    tmp_Scaffold.set_Current_Node_NID(tmp_Node);
                    
                    //Gather the dendrite references from the tree.
                    tmp_Scaffold.search(tmp_L);
                    tmp_D_L = tmp_Scaffold.get_Current_Node_NID();
                    
                    //*cout << "  D_L->" << tmp_D_L << " L->" << tmp_L.I << " " << tmp_Scaffold.flg_Foundit;
                    
                    tmp_Scaffold.search(tmp_R);
                    tmp_D_R = tmp_Scaffold.get_Current_Node_NID();
                    
                    //*cout << "  D_R->" << tmp_D_R << " R->" << tmp_R.I << " " << tmp_Scaffold.flg_Foundit;
                    
                    //Create the lower connections.
                    create_Connection_L(tmp_D_L, tmp_Node);
                    create_Connection_R(tmp_D_R, tmp_Node);
                    
                    
                    //*cout << " tmp_Node->NID->" << tmp_Node->NID.I;
               }
                              
               if (tmp_Node_Type == "STN")
               {
                    *p_LF >> tmp_State.I;
                    //*cout << " St " << tmp_State.I;
                    
                    *p_LF >> tmp_NID.I;
                    //*cout << " NID " << tmp_NID.I;
                    
                    tmp_Node = get_State_Node(tmp_State);
                    convert_To_Treetop_Node(tmp_Node);
                    
                    tmp_Scaffold.search(tmp_NID);
                    tmp_Scaffold.set_Current_Node_NID(tmp_Node);
                    
               }
               if (tmp_Node_Type == "TN")
               {
                    *p_LF >> tmp_NID.I;
                    //*cout << " NID " << tmp_NID.I;
                    
                    *p_LF >> tmp_L.I;
                    //*cout << " L " << tmp_L.I;
                    
                    *p_LF >> tmp_R.I;
                    //*cout << " R " << tmp_R.I;
                    
                    //Create the node and add it to the state tree with the NID as an identifier.
                    tmp_Node = new_Node();
                    tmp_Scaffold.search(tmp_NID);
                    tmp_Scaffold.set_Current_Node_NID(tmp_Node);
                    convert_To_Treetop_Node(tmp_Node);
                    
                    //Gather the dendrite references from the tree.
                    tmp_Scaffold.search(tmp_L);
                    tmp_D_L = tmp_Scaffold.get_Current_Node_NID();
                    
                    //*cout << "  D_L->" << tmp_D_L << " L->" << tmp_L.I << " " << tmp_Scaffold.flg_Foundit;
                    
                    tmp_Scaffold.search(tmp_R);
                    tmp_D_R = tmp_Scaffold.get_Current_Node_NID();
                    
                    //*cout << "  D_R->" << tmp_D_R << " R->" << tmp_R.I << " " << tmp_Scaffold.flg_Foundit;
                    
                    //Create the lower connections.
                    create_Connection_L(tmp_D_L, tmp_Node);
                    create_Connection_R(tmp_D_R, tmp_Node);
                    
                    tmp_Node->set_Type(3);
                    
                    //*cout << " tmp_Node->NID->" << tmp_Node->NID.I;
               }
               
               *p_LF >> tmp_RC_Lvl.F;
               //*cout << " tmp_RC_Lvl " << tmp_RC_Lvl.F;
               
               tmp_Node->RC_Lvl = tmp_RC_Lvl.F;
               
               //*cout << " " << tmp_Node << " ";
               //tmp_Node->bp_O();
          }
          xy(tmp_X, tmp_Y); cout << "100.00%      ";
          cout << "\n Resetting Scaffold.....";
          tmp_Scaffold.reset();
          cout << "Done\n";
     }
     
     
     //Loads the node network, new faster method, dendrites only.
     void load(ifstream * p_LF)
     {
          //Create the file.
          cout << "\n\n Node Network Loading..........";
          if (p_LF->is_open()){ cout << "Authorized"; } else { cout << "Denied"; return; }
          
          u_Data_3 Node_Count;
          Node_Count.U = 0;
          
          //The new scaffold.
          c_NT3_Base_Node_1D ** tmp_NScaffold = NULL;
          
          c_NT3_Base_Node_1D * tmp_Node = NULL;
          
          string tmp_Node_Type = "";
          
          u_Data_3 tmp_State;
          tmp_State.I = 0;
          
          u_Data_3 tmp_NID;
          tmp_NID.I = 0;
          
          u_Data_3 tmp_L;
          tmp_L.I = 0;
          u_Data_3 tmp_R;
          tmp_R.I = 0;
          
          u_Data_3 tmp_RC_Lvl;
          tmp_RC_Lvl.F = 0.0f;
          
          int tmp_Axon_Count_L = 0;
          int tmp_Axon_Count_R = 0;
          
          *p_LF >> tmp_Node_Type;
          tmp_Node_Type = "";
          *p_LF >> Node_Count.U;
          cout << "\n " << Node_Count.U << " Nodes Found..........";
          
          //Setup the NScaffold
          if (Node_Count.U == 0){ return; }
          tmp_NScaffold = new c_NT3_Base_Node_1D*[unsigned int (Node_Count.U)];
          for (unsigned long long int cou_Index=0;cou_Index<Node_Count.U;cou_Index++)
          {
               tmp_NScaffold[cou_Index] = NULL;
          }
          
          int tmp_X = get_Console_Cursor_X();
          int tmp_Y = get_Console_Cursor_Y();
          u_Data_3 Node_Counter;
          Node_Counter.U = 1;
          int tmp_Div = int(Node_Count.I) / 1000;
          if (tmp_Div == 0){ tmp_Div = 1; }
          
          while(!(*p_LF).eof())
          {
               if (!(Node_Counter.U % tmp_Div))
               { 
                    xy(tmp_X, tmp_Y); 
                    cout << ((double (Node_Counter.U) / double (Node_Count.U)) * 100) << "%      "; 
               }
               Node_Counter.U++;
               
               *p_LF >> tmp_Node_Type;
               //*cout << "\n\n" << tmp_Node_Type;
               if (tmp_Node_Type == "SN")
               {
                    *p_LF >> tmp_State.I;
                    //*cout << " St " << tmp_State.I;
                    
                    *p_LF >> tmp_NID.I;
                    //*cout << " NID " << tmp_NID.I;
                    
                    //Get the axon counts.
                    *p_LF >> tmp_Axon_Count_L;
                    *p_LF >> tmp_Axon_Count_R;
                    
                    tmp_Node = get_State_Node(tmp_State, tmp_Axon_Count_L, tmp_Axon_Count_R);
                    
                    //Add the tmp_Node to the tmp_NScaffold
                    tmp_NScaffold[tmp_NID.I] = tmp_Node;
                    
               }
               if (tmp_Node_Type == "N")
               {
                    *p_LF >> tmp_NID.I;
                    //*cout << " NID " << tmp_NID.I;
                    
                    //Get the axon counts.
                    *p_LF >> tmp_Axon_Count_L;
                    *p_LF >> tmp_Axon_Count_R;
                    
                    *p_LF >> tmp_L.I;
                    //*cout << " L " << tmp_L.I;
                    
                    *p_LF >> tmp_R.I;
                    //*cout << " R " << tmp_R.I;
                    
                    //Create the node and add it to the state tree with the NID as an identifier.
                    tmp_Node = new_Node(tmp_Axon_Count_L, tmp_Axon_Count_R);
                    
                    //Add the tmp_Node to the tmp_NScaffold
                    tmp_NScaffold[tmp_NID.I] = tmp_Node;
                    
                    //Create the lower connections.
                    create_Predefined_Connection_L(tmp_NScaffold[tmp_L.I], tmp_Node);
                    create_Predefined_Connection_R(tmp_NScaffold[tmp_R.I], tmp_Node);
               }
                              
               if (tmp_Node_Type == "STN")
               {
                    *p_LF >> tmp_State.I;
                    //*cout << " St " << tmp_State.I;
                    
                    *p_LF >> tmp_NID.I;
                    //*cout << " NID " << tmp_NID.I;
                    
                    //Get the axon counts.
                    *p_LF >> tmp_Axon_Count_L;
                    *p_LF >> tmp_Axon_Count_R;
                    
                    tmp_Node = get_State_Node(tmp_State, tmp_Axon_Count_L, tmp_Axon_Count_R);
                    convert_To_Treetop_Node(tmp_Node);
                    
                    //Add the tmp_Node to the tmp_NScaffold
                    tmp_NScaffold[tmp_NID.I] = tmp_Node;
                    
               }
               if (tmp_Node_Type == "TN")
               {
                    *p_LF >> tmp_NID.I;
                    //*cout << " NID " << tmp_NID.I;
                    
                    //Get the axon counts.
                    *p_LF >> tmp_Axon_Count_L;
                    *p_LF >> tmp_Axon_Count_R;
                    
                    *p_LF >> tmp_L.I;
                    //*cout << " L " << tmp_L.I;
                    
                    *p_LF >> tmp_R.I;
                    //*cout << " R " << tmp_R.I;
                    
                    //Create the node and add it to the state tree with the NID as an identifier.
                    tmp_Node = new_Node(tmp_Axon_Count_L, tmp_Axon_Count_R);
                    convert_To_Treetop_Node(tmp_Node);
                    
                    //Add the tmp_Node to the tmp_NScaffold
                    tmp_NScaffold[tmp_NID.I] = tmp_Node;
                    
                    //Create the lower connections.
                    create_Predefined_Connection_L(tmp_NScaffold[tmp_L.I], tmp_Node);
                    create_Predefined_Connection_R(tmp_NScaffold[tmp_R.I], tmp_Node);
                    
                    tmp_Node->set_Type(3);
                    
                    //*cout << " tmp_Node->NID->" << tmp_Node->NID.I;
               }
               
               *p_LF >> tmp_RC_Lvl.F;
               //*cout << " tmp_RC_Lvl " << tmp_RC_Lvl.F;
               
               tmp_Node->RC_Lvl = tmp_RC_Lvl.F;
               
               //*cout << " " << tmp_Node << " ";
               //tmp_Node->bp_O();
          }
          xy(tmp_X, tmp_Y); cout << "100.00%      ";
          cout << "\n Resetting Scaffold.....";
          
          //Derete the tmp_NScaffold
          for (unsigned long long int cou_Index=0;cou_Index<Node_Count.U;cou_Index++)
          {
               /*
               cout << "\n ["; oint(0, 13, cou_Index); cout << "] ";
               if (tmp_NScaffold[cou_Index] != NULL)
               { 
                    oint(0, 8, tmp_NScaffold[cou_Index]->NID.U);
                    cout << " ";
                    tmp_NScaffold[cou_Index]->bp_O(); 
                    
                    if (tmp_NScaffold[cou_Index]->Dendrite_L != NULL)
                    {
                         cout << " ";
                         tmp_NScaffold[cou_Index]->Dendrite_L->bp_O();
                    }
                    if (tmp_NScaffold[cou_Index]->Dendrite_R != NULL)
                    {
                         cout << " ";
                         tmp_NScaffold[cou_Index]->Dendrite_R->bp_O();
                    }
               }
               */
               tmp_NScaffold[cou_Index] = NULL;
          }
          delete [] tmp_NScaffold;
          
          cout << "Done\n";
     }
     

     ////==---------------------+
     //==--   OUTPUT FUNCTIONS
     ////==---------------------+
     
     void output_Treetops()
     {
          Treetops_Tree.output_Tree();
     }
     
     //Outputs the treetops as backpropagated nodes.
     void output_Treetops_BP()
     {
          Treetops_Tree.output_Tree_BP();
     }
     
     
     //Oututs all of the nodes.
     void output_Nodes()
     {
          c_NT3_Base_Node_1D * tmp_LL = Root;
          //Root = NULL;
          //Root = NULL;
          c_NT3_Base_Node_1D * tmp_LL_Next = NULL;
          
          while (tmp_LL != NULL)
          {
               tmp_LL_Next = tmp_LL->Next;
               cout << "\n  ";
               tmp_LL->bp_O();
               tmp_LL = tmp_LL_Next;
          }
     }
};





















































