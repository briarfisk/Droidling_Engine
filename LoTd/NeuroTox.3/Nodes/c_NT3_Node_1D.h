//The basic polymorphic node to use for the node network.
class c_NT3_Base_Node_1D
{
public:
     
     virtual ~c_NT3_Base_Node_1D(){};
     
     //The ID of this node.
     u_Data_3 NID;
     
     //The next node in the Linked List.
     c_NT3_Base_Node_1D * Next;
     
     //Axons and their counters.
     c_NT3_Base_Node_1D ** Axons_L;
     int Axon_Count_L;
     
     c_NT3_Base_Node_1D ** Axons_R;
     int Axon_Count_R;
     
     //Dendrites.
     c_NT3_Base_Node_1D * Dendrite_L;
     c_NT3_Base_Node_1D * Dendrite_R;
     
     //Reinforcement Counters.
     double RC_Lvl;
     
     //Whether or not the node is a state node.
     char Type;
     
     //This is used for things such as the buffer trees or anything that relies on references.
     void * Ref;
     
     //Member Functions.
     virtual void add_Axon_L(c_NT3_Base_Node_1D * p_Axon)=0; //Adds an axon on the left leg.
     virtual void add_Axon_R(c_NT3_Base_Node_1D * p_Axon)=0; //Adds an axon on the right leg.
     
     virtual void add_Predefined_Axon_L(c_NT3_Base_Node_1D * p_Axon)=0; //Adds an axon on the left leg.
     virtual void add_Predefined_Axon_R(c_NT3_Base_Node_1D * p_Axon)=0; //Adds an axon on the right leg.
     
     virtual void expand_Axon_L(int p_Count)=0; //Adds an axon on the left leg.
     virtual void expand_Axon_R(int p_Count)=0; //Adds an axon on the right leg.
     
     virtual c_NT3_Base_Node_1D * does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_1D * p_Node)=0;//Searches the axons to see if an upper tier connection exists on the left leg.
     
     virtual bool does_Lower_Connection_Exist_LL(c_NT3_Base_Node_1D * p_Node)=0; //Checks if the given node matches a dendrite on the left leg.
     virtual bool does_Lower_Connection_Exist_LR(c_NT3_Base_Node_1D * p_Node)=0; //Checks if the given node matches a dendrite on the right leg.
     
     virtual void set_Dendrite_L(c_NT3_Base_Node_1D * p_Dendrite)=0; //Sets the left dendrite to the given dendrite.
     virtual void set_Dendrite_R(c_NT3_Base_Node_1D * p_Dendrite)=0; //Sets the right dendrite to the given dendrite.
     
     virtual void reinforce()=0; //Reinforces the node.
     virtual double get_RC_Score()=0; //Returns the Reinforcement score of the node. double (RC_Lvl + (RC_XP / RC_Lvl))
     virtual double get_RC_Lvl()=0; //Returns the current reinforcement Lvl.
     
     virtual void set_State(u_Data_3 p_State)=0; //Sets the state if the node is a state node, otherwise it does nothing.
     virtual long long int get_State()=0; //Returns the state if it is a state node otherwise it does nothing.
     
     virtual int get_Type()=0; //Returns the type of node, 0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
     virtual void set_Type(int p_Type)=0; //Sets the type of node.
     
     virtual void bp_O()=0; //Initiates a backpropagation that outputs to the given table with the given index.
     virtual void bp_L()=0; //bp_Output the left node.
     virtual void bp_R()=0; //bp_Output the right node.
};

//The normal node to use.
class c_NT3_Node_1D: public c_NT3_Base_Node_1D
{
public:
     
     c_NT3_Node_1D()
     {
          //The next node.
          Next = NULL;
          
          //Axons and their counters.
          Axons_L = NULL;
          Axon_Count_L = 0;
          
          Axons_R = NULL;
          Axon_Count_R = 0;
          
          //Dendrites.
          Dendrite_L = NULL;
          Dendrite_R = NULL;
          
          //Reinforcement Counters.
          RC_Lvl = 1.0;
               
          //The node type.
          Type = 1;
          
          Ref = NULL;
     }
     
