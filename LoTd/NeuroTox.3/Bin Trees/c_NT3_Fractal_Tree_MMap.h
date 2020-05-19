#ifndef c_NT3_FRACTAL_TREE_MMAP_H
#define c_NT3_FRACTAL_TREE_MMAP_H

class c_NT3_Fractal_State_Node_MMap
{
     friend class c_NT3_Fractal_State_Tree_MMap; 
     private:
          c_NT3_Fractal_State_Node_MMap * Right;
          c_NT3_Fractal_State_Node_MMap * Center;
          c_NT3_Fractal_State_Node_MMap * Left;
          
     public:
          u_Data_3 Data;
          u_Data_3 NID;
          
          c_NT3_Fractal_State_Node_MMap()
          {
               Right = NULL;
               Center = NULL;
               Left = NULL;
               Data.I = 0;
          }
          
          ~c_NT3_Fractal_State_Node_MMap()
          {
               delete Right; Right = NULL;
               delete Center; Center = NULL;
               delete Left; Left = NULL;
          }
                
};

class c_NT3_Fractal_State_Tree_MMap
{
private:
             
             
     public:
          c_NT3_Fractal_State_Node_MMap * Root;
          c_NT3_Fractal_State_Node_MMap ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          u_Data_3 tmp_Data;
          
          c_NT3_Fractal_State_Tree_MMap()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
               
               tmp_Data.U = 0;
          }
          
          ~c_NT3_Fractal_State_Tree_MMap()
          {
               delete Root;
          }
          
          void reset()
          {
               delete Root;
               Root = NULL;
          }
          
          u_Data_3 tmp_Search_Data;
          void search(unsigned long long int p_Data)
          {
               flg_Foundit = false;
               
               tmp_Search_Data.U = p_Data;
               tmp_Data.U = 0;
               
               query_Node(Root, tmp_Search_Data, 63);
          }
          
          void search(u_Data_3 p_Data)
          {
               //---std::cout << "\n\n Fractal Search " << p_Data.U;
               flg_Foundit = false;
               
               tmp_Data.U = 0;
               
               query_Node(Root, p_Data, 63);
          }
          
          int query_Node(c_NT3_Fractal_State_Node_MMap *&p_Node, u_Data_3 p_Data, int p_Shift)
          {
               tmp_Data.U = p_Data.U >> p_Shift;
               
               //---std::cout << "\n\t __QN__ tmp_Data.U->" << tmp_Data.U << " p_Data.U->" << p_Data.U << " p_Shift->" << p_Shift;
               
               if (p_Node == NULL)
               {      
                    //---std::cout << "\n\t   Node is NULL";
                    p_Node = new c_NT3_Fractal_State_Node_MMap;
                    p_Node->Right = NULL;
                    p_Node->Center = NULL;
                    p_Node->Left = NULL;
                    p_Node->NID.U = 0;
                    if (p_Shift >= 1)
                    {
                         //---std::cout << "\n\t   p_Shift >= 1";
                         p_Node->Data = tmp_Data;
                    }
                    else 
                    {
                         //---std::cout << "\n\t   p_Shift == 1";
                         p_Node->Data = p_Data;
                         Current = &p_Node; 
                         flg_Foundit = false;
                         return 0;
                    }
               }
               
               //If the current data matches the tmp data then another depth is explored.
               if (p_Node->Data.U == tmp_Data.U && p_Shift > 0)
               {
                    //---std::cout << "\n\t   p_Node->Data.U (" << p_Node->Data.U << ") == tmp_Data.U (" << tmp_Data.U << ")";
                    //if (p_Shift == 1){ query_Node(p_Node->Center, p_Data, 0); return 0; }
                    query_Node(p_Node->Center, p_Data, (p_Shift - 1));
                    return 0;  
               }
               
               //If the node data matches the given data exactly the node has been found.
               if (p_Node->Data.U == p_Data.U && p_Shift <= 0)
               {
                    //---std::cout << "\n\t    p_Node->Data.NR(" << p_Node->Data.NR << ") == p_Data.NR(" << p_Data.NR << ")";
                    Current = &p_Node;  
                    if (p_Node->NID.U != 0){ flg_Foundit = true; }
                    return 0;
               }
               
               
               if (tmp_Data.U < p_Node->Data.U)
               {
                    //---std::cout << "\n\t    tmp_Data.U (" << tmp_Data.U << ") < p_Node->Data.U(" << p_Node->Data.U << ")";
                    query_Node(p_Node->Left, p_Data, (p_Shift - 1));
                    return 0;
               }
               if (tmp_Data.U > p_Node->Data.U)
               {
                    //---std::cout << "\n\t    tmp_Data.U (" << tmp_Data.U << ") > p_Node->Data.U(" << p_Node->Data.U << ")";
                    query_Node(p_Node->Right, p_Data, (p_Shift - 1));
                    return 0;
               }
               return 0;
          }
          
          
          
          void output_Tree()
          {
               output_Node(Root, 0);
          }
          
          void output_Node(c_NT3_Fractal_State_Node_MMap *&p_Node, int p_Tab)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left, (p_Tab));
               std::cout << "\n";
               for (int cou_Index=0;cou_Index<p_Tab;cou_Index++)
               {
                    std::cout << "  ";
               }
               std::cout << (p_Node->Data).I << "->" << (p_Node->NID.U);
               output_Node(p_Node->Center, (p_Tab + 1));
               output_Node(p_Node->Right, (p_Tab));
          }
          /*
          void output_Tree_BP()
          {
               std::cout << "\n\t Fractal Tree Backpropagation Station";
               output_Node_BP(Root);
          }
          
          void output_Node_BP(c_NT3_Fractal_State_Node_MMap *&p_Node)
          {
               if (p_Node == NULL){ return; }
               output_Node_BP(p_Node->Left);
               if ((p_Node->NID.U) != NULL){ std::cout << "\n\t" << (p_Node->Data).I << "->" << (p_Node->NID); (p_Node->NID)->bp_O(); }
               output_Node_BP(p_Node->Center);
               output_Node_BP(p_Node->Right);
          }
          */
          unsigned long long int get_Node_NID(c_NT3_Fractal_State_Node_MMap * &p_Node)
          {
               if (p_Node != NULL)
               {     
                    return p_Node->NID.U;      
               } else {
                    return 0;
               }
          }
          
          void set_Node_NID(c_NT3_Fractal_State_Node_MMap * &p_Node, u_Data_3 p_NID)
          {
               if (p_Node != NULL)
               {
                    p_Node->NID = p_NID;
               }
          }
          
          unsigned long long int set_Current_Node_NID(u_Data_3 p_NID)
          {
               set_Node_NID(*Current, p_NID);
               return p_NID.U;
          }
          
          unsigned long long int get_Current_Node_NID()
          {
               return get_Node_NID(*Current);
          }
                    
};

#endif // !c_NT3_FRACTAL_TREE_MMAP_H