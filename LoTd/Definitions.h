#ifndef DEFINITIONS_H
#define DEFINITIONS_H

//This file contains definitions used in the NeuroTox.2 that
//are to small to justify a separate header file.

//The data type used for internal manipulation.
union u_Data
{
     int I;
     float F;
     char C;
     bool B;
     void * NR;
     //char binary[16];
};

//Stolen from >>>/g/68965893
//Defaults to setting the first 8 bits to the given byte.
uint64_t u_Data_3_Set_Byte(uint64_t number, uint64_t the_byte, uint8_t offset=56) 
{
    uint64_t mask = ~(0xFFULL << offset);
    return (number & mask) + (the_byte << offset);
}

uint64_t u_Data_3_Get_Byte(uint64_t number, uint8_t offset=56) 
{
    return (number >> offset);
}

uint64_t u_Data_3_Get_Unsalted(uint64_t number)
{
     uint64_t tmp_Num = number << 8;
     tmp_Num = tmp_Num >> 8;
     return tmp_Num;
}

//The union holding the IO data for the NT3 constructs.
union u_Data_3
{
     void * NR;
     long long int I;
     uint64_t U;
     double F;
     char C;
     
     //The u_Data_3 should be 64 bits long.
     unsigned char B[8];
};

//This is the linked list for the buffer.
class c_LoTd_Linked_List
{
public:
     
     //The next node in the chain.
     c_LoTd_Linked_List * Next;
     c_LoTd_Linked_List * Previous;
     u_Data_3 ID;
     u_Data_3 Data;
     
     c_LoTd_Linked_List()
     {
          Next = NULL;
          Previous = NULL;
          ID.U = 0;
          Data.U = 0;
     }
     
     ~c_LoTd_Linked_List()
     {
          ID.U = 0;
          Data.U = 0;
          //delete Next; Handled by the handler.
     }
     
     //Outputs the linked list.
     //-- !WARNING WILL CAUSE STACK CORRUPTION ERROR WITH ENOUGH NODES!
     void output_LL()
     {
          //*std::cout << " [";
          std::cout << "[" << ID.U << " " << Data.U << "]  ";
          if (Next != NULL){ Next->output_LL(); }
     }
};

//This handles the linked list creation and deletion so that recursive destruction does not crash the stack.
class c_LoTd_Linked_List_Handler
{
public:
     
     //The nodes in the linked list.
     c_LoTd_Linked_List * Root;
     c_LoTd_Linked_List ** Current_LL;
     long long int Depth;
     
     c_LoTd_Linked_List_Handler()
     {
          Root = NULL;
          Current_LL = &Root;
          Depth = 0;
     }
     
     ~c_LoTd_Linked_List_Handler()
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
     c_LoTd_Linked_List * new_LL(u_Data_3 p_ID, u_Data_3 p_Data)
     {
          c_LoTd_Linked_List * tmp_LL = NULL;
          
          *Current_LL = new c_LoTd_Linked_List;
          
          (*Current_LL)->ID = p_ID;
          (*Current_LL)->Data = p_Data;
          
          tmp_LL = *Current_LL;
          
          (*Current_LL)->Previous = tmp_LL;
          
          Current_LL = &(*Current_LL)->Next;
          
          Depth++;
          
          return tmp_LL;
     }

	 //Adds a node with a pointer as the data.
	 c_LoTd_Linked_List* new_LL(__int64 p_ID, void* p_Data)
	 {
		 u_Data_3 tmp_ID;
		 u_Data_3 tmp_Data;
		 tmp_ID.I = p_ID;
		 tmp_Data.NR = p_Data;
		 return new_LL(tmp_ID, tmp_Data);
	 }
     
