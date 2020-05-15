
//For the lookup tree I will be using get() and set() functions to set and retrieve data as the array holding it will be dynamic.
class c_CMD_Variable_Node
{
     friend class c_Lookup_Tree; 
private:
     
     c_CMD_Variable_Node * Right;
     c_CMD_Variable_Node * Left;
     
     
     u_Data_3 * Data;
     int Data_Length;
     int Var_Type;
          
public:
     
     c_CMD_Variable_Node()
     {
          Data = NULL;
          Var_Type = 0;
          Data_Length = 0;
          Right = NULL;
          Left = NULL;
     }
     
     ~c_CMD_Variable_Node()
     {
          delete [] Data;
          Data = NULL;
          
          delete Right;
          Right = NULL;
          
          delete Left;
          Left = NULL;
     }
     
     string Var_Name;
     
     //--     SET FUNCTIONS FOR THE DATA
     
     //String
     void set_string(string p_Data)
     {
          delete [] Data;
		  if (p_Data.size() == 0) { Data = new u_Data_3[1]; Data[0].U = 0; return; }
          Data = new u_Data_3[p_Data.size()];
          
          //cout << "\n\n\t Setting Characters->";
		  if (p_Data.size() > 0)
		  {
			  for (unsigned int cou_Index = 0; cou_Index < p_Data.size(); cou_Index++)
			  {
				  Data[cou_Index].U = unsigned long long int (p_Data[cou_Index]);
				  //cout << "\n\t\t" << Data[cou_Index].I;
			  }
		  }
          //cout << "\n\t Finished Gathering...";
          Var_Type = 0;
          Data_Length = p_Data.size();
          
          //cout << "\n\t\t\t set_string(" << p_Data << ")";
     }
     void set_int(int p_Data)          { Data = new u_Data_3[1]; Data[0].I =  p_Data; Var_Type = 1; Data_Length = 1; }// cout << "\n\t\t\t set_int(" << p_Data << ")"; }
     void set_float(float p_Data)      { Data = new u_Data_3[1]; Data[0].F =  p_Data; Var_Type = 2; Data_Length = 1; }// cout << "\n\t\t\t set_float(" << p_Data << ")"; }
     void set_reference(void * p_Data) { Data = new u_Data_3[1]; Data[0].NR = p_Data; Var_Type = 3; Data_Length = 1; }// cout << "\n\t\t\t set_reference(" << p_Data << ")"; }
     void set_bool(bool p_Data)        { Data = new u_Data_3[1]; Data[0].U =  p_Data; Var_Type = 4; Data_Length = 1; }// cout << "\n\t\t\t set_bool(" << p_Data << ")"; }
	 
     void set_raw(string p_Var_Name, u_Data_3 p_Data[], int p_Data_Length, int p_Data_Type)
     {
          delete [] Data;
          Data = new u_Data_3[p_Data_Length];
          Data_Length = p_Data_Length; 
          Var_Type = p_Data_Type;
          Var_Name = p_Var_Name;
          
          for (int cou_Index=0;cou_Index<Data_Length;cou_Index++)
          {
               Data[cou_Index].NR = p_Data[cou_Index].NR;
          }
     }
     
     
     //--     GET FUNCTIONS FOR THE DATA
     
     string get_string()
     {    
          if (Data == NULL){ return "";   } 
          string tmp_String = ""; 
          
          //cout << "\n\n\t Gathering Characters->";
          for (int cou_Index=0;cou_Index<Data_Length;cou_Index++)
          {
               //cout << "\n\t\t" << Data[cou_Index].I;
               tmp_String += char (Data[cou_Index].I);
          }
          //cout << "\n\t Finished Gathering...";
          
          
          
          //cout << "\n\n\t [" << tmp_String << "]";
          
          
          return tmp_String;
     }
     
     long long int    get_int(){       if (Data == NULL){ return 0;    } return Data[0].I;}
     double  get_float(){     if (Data == NULL){ return 0.0;  } return Data[0].F;}
     void * get_reference(){ if (Data == NULL){ return NULL; } return Data[0].NR;}
     bool   get_bool(){      if (Data == NULL){ return 0;    } return Data[0].B;}
     
     //Outputs the current node.
     void output_Node()
     {
          cout << "\n\t\t " << this << " Var_Name->" << Var_Name << " Var_Type->" << Var_Type << "  Data_Length->" << Data_Length << "  Data->";
          if (Var_Type == 0){ cout << get_string(); }
          if (Var_Type == 1){ cout << get_int(); }
          if (Var_Type == 2){ cout << get_float(); }
          if (Var_Type == 3){ cout << get_reference(); }
          if (Var_Type == 4){ cout << get_bool(); }
     }
};

class c_Lookup_Tree
{
     private:
             
     public:
          c_CMD_Variable_Node * Root;
          c_CMD_Variable_Node ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          
          c_Lookup_Tree()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          ~c_Lookup_Tree()
          {
               delete Root;
               //Current does not get deleted as it only points to other nodes that may not exist after root is deleted and the node chain is gone.
               //delete *Current;
          }
          
          int search(string p_Var_Name)
          {
               flg_Foundit = false;
               return query_Name(Root, p_Var_Name);
          }
          
