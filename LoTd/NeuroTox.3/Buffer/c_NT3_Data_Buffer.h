//This is the linked list for the buffer.
class c_NT3_Linked_Data_List
{
public:
     
     //The next node in the chain.
     c_NT3_Linked_Data_List * Next;
     u_Data_3 Charge;
     u_Data_3 NID;
     
     c_NT3_Linked_Data_List()
     {
          Next = NULL;
          Charge.I = 0;
          NID.U = 0;
     }
     
     ~c_NT3_Linked_Data_List()
     {
          Charge.I = 0;
          NID.U = 0;
          //delete Next; Handled by the handler.
     }
     
     //Outputs the linked list.
     void output_LL()
     {
          std::cout << " [";
          std::cout << NID.U << " ";
          std::cout << NID.C;
          std::cout << " $" << Charge.I << "]  ";
          if (Next != NULL){ Next->output_LL(); }
     }
};

//This handles the linked list creation and deletion so that recursive destruction does not crash the stack.
class c_NT3_Linked_Data_List_Handler
{
public:
     
     //The nodes in the linked list.
     c_NT3_Linked_Data_List * Root;
     c_NT3_Linked_Data_List ** Current_LL;
     
     c_NT3_Linked_Data_List_Handler()
     {
          Root = NULL;
          Current_LL = &Root;
     }
     
     ~c_NT3_Linked_Data_List_Handler()
     {
          reset();
     }
     
     //Adds a node.
     c_NT3_Linked_Data_List * new_LL(u_Data_3 p_NID, long long int p_Charge)
     {
          c_NT3_Linked_Data_List * tmp_LL = NULL;
          
          *Current_LL = new c_NT3_Linked_Data_List;
          
          (*Current_LL)->Charge.I = p_Charge;
          (*Current_LL)->NID = p_NID;
          
          tmp_LL = *Current_LL;
          
          Current_LL = &(*Current_LL)->Next;
          
          return tmp_LL;
     }
     
     //Resets the linked list.
     void reset()
     {
          c_NT3_Linked_Data_List * tmp_LL = Root;
          Root = NULL;
          c_NT3_Linked_Data_List * tmp_LL_Next = NULL;
          
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
          Current_LL = &Root;
     }
     
};


class c_NT3_Data_Buffer_Node
{
     friend class c_NT3_Data_Buffer_Tree; 
     private:
          c_NT3_Data_Buffer_Node * Right;
          c_NT3_Data_Buffer_Node * Center;
          c_NT3_Data_Buffer_Node * Left;
          
     public:
          u_Data_3 Data;
          
          c_NT3_Linked_Data_List * LL;
          
          c_NT3_Data_Buffer_Node()
          {
               Right = NULL;
               Center = NULL;
               Left = NULL;
               LL = NULL;
               Data.I = 0;
          }
          
          ~c_NT3_Data_Buffer_Node()
          {
               LL = NULL;
               delete Right; Right = NULL;
               delete Center; Center = NULL;
               delete Left; Left = NULL;
          }
                
};

class c_NT3_Data_Buffer_Tree
{
private:
             
             
     public:
          c_NT3_Data_Buffer_Node * Root;
          c_NT3_Data_Buffer_Node ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          
          //For handling the current search.
          u_Data_3 tmp_Data;
          u_Data_3 tmp_SData;
          
          c_NT3_Data_Buffer_Tree()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          ~c_NT3_Data_Buffer_Tree()
          {
               delete Root;
          }
          
          void search(u_Data_3 p_Data)
          {
               flg_Foundit = false;
               
               tmp_Data.U = 0;
               
               query_Node(Root, p_Data, 63);
          }
          
