























class c_NT3_Base_Node_MMap_2D
{
public:
     
     virtual ~c_NT3_Base_Node_MMap_2D(){};
     
     //The ID of this node.
     u_Data_3 NID;
     
     //The next node in the Linked List.
     c_NT3_Base_Node_MMap_2D* Next;
     
     //Axons and their counters.
     c_NT3_Base_Node_MMap_2D** Axons[4];
     int Axon_Count[4];
     
     //Dendrites.
     c_NT3_Base_Node_MMap_2D * Dendrite[4];
     
     //Reinforcement Counters.
     double RC_Lvl;
     
     //Whether or not the node is a state node.
     char Type;
     
     //This is used for things such as the buffer trees or anything that relies on references.
     void * Ref;
     
     //Member Functions.
     virtual void add_Axon(c_NT3_Base_Node_MMap_2D * p_Axon, int p_Axon_Ind)=0; //Adds an axon on the 0 leg.
     
     virtual void add_Predefined_Axon(c_NT3_Base_Node_MMap_2D * p_Axon, int p_Axon_Ind)=0; //Adds an axon on the 0 leg.
     
     virtual void expand_Axon(int p_Count, int p_Axon_Ind)=0; //Adds an axon on the 0 leg.
     
     virtual c_NT3_Base_Node_MMap_2D * does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_MMap_2D * p_Nodes[3])=0;//Searches the axons to see if an upper tier connection exists on the left leg.
     
     virtual bool does_Lower_Connection_Exist(c_NT3_Base_Node_MMap_2D * p_Nodes[3])=0; //Checks if the given node matches a dendrite on the last 3 legs. (1, 2, 3)
     
     virtual void set_Dendrite(c_NT3_Base_Node_MMap_2D * p_Dendrite, int p_Dendrite_Ind)=0; //Sets the 0 dendrite to the given dendrite.
     
     virtual void reinforce()=0; //Reinforces the node.
     virtual double get_RC_Score()=0; //Returns the Reinforcement score of the node. double (RC_Lvl + (RC_XP / RC_Lvl))
     virtual double get_RC_Lvl()=0; //Returns the current reinforcement Lvl.
     
	 virtual void set_State(u_Data_3 p_State)=0; //Sets the state if the node is a state node, otherwise it does nothing.
     virtual unsigned long long int get_State()=0; //Returns the state if it is a state node otherwise it does nothing.
     
     virtual int get_Type()=0; //Returns the type of node, 0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
     virtual void set_Type(int p_Type)=0; //Sets the type of node.
     
     virtual void bp_O()=0; //Initiates a backpropagation that outputs to the given table with the given index.
     virtual void bp_0()=0; //bp_Output the left node.
     virtual void bp_1()=0; //bp_Output the left node.
     virtual void bp_2()=0; //bp_Output the left node.
     virtual void bp_3()=0; //bp_Output the left node.
};

//The normal node to use.
class c_NT3_Node_MMap_2D: public c_NT3_Base_Node_MMap_2D 
{
public:
     
     c_NT3_Node_MMap_2D()
     {
          //The next node.
          Next = NULL;
          
          //Axons and their counters.
          Axons[0] = NULL;
          Axons[1] = NULL;
          Axons[2] = NULL;
          Axons[3] = NULL;
          Axon_Count[0] = 0;
          Axon_Count[1] = 0;
          Axon_Count[2] = 0;
          Axon_Count[3] = 0;
          
          //Dendrites.
          Dendrite[0] = NULL;
          Dendrite[1] = NULL;
          Dendrite[2] = NULL;
          Dendrite[3] = NULL;
          
          //Reinforcement Counters.
          RC_Lvl = 1.0;
               
          //The node type.
          Type = 1;
          
          Ref = NULL;
     }
     
     ~c_NT3_Node_MMap_2D()
     {
          /*
          cout << "\n   ~c_NT3_Node_MMap_2D " << this << " NID->" << NID.U; cout.flush();
          cout << " Axons_L->" << Axons_L; cout.flush();
          cout << " Axon_Count_L->" << Axon_Count_L; cout.flush();
          cout << " Axons_R->" << Axons_R; cout.flush();
          cout << " Axon_Count_R->" << Axon_Count_R; cout.flush();
          cout << " Next->" << Next; cout.flush();
          */
          
		 for (int cou_L = 0; cou_L < 4; cou_L++)
		 {
			 if (Axons[cou_L] != NULL)
			 {
				 for (int cou_Index = 0; cou_Index < Axon_Count[cou_L]; cou_Index++)
				 {
					 Axons[cou_L][cou_Index] = NULL; //Node destruction is handled by the linked list.
				 }
				 delete[] Axons[cou_L];
				 Axons[cou_L] = NULL;
			 }

			 Dendrite[cou_L] = NULL;
		 }
          
          //cout << " delete Next->" << Next; cout.flush();
          //delete Next; //This one.
          Next = NULL;
          
          Ref = NULL;
     }
     
     
     //==--  Member Functions.
     