     ~c_NT3_Node_1D()
     {
          /*
          cout << "\n   ~c_NT3_Node_1D " << this << " NID->" << NID.U; cout.flush();
          cout << " Axons_L->" << Axons_L; cout.flush();
          cout << " Axon_Count_L->" << Axon_Count_L; cout.flush();
          cout << " Axons_R->" << Axons_R; cout.flush();
          cout << " Axon_Count_R->" << Axon_Count_R; cout.flush();
          cout << " Next->" << Next; cout.flush();
          */
          
          if (Axons_L != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count_L;cou_Index++)
               {
                    Axons_L[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons_L;
               Axons_L = NULL;
          }
          
          if (Axons_R != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count_R;cou_Index++)
               {
                    Axons_R[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons_R;
               Axons_R = NULL;
          }
          
          Dendrite_L = NULL;
          Dendrite_R = NULL;
          
          //cout << " delete Next->" << Next; cout.flush();
          //delete Next; //This one.
          Next = NULL;
          
          Ref = NULL;
     }
     
     
     //==--  Member Functions.
     
     //Adds an axon on the left leg.
     void add_Axon_L(c_NT3_Base_Node_1D * p_Axon)
     {
          c_NT3_Base_Node_1D ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_1D*[Axon_Count_L];
          for (int cou_A=0;cou_A<Axon_Count_L;cou_A++)
          {
               tmp_Axons[cou_A] = Axons_L[cou_A];
               Axons_L[cou_A] = NULL;
          }
          
          delete [] Axons_L;
          Axons_L = NULL;
          
          Axons_L = new c_NT3_Base_Node_1D*[Axon_Count_L + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count_L;cou_A++)
          {
               Axons_L[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons_L[Axon_Count_L] = p_Axon; 
          Axon_Count_L++; 
     }
     
     //Adds an axon on the right leg.
     void add_Axon_R(c_NT3_Base_Node_1D * p_Axon)
     {
          c_NT3_Base_Node_1D ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_1D*[Axon_Count_R];
          for (int cou_A=0;cou_A<Axon_Count_R;cou_A++)
          {
               tmp_Axons[cou_A] = Axons_R[cou_A];
               Axons_R[cou_A] = NULL;
          }
          
          delete [] Axons_R;
          Axons_R = NULL;
          
          Axons_R = new c_NT3_Base_Node_1D*[Axon_Count_R + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count_R;cou_A++)
          {
               Axons_R[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons_R[Axon_Count_R] = p_Axon; 
          Axon_Count_R++; 
     }
     
     //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
     void add_Predefined_Axon_L(c_NT3_Base_Node_1D * p_Axon)
     {
          Axons_L[Axon_Count_L] = p_Axon; 
          Axon_Count_L++;
     }
     
     void add_Predefined_Axon_R(c_NT3_Base_Node_1D * p_Axon)
     {
          Axons_R[Axon_Count_R] = p_Axon; 
          Axon_Count_R++;
     }
     
     //Destroys the current axons if any and expands the array to the given depth.
     void expand_Axon_L(int p_Count)
     {
          if (Axons_L != NULL){ delete [] Axons_L; }
          Axons_L = NULL;
          
          Axons_L = new c_NT3_Base_Node_1D*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons_L[cou_A] = NULL;
          }
           
          Axon_Count_L = 0; 
     }
     
     void expand_Axon_R(int p_Count)
     {
          if (Axons_R != NULL){ delete [] Axons_R; }
          Axons_R = NULL;
          
          Axons_R = new c_NT3_Base_Node_1D*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons_R[cou_A] = NULL;
          }
           
          Axon_Count_R = 0; 
     }
     
     //Searches the axons to see if an upper tier connection exists.
     c_NT3_Base_Node_1D * does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_1D * p_Node)
     {
          //Check the axons on the right side for the 
          for (int cou_A=0;cou_A<Axon_Count_L;cou_A++)
          {
               if (Axons_L[cou_A]->does_Lower_Connection_Exist_LR(p_Node))
               {
                    return Axons_L[cou_A];
               }
          }
          return NULL; 
     }
     
     //Checks if the given node matches a dendrite on the left leg.
     bool does_Lower_Connection_Exist_LL(c_NT3_Base_Node_1D * p_Node)
     { 
          if (Dendrite_L == p_Node){ return 1; }
          return 0; 
     }
     
     //Checks if the given node matches a dendrite on the right leg.
     bool does_Lower_Connection_Exist_LR(c_NT3_Base_Node_1D * p_Node)
     { 
          if (Dendrite_R == p_Node){ return 1; }
          return 0; 
     }
     
     //Sets the left dendrite to the given dendrite.
     void set_Dendrite_L(c_NT3_Base_Node_1D * p_Dendrite)
     {
          Dendrite_L = p_Dendrite;
     }
     
     //Sets the right dendrite to the given dendrite.
     void set_Dendrite_R(c_NT3_Base_Node_1D * p_Dendrite)
     {
          Dendrite_R = p_Dendrite;
     }
     
     //Reinforces the node.
     void reinforce()
     {
          RC_Lvl += double (1.0f / int (RC_Lvl));
     }
     
     //Returns the Reinforcement score of the node. double (RC_Lvl + (RC_XP / RC_Lvl))
     double get_RC_Score(){ return RC_Lvl; }
     
     //Returns the current reinforcement Lvl.
     double get_RC_Lvl(){ return RC_Lvl; }
     
     //Sets the state if the node is a state node, otherwise it does nothing.
     void set_State(u_Data_3 p_State){}
     
     //Returns the state if it is a state node otherwise it does nothing.
     long long int get_State(){ return 0; }
     
     //Returns the type of node, 0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
     int get_Type(){ return Type; }
     
     //Sets the type of node.
     void set_Type(int p_Type){ Type = p_Type; }
     
     //Initiates a backpropagation that outputs to the given table with the given index.
     void bp_O() 
     {
          cout << "<";
          if (Dendrite_L != NULL){ Dendrite_L->bp_L(); } else { cout << "(LNULL)"; }
          if (Dendrite_R != NULL){ Dendrite_R->bp_R(); } else { cout << "(RNULL)"; }
          cout << ">";
     }
     
     //bp_Output the left node.
     void bp_L()
     {
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (Dendrite_L != NULL)
          {
               if (Dendrite_L != NULL){ Dendrite_L->bp_L(); }
               if (Dendrite_R != NULL){ Dendrite_R->bp_R(); }
          }
          else
          {
                    char tmp_State = char (get_State());
                    if (tmp_State == char (0)){ tmp_State = '0'; }
                    if (tmp_State == char (7)){ tmp_State = '7'; }
                    if (tmp_State == char (8)){ tmp_State = '8'; }
                    if (tmp_State == char (9)){ tmp_State = '9'; }
                    if (tmp_State == char (10)){ tmp_State = 'a'; }
                    if (tmp_State == char (13)){ tmp_State = 'd'; }
                    cout << tmp_State;
          }
     }
     
     //bp_Output the right node.
     void bp_R()
     {
          //If a right leg exists then initiate a backpropagation.
          if (Dendrite_R != NULL)
          {
               if (Dendrite_R != NULL){ Dendrite_R->bp_R(); }
          }
          else
          {
                    char tmp_State = char (get_State());
                    if (tmp_State == char (0)){ tmp_State = '0'; }
                    if (tmp_State == char (7)){ tmp_State = '7'; }
                    if (tmp_State == char (8)){ tmp_State = '8'; }
                    if (tmp_State == char (9)){ tmp_State = '9'; }
                    if (tmp_State == char (10)){ tmp_State = 'a'; }
                    if (tmp_State == char (13)){ tmp_State = 'd'; }
                    cout << tmp_State;
          }
     }
};

//State node.
class c_NT3_State_Node_1D: public c_NT3_Base_Node_1D
{
private:
     
public:
     
     //The state.
     u_Data_3 State;
     
     c_NT3_State_Node_1D()
     {
          State.I = 0;
          
          //The linked list.
          Next = NULL;
          
          //Axons and their counters.
          Axons_L = NULL;
          Axon_Count_L = 0;
          
          Axons_R = NULL;
          Axon_Count_R = 0;
          
          //Dendrites.
          Dendrite_L = NULL;
          Dendrite_R = NULL;
          
          //Reinforcement Counters.
          RC_Lvl = 1.0;
          
          //The nodes type.
          Type = 0;
          
          Ref = NULL;
     }
     
     ~c_NT3_State_Node_1D()
     {
          //cout << " ~c_NT3_State_Node_1D " << this << " NID->" << NID.U; cout.flush();
          
          if (Axons_L != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count_L;cou_Index++)
               {
                    Axons_L[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons_L;
               Axons_L = NULL;
          }
          
          if (Axons_R != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count_R;cou_Index++)
               {
                    Axons_R[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons_R;
               Axons_R = NULL;
          }
          
          Dendrite_L = NULL;
          Dendrite_R = NULL;
          
          //cout << " delete Next->" << Next; cout.flush();
          
          delete Next; //This one.
          
          //cout << "   ~~~c_NT3_State_Node_1D " << this << " NID->" << NID.U; cout.flush();
          
          Ref = NULL;
     }
     
     
     //==--  Member Functions.
     
     //Adds an axon on the left leg.
     void add_Axon_L(c_NT3_Base_Node_1D * p_Axon)
     {
          c_NT3_Base_Node_1D ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_1D*[Axon_Count_L];
          for (int cou_A=0;cou_A<Axon_Count_L;cou_A++)
          {
               tmp_Axons[cou_A] = Axons_L[cou_A];
               Axons_L[cou_A] = NULL;
          }
          
          delete [] Axons_L;
          Axons_L = NULL;
          
          Axons_L = new c_NT3_Base_Node_1D*[Axon_Count_L + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count_L;cou_A++)
          {
               Axons_L[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons_L[Axon_Count_L] = p_Axon; 
          Axon_Count_L++; 
     }
     
     //Adds an axon on the right leg.
     void add_Axon_R(c_NT3_Base_Node_1D * p_Axon)
     {
          c_NT3_Base_Node_1D ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_1D*[Axon_Count_R];
          for (int cou_A=0;cou_A<Axon_Count_R;cou_A++)
          {
               tmp_Axons[cou_A] = Axons_R[cou_A];
               Axons_R[cou_A] = NULL;
          }
          
          delete [] Axons_R;
          Axons_R = NULL;
          
          Axons_R = new c_NT3_Base_Node_1D*[Axon_Count_R + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count_R;cou_A++)
          {
               Axons_R[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons_R[Axon_Count_R] = p_Axon; 
          Axon_Count_R++; 
     }
     
     //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
     void add_Predefined_Axon_L(c_NT3_Base_Node_1D * p_Axon)
     {
          Axons_L[Axon_Count_L] = p_Axon; 
          Axon_Count_L++;
     }
     
     void add_Predefined_Axon_R(c_NT3_Base_Node_1D * p_Axon)
     {
          Axons_R[Axon_Count_R] = p_Axon; 
          Axon_Count_R++;
     }
     
     //Destroys the current axons if any and expands the array to the given depth.
     void expand_Axon_L(int p_Count)
     {
          if (Axons_L != NULL){ delete [] Axons_L; }
          Axons_L = NULL;
          
          Axons_L = new c_NT3_Base_Node_1D*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons_L[cou_A] = NULL;
          }
           
          Axon_Count_L = 0; 
     }
     
     void expand_Axon_R(int p_Count)
     {
          if (Axons_R != NULL){ delete [] Axons_R; }
          Axons_R = NULL;
          
          Axons_R = new c_NT3_Base_Node_1D*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons_R[cou_A] = NULL;
          }
           
          Axon_Count_R = 0; 
     }
     //Searches the axons to see if an upper tier connection exists.
     c_NT3_Base_Node_1D * does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_1D * p_Node)
     {
          //Check the axons on the right side for the 
          for (int cou_A=0;cou_A<Axon_Count_L;cou_A++)
          {
               if (Axons_L[cou_A]->does_Lower_Connection_Exist_LR(p_Node))
               {
                    return Axons_L[cou_A];
               }
          }
          return NULL; 
     }
     
     //Checks if the given node matches a dendrite on the left leg.
     bool does_Lower_Connection_Exist_LL(c_NT3_Base_Node_1D * p_Node)
     { 
          if (Dendrite_L == p_Node){ return 1; }
          return 0; 
     }
     
     //Checks if the given node matches a dendrite on the right leg.
     bool does_Lower_Connection_Exist_LR(c_NT3_Base_Node_1D * p_Node)
     { 
          if (Dendrite_R == p_Node){ return 1; }
          return 0; 
     }
     
     //Sets the left dendrite to the given dendrite.
     void set_Dendrite_L(c_NT3_Base_Node_1D * p_Dendrite)
     {
          Dendrite_L = p_Dendrite;
     }
     
     //Sets the right dendrite to the given dendrite.
     void set_Dendrite_R(c_NT3_Base_Node_1D * p_Dendrite)
     {
          Dendrite_R = p_Dendrite;
     }
     
     //Reinforces the node.
     void reinforce()
     {
          RC_Lvl += double (1.0f / int (RC_Lvl));
     }
     
     //Returns the Reinforcement score of the node. double (RC_Lvl + (RC_XP / RC_Lvl))
     double get_RC_Score(){ return RC_Lvl; }
     
     //Returns the current reinforcement Lvl.
     double get_RC_Lvl(){ return RC_Lvl; }
     
     //Sets the state if the node is a state node, otherwise it does nothing.
     void set_State(u_Data_3 p_State){ State = p_State; }
     
     //Returns the state if it is a state node otherwise it does nothing.
     long long int get_State(){ return State.I; }
     
     //Returns the type of node, 0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
     int get_Type(){ return Type; }
     
     //Sets the type of node.
     void set_Type(int p_Type){ Type = p_Type; }
     
     //Initiates a backpropagation that outputs to the given table with the given index.
     void bp_O() 
     {
          cout << "~<";
          cout << State.C;
          cout << ">~";
     }
     
     //bp_Output the left node.
     void bp_L()
     {
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (Dendrite_L != NULL)
          {
               if (Dendrite_L != NULL){ Dendrite_L->bp_L(); }
               if (Dendrite_R != NULL){ Dendrite_R->bp_R(); }
          }
          else
          {
               char tmp_State = char (get_State());
               if (tmp_State == char (0)){ tmp_State = '?'; }
               if (tmp_State == char (7)){ tmp_State = '?'; }
               if (tmp_State == char (8)){ tmp_State = '?'; }
               if (tmp_State == char (9)){ tmp_State = '?'; }
               if (tmp_State == char (10)){ tmp_State = '?'; }
               if (tmp_State == char (13)){ tmp_State = '?'; }
               cout << tmp_State;
          }
     }
     
     //bp_Output the right node.
     void bp_R()
     {
          //If a right leg exists then initiate a backpropagation.
          if (Dendrite_R != NULL)
          {
               if (Dendrite_R != NULL){ Dendrite_R->bp_R(); }
          }
          else
          {
               char tmp_State = char (get_State());
               if (tmp_State == char (0)){ tmp_State = '?'; }
               if (tmp_State == char (7)){ tmp_State = '?'; }
               if (tmp_State == char (8)){ tmp_State = '?'; }
               if (tmp_State == char (9)){ tmp_State = '?'; }
               if (tmp_State == char (10)){ tmp_State = '?'; }
               if (tmp_State == char (13)){ tmp_State = '?'; }
               cout << tmp_State;
          }
     }
};
























