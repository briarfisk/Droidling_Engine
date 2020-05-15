//This contains the base construct polymorphic class.
class c_NT3_Base_Construct: public c_NT3_Base
{
private:
     
public:
     
     int X;
     int Y;
     
     virtual ~c_NT3_Base_Construct(){};
     
     ////==-----------------------+
     //==--   EVAL FUNCTIONS
     ////==-----------------------+
     
     virtual void Eval()=0; //Eval with no RC.
     virtual void Eval_RC()=0; //Eval with RC.
     
     ////==-----------------------+
     //==--   BUILDING FUNCTIONS
     ////==-----------------------+
     
     virtual void Build()=0; //Builds full CAN.
     virtual void Build_RC()=0; //Builds RC CAN.
     virtual void Build_Query()=0; //Builds for query.
     
     ////==--------------------------+
     //==--   DISCHARGING FUNCTIONS
     ////==--------------------------+
     
     //Discharges a given treetop.
     //virtual void discharge_Treetop_By_NID_L(int p_Input, u_Data_3 p_NID, u_Data_3 p_Charge)=0;
     
     ////==---------------------+
     //==--   OUTPUT FUNCTIONS
     ////==---------------------+
     
     //virtual void output_Input_Tables()=0; //Outputs the input tables.
     //virtual long long int get_Node_Count()=0; //Returns the number of nodes in the network.
     //virtual void output_Treetops()=0; //Outputs the treetops tree in the node network.
     //virtual void output_Treetops_BP()=0; //Outputs the treetops tree in a compressed BP format.
      
     ////==---------------------------------+
     //==--   SAVING AND LOADING FUNCTIONS
     ////==---------------------------------+
     
     virtual void Save(string p_DIR, string p_Name)=0; //Saves the construct.
     virtual void Load(string p_DIR, string p_Name)=0; //Loads a construct.     
};