          int query_Node(c_NT3_Data_Buffer_Node *&p_Node, u_Data_3 p_Data, int p_Shift)
          {
               tmp_Data.U = p_Data.U >> p_Shift;
               //std::cout << "\n\n\n tmp_Data.U = " << tmp_Data.U << " p_Shift = " << p_Shift;
               
               
               //std::cout << "\n\n\t __QN__ tmp_Data.U->" << tmp_Data.U << " p_Data.U->" << p_Data.U;
               
               if (p_Node == NULL)
               {      
                    //std::cout << "\n\t   Node is NULL";
                    p_Node = new c_NT3_Data_Buffer_Node;
                    p_Node->Right = NULL;
                    p_Node->Center = NULL;
                    p_Node->Left = NULL;
                    if (p_Shift > 1)
                    {
                         //std::cout << "\n\t   tmp_Data.U > 1";
                         p_Node->Data = tmp_Data;
                    }
                    else 
                    {
                         //std::cout << "\n\t   tmp_Data.U !> 1";
                         p_Node->Data = p_Data;
                         Current = &p_Node; 
                         flg_Foundit = false;
                         return 0;
                    }
               }
               
               //If the current data matches the tmp data then another depth is explored.
               if (p_Node->Data.U == tmp_Data.U && p_Shift > 0)
               {
                    //std::cout << "\n\t   p_Node->Data.U (" << p_Node->Data.U << ") == tmp_Data.U (" << tmp_Data.U << ")";
                    
                    //std::cout << "\t tmp_Data.U = " << tmp_Data.U;
                    
                    query_Node(p_Node->Center, p_Data, (p_Shift - 2));
                    return 0;  
               }
               
               //If the node data matches the given data exactly the node has been found.
               if (p_Node->Data.U == p_Data.U)
               {
                    //std::cout << "\n\t    p_Node->Data.NR(" << p_Node->Data.NR << ") == p_Data.NR(" << p_Data.NR << ")";
                    Current = &p_Node;  
                    flg_Foundit = true;
                    return 1;
               }
               
               
               if (tmp_Data.U < p_Node->Data.U)
               {
                    //std::cout << "\n\t    tmp_Data.U (" << tmp_Data.U << ") < p_Node->Data.U(" << p_Node->Data.U << ")";
                    query_Node(p_Node->Left, p_Data, p_Shift);
                    return 0;
               }
               if (tmp_Data.U > p_Node->Data.U)
               {
                    //std::cout << "\n\t    tmp_Data.U (" << tmp_Data.U << ") > p_Node->Data.U(" << p_Node->Data.U << ")";
                    query_Node(p_Node->Right, p_Data, p_Shift);
                    return 0;
               }
               return 0;
          }
          
          void output_Tree()
          {
               output_Node(Root, 0);
          }
          
          void output_Node(c_NT3_Data_Buffer_Node *&p_Node, int p_Tab)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left, (p_Tab));
               std::cout << "\n";
               for (int cou_Index=0;cou_Index<p_Tab;cou_Index++)
               {
                    std::cout << "  ";
               }
               std::cout << (p_Node->Data).I;
               if (p_Node->LL != NULL){ std::cout << "->" << (p_Node->LL->NID.U); }
               output_Node(p_Node->Center, (p_Tab + 1));
               output_Node(p_Node->Right, (p_Tab));
          }
          
          c_NT3_Linked_Data_List * get_Node_LL(c_NT3_Data_Buffer_Node * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->LL;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_LL(c_NT3_Data_Buffer_Node * &p_Node, c_NT3_Linked_Data_List * p_LL)
          {
               if (p_Node != NULL)
               {
                    p_Node->LL = p_LL;
               }
          }
          
          c_NT3_Linked_Data_List * set_Current_Node_LL(c_NT3_Linked_Data_List * p_LL)
          {
               set_Node_LL(*Current, p_LL);
               return p_LL;
          }
          
          c_NT3_Linked_Data_List * get_Current_Node_LL()
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



//The class that houses the charging buffer tree and the LL.
class c_NT3_Data_Buffer
{
private:
     
public:
     
     //The search tree.
     c_NT3_Data_Buffer_Tree Tree;
     
     //The linked lists.
     c_NT3_Linked_Data_List_Handler Data_Buffer;
     
     //Flags
     bool flg_Not_Done;
     
     int Input_Position;
     
     //Settings & trackers for the settings.
     long long int Base_Charge;
     long long int Current_Highest_Charge;
     long long int Previous_Highest_Charge;
     
     double Modifier_Charge;
     double Action_Potential_Threshold;
     
     
     c_NT3_Data_Buffer()
     {
          flg_Not_Done = 0;
          Input_Position = 0;
          
          Current_Highest_Charge = 0;
          Previous_Highest_Charge = 0;
          Base_Charge = 0;
          Modifier_Charge = 0.0f;
          Action_Potential_Threshold = 0.0f;
     }
     
     ~c_NT3_Data_Buffer()
     {
     }
     
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
          
          if (!Tree.flg_Foundit)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               Tree.set_Current_Node_LL(Data_Buffer.new_LL(p_Node, p_Charge));
          }
     }
     
     //Outputs the buffer.
     void output_Buffer()
     {
          output_Charge_LL();
     }
     
     //Outputs the Data_Buffer_LL.
     void output_Charge_LL()
     {
          std::cout << "\n\t Data_Buffer_LL->";
          if (Data_Buffer.Root == NULL){ std::cout << "empty..."; return; }
          Data_Buffer.Root->output_LL();
     }
};