          int query_Name(c_CMD_Variable_Node *&p_Node, string p_Var_Name)
          {
               if (p_Node == NULL)
               {
                    p_Node = new c_CMD_Variable_Node;
                    p_Node->Right = NULL;
                    p_Node->Left = NULL;
                    p_Node->Var_Name = p_Var_Name;
                    p_Node->set_int(-1);
                    Current = &p_Node;
                    //cout << "\n\n\t New Variable Created";
                    return 0;
               }
               
               if (p_Node->Var_Name == p_Var_Name)
               {
                    Current = &p_Node;  
                    flg_Foundit = true;
                    return p_Node->Var_Type;
               }
                    
               if (p_Var_Name < p_Node->Var_Name)
               {
                    query_Name(p_Node->Left, p_Var_Name);
               }
                    
               if (p_Var_Name > p_Node->Var_Name)
               {
                    query_Name(p_Node->Right, p_Var_Name);
               }
                    
               return 0;
          }
          
          int exists(string p_Var_Name)
          {
               return exists_query_Name(Root, p_Var_Name);
          }
          
          int exists_query_Name(c_CMD_Variable_Node *&p_Node, string p_Var_Name)
          {
               if (p_Node == NULL)
               {
                    //cout << "\n\t Variable NOT Found->" << p_Var_Name;
                    return 0;
               }
               
               if (p_Node->Var_Name == p_Var_Name)
               {
                    //cout << "\n\t Variable Found->" << p_Var_Name;
                    return 1;
               }
                    
               if (p_Var_Name < p_Node->Var_Name)
               {
                    return exists_query_Name(p_Node->Left, p_Var_Name);
               }
                    
               if (p_Var_Name > p_Node->Var_Name)
               {
                    return exists_query_Name(p_Node->Right, p_Var_Name);
               }
                    
               return 0;
          }
          
          
          void * get_Bin_Node_Reference(string p_Var_Name)
          {
               flg_Foundit = false;
               return get_Bin_Node_Reference(Root, p_Var_Name);
          }
          
          void * get_Bin_Node_Reference(c_CMD_Variable_Node *&p_Node, string p_Var_Name)
          {
               if (p_Node == NULL)
               {
                    return NULL;
               }
               
               if (p_Node->Var_Name == p_Var_Name)
               {
                    cout << "\n\n\t Returning Address " << p_Node;
                    return ((void*) p_Node);
               }
                    
               if (p_Var_Name < p_Node->Var_Name)
               {
                    return get_Bin_Node_Reference(p_Node->Left, p_Var_Name);
               }
                    
               if (p_Var_Name > p_Node->Var_Name)
               {
                    return get_Bin_Node_Reference(p_Node->Right, p_Var_Name);
               }
                    
               return NULL;
          }
          
          int submit_Old_Node(c_CMD_Variable_Node *&p_Node)
          {
               return find_Spot_For_Old_Node(Root, p_Node);
          }
          
          int find_Spot_For_Old_Node(c_CMD_Variable_Node *&p_Node, c_CMD_Variable_Node *&p_Old_Node)
          {
               if (p_Node == NULL)
               {
                    p_Node = p_Old_Node;
                    Current = &p_Node;
                    return 0;
               }
               
               if (p_Node->Var_Name == p_Old_Node->Var_Name)
               {
                    Current = &p_Node;  
                    flg_Foundit = true;
                    
                    cout << "\n\n\t During Replacement A Node With The Same Variable Name Was Found.";
                    
                    return p_Node->Var_Type;
               }
                    
               if (p_Old_Node->Var_Name < p_Node->Var_Name)
               {
                    find_Spot_For_Old_Node(p_Node->Left, p_Old_Node);
               }
                    
               if (p_Old_Node->Var_Name > p_Node->Var_Name)
               {
                    find_Spot_For_Old_Node(p_Node->Right, p_Old_Node);
               }
                    
               return 0;
          }
          
          int null_Leg(c_CMD_Variable_Node *&p_Node, string p_Var_Name)
          {
               if (p_Node == NULL)
               {
                    return 0;
               }
               
               if (p_Node->Var_Name == p_Var_Name)
               {
                    return 0;
               }
                    
               if (p_Var_Name < p_Node->Var_Name)
               {
                    if ((p_Node->Left)->Var_Name == p_Var_Name)
                    {
                         delete p_Node->Left;
                         p_Node->Left = NULL;
                    }
                    
                    null_Leg(p_Node->Left, p_Var_Name);
               }
                    
               if (p_Var_Name > p_Node->Var_Name)
               {
                    if ((p_Node->Right)->Var_Name == p_Var_Name)
                    {
                         delete p_Node->Right;
                         p_Node->Right = NULL;
                    }
                    
                    null_Leg(p_Node->Right, p_Var_Name);
               }
                    
               return 0;
          }
          
