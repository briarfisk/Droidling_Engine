//For now I redid the *ref for the nodes
//Before it was used as a shortcut to find if a node has already been submitted
//Now that we are not using virtual nodes I cannot check for that, so for now we are using a fractal tree.
//Multi threading has been stripped as well.

//This is the linked list for the buffer.
class c_NT3_Linked_List_MMap
{
public:
     
     //The next node in the chain.
     c_NT3_Linked_List_MMap * Next;
     c_NT3_Linked_List_MMap * Previous;
     u_Data_3 Charge;
     u_Data_3 NID;
     
     c_NT3_Linked_List_MMap()
     {
          Next = NULL;
          Previous = NULL;
          Charge.I = 0;
          NID.U = 0;
     }
     
     ~c_NT3_Linked_List_MMap()
     {
          Charge.I = 0;
          NID.U = 0;
          //delete Next; Handled by the handler.
     }
     
     //Outputs the linked list.
     //-- !WARNING WILL CAUSE STACK CORRUPTION ERROR WITH ENOUGH NODES!
     void output_LL()
     {
          //*cout << " [";
          if (NID.U != 0){ cout << " NID:" << NID.U; }
          cout << " $" << Charge.I << "]  ";
          if (Next != NULL){ Next->output_LL(); }
     }
};

//This handles the linked list creation and deletion so that recursive destruction does not crash the stack.
class c_NT3_Linked_List_Handler_MMap
{
public:
     
     //The nodes in the linked list.
     c_NT3_Linked_List_MMap * Root;
     c_NT3_Linked_List_MMap ** Current_LL;
     long long int Depth;
     
     c_NT3_Linked_List_Handler_MMap()
     {
          Root = NULL;
          Current_LL = &Root;
          Depth = 0;
     }
     
     ~c_NT3_Linked_List_Handler_MMap()
     {
          reset();
     }
     
     //Init
     void init()
     {
          Root = NULL;
          Current_LL = &Root;
          Depth = 0;
     }
     
     //Adds a node.
     c_NT3_Linked_List_MMap * new_LL(u_Data_3 p_NID, long long int p_Charge)
     {
		 //IF this is the first node to be created then Current_LL will be NULL
		 if (*Current_LL == NULL)
		 {
			 *Current_LL = new c_NT3_Linked_List_MMap;

			 (*Current_LL)->Charge.I = p_Charge;
			 (*Current_LL)->NID = p_NID;

			 (*Current_LL)->Previous = NULL;
			 Depth++;
			 return *Current_LL;
		 }
          
		 (*Current_LL)->Next = new c_NT3_Linked_List_MMap;

		 (*Current_LL)->Next->Charge.I = p_Charge;
		 (*Current_LL)->Next->NID = p_NID;

		 (*Current_LL)->Next->Previous = (*Current_LL);

		  Current_LL = &((*Current_LL)->Next);

          Depth++;
          
          return *Current_LL;
     }
     
     //Resets the linked list.
     void reset()
     {
		 //ostr(0, 13, "\n\n\n resetting LL Depth:"); oint(0, 8, Depth);
		 //cout << "\n Root:" << Root;

          c_NT3_Linked_List_MMap * tmp_LL = Root;
          Root = NULL;
          c_NT3_Linked_List_MMap * tmp_LL_Next = NULL;
		  //cout << "\n tmp_LL: " << tmp_LL;
		  //cout << "\n tmp_LL_Next: " << tmp_LL_Next;
          while (tmp_LL != NULL)
          {
			  //ostr(0, 11, "\n\nIn Loop:");
			  //cout << "\n tmp_LL: " << tmp_LL;
               tmp_LL_Next = tmp_LL->Next;
			   //cout << "\n tmp_LL_Next: " << tmp_LL_Next;
               tmp_LL->Next = NULL;
               tmp_LL->Previous = NULL;
               delete tmp_LL;
               tmp_LL = NULL;
               tmp_LL = tmp_LL_Next;

			   //cout << "\n --tmp_LL: " << tmp_LL;
			   //cout << "\n --tmp_LL_Next: " << tmp_LL_Next;
          }
          
          delete Root;
          Root = NULL;
          Current_LL = &Root;
          
          Depth = 0;
     }
     
     /*
     //Bubble sorts the linked list.
     void bubble_Sort()
     {
          //Tmp vars
          c_NT3_Linked_List_MMap * tmp_Prv = NULL;
          c_NT3_Linked_List_MMap * tmp_Next = NULL;
          c_NT3_Linked_List_MMap * tmp_Current = NULL;
          bool flg_Done = false;
          
          while(!flg_Done)
          {
               flg_Done = true;
               
               
          }
     }
      * */
};


class c_NT3_Buffer_Node_MMap
{
     friend class c_NT3_Buffer_Tree_MMap; 
     private:
          c_NT3_Buffer_Node_MMap * Right;
          c_NT3_Buffer_Node_MMap * Center;
          c_NT3_Buffer_Node_MMap * Left;
          
     public:
          u_Data_3 Data;
          
          c_NT3_Linked_List_MMap * LL;
          
          c_NT3_Buffer_Node_MMap()
          {
               Right = NULL;
               Center = NULL;
               Left = NULL;
               LL = NULL;
               Data.I = 0;
          }
          