     //Adds an axon on the left leg.
     void add_Axon(c_NT3_Base_Node_MMap_2D  * p_Axon, int p_Axon_Ind)
     {
          c_NT3_Base_Node_MMap_2D ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_MMap_2D*[Axon_Count[p_Axon_Ind]];
          for (int cou_A=0;cou_A<Axon_Count[p_Axon_Ind];cou_A++)
          {
               tmp_Axons[cou_A] = Axons[p_Axon_Ind][cou_A];
               Axons[p_Axon_Ind][cou_A] = NULL;
          }
          
          delete [] Axons[p_Axon_Ind];
          Axons[p_Axon_Ind] = NULL;
          
          Axons[p_Axon_Ind] = new c_NT3_Base_Node_MMap_2D*[Axon_Count[p_Axon_Ind] + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count[p_Axon_Ind];cou_A++)
          {
               Axons[p_Axon_Ind][cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons[p_Axon_Ind][Axon_Count[p_Axon_Ind]] = p_Axon; 
          Axon_Count[p_Axon_Ind]++; 
     }
     
     //f11 {BACKSPACE}{BACKSPACE}[p_Axon_Ind]
     //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
     void add_Predefined_Axon(c_NT3_Base_Node_MMap_2D* p_Axon, int p_Axon_Ind)
     {
		 Axons[p_Axon_Ind][Axon_Count[p_Axon_Ind]] = p_Axon; 
          Axon_Count[p_Axon_Ind]++;
     }
     
     //Destroys the current axons if any and expands the array to the given depth.
     void expand_Axon(int p_Count, int p_Axon_Ind)
     {
          if (Axons[p_Axon_Ind] != NULL){ delete [] Axons[p_Axon_Ind]; }
          Axons[p_Axon_Ind] = NULL;
          
          Axons[p_Axon_Ind] = new c_NT3_Base_Node_MMap_2D*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons[p_Axon_Ind][cou_A] = NULL;
          }
           
          Axon_Count[p_Axon_Ind] = 0; 
     }
     
     //Searches the axons to see if an upper tier connection exists.
     c_NT3_Base_Node_MMap_2D* does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_MMap_2D* p_Nodes[3])
     {
          //Check the axons on the right side for the 
          for (int cou_A=0;cou_A<Axon_Count[0];cou_A++)
          {
               if (Axons[0][cou_A]->does_Lower_Connection_Exist(p_Nodes))
               {
                    return Axons[0][cou_A];
               }
          }
          return NULL; 
     }
     
     //Checks if the given node matches a dendrite on the left leg.
     bool does_Lower_Connection_Exist(c_NT3_Base_Node_MMap_2D* p_Nodes[3])
     { 
		 //The offset might be a bit confusing but we are comparing only the latter 3 legs instead of all 4.
          if ((Dendrite[1] == p_Nodes[0]) && (Dendrite[2] == p_Nodes[1]) && (Dendrite[3] == p_Nodes[2])){ return 1; }
          return 0; 
     }
     