          void remove_Node(c_CMD_Variable_Node *&p_Node)
          {
               //Get the Right leg and set the passed nodes right leg to NULL to avoid deletion.
               c_CMD_Variable_Node * tmp_Right = p_Node->Right;
               p_Node->Right = NULL;
               
               //Get the left leg and set the given nodes to NULL.
               c_CMD_Variable_Node * tmp_Left = p_Node->Left;
               p_Node->Left = NULL;
               
               //NULL the leg containing the passed node.
               null_Leg(Root, p_Node->Var_Name);
               
               //Submits a node that is already created to the tree.
               //Make shure that it is allocated in the first place, pass a NULL and receive and error.
               if (tmp_Left){ submit_Old_Node(tmp_Left); }
               if (tmp_Right){ submit_Old_Node(tmp_Right); }
          }
          
          void remove_Node(string p_Var_Name)
          {
               search(p_Var_Name);
               remove_Node(*Current);
          }
          
          void output_Tree()
          {
               output_Node(Root);
          }
          
          void output_Node(c_CMD_Variable_Node *&p_Node)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left);
               p_Node->output_Node();
               output_Node(p_Node->Right);
          }
          
          void output_Current_Node()
          {
               if (Current == NULL){ return; }
               output_Node(*Current);
          }
          
          
          void funger_Tree(void (*p_Function(string)))
          {
               funger_Node(Root, p_Function);
          }
          
          void funger_Node(c_CMD_Variable_Node *&p_Node, void (*p_Function(string)))
          {
               if (p_Node == NULL){ return; }
               funger_Node(p_Node->Left, p_Function);
               (*p_Function)(p_Node->Var_Name);
               funger_Node(p_Node->Right, p_Function);
          }
          
          //Gets the data type from the current node after searching the tree.
          int get_Var_Type(string p_Var_Name)            { search(p_Var_Name); return get_Var_Type(*Current);    }
          int get_Var_Type(c_CMD_Variable_Node * &p_Node){ if (p_Node != NULL){ return p_Node->Var_Type; } else { return 0; } }
          
          
          //Changes the name of a registered variable, if it already exists then I dunno, don't do that.
          string change_name(string p_Var_Name, string p_Var_New_Name){ search(p_Var_Name); change_name(*Current, p_Var_New_Name); return "1"; }
          string change_name(c_CMD_Variable_Node * &p_Node, string p_Var_New_Name){ p_Node->Var_Name = p_Var_New_Name; return "1"; }
          
          
          //Gets the data from the current node after searching the tree.
          string get_string(string p_Var_Name)    { search(p_Var_Name); return get_string(*Current);    }
          int    get_int(string p_Var_Name)       { search(p_Var_Name); return get_int(*Current);       }
          float  get_float(string p_Var_Name)     { search(p_Var_Name); return get_float(*Current);     }
          void * get_reference(string p_Var_Name) { search(p_Var_Name); return get_reference(*Current); }
          bool   get_bool(string p_Var_Name)      { search(p_Var_Name); return get_bool(*Current);      }
          
          string get_string(c_CMD_Variable_Node * &p_Node)    { if (p_Node != NULL){ return p_Node->get_string();    } else { return "";    } }
          int    get_int(c_CMD_Variable_Node * &p_Node)       { if (p_Node != NULL){ return p_Node->get_int();       } else { return -1;     } }
          float  get_float(c_CMD_Variable_Node * &p_Node)     { if (p_Node != NULL){ return p_Node->get_float();     } else { return -1.0;   } }
          void * get_reference(c_CMD_Variable_Node * &p_Node) { if (p_Node != NULL){ return p_Node->get_reference(); } else { return NULL;  } }
          bool   get_bool(c_CMD_Variable_Node * &p_Node)      { if (p_Node != NULL){ return p_Node->get_bool();      } else { return false; } }
          
          
          //Searches for the variable name given, if not found a node is created for it. After establishing which one the node is the data is set.
          void set_string(string p_Var_Name, string p_Data)    { search(p_Var_Name); set_string(*Current, p_Data);    return; }
          void set_int(string p_Var_Name, int p_Data)          { search(p_Var_Name); set_int(*Current, p_Data);       return; }
          void set_float(string p_Var_Name, float p_Data)      { search(p_Var_Name); set_float(*Current, p_Data);     return; }
          void set_reference(string p_Var_Name, void * p_Data) { search(p_Var_Name); set_reference(*Current, p_Data); return; }
          void set_bool(string p_Var_Name, bool p_Data)        { search(p_Var_Name); set_bool(*Current, p_Data);      return; }
          
          void set_string(c_CMD_Variable_Node * &p_Node, string p_Data)    { if (p_Node != NULL){ p_Node->set_string(p_Data);    } }
          void set_int(c_CMD_Variable_Node * &p_Node, int p_Data)          { if (p_Node != NULL){ p_Node->set_int(p_Data);       } }
          void set_float(c_CMD_Variable_Node * &p_Node, float p_Data)      { if (p_Node != NULL){ p_Node->set_float(p_Data);     } }
          void set_reference(c_CMD_Variable_Node * &p_Node, void * p_Data) { if (p_Node != NULL){ p_Node->set_reference(p_Data); } }
          void set_bool(c_CMD_Variable_Node * &p_Node, bool p_Data)        { if (p_Node != NULL){ p_Node->set_bool(p_Data);      } }
          
                    
};