          ~c_NT3_Buffer_Node_MMap()
          {
               LL = NULL;
               delete Right; Right = NULL;
               delete Center; Center = NULL;
               delete Left; Left = NULL;
          }
                
};

class c_NT3_Buffer_Tree_MMap
{
private:
             
             
     public:
          c_NT3_Buffer_Node_MMap * Root;
          c_NT3_Buffer_Node_MMap ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          
          //For handling the current search.
          u_Data_3 tmp_Data;
          u_Data_3 tmp_SData;
          
          c_NT3_Buffer_Tree_MMap()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          ~c_NT3_Buffer_Tree_MMap()
          {
               delete Root;
          }
          
          void init()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          //Now returning whether or not the node was found as the foundit flag does not work with multiple threads.
          //This returns the node at the end of the chain, whether it was found or not does not matter the 
          //buffer node is returned and has to be checked for values.
          c_NT3_Buffer_Node_MMap * search(u_Data_3 p_Data)
          {
               flg_Foundit = false;
               
               if (p_Data.U == 0){ Current = NULL; return NULL; }
               
               u_Data_3 tmp_SSData;
               tmp_SSData.NR = p_Data.NR;
               
               
               
               //---cout << "\n\n SEARCHING ->" << tmp_SSData.U;
               //system("PAUSE > NULL");
               return query_Node(Root, tmp_SSData, 31);
          }
          
          c_NT3_Buffer_Node_MMap * query_Node(c_NT3_Buffer_Node_MMap *&p_Node, u_Data_3 p_Data, int p_Shift)
          {
               //These are used because we have to be carefull not to leave Mutex locked with a return.
               c_NT3_Buffer_Node_MMap * tmp_Return = NULL;
               bool flg_Done = false;
               
               u_Data_3 tmp_DData;
               tmp_DData.U = p_Data.U;
               tmp_DData.U = tmp_DData.U >> p_Shift;
               //---cout << "\n tmp_DData.U = " << tmp_DData.U << " p_Data.U->" << p_Data.U << " p_Shift = " << p_Shift;
               //---cout << "\n\n tmp_DData.U = " << tmp_DData.U << " p_Shift = " << p_Shift;
               //---cout << "\n\t __QN__ tmp_DData.U->" << tmp_DData.U << " p_Data.U->" << p_Data.U;
               
               if (p_Node == NULL)
               {      
                    //---cout << "\n\t   Node is NULL";
                    p_Node = new c_NT3_Buffer_Node_MMap;
                    p_Node->Right = NULL;
                    p_Node->Center = NULL;
                    p_Node->Left = NULL;
                    if (p_Shift >= 1)
                    {
                         //---cout << "\n\t   tmp_DData.U > 1";
                         p_Node->Data = tmp_DData;
                    }
                    else 
                    {
                         //---cout << "\n\t   tmp_DData.U !> 1";
                         p_Node->Data = p_Data;
                         Current = &p_Node; 
                         flg_Foundit = false;
                         tmp_Return = p_Node;
                         flg_Done = true;
                         //return 0;
                    }
               }
               //If the node was not found then return NULL;
               if (flg_Done){ return tmp_Return; }
               
               //If the current data matches the tmp data then another depth is explored.
               if (p_Node->Data.U == tmp_DData.U && p_Shift > 0)
               {
                    //---cout << "\n\t   p_Node->Data.U (" << p_Node->Data.U << ") == tmp_DData.U (" << tmp_DData.U << ")";
                    
                    //---cout << "\t tmp_DData.U = " << tmp_DData.U;
                    
                    return query_Node(p_Node->Center, p_Data, (p_Shift - 1));
               }
               
               //If the node data matches the given data exactly the node has been found.
               //This is not locked because even if the Current & flg_Foundit are overwritten they will not be
               //used by the threads, but will work with the standard single thread.
               //--Having error because now that node references can be single digits we have the issue of a NID 1 triggering this function when shift > 0.
               if (p_Node->Data.U == p_Data.U && p_Shift <= 0)
               {
                    //---cout << "\n\t    p_Node->Data.NR(" << p_Node->Data.NR << ") == p_Data.NR(" << p_Data.NR << ")";
                    Current = &p_Node;  
                    flg_Foundit = true;
                    tmp_Return = p_Node;
                    flg_Done = true;
                    //return 1;
               }
               
               //If the node was found then return tmp_Return;
               if (flg_Done){ return tmp_Return; }
               
               if (tmp_DData.U < p_Node->Data.U)
               {
                    //---cout << "\n\t    tmp_DData.U (" << tmp_DData.U << ") < p_Node->Data.U(" << p_Node->Data.U << ")";
                    return query_Node(p_Node->Left, p_Data, (p_Shift - 1));
               }
               if (tmp_DData.U > p_Node->Data.U)
               {
                    //---cout << "\n\t    tmp_DData.U (" << tmp_DData.U << ") > p_Node->Data.U(" << p_Node->Data.U << ")";
                    return query_Node(p_Node->Right, p_Data, (p_Shift - 1));
               }
               return tmp_Return;
          }
          
          void output_Tree()
          {
               output_Node(Root, 0);
          }
          
