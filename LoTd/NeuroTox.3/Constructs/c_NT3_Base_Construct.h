//This contains the base construct polymorphic class.
class c_NT3_Base_Construct: public c_NT3_Base
{
private:
     
public:
     
     //The ID of the construct.
     u_Data_3 CID;
     
     int X;
     int Y;
     int Dimension;
     
     //Tables
     c_Raw_Table_1D tbl_Input;
     
     c_Raw_Table_1D tbl_Input_Aux;
     c_Raw_Table_1D tbl_Input_Charges;
     c_Raw_Table_1D tbl_Input_Position;
     c_Raw_Table_1D tbl_Input_Depths;
     
     c_Raw_Table_1D tbl_Pattern_Output;
     c_Raw_Table_1D tbl_Salt_Output;
     c_Raw_Table_1D tbl_Charge_Output;
     c_Raw_Table_1D tbl_Position_Output;
     c_Raw_Table_1D tbl_RC_Output;
     
     c_Raw_Table_1D tbl_Vision_Output;
     c_Raw_Table_1D tbl_Vision_Charge_Output;
     
     c_Raw_Table_1D tbl_Treetops_Output;
     
     //Settings
     
     //--  Settings
     
     //Charging legs.
     bool setting_Charge_L;
     bool setting_Charge_R;
     
     //The tier to start charging.
     int settings_Tier;
     
     //Fixed vrs floating tier depth. If floating then Tier = (Input_Depth * Tier_Depth)
     int settings_Tier_Is_Floating;
     
     //The floating tier modifier.
     double settings_Tier_Depth;
     
     //The Action Potential Threshold as percentage of the base charge.
     double settings_Action_Potential_Threshold;
     
     //The base charge to apply from the CAN.
     int settings_Base_Charge;
     
     //The multiple to reduce each recursive charge by.
     double settings_Modifier_Charge;
     
     //Whether or not to use the input charges for charging.
     bool setting_Use_Input_Charges;
     
     //Build a vision or do not build a vision.
     bool settings_Build_Vision;
     
     //Sort the tables by charge.
     bool settings_Bubble_Sort_Output;
     
     //The percentage of the input string to charge starting at index 0.
     double settings_Input_Charging_Depth;
     
     //Cross CAN Charging, when charging a leg all nodes are charged in the CAN to the (L||R) of the current,
     //rather than just charging a column of the CAN.
     // 0 = Normal, 1 = Normal + TNT, 2 = Full Charge
     int settings_Cross_CAN_Charging;
     
     //Determines whether or not to stack outputs when discharging treetops from foreign constructs.
     int settings_Stack_Output_From_Foreign_Treetops;
     
     //The flag for using the charging spike in the buffer charging.
     int setting_Charge_Spike;
     
     //Whether or not to make a new cell in the output tables.
     int settings_Flat_Output;
     
     //This determines the method for handling the inputs and outputs from the tables.
     //0: Build each cell as a single input and reads the treetop into the corresponding output cell.
     //1: Buids all the cells together as a single input, [1] [2] [3] would be read in as [123]. Used for MSC.
     //2: Build each column as a single input, useful for multiple construct treetop input to MSC. 
     // [a] [b] [c]
     // [q] [w] [e]
     //Read in as aq, bw, ce and output as [Tt:aq] [Tt:bw] [Tt:ce]
     int settings_Build_Method;
     //0: Evals each cell as a single input and appends the outputs to the same row as the input row.
     //1: Evals a whole row as a single input, [1] [2] [3] would be read in as [123]. Used for MSC.
     int settings_Eval_Method;
     
     //Whether or not to use RC.
     int settings_RC;
     
     virtual ~c_NT3_Base_Construct(){};
     
     ////==-----------------------+
     //==--   API FUNCTIONS
     ////==-----------------------+
     
     virtual c_Raw_Table_1D * get_Table_1D(string p_Table)=0;
     virtual c_Raw_Table_2D * get_Table_2D(string p_Table)=0;
     
     virtual void set_Name(string p_Name)=0;
     virtual void set_CID(unsigned long long int p_CID)=0;
     
	 virtual int first_Run(string p_Dir, string p_Name)=0;
	 virtual int init(string p_Dir, string p_Name)=0;

     //Used to reset the IO tables of the construct.
     virtual void reset_IO()=0;
     
     ////==-----------------------+
     //==--   EVAL FUNCTIONS
     ////==-----------------------+
     
     virtual void Eval(int p_Discharge)=0; //Eval with no RC.
     
     ////==-----------------------+
     //==--   BUILDING FUNCTIONS
     ////==-----------------------+
     
     virtual void Build()=0; //Builds full CAN.
     virtual void Build_Query()=0; //Builds for query.
     
     ////==--------------------------+
     //==--   DISCHARGING FUNCTIONS
     ////==--------------------------+
     
     //Discharges a given treetop.
     //virtual void discharge_Treetop_By_NID_L(int p_Input, u_Data_3 p_NID, u_Data_3 p_Charge)=0;
     
     //Discharges the treetops in the treetops_output table. Used for discharging treetops backpropagated from higher constructs.
     virtual void Discharge()=0;
     
     ////==---------------------+
     //==--   OUTPUT FUNCTIONS
     ////==---------------------+
     
     virtual void output_Input_Tables()=0; //Outputs the input tables.
     virtual void output_Output_Tables()=0; //Outputs the output tables.
     virtual void output_CAN()=0; //Outputs the CAN.
     virtual long long int get_Node_Count()=0; //Returns the number of nodes in the network.
     virtual void output_Treetops()=0; //Outputs the treetops tree in the node network.
     virtual void output_Treetops_BP()=0; //Outputs the treetops tree in a compressed BP format.
     virtual void output_Nodes_As_HTML(string p_Dir, string p_File, int p_X_Padd, int p_Y_Padd)=0; //Outputs the node network to a HTML for visual output.
      
     ////==---------------------------------+
     //==--   SAVING AND LOADING FUNCTIONS
     ////==---------------------------------+
     
     virtual void Save(string p_DIR, string p_Name)=0; //Saves the construct.
     virtual void Load(string p_DIR, string p_Name)=0; //Loads a construct.     
};