     //Resets the linked list.
     void reset()
     {
          c_LoTd_Linked_List * tmp_LL = Root;
          Root = NULL;
          c_LoTd_Linked_List * tmp_LL_Next = NULL;
          
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
};



//--     SMALL COMMOMLY USED FUNCTIONS.


//Gets the datatype of the std::string submitted.
std::string gdt(std::string p_Data)
{
     // > 57 = char
     // < 48 && !46 = char
     // = 46 = float
     bool flg_Int = true;
     bool flg_Float = false;
     
     for (unsigned int cou_Index=0;cou_Index<(p_Data.size());cou_Index++)
     {
          if (int (p_Data[cou_Index]) > 57){ return "string"; }
          if (int (p_Data[cou_Index]) < 48 && int (p_Data[cou_Index]) != 46 && int (p_Data[cou_Index]) != 45){ return "string"; }
          if (int (p_Data[cou_Index]) == 45 && cou_Index > 0){ return "string"; }
          if (int (p_Data[cou_Index]) == 46){ flg_Float = true; flg_Int = false; }
     }
     if (flg_Float){ return "float"; }
     if (flg_Int){ return "int"; }
     return "string";
}
//Gets the datatype and returns an integer for the type rather than a std::string as gdt().
int gdti(std::string p_Data)
{
     // > 57 = char
     // < 48 && !46 = char
     // = 46 = float
     bool flg_Int = true;
     bool flg_Float = false;
     
     for (unsigned int cou_Index=0;cou_Index<(p_Data.size());cou_Index++)
     {
          if (int (p_Data[cou_Index]) > 57){ return 2; }
          if (int (p_Data[cou_Index]) < 48 && int (p_Data[cou_Index]) != 46 && int (p_Data[cou_Index]) != 45){ return 2; }
          if (int (p_Data[cou_Index]) == 45 && cou_Index > 0){ return 2; }
          if (int (p_Data[cou_Index]) == 46){ flg_Float = true; flg_Int = false; }
     }
     if (flg_Float){ return 1; }
     if (flg_Int){ return 0; }
     return 2;
}


//Takes a std::string and converts it to an array of u_Data.
void str2data(std::string p_String, u_Data p_Converted_Data[])
{
     
     //Read the std::string into the data array.
     for (unsigned int cou_Index=0;cou_Index<p_String.size();cou_Index++)
     {
          p_Converted_Data[cou_Index].I = int (p_String[cou_Index]);
     }
}

//Finds a power
int pow(int p_Base, int p_Exponet)
{
     int tmp_Base = p_Base;
     
     if (p_Exponet == 0){ return 1; }
     if (p_Exponet == 1){ return p_Base; }
     
     for (int cou_Index=1;cou_Index<p_Exponet;cou_Index++)
     {
          tmp_Base *= p_Base;
     }
     
     return tmp_Base;
}


//Takes a std::string and converts it to an integer.
int str2int(std::string p_String)
{
     if (gdt(p_String) != "float" && gdt(p_String) != "int"){ return 0; }
     
     std::stringstream tmp_Cin;
     tmp_Cin.flush();
     tmp_Cin << p_String;
     int tmp_Int;
     tmp_Cin >> tmp_Int;
     return tmp_Int;
}


//Takes a std::string and converts it to an integer.
unsigned long long  str2ullint(std::string p_String)
{
     if (gdt(p_String) != "float" && gdt(p_String) != "int"){ return 0; }
     
     std::stringstream tmp_Cin;
     tmp_Cin.flush();
     tmp_Cin << p_String;
     unsigned long long int tmp_Int;
     tmp_Cin >> tmp_Int;
     return tmp_Int;
}


float str2float(std::string p_String)
{
     if (gdt(p_String) != "float" && gdt(p_String) != "int"){ return 0.0; }
     
     std::stringstream tmp_Cin;
     tmp_Cin.flush();
     tmp_Cin << p_String;
     float tmp_Float;
     tmp_Cin >> tmp_Float;
     return tmp_Float;
}

//Takes a std::string and returns a bool.
bool str2bool(std::string p_String)
{
     if (p_String[0] == '0'){ return 0; }
     if (p_String[0] == '1'){ return 1; }
     if (p_String == "false"){ return 0; }
     if (p_String == "true"){ return 1; }
     return 0;
}

//Takes an int and returns a std::string.
std::string int2str(int p_Integer)
{
     std::stringstream tmp_Cin;
     tmp_Cin.flush();
     tmp_Cin << p_Integer;
     return tmp_Cin.str();
}
//Takes an int and returns a std::string.
std::string int2str(unsigned long long int p_Integer)
{
	 std::stringstream tmp_Cin;
	 tmp_Cin.flush();
	 tmp_Cin << p_Integer;
	 return tmp_Cin.str();
}

//Takes a float and returns a std::string.
std::string float2str(double p_Float)
{
     std::stringstream tmp_Cin;
     tmp_Cin << p_Float;
     return tmp_Cin.str();
}

//Takes a float and returns a std::string.
std::string dbl2str(double p_Double)
{
     std::stringstream tmp_Cin;
     tmp_Cin << p_Double;
     return tmp_Cin.str();
}

//Takes a bool and returns a std::string.
std::string bool2str(bool p_Bool)
{
     std::stringstream tmp_Cin;
     tmp_Cin << p_Bool;
     return tmp_Cin.str();
}

//Takes a bool and returns a std::string.
u_Data_3 poi2x_Tmp_Data;
std::string poi2str(void * p_Void)
{
     poi2x_Tmp_Data.NR = p_Void;
     return int2str(poi2x_Tmp_Data.U);
}

//Takes an adress and returns its value in base ten.
unsigned long long int poi2int(void * p_Void)
{
     poi2x_Tmp_Data.NR = p_Void;
     return poi2x_Tmp_Data.U;
}


//Gets the first digit of a given float.
int gfdf(double p_Data)
{
     if (p_Data < 1 && p_Data > 0)
     {
          while (p_Data < 1)
          {
               p_Data *= 10;
          }
     }
     while (p_Data >= 10)
     {
          p_Data *= .1;
     }
     
     return int(p_Data);
}

//Gets the first two digits of a given float.
int gf2df(double p_Data)
{
     if (p_Data < 1 && p_Data > 0)
     {
          while (p_Data < 10)
          {
               p_Data *= 10;
          }
     }
     while (p_Data >= 100)
     {
          p_Data *= .1;
     }
     
     return int(p_Data);
}

//Gets the first two digits of a given float.
int gf3df(double p_Data)
{
     if (p_Data < 1 && p_Data > 0)
     {
          while (p_Data < 100)
          {
               p_Data *= 10;
          }
     }
     while (p_Data >= 1000)
     {
          p_Data *= .1;
     }
     
     return int(p_Data);
}

//Gets the first digit of a given int.
int gfdi(double p_Data)
{
     while (p_Data >= 10)
     {
          p_Data *= .1;
     }
     
     return int(p_Data);
}

//Gets the first two digits of a given int.
int gf2di(double p_Data)
{
     while (p_Data >= 100)
     {
          p_Data *= .1;
     }
     
     return int(p_Data);
}

//Gets the first two digits of a given int.
int gf3di(double p_Data)
{
     while (p_Data >= 1000)
     {
          p_Data *= .1;
     }
     
     return int(p_Data);
}

//Gets the last digit of a given int.
int gldi(long int p_Data)
{
     if (p_Data < 10){ return p_Data; }
     
     int tmp_Remainder = p_Data;
     int tmp_Takaway = int ((p_Data * .1)) * 10;
     tmp_Remainder = tmp_Remainder - tmp_Takaway;
     return tmp_Remainder;
}

//Gets the last two digits of a given int.
int gl2di(long int p_Data)
{
     if (p_Data < 100){ return p_Data; }
     
     int tmp_Remainder = p_Data;
     int tmp_Takaway = int ((p_Data * .01)) * 100;
     tmp_Remainder = tmp_Remainder - tmp_Takaway;
     return tmp_Remainder;
}

//Gets the last two digits of a given int.
int gl3di(long int p_Data)
{
     if (p_Data < 1000){ return p_Data; }
     
     int tmp_Remainder = p_Data;
     int tmp_Takaway = int ((p_Data * .001)) * 1000;
     tmp_Remainder = tmp_Remainder - tmp_Takaway;
     //std::cout << "\n\t p_Data->" << p_Data << " tmp_R->" << tmp_Remainder << " tmp_T->" << tmp_Takaway;
     return tmp_Remainder;
}

//Gets the last digit of a given int.
u_Data gldd(u_Data p_Data)
{
     if (p_Data.I < 0){ p_Data.I *= -1; }
     
     if (p_Data.I < 10){ return p_Data; }
     
     u_Data tmp_Remainder;
     tmp_Remainder.I = p_Data.I;
	 p_Data.I = int(p_Data.I * .1);
	 p_Data.I = int(p_Data.I * 10);

     tmp_Remainder.I -= p_Data.I;
     if (tmp_Remainder.I > 0){ tmp_Remainder.I -= 1; }
     return tmp_Remainder;
}

//Gets the last two digits of a given int.
u_Data gl2dd(u_Data p_Data)
{
     if (p_Data.I < 0){ p_Data.I *= -1; }
     
     if (p_Data.I < 100){ return p_Data; }
     
     u_Data tmp_Remainder;
     tmp_Remainder.I = p_Data.I;
	 p_Data.I = int(p_Data.I * .01);
	 p_Data.I = int(p_Data.I * 100);
     tmp_Remainder.I -= p_Data.I;
     if (tmp_Remainder.I > 0){ tmp_Remainder.I -= 1; }
     return tmp_Remainder;
}

//Gets the last two digits of a given int.
u_Data gl3dd(u_Data p_Data)
{
     if (p_Data.I < 0){ p_Data.I *= -1; }
     
     if (p_Data.I < 1000){ return p_Data; }
     
     //std::cout << "\n ->" << p_Data.I;
     u_Data tmp_Remainder;
     tmp_Remainder.I = p_Data.I;
	 p_Data.I = int(p_Data.I * .001);
	 p_Data.I = int(p_Data.I * 1000);
     tmp_Remainder.I -= p_Data.I;
     //std::cout << " ->" << p_Data.I << " ->" << tmp_Remainder.I;
     if (tmp_Remainder.I > 0){ tmp_Remainder.I -= 1; }
     return tmp_Remainder;
}

//Generates a folder.
void make_Dir(std::string p_Dir)
{
	//Create the subfolder.
	std::string tmp_MD = "md " + p_Dir;
	system(tmp_MD.c_str());
}

#endif // !DEFINITIONS_H