          void output_Node(c_NT3_Buffer_Node_MMap *&p_Node, int p_Tab)
          {
               /*
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left, (p_Tab));
               //cout << "\n";
               for (int cou_Index=0;cou_Index<p_Tab;cou_Index++)
               {
                    //cout << "  ";
               }
               //cout << (p_Node->Data).I;
               if (p_Node->LL != NULL){ cout << "->" << (p_Node->LL->NID); }
               output_Node(p_Node->Center, (p_Tab + 1));
               output_Node(p_Node->Right, (p_Tab));
                * */
          }
          
          c_NT3_Linked_List_MMap * get_Node_LL(c_NT3_Buffer_Node_MMap * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->LL;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_LL(c_NT3_Buffer_Node_MMap * &p_Node, c_NT3_Linked_List_MMap * p_LL)
          {
               if (p_Node != NULL)
               {
                    p_Node->LL = p_LL;
               }
          }
          
          c_NT3_Linked_List_MMap * set_Current_Node_LL(c_NT3_Linked_List_MMap * p_LL)
          {
               set_Node_LL(*Current, p_LL);
               return p_LL;
          }
          
          c_NT3_Linked_List_MMap * get_Current_Node_LL()
          {
               return get_Node_LL(*Current);
          }
          
          
          //Resets the tree.
          void reset()
          {
               delete Root;
               Root = NULL;
               
               Current = NULL;
               flg_Foundit = false;
          }
                    
};

class c_Axon_List_MMap
{
public:
     
     int MAX_Size;
     int Current_Size;
     u_Data_3 * Axons;
     
     c_Axon_List_MMap()
     {
          Axons = NULL;
          resize(10);
     }
     
     ~c_Axon_List_MMap()
     {
          reset();
     }
     
     void reset()
     {
          if (Axons != NULL)
          {
               delete [] Axons;
               Axons = NULL;
          }
          MAX_Size = 0;
          Current_Size = 0;
     }
     
     //Does not save info.
     void resize(int p_Size)
     {
          //This avoid the tedious resizing if the amount is less than the current allocated amount.
          if (p_Size <= MAX_Size){ Current_Size = p_Size; return; }
          
          reset();
          
          Axons = new u_Data_3[p_Size];
          Current_Size = p_Size;
     }
};

//The class that houses the charging buffer tree and the LL.
class c_NT3_Buffer_MMap
{
private:
     
public:
     
     //The search tree.
     c_NT3_Buffer_Tree_MMap Tree;
     
     //The linked lists.
     c_NT3_Linked_List_Handler_MMap Current_Charge;
     
     c_NT3_Linked_List_Handler_MMap Output;
     
     c_NT3_Linked_List_Handler_MMap Treetops;
     
     c_NT3_Node_Network_MMap_1D * Nodes;
     
     //Flags
     bool flg_Not_Done;
     
     int Input_Position;
     
     //Settings & trackers for the settings.
     long long int Base_Charge;
     long long int Current_Highest_Charge;
     long long int Previous_Highest_Charge;
     
     double Modifier_Charge;
     double Action_Potential_Threshold;
     
     //The objects to hold the temporary axon lists that are passed to the node object.
     c_Axon_List_MMap Axon_List_L;
     c_Axon_List_MMap Axon_List_R;
          
     c_NT3_Buffer_MMap()
     {
          flg_Not_Done = 0;
          Input_Position = 0;
          
          Current_Highest_Charge = 0;
          Previous_Highest_Charge = 0;
          Base_Charge = 0;
          Modifier_Charge = 0.0f;
          Action_Potential_Threshold = 0.0f;
          
          Nodes = NULL;
     }
     
     ~c_NT3_Buffer_MMap()
     {
          Current_Charge.reset();
          Output.reset();
          Treetops.reset();
     }
     
     //Sets the reference to the node networks to query.
     void init_Network(c_NT3_Node_Network_MMap_1D * p_Nodes = NULL)
     {
          Nodes = p_Nodes;
     }
     