     //Sets the given dendrite to the given dendrite.
     void set_Dendrite(c_NT3_Base_Node_MMap_2D* p_Dendrite, int p_Dendrite_Ind)
     {
          Dendrite[p_Dendrite_Ind] = p_Dendrite;
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
     unsigned long long int get_State(){ return 0; }
     
     //Returns the type of node, 0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
     int get_Type(){ return Type; }
     
     //Sets the type of node.
     void set_Type(int p_Type){ Type = p_Type; }
     
     //Initiates a backpropagation that outputs to the given table with the given index.
     void bp_O() 
     {
          cout << "<";
          if (Dendrite[0] != NULL){ Dendrite[0]->bp_0(); } else { cout << "(LNULL)"; }
          if (Dendrite[1] != NULL){ Dendrite[1]->bp_1(); } else { cout << "(RNULL)"; }
          if (Dendrite[1] != NULL){ Dendrite[2]->bp_2(); } else { cout << "(RNULL)"; }
          if (Dendrite[1] != NULL){ Dendrite[3]->bp_3(); } else { cout << "(RNULL)"; }
          cout << ">";
     }

	 //bp_Output the left node.
	 void bp_0()
	 {
		 //If a left leg exists then initiate a backpropagation along it, then along the right side.
		 if (Dendrite[0] != NULL)
		 {
			 if (Dendrite[0] != NULL) { Dendrite[0]->bp_0(); }
			 if (Dendrite[1] != NULL) { Dendrite[1]->bp_1(); }
			 if (Dendrite[2] != NULL) { Dendrite[1]->bp_2(); }
			 if (Dendrite[3] != NULL) { Dendrite[1]->bp_3(); }
		 }
		 else
		 {
			 char tmp_State = char(get_State());
			 if (tmp_State == char(0)) { tmp_State = '0'; }
			 if (tmp_State == char(7)) { tmp_State = '7'; }
			 if (tmp_State == char(8)) { tmp_State = '8'; }
			 if (tmp_State == char(9)) { tmp_State = '9'; }
			 if (tmp_State == char(10)) { tmp_State = 'a'; }
			 if (tmp_State == char(13)) { tmp_State = 'd'; }
			 cout << tmp_State;
		 }
	 }

	 //bp_Output the left node.
	 void bp_1()
	 {
		 //If a left leg exists then initiate a backpropagation along it, then along the right side.
		 if (Dendrite[0] != NULL)
		 {
			 if (Dendrite[1] != NULL) { Dendrite[1]->bp_1(); }
		 }
		 else
		 {
			 char tmp_State = char(get_State());
			 if (tmp_State == char(0)) { tmp_State = '0'; }
			 if (tmp_State == char(7)) { tmp_State = '7'; }
			 if (tmp_State == char(8)) { tmp_State = '8'; }
			 if (tmp_State == char(9)) { tmp_State = '9'; }
			 if (tmp_State == char(10)) { tmp_State = 'a'; }
			 if (tmp_State == char(13)) { tmp_State = 'd'; }
			 cout << tmp_State;
		 }
	 }

	 //bp_Output the left node.
	 void bp_2()
	 {
		 //If a left leg exists then initiate a backpropagation along it, then along the right side.
		 if (Dendrite[0] != NULL)
		 {
			 if (Dendrite[2] != NULL) { Dendrite[1]->bp_2(); }
		 }
		 else
		 {
			 char tmp_State = char(get_State());
			 if (tmp_State == char(0)) { tmp_State = '0'; }
			 if (tmp_State == char(7)) { tmp_State = '7'; }
			 if (tmp_State == char(8)) { tmp_State = '8'; }
			 if (tmp_State == char(9)) { tmp_State = '9'; }
			 if (tmp_State == char(10)) { tmp_State = 'a'; }
			 if (tmp_State == char(13)) { tmp_State = 'd'; }
			 cout << tmp_State;
		 }
	 }

	 //bp_Output the left node.
	 void bp_3()
	 {
		 //If a left leg exists then initiate a backpropagation along it, then along the right side.
		 if (Dendrite[0] != NULL)
		 {
			 if (Dendrite[3] != NULL) { Dendrite[1]->bp_3(); }
		 }
		 else
		 {
			 char tmp_State = char(get_State());
			 if (tmp_State == char(0)) { tmp_State = '0'; }
			 if (tmp_State == char(7)) { tmp_State = '7'; }
			 if (tmp_State == char(8)) { tmp_State = '8'; }
			 if (tmp_State == char(9)) { tmp_State = '9'; }
			 if (tmp_State == char(10)) { tmp_State = 'a'; }
			 if (tmp_State == char(13)) { tmp_State = 'd'; }
			 cout << tmp_State;
		 }
	 }
     
};

//State node.
class c_NT3_State_Node_MMap_2D: public c_NT3_Base_Node_MMap_2D 
{
private:
     
public:
     
     //The state.
     u_Data_3 State;
     
     c_NT3_State_Node_MMap_2D()
     {
          State.U = 0;
          
          //The linked list.
          Next = NULL;

		  //Axons and their counters.
		  Axons[0] = NULL;
		  Axons[1] = NULL;
		  Axons[2] = NULL;
		  Axons[3] = NULL;
		  Axon_Count[0] = 0;
		  Axon_Count[1] = 0;
		  Axon_Count[2] = 0;
		  Axon_Count[3] = 0;

		  //Dendrites.
		  Dendrite[0] = NULL;
		  Dendrite[1] = NULL;
		  Dendrite[2] = NULL;
		  Dendrite[3] = NULL;
          
          //Reinforcement Counters.
          RC_Lvl = 1.0;
          
          //The nodes type.
          Type = 0;
          
          Ref = NULL;
     }
     
     ~c_NT3_State_Node_MMap_2D()
     {
          //cout << " ~c_NT3_State_Node_MMap_2D " << this << " NID->" << NID.U; cout.flush();


		 for (int cou_L = 0; cou_L < 4; cou_L++)
		 {
			 if (Axons[cou_L] != NULL)
			 {
				 for (int cou_Index = 0; cou_Index < Axon_Count[cou_L]; cou_Index++)
				 {
					 Axons[cou_L][cou_Index] = NULL; //Node destruction is handled by the linked list.
				 }
				 delete[] Axons[cou_L];
				 Axons[cou_L] = NULL;
			 }

			 Dendrite[cou_L] = NULL;
		 }
          
          //cout << " delete Next->" << Next; cout.flush();
          
          delete Next; //This one.
          
          //cout << "   ~~~c_NT3_State_Node_MMap_2D " << this << " NID->" << NID.U; cout.flush();
          
          Ref = NULL;
     }
     
     
     //==--  Member Functions.

	 //==--  Member Functions.

	 //Adds an axon on the left leg.
	 void add_Axon(c_NT3_Base_Node_MMap_2D* p_Axon, int p_Axon_Ind)
	 {
		 c_NT3_Base_Node_MMap_2D** tmp_Axons;
		 tmp_Axons = new c_NT3_Base_Node_MMap_2D * [Axon_Count[p_Axon_Ind]];
		 for (int cou_A = 0; cou_A < Axon_Count[p_Axon_Ind]; cou_A++)
		 {
			 tmp_Axons[cou_A] = Axons[p_Axon_Ind][cou_A];
			 Axons[p_Axon_Ind][cou_A] = NULL;
		 }

		 delete[] Axons[p_Axon_Ind];
		 Axons[p_Axon_Ind] = NULL;

		 Axons[p_Axon_Ind] = new c_NT3_Base_Node_MMap_2D * [Axon_Count[p_Axon_Ind] + 1];

		 for (int cou_A = 0; cou_A < Axon_Count[p_Axon_Ind]; cou_A++)
		 {
			 Axons[p_Axon_Ind][cou_A] = tmp_Axons[cou_A];
			 tmp_Axons[cou_A] = NULL;
		 }
		 delete[] tmp_Axons;
		 tmp_Axons = NULL;

		 Axons[p_Axon_Ind][Axon_Count[p_Axon_Ind]] = p_Axon;
		 Axon_Count[p_Axon_Ind]++;
	 }

	 //f11 {BACKSPACE}{BACKSPACE}[p_Axon_Ind]
	 //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
	 void add_Predefined_Axon(c_NT3_Base_Node_MMap_2D* p_Axon, int p_Axon_Ind)
	 {
		 Axons[p_Axon_Ind][Axon_Count[p_Axon_Ind]] = p_Axon;
		 Axon_Count[p_Axon_Ind]++;
	 }

	 //Destroys the current axons if any and expands the array to the given depth.
	 void expand_Axon(int p_Count, int p_Axon_Ind)
	 {
		 if (Axons[p_Axon_Ind] != NULL) { delete[] Axons[p_Axon_Ind]; }
		 Axons[p_Axon_Ind] = NULL;

		 Axons[p_Axon_Ind] = new c_NT3_Base_Node_MMap_2D * [p_Count];

		 for (int cou_A = 0; cou_A < p_Count; cou_A++)
		 {
			 Axons[p_Axon_Ind][cou_A] = NULL;
		 }

		 Axon_Count[p_Axon_Ind] = 0;
	 }

	 //Searches the axons to see if an upper tier connection exists.
	 c_NT3_Base_Node_MMap_2D* does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_MMap_2D* p_Nodes[3])
	 {
		 //Check the axons on the right side for the 
		 for (int cou_A = 0; cou_A < Axon_Count[0]; cou_A++)
		 {
			 if (Axons[0][cou_A]->does_Lower_Connection_Exist(p_Nodes))
			 {
				 return Axons[0][cou_A];
			 }
		 }
		 return NULL;
	 }

	 //Checks if the given node matches a dendrite on the left leg.
	 bool does_Lower_Connection_Exist(c_NT3_Base_Node_MMap_2D* p_Nodes[3])
	 {
		 //The offset might be a bit confusing but we are comparing only the latter 3 legs instead of all 4.
		 if ((Dendrite[1] == p_Nodes[0]) && (Dendrite[2] == p_Nodes[1]) && (Dendrite[3] == p_Nodes[2])) { return 1; }
		 return 0;
	 }