     //=====--            --=====//
     //==--     CHARGING     --==//
     //=====--            --=====//
     
     
     //--  CHARGING FOR ORIGINAL ALGO, USED FOR MSC WHERE ALL PATTERNS ARE TO BE FOUND
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs(int p_Use_Spike, int p_Left_Leg = 1, int p_Right_Leg = 1)
     {
          //*---cout << "\n\n charge_Outputs(" << p_Use_Spike << ", " << p_Left_Leg << ", " << p_Right_Leg << ");";
          c_NT3_Linked_List_MMap * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          //*---long int tmp_Tracker = 0;
          while(tmp_LL != NULL)
          {
               //*---cout << "\n [" << tmp_Tracker << "] "; oull(0, 13, tmp_LL->NID.U); cout << " Tp->" << Nodes->get_Type(tmp_LL->NID.U) << " "; tmp_Tracker++;
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID.U == 0){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " (((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               if (p_Use_Spike == 2){ tmp_Charge = ((tmp_Charge_Percentage + ((1 - tmp_Charge_Percentage) * .1)) * Base_Charge); }
               
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               //*---cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               Axon_List_L.resize(Nodes->get_Axon_Count_L(tmp_LL->NID.U));
               //*---cout << "\n   aL->" << Axon_List_L.Current_Size;
               if (p_Left_Leg)
               {
                    //Gather the left axons from the node.
                    Nodes->get_Axons_L(tmp_LL->NID.U, Axon_List_L.Axons);
                    for (int cou_A=0;cou_A<Axon_List_L.Current_Size;cou_A++)
                    {
                         //submit(Nodes->get_Axon_L(tmp_LL->NID, cou_A), tmp_Charge);
                         submit(Axon_List_L.Axons[cou_A], tmp_Charge);
                         //*---cout << " A->" << Axon_List_L.Axons[cou_A].U;
                         //*Nodes->get_Axon_L(tmp_LL->NID, cou_A)->bp_O();
                    }
               }
               
               Axon_List_R.resize(Nodes->get_Axon_Count_R(tmp_LL->NID.U));
               //*---cout << "\n   aR->" << Axon_List_R.Current_Size;
               if (p_Right_Leg)
               {
                    //Gather the left axons from the node.
                    Nodes->get_Axons_R(tmp_LL->NID.U, Axon_List_R.Axons);
                    for (int cou_A=0;cou_A<Axon_List_R.Current_Size;cou_A++)
                    {
                         //submit(Nodes->get_Axon_R(tmp_LL->NID, cou_A), tmp_Charge);
                         submit(Axon_List_R.Axons[cou_A], tmp_Charge);
                         //*---cout << " A->" << Axon_List_R.Axons[cou_A].U;
                         //*Nodes->get_Axon_L(tmp_LL->NID, cou_A)->bp_O();
                    }
               }
               
               if (Nodes->get_Type(tmp_LL->NID.U) == 2 || Nodes->get_Type(tmp_LL->NID.U) == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_RC(int p_Use_Spike, int p_Left_Leg = 1, int p_Right_Leg = 1)
     {
          //*---cout << "\n\n charge_Outputs_RC(" << p_Use_Spike << ", " << p_Left_Leg << ", " << p_Right_Leg << ");";
          c_NT3_Linked_List_MMap * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          //*---long int tmp_Tracker = 0;
          while(tmp_LL != NULL)
          {
               //*---cout << "\n [" << tmp_Tracker << "] "; oull(0, 13, tmp_LL->NID.U); cout << " Tp->" << Nodes->get_Type(tmp_LL->NID.U) << " "; tmp_Tracker++;
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID.U == 0){ tmp_LL = tmp_LL->Next; continue; }
               
               //---cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << Nodes->get_RC_Score(tmp_LL->NID) << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               tmp_Charge_Percentage = (((tmp_LL->Charge.I * Modifier_Charge) * Nodes->get_RC_Score(tmp_LL->NID.U)) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
			   if (Nodes->get_flg_Double_Legged(tmp_LL->NID.U)) 
			   {
				   //***cout << "\n double legged node found:" << tmp_LL->NID.U;
				   tmp_Charge /= 2; 
			   }
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * Nodes->get_RC_Score(tmp_LL->NID)) / Previous_Highest_Charge) * Base_Charge);
               
               //*---cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               Axon_List_L.resize(Nodes->get_Axon_Count_L(tmp_LL->NID.U));
               //*---cout << "\n   aL->" << Axon_List_L.Current_Size;
               if (p_Left_Leg)
               {
                    //Gather the left axons from the node.
                    Nodes->get_Axons_L(tmp_LL->NID.U, Axon_List_L.Axons);
                    for (int cou_A=0;cou_A<Axon_List_L.Current_Size;cou_A++)
                    {
                         submit_RC(Axon_List_L.Axons[cou_A], tmp_Charge);
                         //*---cout << " A->" << Axon_List_L.Axons[cou_A].U;
                         //*Nodes->get_Axon_L(tmp_LL->NID, cou_A)->bp_O();
                    }
               }
               
               Axon_List_R.resize(Nodes->get_Axon_Count_R(tmp_LL->NID.U));
               //*---cout << "\n   aR->" << Axon_List_R.Current_Size;
               if (p_Right_Leg)
               {
                    //Gather the left axons from the node.
                    Nodes->get_Axons_R(tmp_LL->NID.U, Axon_List_R.Axons);
                    for (int cou_A=0;cou_A<Axon_List_R.Current_Size;cou_A++)
                    {
                         submit_RC(Axon_List_R.Axons[cou_A], tmp_Charge);
                         //*---cout << " A->" << Axon_List_R.Axons[cou_A].U;
                         //*Nodes->get_Axon_L(tmp_LL->NID, cou_A)->bp_O();
                    }
               }
               
               if (Nodes->get_Type(tmp_LL->NID.U) == 2 || Nodes->get_Type(tmp_LL->NID.U) == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_L(int p_Use_Spike)
     {
          c_NT3_Linked_List_MMap * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << Nodes->get_Type(tmp_LL->NID) << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID.U == 0){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " (((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aL->" << Nodes->get_Axon_Count_L(tmp_LL->NID);
               for (int cou_A=0;cou_A<Nodes->get_Axon_Count_L(tmp_LL->NID.U);cou_A++)
               {
                    submit(Nodes->get_Axon_L(tmp_LL->NID.U, cou_A), tmp_Charge);
                    //*cout << " A->" << Nodes->get_Axon_L(tmp_LL->NID, cou_A);
                    //*Nodes->get_Axon_L(tmp_LL->NID, cou_A)->bp_O();
               }
               
               if (Nodes->get_Type(tmp_LL->NID.U) == 2 || Nodes->get_Type(tmp_LL->NID.U) == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_R(int p_Use_Spike)
     {
          c_NT3_Linked_List_MMap * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << Nodes->get_Type(tmp_LL->NID) << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID.U == 0){ tmp_LL = tmp_LL->Next; continue; }
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge);
               tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aR->" << Nodes->get_Axon_Count_R(tmp_LL->NID);
               for (int cou_A=0;cou_A<Nodes->get_Axon_Count_R(tmp_LL->NID.U);cou_A++)
               {
                    submit(Nodes->get_Axon_R(tmp_LL->NID.U, cou_A), tmp_Charge);
                    //*cout << " A->" << Nodes->get_Axon_R(tmp_LL->NID, cou_A);
                    //*Nodes->get_Axon_R(tmp_LL->NID, cou_A)->bp_O();
               }
               
               if (Nodes->get_Type(tmp_LL->NID.U) == 2 || Nodes->get_Type(tmp_LL->NID.U) == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_RC_L(int p_Use_Spike)
     {
          c_NT3_Linked_List_MMap * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << Nodes->get_Type(tmp_LL->NID) << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID.U == 0){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << Nodes->get_RC_Score(tmp_LL->NID) << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               tmp_Charge_Percentage = (((tmp_LL->Charge.I * Modifier_Charge) * Nodes->get_RC_Score(tmp_LL->NID.U)) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * Nodes->get_RC_Score(tmp_LL->NID)) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aL->" << Nodes->get_Axon_Count_L(tmp_LL->NID);
               for (int cou_A=0;cou_A<Nodes->get_Axon_Count_L(tmp_LL->NID.U);cou_A++)
               {
                    submit_RC(Nodes->get_Axon_L(tmp_LL->NID.U, cou_A), tmp_Charge);
                    //*cout << " A->" << Nodes->get_Axon_L(tmp_LL->NID, cou_A);
                    //*Nodes->get_Axon_L(tmp_LL->NID, cou_A)->bp_O();
               }
               
               if (Nodes->get_Type(tmp_LL->NID.U) == 2 || Nodes->get_Type(tmp_LL->NID.U) == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_RC_R(int p_Use_Spike)
     {
          c_NT3_Linked_List_MMap * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          while(tmp_LL != NULL)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << Nodes->get_Type(tmp_LL->NID) << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID.U == 0){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << Nodes->get_RC_Score(tmp_LL->NID) << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               tmp_Charge_Percentage = (((tmp_LL->Charge.I * Modifier_Charge) * Nodes->get_RC_Score(tmp_LL->NID.U)) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * Nodes->get_RC_Score(tmp_LL->NID)) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aR->" << Nodes->get_Axon_Count_R(tmp_LL->NID);
               for (int cou_A=0;cou_A<Nodes->get_Axon_Count_R(tmp_LL->NID.U);cou_A++)
               {
                    submit_RC(Nodes->get_Axon_R(tmp_LL->NID.U, cou_A), tmp_Charge);
                    //*cout << " A->" << Nodes->get_Axon_R(tmp_LL->NID, cou_A);
                    //*Nodes->get_Axon_R(tmp_LL->NID, cou_A)->bp_O();
               }
               
               if (Nodes->get_Type(tmp_LL->NID.U) == 2 || Nodes->get_Type(tmp_LL->NID.U) == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
     }

     //=====--              --=====//
     //==--     SUBMISSION     --==//
     //=====--              --=====//
     
     
     //Accepts a submission of nodes to charge, used for CAN.
     void submit(u_Data_3 p_Node, long long int p_Charge = 10000)
     {
          //Search for the submitted node, if found add the charge.
          Tree.search(p_Node);
          
          if (Tree.flg_Foundit)
          {
               (Tree.get_Current_Node_LL())->Charge.I += p_Charge;
               if ((Tree.get_Current_Node_LL())->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = (Tree.get_Current_Node_LL())->Charge.I; }
          }
          
          if (!Tree.flg_Foundit && p_Node.U != 0)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               Tree.set_Current_Node_LL(Current_Charge.new_LL(p_Node, p_Charge));
          }
     }
          
     void submit(unsigned long long int p_Node, long long int p_Charge = 10000)
     {
          u_Data_3 tmp_Data;
          tmp_Data.U = p_Node;
          submit(tmp_Data, p_Charge);
     }
     
     //Accepts a submission of nodes to charge, used for CAN.
     void submit_RC(u_Data_3 p_Node, long long int p_Charge = 10000)
     {
          
          //Search for the submitted node, if found add the charge.
          Tree.search(p_Node);
          
          if (Tree.flg_Foundit)
          {
               (Tree.get_Current_Node_LL())->Charge.I += (p_Charge * (Nodes->get_RC_Score((Tree.get_Current_Node_LL()->NID.U))));
               if ((Tree.get_Current_Node_LL())->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = (Tree.get_Current_Node_LL())->Charge.I; }
          }
          
          if (!Tree.flg_Foundit && p_Node.U != 0)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               Tree.set_Current_Node_LL(Current_Charge.new_LL(p_Node, p_Charge));
          }
     }
     
     void submit_RC(unsigned long long int p_Node, long long int p_Charge = 10000)
     {
          u_Data_3 tmp_Data;
          tmp_Data.U = p_Node;
          submit_RC(tmp_Data, p_Charge);
     }
     
     
     //=====--             --=====//
     //==--     GATHERING     --==//
     //=====--             --=====//
     
     //For gathering the Opp
     //>Target Tier - 1
     //>charge
     //>gather
     //>flush(Target_Tier)
     
     //Gathers the output nodes and resets the tree for the next iteration.
     void gather()
     {
          //Delete the old Output.
          Output.reset();
          
          //Reassign the linked lists.
          Output.Root = Current_Charge.Root;
          Output.Depth = Current_Charge.Depth;
		  Output.Current_LL = Current_Charge.Current_LL;
          
          Current_Charge.Root = NULL;
		  Current_Charge.Depth = 0;
          Current_Charge.reset();
          
          //Delete the tree.
          Tree.reset();
          
          //Sets the done flag if Output == NULL.
          flg_Not_Done = 1;
          if ((Output.Root == NULL)){ flg_Not_Done = 0; }
     }
     
     //Charges the outputs back into the charging buffer.
     long long int gather_Treetops()
     {

          c_NT3_Linked_List_MMap * tmp_LL = Treetops.Root;

		  c_NT3_Linked_List_Handler_MMap tmp_TLL;
		  tmp_TLL.Root = Treetops.Root;
		  tmp_TLL.Depth = Treetops.Depth;
		  tmp_TLL.Current_LL = Treetops.Current_LL;

		  Treetops.Depth = 0;
		  Treetops.Root = NULL;
		  Treetops.reset();

          long long int tmp_Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          
          //Find the current highest charge.
          while(tmp_LL != NULL)
          {
               if (tmp_LL->NID.U == 0){ tmp_LL = tmp_LL->Next; continue; }
               
               if (tmp_Current_Highest_Charge < tmp_LL->Charge.I){ tmp_Current_Highest_Charge = tmp_LL->Charge.I; }
               
               tmp_LL = tmp_LL->Next;    
          }
          
          tmp_LL = tmp_TLL.Root;
          
          while(tmp_LL != NULL)
          {
               if (tmp_LL->NID.U == 0){ tmp_LL = tmp_LL->Next; continue; }
               
               tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / tmp_Current_Highest_Charge) * Base_Charge);
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               Treetops.new_LL(tmp_LL->NID, (tmp_Charge));
               
               tmp_LL = tmp_LL->Next;
          }
          
		  tmp_TLL.reset();
          
          return tmp_Current_Highest_Charge;
     }
     
     //=====--                       --=====//
     //==--     CLEANUP & ALLOCATION    --==//
     //=====--                       --=====//
     
     
     //Destroys the output buffers.
     void destroy_Output_Buffers()
     {
          //Delete the old Output.
          Output.reset();    
     }
     
     //=====--          --=====//
     //==--     OUTPUT     --==//
     //=====--          --=====//
     
     //Outputs all of the buffers.
     void output_All_Buffers()
     {
          //system("CLS");
          cout << "\n Charge_LL->" << Current_Charge.Depth << " ->";
          output_Charge_LL();
          cout << "\n Output_LL->" << Output.Depth << " ->";
          output_Output_LL();
          cout << "\n Treetops_LL->" << Output.Depth << " ->";
          output_Treetops();
          cout << "\n flg_Not_Done->" << flg_Not_Done;
     }
     
     //Outputs the buffer.
     void output_Buffer()
     {
          Tree.output_Tree();
          output_Charge_LL();
          output_Output_LL();
          output_Treetops();
     }
     
     //Outputs the Current_Charge_LL.
     void output_Charge_LL()
     {
          //*cout << "\n\t Current_Charge_LL->";
          if (Current_Charge.Root == NULL){ cout << "empty..."; return; }
          Current_Charge.Root->output_LL();
          //*cout << " ~Fing"; cout.flush();
     }
     
     //Outputs the Output_LL.
     void output_Output_LL()
     {
          //*cout << "\n\t Output_LL->";
          if (Output.Root == NULL){ cout << "empty..."; return; }
          Output.Root->output_LL();
          //*cout << " ~Fing"; cout.flush();
     }
     
     //Outputs the Treetops.
     void output_Treetops()
     {
          //*cout << "\n\t Treetops_LL->";
          if (Treetops.Root == NULL){ cout << "empty..."; return; }
          Treetops.Root->output_LL();
          //*cout << " ~Fing"; cout.flush();
     }
};





















































//This is the linked list for the buffer.
class c_NT3_Linked_List_MMap_2D
{
public:
     
     //The next node in the chain.
     c_NT3_Linked_List_MMap_2D * Next;
     c_NT3_Linked_List_MMap_2D * Previous;
     u_Data_3 Charge;
     u_Data_3 NID;
     
     c_NT3_Linked_List_MMap_2D()
     {
          Next = NULL;
          Previous = NULL;
          Charge.I = 0;
          NID.U = 0;
     }
     
     ~c_NT3_Linked_List_MMap_2D()
     {
          Charge.I = 0;
          NID.U = 0;
          //delete Next; Handled by the handler.
     }
     
     //Outputs the linked list.
     //-- !WARNING WILL CAUSE STACK CORRUPTION ERROR WITH ENOUGH NODES!
     void output_LL()
     {
          cout << "\n[ " << NID.U << " ";
          //if (NID != NULL){ NID->bp_0(); } else { ostr(0, 12, "NULL"); }
          cout << " $"; oint(0, 14, Charge.I); cout << "]  ";
          if (Next != NULL){ Next->output_LL(); }
     }
};

//This handles the linked list creation and deletion so that recursive destruction does not crash the stack.
class c_NT3_Linked_List_Handler_MMap_2D
{
public:
     
     //The nodes in the linked list.
     c_NT3_Linked_List_MMap_2D * Root;
     c_NT3_Linked_List_MMap_2D ** Current_LL;
     long long int Depth;
     
     c_NT3_Linked_List_Handler_MMap_2D()
     {
          Root = NULL;
          Current_LL = &Root;
          Depth = 0;
     }
     
     ~c_NT3_Linked_List_Handler_MMap_2D()
     {
          reset();
     }
     
     //Init
     void init()
     {
          Root = NULL;
          Current_LL = &Root;
          Depth = 0;
     }
     
     //Adds a node.
     c_NT3_Linked_List_MMap_2D * new_LL(u_Data_3 p_NID, long long int p_Charge)
     {
          c_NT3_Linked_List_MMap_2D * tmp_LL = NULL;
          
          *Current_LL = new c_NT3_Linked_List_MMap_2D;
          
          (*Current_LL)->Charge.I = p_Charge;
          (*Current_LL)->NID = p_NID;
          
          tmp_LL = *Current_LL;
          
          (*Current_LL)->Previous = tmp_LL;
          
          Current_LL = &(*Current_LL)->Next;
          
          Depth++;
          
          return tmp_LL;
     }
     
     //Resets the linked list.
     void reset()
     {
          c_NT3_Linked_List_MMap_2D * tmp_LL = Root;
          Root = NULL;
          c_NT3_Linked_List_MMap_2D * tmp_LL_Next = NULL;
          
          while (tmp_LL != NULL)
          {
               tmp_LL_Next = tmp_LL->Next;
               tmp_LL->Next = NULL;
               tmp_LL->Previous = NULL;
               delete tmp_LL;
               tmp_LL = NULL;
               tmp_LL = tmp_LL_Next;
          }
          
          delete Root;
          Root = NULL;
          Current_LL = &Root;
          
          Depth = 0;
     }
     
     /*
     //Bubble sorts the linked list.
     void bubble_Sort()
     {
          //Tmp vars
          c_NT3_Linked_List_MMap_2D * tmp_Prv = NULL;
          c_NT3_Linked_List_MMap_2D * tmp_Next = NULL;
          c_NT3_Linked_List_MMap_2D * tmp_Current = NULL;
          bool flg_Done = false;
          
          while(!flg_Done)
          {
               flg_Done = true;
               
               
          }
     }
      * */
};


class c_NT3_Buffer_Node_MMap_2D
{
     friend class c_NT3_Buffer_Tree_MMap_2D; 
     private:
          c_NT3_Buffer_Node_MMap_2D * Right;
          c_NT3_Buffer_Node_MMap_2D * Center;
          c_NT3_Buffer_Node_MMap_2D * Left;
          
     public:
          u_Data_3 Data;
          
          c_NT3_Linked_List_MMap_2D * LL;
          
          c_NT3_Buffer_Node_MMap_2D()
          {
               Right = NULL;
               Center = NULL;
               Left = NULL;
               LL = NULL;
               Data.I = 0;
          }
          
          ~c_NT3_Buffer_Node_MMap_2D()
          {
               LL = NULL;
               delete Right; Right = NULL;
               delete Center; Center = NULL;
               delete Left; Left = NULL;
          }
                
};
/*
class c_NT3_Buffer_Tree_MMap_2D
{
private:
             
             
     public:
          c_NT3_Buffer_Node_MMap_2D * Root;
          c_NT3_Buffer_Node_MMap_2D ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          
          //For handling the current search.
          u_Data_3 tmp_Data;
          u_Data_3 tmp_SData;
          
          c_NT3_Buffer_Tree_MMap_2D()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          ~c_NT3_Buffer_Tree_MMap_2D()
          {
               delete Root;
          }
          
          void init()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          //Now returning whether or not the node was found as the foundit flag does not work with multiple threads.
          //This returns the node at the end of the chain, whether it was found or not does not matter the 
          //buffer node is returned and has to be checked for values.
          c_NT3_Buffer_Node_MMap_2D * search(c_NT3_Base_Node_2D * p_Data)
          {
               flg_Foundit = false;
               
               if (p_Data == NULL){ Current = NULL; return NULL; }
               
               u_Data_3 tmp_SSData;
               tmp_SSData.NR = p_Data;
               
               
               
               //--cout << "\n\n SEARCHING ->" << tmp_SData.U;
               //system("PAUSE > NULL");
               return query_Node(Root, tmp_SSData, 31);
          }
          
          c_NT3_Buffer_Node_MMap_2D * query_Node(c_NT3_Buffer_Node_MMap_2D *&p_Node, u_Data_3 p_Data, int p_Shift)
          {
               //These are used because we have to be carefull not to leave Mutex locked with a return.
               c_NT3_Buffer_Node_MMap_2D * tmp_Return = NULL;
               bool flg_Done = false;
               
               u_Data_3 tmp_DData;
               tmp_DData.U = p_Data.U;
               tmp_DData.U = tmp_DData.U >> p_Shift;
               //--cout << "\n\n\n tmp_DData.U = " << tmp_DData.U << " p_Shift = " << p_Shift;
               
               
               //--cout << "\n\n\t __QN__ tmp_DData.U->" << tmp_DData.U << " p_Data.U->" << p_Data.U;
               
               Mutex.lock();
               if (p_Node == NULL)
               {      
                    //--cout << "\n\t   Node is NULL";
                    p_Node = new c_NT3_Buffer_Node_MMap_2D;
                    p_Node->Right = NULL;
                    p_Node->Center = NULL;
                    p_Node->Left = NULL;
                    if (p_Shift > 1)
                    {
                         //--cout << "\n\t   tmp_DData.U > 1";
                         p_Node->Data = tmp_DData;
                    }
                    else 
                    {
                         //--cout << "\n\t   tmp_DData.U !> 1";
                         p_Node->Data = p_Data;
                         Current = &p_Node; 
                         flg_Foundit = false;
                         tmp_Return = p_Node;
                         flg_Done = true;
                         //return 0;
                    }
               }
               Mutex.unlock();
               //If the node was not found then return NULL;
               if (flg_Done){ return tmp_Return; }
               
               //If the current data matches the tmp data then another depth is explored.
               if (p_Node->Data.U == tmp_DData.U && p_Shift > 0)
               {
                    //--cout << "\n\t   p_Node->Data.U (" << p_Node->Data.U << ") == tmp_DData.U (" << tmp_DData.U << ")";
                    
                    //--cout << "\t tmp_DData.U = " << tmp_DData.U;
                    
                    return query_Node(p_Node->Center, p_Data, (p_Shift - 1));
               }
               
               //If the node data matches the given data exactly the node has been found.
               //This is not locked because even if the Current & flg_Foundit are overwritten they will not be
               //used by the threads, but will work with the standard single thread.
               if (p_Node->Data.U == p_Data.U)
               {
                    //--cout << "\n\t    p_Node->Data.NR(" << p_Node->Data.NR << ") == p_Data.NR(" << p_Data.NR << ")";
                    Current = &p_Node;  
                    flg_Foundit = true;
                    tmp_Return = p_Node;
                    flg_Done = true;
                    //return 1;
               }
               
               //If the node was found then return tmp_Return;
               if (flg_Done){ return tmp_Return; }
               
               if (tmp_DData.U < p_Node->Data.U)
               {
                    //--cout << "\n\t    tmp_DData.U (" << tmp_DData.U << ") < p_Node->Data.U(" << p_Node->Data.U << ")";
                    return query_Node(p_Node->Left, p_Data, p_Shift);
               }
               if (tmp_DData.U > p_Node->Data.U)
               {
                    //--cout << "\n\t    tmp_DData.U (" << tmp_DData.U << ") > p_Node->Data.U(" << p_Node->Data.U << ")";
                    return query_Node(p_Node->Right, p_Data, p_Shift);
               }
               return tmp_Return;
          }
          
          void output_Tree()
          {
               output_Node(Root, 0);
          }
          
          void output_Node(c_NT3_Buffer_Node_MMap_2D *&p_Node, int p_Tab)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left, (p_Tab));
               //*cout << "\n";
               for (int cou_Index=0;cou_Index<p_Tab;cou_Index++)
               {
                    //*cout << "  ";
               }
               //*cout << (p_Node->Data).I;
               if (p_Node->LL != NULL){ cout << "->" << (p_Node->LL->NID.U); }
               output_Node(p_Node->Center, (p_Tab + 1));
               output_Node(p_Node->Right, (p_Tab));
          }
          
          c_NT3_Linked_List_MMap_2D * get_Node_LL(c_NT3_Buffer_Node_MMap_2D * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->LL;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_LL(c_NT3_Buffer_Node_MMap_2D * &p_Node, c_NT3_Linked_List_MMap_2D * p_LL)
          {
               if (p_Node != NULL)
               {
                    p_Node->LL = p_LL;
               }
          }
          
          c_NT3_Linked_List_MMap_2D * set_Current_Node_LL(c_NT3_Linked_List_MMap_2D * p_LL)
          {
               set_Node_LL(*Current, p_LL);
               return p_LL;
          }
          
          c_NT3_Linked_List_MMap_2D * get_Current_Node_LL()
          {
               return get_Node_LL(*Current);
          }
          
          
          //Resets the tree.
          void reset()
          {
               delete Root;
               Root = NULL;
               
               Current = NULL;
               flg_Foundit = false;
          }
                    
};
*/