	 //Sets the given dendrite to the given dendrite.
	 void set_Dendrite(c_NT3_Base_Node_MMap_2D* p_Dendrite, int p_Dendrite_Ind)
	 {
		 Dendrite[p_Dendrite_Ind] = p_Dendrite;
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
     unsigned long long int get_State(){ return State.I; }
     
     //Returns the type of node, 0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
     int get_Type(){ return Type; }
     
     //Sets the type of node.
     void set_Type(int p_Type){ Type = p_Type; }
     
     //Initiates a backpropagation that outputs to the given table with the given index.
     void bp_O() 
     {
          cout << " ~<";
          ochr(0, 7, State.C);
		  cout << " ";
		  odbl(0, 13, State.F);
          cout << ">~ ";
     }



	 //bp_Output the left node.
	 void bp_0()
	 {
		 //If a left leg exists then initiate a backpropagation along it, then along the right side.
		 if (Dendrite[0] != NULL)
		 {
			 if (Dendrite[0] != NULL) { Dendrite[0]->bp_0(); }
			 if (Dendrite[1] != NULL) { Dendrite[1]->bp_1(); }
			 if (Dendrite[2] != NULL) { Dendrite[1]->bp_2(); }
			 if (Dendrite[3] != NULL) { Dendrite[1]->bp_3(); }
		 }
		 else
		 {
			 char tmp_State = char(get_State());
			 if (tmp_State == char(0)) { tmp_State = '0'; }
			 if (tmp_State == char(7)) { tmp_State = '7'; }
			 if (tmp_State == char(8)) { tmp_State = '8'; }
			 if (tmp_State == char(9)) { tmp_State = '9'; }
			 if (tmp_State == char(10)) { tmp_State = 'a'; }
			 if (tmp_State == char(13)) { tmp_State = 'd'; }
			 cout << tmp_State;
		 }
	 }

	 //bp_Output the left node.
	 void bp_1()
	 {
		 //If a left leg exists then initiate a backpropagation along it, then along the right side.
		 if (Dendrite[0] != NULL)
		 {
			 if (Dendrite[1] != NULL) { Dendrite[1]->bp_1(); }
		 }
		 else
		 {
			 char tmp_State = char(get_State());
			 if (tmp_State == char(0)) { tmp_State = '0'; }
			 if (tmp_State == char(7)) { tmp_State = '7'; }
			 if (tmp_State == char(8)) { tmp_State = '8'; }
			 if (tmp_State == char(9)) { tmp_State = '9'; }
			 if (tmp_State == char(10)) { tmp_State = 'a'; }
			 if (tmp_State == char(13)) { tmp_State = 'd'; }
			 cout << tmp_State;
		 }
	 }

	 //bp_Output the left node.
	 void bp_2()
	 {
		 //If a left leg exists then initiate a backpropagation along it, then along the right side.
		 if (Dendrite[0] != NULL)
		 {
			 if (Dendrite[2] != NULL) { Dendrite[1]->bp_2(); }
		 }
		 else
		 {
			 char tmp_State = char(get_State());
			 if (tmp_State == char(0)) { tmp_State = '0'; }
			 if (tmp_State == char(7)) { tmp_State = '7'; }
			 if (tmp_State == char(8)) { tmp_State = '8'; }
			 if (tmp_State == char(9)) { tmp_State = '9'; }
			 if (tmp_State == char(10)) { tmp_State = 'a'; }
			 if (tmp_State == char(13)) { tmp_State = 'd'; }
			 cout << tmp_State;
		 }
	 }

	 //bp_Output the left node.
	 void bp_3()
	 {
		 //If a left leg exists then initiate a backpropagation along it, then along the right side.
		 if (Dendrite[0] != NULL)
		 {
			 if (Dendrite[3] != NULL) { Dendrite[1]->bp_3(); }
		 }
		 else
		 {
			 char tmp_State = char(get_State());
			 if (tmp_State == char(0)) { tmp_State = '0'; }
			 if (tmp_State == char(7)) { tmp_State = '7'; }
			 if (tmp_State == char(8)) { tmp_State = '8'; }
			 if (tmp_State == char(9)) { tmp_State = '9'; }
			 if (tmp_State == char(10)) { tmp_State = 'a'; }
			 if (tmp_State == char(13)) { tmp_State = 'd'; }
			 cout << tmp_State;
		 }
	 }
};





















