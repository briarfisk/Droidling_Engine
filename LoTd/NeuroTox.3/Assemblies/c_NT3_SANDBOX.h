

//This class is the MSC for working with multiple inputs.
class c_NT3_SANDBOX
{
private:

	static int stat_MAX_CONSTRUCTS;

	//The Discharging_Buffers. One for each input.
	c_NT3_Data_Buffer* Discharging_Buffers;

	//Name of the sandbox.
	std::string Name;


public:

	//Whether the sandbox is initialized or not.
	bool flg_Initialized;

	//The directory that the sandbox is stored in.
	std::string DIR;

	//The names of the constructs in the assembly.
	std::string Construct_Names[1000];

	//Dimension of construct.
	int Construct_Types[1000];

	//The tier of each construct.
	int Construct_Tiers[1000];

	//The current highest tier, used to calculate position of outputs.
	int Construct_Highest_Tier;

	//The constructs in the assembly.
	c_NT3_Base_Construct* Constructs[1000];

	//The current number of constructs.
	int Construct_Count;

	//The positions of the constructs.
	COORD Construct_Positions[1000];

	//Whether or not a construct had outputs.
	int Construct_Has_Output[1000];

	//The pipe handler.
	c_NT3_Pipe_Handler Data_Pipes;

	//Inputs.
	c_NT3_IO Inputs[1000];

	//The current number of inputs.
	int Input_Count;

	//The current input types.
	int Input_Dimensions[1000];

	//Outputs.
	c_NT3_IO Outputs[1000];

	//The current number of outputs.
	int Output_Count;

	//The output dimensions.
	int Output_Dimensions[1000];


	//Input label attachment points, set by the higher classes which deal with the labels.
	//[0: left of string, 1: right of string]
	int Inputs_Attachment_Point[1000][2];
	int Construct_Attachment_Point[1000][2];
	int Output_Attachment_Point[1000][2];

	//The Register, tracks every input, construct, output, and any other objects with an ID. Used for referencing the objects a pipe attaches too.
	int Register[10000];
	int Register_Type[10000];
	int Register_Current_Count;

	//The command parser that is passed to the sandbox. If no command parser is passed one is created.
	///c_NT3_Command_Parser * CMD;

	//Tables for the Eval, Build, and General Use



	c_NT3_SANDBOX()//(c_NT3_Command_Parser * p_CMD)
	{
		std::cout << "\n c_NT3_SANDBOX"; std::cout.flush();
		Name = "Generic_Sandbox_Assembly";
		Discharging_Buffers = NULL;

		for (int cou_Index = 0; cou_Index < 1000; cou_Index++)
		{
			Construct_Names[cou_Index] = ("Construct_" + int2str(cou_Index));
			Constructs[cou_Index] = NULL;
			Construct_Positions[cou_Index].X = 0;
			Construct_Positions[cou_Index].Y = 0;
			Construct_Has_Output[cou_Index] = 0;
			Construct_Types[cou_Index] = 0;
			Construct_Tiers[cou_Index] = 0;
			Construct_Highest_Tier = 0;

			Input_Dimensions[cou_Index] = 0;
			Output_Dimensions[cou_Index] = 0;

			Inputs[cou_Index].reset();
			Outputs[cou_Index].reset();

			Inputs_Attachment_Point[cou_Index][0] = 0;
			Inputs_Attachment_Point[cou_Index][1] = 0;
			Construct_Attachment_Point[cou_Index][0] = 0;
			Construct_Attachment_Point[cou_Index][1] = 0;
			Output_Attachment_Point[cou_Index][0] = 0;
			Output_Attachment_Point[cou_Index][1] = 0;
		}

		for (int cou_Index = 0; cou_Index < 10000; cou_Index++)
		{
			Register[cou_Index] = 0;
			Register_Type[cou_Index] = 0;
		}

		Construct_Count = 0;
		Input_Count = 0;
		Output_Count = 0;
		Register_Current_Count = 0;
		//CMD = p_CMD;
		DIR = ".";

		//The sandbox is not initialized yet.
		flg_Initialized = false;
	}

	~c_NT3_SANDBOX()
	{
		std::cout << "\n ~c_NT3_SANDBOX " << Name << " " << this << ".........."; std::cout.flush();

		for (int cou_Index = 0; cou_Index < Construct_Count; cou_Index++)
		{
			delete Constructs[cou_Index];
			Constructs[cou_Index] = NULL;
		}
		std::cout << "\n ~~~c_NT3_SANDBOX " << Name << " " << this << ".........."; std::cout.flush();
	}

	//Sets the name of the assembly.
	void set_Name(std::string p_Name)
	{
		Name = p_Name;
	}




	//===-----------------------------+
	//==--  Creation and Destruction
	//===-----------------------------+

	//Creates a sandbox.
	//The name is what is used to save it and initialize new constructs.
	void create_Sandbox(std::string p_Name)
	{
		//Set the name of the sandbox.
		Name = p_Name;

		//Save the sandbox to setup the directories.
		Save();
	}

	//Initialize all the constructs.
	void init_Constructs()
	{
		/*
		std::string tmp_Name = "";
		std::string tmp_Dir = "NT3.Save/" + Name + "/";
		std::string tmp_MK_Dir = "md \"" + tmp_Dir + "\"";
		std::cout << "\n " << tmp_MK_Dir;

		std::cout << "\n Saving constructs to \"" << tmp_Dir << "\"";

		system(tmp_MK_Dir.c_str());

		for (int cou_Index = 0; cou_Index < Construct_Count; cou_Index++)
		{
			std::cout << "\n  Saving Construct[" << cou_Index << "]";
			Constructs[cou_Index]->Save(tmp_Dir, "NO_NAME");
		}*/
		Save();
	}

	//Loads a construct.
	void load_Construct(std::string p_Name, int p_Type = 1, int p_Tier = 1)
	{
		ostr(0, 7, "\n void load_Construct"); ostr(0, 12, "("); std::cout << p_Name; ostr(0, 12, ", "); std::cout << p_Type; ostr(0, 12, ", "); std::cout << p_Tier; ostr(0, 12, ")");
		std::string tmp_Dir = "NT3.Save/" + Name;
		ostr(0, 8, "\n   loading from:"); std::cout << tmp_Dir;

		if (p_Type == 0)
		{
			Constructs[Construct_Count] = new c_NT3_Construct_MMap_1D;
			Constructs[Construct_Count]->settings_Build_Method = 1;
			Constructs[Construct_Count]->settings_Eval_Method = 1;
		}
		if (p_Type == 1)
		{
			Constructs[Construct_Count] = new c_NT3_Construct_MMap_1D;
		}
		if (p_Type == 2)
		{
			//Constructs[Construct_Count] = new c_NT3_Construct_2D;
		}
		if (p_Type == 3)
		{
			//Constructs[Construct_Count] = new c_NT3_Construct_3D;
		}
		Construct_Names[Construct_Count] = p_Name;
		Constructs[Construct_Count]->set_Name(p_Name);
		Construct_Types[Construct_Count] = p_Type;
		Construct_Tiers[Construct_Count] = p_Tier;
		if (p_Tier > Construct_Highest_Tier) { Construct_Highest_Tier = p_Tier; }
		Constructs[Construct_Count]->set_CID(((unsigned long long int) Construct_Count));

		std::cout << "\n" << Constructs[Construct_Count]->CID.U;

		Construct_Positions[Construct_Count].X = (p_Tier * 128);
		Constructs[Construct_Count]->X = (p_Tier * 16);

		Construct_Positions[Construct_Count].Y = (Construct_Count * 32);
		Constructs[Construct_Count]->Y = (Construct_Count * 32);

		//Init the construct with directory and save spot.
		Constructs[Construct_Count]->init(tmp_Dir, p_Name);

		Register[Register_Current_Count] = Construct_Count;
		Register_Type[Register_Current_Count] = NT3CONSTRUCT1D; //2 is for a construct.
		Register_Current_Count++;

		//All constructs, inputs, and outputs are held in a bin tree with universal IDs

		Construct_Count++;
	}

	//Create new construct.
	void create_Construct(std::string p_Name, int p_Type = 1, int p_Tier = 1)
	{
		ostr(0, 7, "\n void create_Construct"); ostr(0, 12, "("); std::cout << p_Name; ostr(0, 12, ", "); std::cout << p_Type; ostr(0, 12, ", "); std::cout << p_Tier; ostr(0, 12, ")");

		std::string tmp_Dir = "NT3.Save/" + Name;

		if (p_Type == 0)
		{
			Constructs[Construct_Count] = new c_NT3_Construct_MMap_1D;
			Constructs[Construct_Count]->settings_Build_Method = 1;
			Constructs[Construct_Count]->settings_Eval_Method = 1;
		}
		if (p_Type == 1)
		{
			Constructs[Construct_Count] = new c_NT3_Construct_MMap_1D;
		}
		if (p_Type == 2)
		{
			//Constructs[Construct_Count] = new c_NT3_Construct_2D;
		}
		if (p_Type == 3)
		{
			//Constructs[Construct_Count] = new c_NT3_Construct_3D;
		}
		Construct_Names[Construct_Count] = p_Name;
		Constructs[Construct_Count]->set_Name(p_Name);
		Construct_Types[Construct_Count] = p_Type;
		Construct_Tiers[Construct_Count] = p_Tier;
		if (p_Tier > Construct_Highest_Tier) { Construct_Highest_Tier = p_Tier; }
		Constructs[Construct_Count]->set_CID(((unsigned long long int) Construct_Count));

		std::cout << "\n" << Constructs[Construct_Count]->CID.U;

		Construct_Positions[Construct_Count].X = (p_Tier * 128);
		Constructs[Construct_Count]->X = (p_Tier * 128);

		Construct_Positions[Construct_Count].Y = (Construct_Count * 32);
		Constructs[Construct_Count]->Y = (Construct_Count * 32);

		//Init the construct with directory and save spot.
		Constructs[Construct_Count]->first_Run(tmp_Dir, p_Name);

		Register[Register_Current_Count] = Construct_Count;
		Register_Type[Register_Current_Count] = NT3CONSTRUCT1D; //2 is for a construct.
		Register_Current_Count++;

		//All constructs, inputs, and outputs are held in a bin tree with universal IDs

		Construct_Count++;
	}

	//Creates a new input and adds it to the input table.
	//Returns the index of the new input.
	int create_Input(int p_Dimension = 1)
	{
		ostr(0, 7, "\n void create_Input"); ostr(0, 12, "("); std::cout << p_Dimension; ostr(0, 12, ")");

		int tmp_Type = -1;
		if (p_Dimension == 1)
		{
			Input_Dimensions[Input_Count] = 1;
			Inputs[Input_Count].X = 0;
			Inputs[Input_Count].Y = (Input_Count * 8);
			tmp_Type = NT3INPUT1D;
			//Create 1D output.
		}
		if (p_Dimension == 2)
		{
			Input_Dimensions[Input_Count] = 2;
			Inputs[Input_Count].X = 0;
			Inputs[Input_Count].Y = (Input_Count * 8);
			tmp_Type = NT3INPUT2D;
			//Create 2D output.
		}

		Register[Register_Current_Count] = Input_Count;
		Register_Type[Register_Current_Count] = tmp_Type; //2 is for a construct.
		Register_Current_Count++;

		Inputs[Input_Count].set_Dimension(p_Dimension);

		Input_Count++;
		return Input_Count - 1;
	}

	//Creates a new output and adds it to the output table.
	//Returns the index of the new output.
	int create_Output(int p_Dimension = 1)
	{
		ostr(0, 7, "\n void create_Output"); ostr(0, 12, "("); std::cout << p_Dimension; ostr(0, 12, ")");

		int tmp_Type = -1;
		if (p_Dimension == 1)
		{
			Output_Dimensions[Output_Count] = 1;
			Outputs[Output_Count].X = ((Construct_Highest_Tier + 1) * 128);
			Outputs[Output_Count].Y = (Output_Count * 32);
			tmp_Type = NT3OUTPUT1D;
			//Create 1D output.
		}
		if (p_Dimension == 2)
		{
			Output_Dimensions[Output_Count] = 2;
			Outputs[Output_Count].X = ((Construct_Highest_Tier + 1) * 128);
			Outputs[Output_Count].Y = (Output_Count * 32);
			tmp_Type = NT3OUTPUT2D;
			//Create 2D output.
		}

		Register[Register_Current_Count] = Output_Count;
		Register_Type[Register_Current_Count] = tmp_Type; //2 is for a construct.
		Register_Current_Count++;

		Outputs[Output_Count].set_Dimension(p_Dimension);

		Output_Count++;
		return Output_Count - 1;
	}

	//Creates a connection using a pipe.
	//Uses X value to determine if the pipe is Angelic, Mortal, or Demonic.
	// From_X < To_X = Angelic
	// From_X == To_X = Mortal
	// From_X > To_X = Demonic
	int create_Connection(int p_From, int p_From_Index, int p_To, int p_To_Index)
	{
		ostr(0, 7, "\n void create_Connection"); ostr(0, 12, "("); std::cout << p_From; ostr(0, 12, ", "); std::cout << p_From_Index; ostr(0, 12, ", "); std::cout << p_To; ostr(0, 12, ", "); std::cout << p_To_Index; ostr(0, 12, ")");

		void* tmp_From = NULL;
		void* tmp_To = NULL;

		//std::cout << "\n\n p_From->" << p_From << " p_To->" << p_To;
		//std::cout << "\n " << NT3INPUT << " " << NT3OUTPUT << " " << NT3CONSTRUCT;
		//std::cout << "\n " << Register_Type[p_From] << " " << Register

		//Get the from connection.
		if ((Register_Type[p_From] == NT3INPUT1D) || (Register_Type[p_From] == NT3INPUT2D))
		{
			tmp_From = &Inputs[Register[p_From]];
		}
		if ((Register_Type[p_From] == NT3OUTPUT1D) || (Register_Type[p_From] == NT3OUTPUT2D))
		{
			tmp_From = &Outputs[Register[p_From]];
		}
		if (Register_Type[p_From] == NT3CONSTRUCT1D)
		{
			tmp_From = Constructs[Register[p_From]];
		}

		//Get the to connection.
		if ((Register_Type[p_To] == NT3INPUT1D) || (Register_Type[p_To] == NT3INPUT2D))
		{
			tmp_To = &Inputs[Register[p_To]];
		}
		if ((Register_Type[p_To] == NT3OUTPUT1D) || (Register_Type[p_To] == NT3OUTPUT2D))
		{
			tmp_To = &Outputs[Register[p_To]];
		}
		if (Register_Type[p_To] == NT3CONSTRUCT1D)
		{
			tmp_To = Constructs[Register[p_To]];
		}

		//Create the connections.
		if (Register_Type[p_To] == NT3CONSTRUCT1D)
		{
			if (Register_Type[p_From] == NT3CONSTRUCT1D)
			{
				Data_Pipes.create_Connection(tmp_From, Register_Type[p_From], p_From, p_From_Index, tmp_To, Register_Type[p_To], p_To, p_To_Index, Constructs[Register[p_To]]->CID.U);
			}
			else
			{
				Data_Pipes.create_Connection(tmp_From, Register_Type[p_From], p_From, p_From_Index, tmp_To, Register_Type[p_To], p_To, p_To_Index, Constructs[Register[p_To]]->CID.U);
			}
		}
		else
		{
			Data_Pipes.create_Connection(tmp_From, Register_Type[p_From], p_From, p_From_Index, tmp_To, Register_Type[p_To], p_To, p_To_Index);
		}

		return 1;
	}

	//Creates a hotlink between an input and a constructs input table at the given construct input index.
	// [1]: Input_ID, [2]: Construct_ID, [3]: Construct_Input_Index.
	int hotlink_Input(int p_Input_ID, int p_CID, int p_CID_In_Index = 0)
	{
		ostr(0, 7, "\n void hotlink_Input"); ostr(0, 12, "("); std::cout << p_Input_ID; ostr(0, 12, ", "); std::cout << p_CID; ostr(0, 12, ", "); std::cout << p_CID_In_Index; ostr(0, 12, ")"); ostr(0, 12, "  --COMMAND CURRENTLY DISABLED");
		/*
		c_Raw_Table_Row_1D * tmp_Input = Inputs[p_Input_ID].Row_1D;
		tmp_Input->output_C();
		Constructs[p_CID]->tbl_Input.set_Row(p_CID_In_Index, tmp_Input);
		Constructs[p_CID]->tbl_Input.output_C();
		Constructs[p_CID]->output_Input_Tables();
		*/
		return 1;
	}

	//===------------------+
	//==--  Manipulations
	//===------------------+


	//Moves a construct.
	void move_Construct_CID(int p_Construct, int p_X, int p_Y)
	{
		ostr(0, 7, "\n void move_Construct_CID"); ostr(0, 12, "("); std::cout << p_Construct; ostr(0, 12, ", "); std::cout << p_X; ostr(0, 12, ", "); std::cout << p_Y; ostr(0, 12, ")");
		if ((p_Construct >= Construct_Count) || (p_Construct < 0))
		{
			std::cout << "\n INVALID CONSTRUCT ID ";
			oint(0, 12, p_Construct);
			std::cout << " of ";
			oint(0, 7, Construct_Count);
			std::cout << "\n xy(";
			oint(0, 13, p_X);
			std::cout << ", ";
			oint(0, 13, p_Y);
			std::cout << ")";
			system("pause > NULL");
			return;
		}

		Construct_Positions[p_Construct].X = p_X;
		Construct_Positions[p_Construct].Y = p_Y;
		Constructs[p_Construct]->X = p_X;
		Constructs[p_Construct]->Y = p_Y;
	}

	void move_Construct(int p_Register, int p_X, int p_Y)
	{
		ostr(0, 7, "\n void move_Construct"); ostr(0, 12, "("); std::cout << p_Register; ostr(0, 12, ", "); std::cout << p_X; ostr(0, 12, ", "); std::cout << p_Y; ostr(0, 12, ")");
		if ((p_Register >= Register_Current_Count) || (p_Register < 0))
		{
			std::cout << "\n INVALID Register ID ";
			oint(0, 12, p_Register);
			std::cout << " of ";
			oint(0, 7, Register_Current_Count);
			std::cout << "\n xy(";
			oint(0, 13, p_X);
			std::cout << ", ";
			oint(0, 13, p_Y);
			std::cout << ")";
			system("pause > NULL");
			return;
		}

		//Check to make sure the object is actually a construct.
		if (Register_Type[p_Register] == NT3CONSTRUCT1D)
		{
			move_Construct_CID(Register[p_Register], p_X, p_Y);
		}
	}


	//==---  IO

	//Resets the IO tables of a given construct.
	void reset_IO(int p_Construct)
	{
		if (p_Construct >= Construct_Count) { return; }
		Constructs[p_Construct]->reset_IO();
	}

	//Sets a given input to the given string.
	void set_Input_String(int p_Input, std::string p_Data)
	{
		ostr(0, 7, "\n void set_Input_String"); ostr(0, 12, "("); std::cout << p_Input; ostr(0, 12, ", "); std::cout << p_Data; ostr(0, 12, ")");
		Inputs[p_Input].set_Input_Data(p_Data);
		//*---std::cout << "\n Inputs[" << p_Input << "].set_string(" << p_Input << ", " << p_Data << "); ";
		//*---output_Input_Data();
	}

	//Adds a given input to the given string.
	void add_Input_String(int p_Input, std::string p_Data)
	{
		ostr(0, 7, "\n void add_Input_String"); ostr(0, 12, "("); std::cout << p_Input; ostr(0, 12, ", "); std::cout << p_Data; ostr(0, 12, ")");
		Inputs[p_Input].add_Input_Data(p_Data);
		//*---std::cout << "\n Inputs[" << p_Input << "].set_string(" << p_Input << ", " << p_Data << "); ";
		//*---output_Input_Data();
	}


	//Gets an output string.
	std::string get_Output_String(int p_Output)
	{
		ostr(0, 7, "\n void get_Pattern_Output_Data"); ostr(0, 12, "("); std::cout << p_Output; ostr(0, 12, ", "); std::cout << Outputs[p_Output].get_Pattern_Output_Data(); ostr(0, 12, ")");
		return Outputs[p_Output].get_Pattern_Output_Data();
	}

	//==--  Pipes

	//Appends cells from the first index to the row in the second index.
	void pipe_Append(int p_Pipe, int p_AnMoDe = 0)
	{
		//ostr(0, 7, "\n void pipe_Append"); ostr(0, 12, "("); std::cout << p_Pipe; ostr(0, 12, ", "); std::cout << p_AnMoDe; ostr(0, 12, ")");

		Data_Pipes.Append(p_Pipe);
	}

	//Resets the output index and copies the input index to it.
	void pipe_Overwrite(int p_Pipe, int p_AnMoDe = 0)
	{
		//ostr(0, 7, "\n void pipe_Overwrite"); ostr(0, 12, "("); std::cout << p_Pipe; ostr(0, 12, ", "); std::cout << p_AnMoDe; ostr(0, 12, ")");
		//*---std::cout << "\n void pipe_Overwrite(" << p_Pipe << ")";

		Data_Pipes.Overwrite(p_Pipe, p_AnMoDe);
	}

	//Resets the output index and copies the input index to it.
	void pipe_Drain(int p_Pipe)
	{
		ostr(0, 7, "\n void pipe_Drain"); ostr(0, 12, "("); std::cout << p_Pipe; ostr(0, 12, ")");
		//*---std::cout << "\n void pipe_Overwrite(" << p_Pipe << ")";

		Data_Pipes.Drain(p_Pipe);
	}

	//Resets the output index and copies the input index to it.
	void pipe_Hotlink(int p_Pipe)
	{
		std::cout << "\n void pipe_Hotlink(" << p_Pipe << ")";
		//*---std::cout << "\n void pipe_Overwrite(" << p_Pipe << ")";

		Data_Pipes.Hotlink(p_Pipe);
	}

	//==--  Constructs

	//Evals one of the eval CMD tables.
	void Eval(int p_Construct, int p_Discharge = 1)
	{
		ostr(0, 7, "\nvoid Eval"); ostr(0, 12, "("); std::cout << p_Construct; ostr(0, 7, ", "); std::cout << p_Discharge; ostr(0, 7, ")");
		if (p_Construct >= Construct_Count) { return; }

		Constructs[p_Construct]->Eval(p_Discharge);
	}

	//Discharges a construct, used for discharging lower level constructs after using the drain pipes.
	void Discharge(int p_Construct)
	{
		ostr(0, 7, "\nvoid Discharge"); ostr(0, 12, "("); std::cout << p_Construct; ostr(0, 7, ")");
		if (p_Construct >= Construct_Count) { return; }

		Constructs[p_Construct]->Discharge();
	}

	//Evals one of the build CMD tables.
	void Build(int p_Construct)
	{
		//ostr(0, 7, "\nvoid Build"); ostr(0, 12, "("); std::cout << p_Construct; ostr(0, 12, ")");
		if (p_Construct >= Construct_Count) { return; }

		Constructs[p_Construct]->Build();
	}


	//===-----------+
	//==--  Output
	//===-----------+

	//Outputs the constructs to the console, bretty basic.
	void output_Raw()
	{
		system("CLS");
		update_Pipe_Position_Data();
		for (int cou_Index = 0; cou_Index < Data_Pipes.get_Data_Pipe_Count(); cou_Index++)
		{
			//We can use data pipe [0] because the other two are the same.
			console_Draw_Line(Data_Pipes.Pipe[cou_Index].get_fX(), Data_Pipes.Pipe[cou_Index].get_fY(), Data_Pipes.Pipe[cou_Index].get_tX(), Data_Pipes.Pipe[cou_Index].get_tY(), (7 + cou_Index));
		}

		for (int cou_Index = 0; cou_Index < Register_Current_Count; cou_Index++)
		{
			int tmp_Type = Register_Type[cou_Index];
			int tmp_ID = Register[cou_Index];

			if ((tmp_Type == NT3INPUT1D) || (tmp_Type == NT3INPUT2D))
			{
				xy(0, Inputs[tmp_ID].Y);
				ostr(0, 13, "<-");
				oint(0, 14, cou_Index);
				std::cout << " ";
				oint(0, 7, Inputs[tmp_ID].Dimension);
				ostr(0, 13, "->");
			}
			if ((tmp_Type == NT3OUTPUT1D) || (tmp_Type == NT3OUTPUT2D))
			{
				xy(Outputs[tmp_ID].X, Outputs[tmp_ID].Y);
				ostr(0, 13, "<-");
				oint(0, 15, cou_Index);
				std::cout << " ";
				oint(0, 8, Outputs[tmp_ID].Dimension);
				ostr(0, 13, "->");
			}
			if (tmp_Type == NT3CONSTRUCT1D)
			{
				xy(Construct_Positions[tmp_ID].X, Construct_Positions[tmp_ID].Y);
				//std::cout << "(";  oint(0, 7, tmp_ID); std::cout << " "; oint(0, 13, Construct_Types[tmp_ID]); std::cout << " " << Construct_Names[tmp_ID] << ")";
				ostr(0, 13, "("); oint(0, 7, cou_Index); std::cout << " " << Construct_Names[tmp_ID]; ostr(0, 13, ")");
			}
		}
		xy(0, (Input_Count * 5));
		output_Registry();
		std::cout << "\n";
		output_Pipe_Data();
	}
	
	void output_Registry()
	{
		std::cout << "\n Registry->";
		for (int cou_Index = 0; cou_Index < Register_Current_Count; cou_Index++)
		{
			std::cout << "\n";
			oint(0, 8, cou_Index);

			std::cout << " "; oint(0, (7 + Register_Type[cou_Index]), Register[cou_Index]);
			std::cout << " "; oint(0, (8 + Register_Type[cou_Index]), Register_Type[cou_Index]);

			if (Register_Type[cou_Index] == NT3INPUT1D)
			{
				ostr(0, (13 + Register_Type[cou_Index]), " Input 1D ");
				std::cout << "  ";
				Inputs[cou_Index].output_C();
			}
			if (Register_Type[cou_Index] == NT3INPUT2D)
			{
				ostr(0, (13 + Register_Type[cou_Index]), " Input 2D ");
				std::cout << "  ";
				Inputs[cou_Index].output_C();
			}
			if (Register_Type[cou_Index] == NT3OUTPUT1D)
			{
				ostr(0, (13 + Register_Type[cou_Index]), " Output 1D ");
				std::cout << "  ";
				Outputs[cou_Index].output_C();
			}
			if (Register_Type[cou_Index] == NT3OUTPUT2D)
			{
				ostr(0, (13 + Register_Type[cou_Index]), " Output 2D ");
				std::cout << "  ";
				Outputs[cou_Index].output_C();
			}
			if (Register_Type[cou_Index] == NT3CONSTRUCT1D)
			{
				ostr(0, (7 + Register_Type[cou_Index]), " Construct "); ostr(0, 8, Construct_Names[Register[cou_Index]]);
			}
		}
	}

	//This should have been worked in before, but due to lack of foresight this must now be done when displaying in case the constructs moved.
	void update_Pipe_Position_Data()
	{
		int tmp_R = 0;
		int tmp_Reg_Type = 0;
		int tmp_ID = 0;

		for (int cou_Index = 0; cou_Index < Data_Pipes.get_Data_Pipe_Count(); cou_Index++)
		{
			//From
			tmp_R = Data_Pipes.Pipe[cou_Index].get_From_Register();
			tmp_Reg_Type = Register_Type[tmp_R];
			tmp_ID = Register[tmp_R];

			if (tmp_Reg_Type == NT3INPUT1D)
			{
				Data_Pipes.Pipe[cou_Index].set_fX(Inputs[tmp_ID].X);
				Data_Pipes.Pipe[cou_Index].set_fY(Inputs[tmp_ID].Y);
			}
			if (tmp_Reg_Type == NT3INPUT2D)
			{
				Data_Pipes.Pipe[cou_Index].set_fX(Inputs[tmp_ID].X);
				Data_Pipes.Pipe[cou_Index].set_fY(Inputs[tmp_ID].Y);
			}
			if (tmp_Reg_Type == NT3OUTPUT1D)
			{
				Data_Pipes.Pipe[cou_Index].set_fX(Outputs[tmp_ID].X);
				Data_Pipes.Pipe[cou_Index].set_fY(Outputs[tmp_ID].Y);
			}
			if (tmp_Reg_Type == NT3OUTPUT2D)
			{
				Data_Pipes.Pipe[cou_Index].set_fX(Outputs[tmp_ID].X);
				Data_Pipes.Pipe[cou_Index].set_fY(Outputs[tmp_ID].Y);
			}
			if (tmp_Reg_Type == NT3CONSTRUCT1D)
			{
				Data_Pipes.Pipe[cou_Index].set_fX(Construct_Positions[tmp_ID].X);
				Data_Pipes.Pipe[cou_Index].set_fY(Construct_Positions[tmp_ID].Y);
			}

			//To
			tmp_R = Data_Pipes.Pipe[cou_Index].get_To_Register();
			tmp_Reg_Type = Register_Type[tmp_R];
			tmp_ID = Register[tmp_R];

			if (tmp_Reg_Type == NT3INPUT1D)
			{
				Data_Pipes.Pipe[cou_Index].set_tX(Inputs[tmp_ID].X);
				Data_Pipes.Pipe[cou_Index].set_tY(Inputs[tmp_ID].Y);
			}
			if (tmp_Reg_Type == NT3INPUT2D)
			{
				Data_Pipes.Pipe[cou_Index].set_tX(Inputs[tmp_ID].X);
				Data_Pipes.Pipe[cou_Index].set_tY(Inputs[tmp_ID].Y);
			}
			if (tmp_Reg_Type == NT3OUTPUT1D)
			{
				Data_Pipes.Pipe[cou_Index].set_tX(Outputs[tmp_ID].X);
				Data_Pipes.Pipe[cou_Index].set_tY(Outputs[tmp_ID].Y);
			}
			if (tmp_Reg_Type == NT3OUTPUT2D)
			{
				Data_Pipes.Pipe[cou_Index].set_tX(Outputs[tmp_ID].X);
				Data_Pipes.Pipe[cou_Index].set_tY(Outputs[tmp_ID].Y);
			}
			if (tmp_Reg_Type == NT3CONSTRUCT1D)
			{
				Data_Pipes.Pipe[cou_Index].set_tX(Construct_Positions[tmp_ID].X);
				Data_Pipes.Pipe[cou_Index].set_tY(Construct_Positions[tmp_ID].Y);
			}
		}
	}


	void output_Pipe_Data()
	{
		int tmp_R = 0;

		std::cout << "\n Data_Pipes->";
		for (int cou_Index = 0; cou_Index < Data_Pipes.get_Data_Pipe_Count(); cou_Index++)
		{
			std::cout << "\n";
			oint(0, 8, cou_Index);


			//From
			tmp_R = Data_Pipes.Pipe[cou_Index].get_From_Register();

			std::cout << " From->"; oint(0, (7 + Register_Type[tmp_R]), Register[tmp_R]);
			std::cout << " "; oint(0, (8 + Register_Type[tmp_R]), Register_Type[tmp_R]);
			std::cout << " "; oint(0, 8, Data_Pipes.Pipe[cou_Index].get_From_Index());

			if (Register_Type[tmp_R] == NT3INPUT1D)
			{
				ostr(0, (13 + Register_Type[tmp_R]), " Input 1D ");
			}
			if (Register_Type[tmp_R] == NT3INPUT2D)
			{
				ostr(0, (13 + Register_Type[tmp_R]), " Input 2D ");
			}
			if (Register_Type[tmp_R] == NT3OUTPUT1D)
			{
				ostr(0, (13 + Register_Type[tmp_R]), " Output 1D ");
			}
			if (Register_Type[tmp_R] == NT3OUTPUT2D)
			{
				ostr(0, (13 + Register_Type[tmp_R]), " Output 2D ");
			}
			if (Register_Type[tmp_R] == NT3CONSTRUCT1D)
			{
				ostr(0, (7 + Register_Type[tmp_R]), " Construct "); ostr(0, 8, Construct_Names[Register[tmp_R]]);
			}

			//To
			tmp_R = Data_Pipes.Pipe[cou_Index].get_To_Register();

			std::cout << " To->"; oint(0, (7 + Register_Type[tmp_R]), Register[tmp_R]);
			std::cout << " "; oint(0, (8 + Register_Type[tmp_R]), Register_Type[tmp_R]);
			std::cout << " "; oint(0, 8, Data_Pipes.Pipe[cou_Index].get_To_Index());

			if (Register_Type[tmp_R] == NT3INPUT1D)
			{
				ostr(0, (13 + Register_Type[tmp_R]), " Input 1D ");
			}
			if (Register_Type[tmp_R] == NT3INPUT2D)
			{
				ostr(0, (13 + Register_Type[tmp_R]), " Input 2D ");
			}
			if (Register_Type[tmp_R] == NT3OUTPUT1D)
			{
				ostr(0, (13 + Register_Type[tmp_R]), " Output 1D ");
			}
			if (Register_Type[tmp_R] == NT3OUTPUT2D)
			{
				ostr(0, (13 + Register_Type[tmp_R]), " Output 2D ");
			}
			if (Register_Type[tmp_R] == NT3CONSTRUCT1D)
			{
				ostr(0, (7 + Register_Type[tmp_R]), " Construct "); ostr(0, 8, Construct_Names[Register[tmp_R]]);
			}
		}
	}

	//Outputs the input information.
	void output_Input_Data()
	{
		std::cout << "\n Inputs->" << Input_Count;
		for (int cou_Index = 0; cou_Index < Input_Count; cou_Index++)
		{
			//std::cout << "\n " << Inputs[cou_Index].get_String();
			std::cout << "\n "; Inputs[cou_Index].output_C();
		}
	}

	//Outputs the input information.
	void output_Output_Data()
	{
		std::cout << "\n Outputs->" << Output_Count;
		for (int cou_Index = 0; cou_Index < Output_Count; cou_Index++)
		{
			std::cout << "\n " << Outputs[cou_Index].get_Input_Data();
			std::cout << "\n " << Outputs[cou_Index].get_Input_Charges_Data();
			std::cout << "\n " << Outputs[cou_Index].get_Pattern_Output_Data();
			std::cout << "\n " << Outputs[cou_Index].get_Charge_Output_Data();
			std::cout << "\n " << Outputs[cou_Index].get_RC_Output_Data();
			std::cout << "\n " << Outputs[cou_Index].get_Treetops_Output_Data();

		}
	}

	void output_Construct_IO(int p_Construct)
	{
		if (p_Construct >= Construct_Count) { return; }

		Constructs[p_Construct]->output_Input_Tables();
		Constructs[p_Construct]->output_Output_Tables();
	}

	//Outputs the IO tables for every construct
	void output_Construct_IO_All()
	{
		for (int cou_Index = 0; cou_Index < Construct_Count; cou_Index++)
		{
			output_Construct_IO(cou_Index);
		}
	}

	//Outputs the CAN for a given construct
	void output_CAN(int p_Construct)
	{
		if (p_Construct >= Construct_Count) { return; }

		Constructs[p_Construct]->output_CAN();
	}

	//Outputs the CAN for every Construct.
	void output_CAN_All()
	{
		for (int cou_Index = 0; cou_Index < Construct_Count; cou_Index++)
		{
			output_CAN(cou_Index);
		}
	}

	//Outputs the nodes as HTML
	void output_Nodes_As_HTML(int p_Construct, std::string p_Dir, std::string p_File, int p_X_Padd, int p_Y_Padd)
	{
		Constructs[p_Construct]->output_Nodes_As_HTML(p_Dir, p_File, p_X_Padd, p_Y_Padd);
	}



	//Saves the sandbox.
	//Saves to a dir passed to it, the dir is under NT3.Save, or it saves to the name of the sandbox in the NT3.Save folder.
	void Save(std::string p_DIR = "NO_DIR")
	{
		std::string DIR = "NT3.Save/" + p_DIR + "/";
		std::string tmp_FName = "NT3.Save/" + p_DIR + "/" + Name + ".sand";

		if (p_DIR == "NO_DIR")
		{
			DIR = "NT3.Save/" + Name + "/";
			tmp_FName = "NT3.Save/" + Name + "/" + Name + ".sand";
		}
		std::cout << "\n Saving Sandbox:" << tmp_FName;

		//Attempt to create the directory.
		std::string tmp_MD = "md \"" + DIR + "\"";

		//Create the directory.
		system(tmp_MD.c_str());

		std::ofstream SF;
		SF.open(tmp_FName, std::ios::trunc);
		if (!SF.is_open()) { SF.close(); std::cout << "\n\n ERROR CANNOT OPEN SAVE FILE " << tmp_FName; system("PAUSE > NULL"); return; }

		SF << "Register " << Register_Current_Count;
		for (int cou_Index = 0; cou_Index < Register_Current_Count; cou_Index++)
		{
			SF << "\n";
			SF << Register_Type[cou_Index];
			if (Register_Type[cou_Index] == NT3INPUT1D)
			{
				SF << " NT3INPUT1D";

				//Save the dimension.
				SF << " D " << Inputs[Register[cou_Index]].Dimension;

				//Coordinates.
				SF << " X " << Inputs[Register[cou_Index]].X;
				SF << " Y " << Inputs[Register[cou_Index]].Y;
			}
			if (Register_Type[cou_Index] == NT3INPUT2D)
			{
				SF << " NT3INPUT2D";

				//Save the dimension.
				SF << " D " << Inputs[Register[cou_Index]].Dimension;

				//Coordinates.
				SF << " X " << Inputs[Register[cou_Index]].X;
				SF << " Y " << Inputs[Register[cou_Index]].Y;
			}
			if (Register_Type[cou_Index] == NT3OUTPUT1D)
			{
				SF << " NT3OUTPUT1D";

				//Save the dimension.
				SF << " D " << Outputs[Register[cou_Index]].Dimension;

				//Coordinates.
				SF << " X " << Outputs[Register[cou_Index]].X;
				SF << " Y " << Outputs[Register[cou_Index]].Y;
			}
			if (Register_Type[cou_Index] == NT3OUTPUT2D)
			{
				SF << " NT3OUTPUT2D";

				//Save the dimension.
				SF << " D " << Outputs[Register[cou_Index]].Dimension;

				//Coordinates.
				SF << " X " << Outputs[Register[cou_Index]].X;
				SF << " Y " << Outputs[Register[cou_Index]].Y;
			}
			if (Register_Type[cou_Index] == NT3CONSTRUCT1D)
			{
				SF << " NT3CONSTRUCT1D";

				//Save the name.
				SF << " " << Construct_Names[Register[cou_Index]];

				//Coordinates.
				SF << " X " << Constructs[Register[cou_Index]]->X;
				SF << " Y " << Constructs[Register[cou_Index]]->Y;

				//Forge position.
				SF << " nX " << Construct_Positions[Register[cou_Index]].X;
				SF << " nY " << Construct_Positions[Register[cou_Index]].Y;

				//Save the type.
				SF << " T " << Construct_Types[Register[cou_Index]];
			}
		}

		SF << "\nData_Pipes " << Data_Pipes.get_Data_Pipe_Count();
		for (int cou_Index = 0; cou_Index < Data_Pipes.get_Data_Pipe_Count(); cou_Index++)
		{
			SF << "\n";

			//From
			SF << "FR " << Data_Pipes.Pipe[cou_Index].get_From_Register();
			SF << " FI " << Data_Pipes.Pipe[cou_Index].get_From_Index();
			SF << " TR " << Data_Pipes.Pipe[cou_Index].get_To_Register();
			SF << " TI " << Data_Pipes.Pipe[cou_Index].get_To_Index();

		}

		SF.close();

		tmp_FName = DIR + "ConIndex.con";
		SF.open(tmp_FName, std::ios::trunc);

		for (int cou_Index = 0; cou_Index < Construct_Count; cou_Index++)
		{
			if (cou_Index != 0) { SF << "\n"; }
			SF << Construct_Names[cou_Index] + ".con";

			tmp_FName = Construct_Names[cou_Index] + ".con";
			//Note that the other constructs still follow the (p_FName, p_Dir) backwards format. MMap is only one using (p_Dir, p_FName).
			Constructs[cou_Index]->Save(DIR, tmp_FName);
		}
		SF.close();

		//Set the flag to initialized.
		flg_Initialized = true;
	}

	//Loads the sandbox.
	void Load(std::string p_FName)
	{
		std::string DIR = "NT3.Save/" + p_FName + "/";

		std::string tmp_FName = DIR + p_FName + ".sand";

		std::ifstream SF;
		SF.open(tmp_FName);
		if (!SF.is_open()) { SF.close(); std::cout << "\n\n ERROR CANNOT OPEN SAVE FILE " << tmp_FName;  system("PAUSE > NULL"); return; }

		//If it has gotten to this point then the loading was successful.
		set_Name(p_FName);

		std::string tmp_Input;
		int tmp_Dimension;
		int tmp_X;
		int tmp_Y;
		int tmp_nX;
		int tmp_nY;
		int tmp_Type;
		std::string tmp_Input_Name;
		int tmp_Register_Count;

		int tmp_Data_Pipe_Count;
		int tmp_Data_Pipe_From_Register;
		int tmp_Data_Pipe_From_Index;
		int tmp_Data_Pipe_To_Register;
		int tmp_Data_Pipe_To_Index;

		//Get the Register std::string and the number of registers.
		SF >> tmp_Input;
		SF >> tmp_Register_Count;
		/*7*/std::cout << "\n " << tmp_Input << " (" << tmp_Register_Count << ")";

		//Loop through the registers reading them into the register based on the type they are.
		for (int cou_Index = 0; cou_Index < tmp_Register_Count; cou_Index++)
		{
			SF >> Register_Type[cou_Index];
			SF >> tmp_Input;
			/*7*/std::cout << "\n " << cou_Index << " " << tmp_Register_Count << " " << tmp_Input << " " << Register_Type[cou_Index];

			if ((Register_Type[cou_Index] == NT3INPUT1D) || (Register_Type[cou_Index] == NT3INPUT2D))
			{
				//Save the dimension.
				SF >> tmp_Input;
				SF >> tmp_Dimension;

				//Coordinates.
				SF >> tmp_Input;
				SF >> tmp_X;

				SF >> tmp_Input;
				SF >> tmp_Y;

				/*7*/std::cout << " INPUT D " << tmp_Dimension << " X " << tmp_X << " Y " << tmp_Y;

				create_Input(tmp_Dimension);
				Inputs[Input_Count - 1].X = tmp_X;
				Inputs[Input_Count - 1].Y = tmp_Y;
			}
			if ((Register_Type[cou_Index] == NT3OUTPUT1D) || (Register_Type[cou_Index] == NT3OUTPUT2D))
			{
				//Save the dimension.
				SF >> tmp_Input;
				SF >> tmp_Dimension;

				//Coordinates.
				SF >> tmp_Input;
				SF >> tmp_X;

				SF >> tmp_Input;
				SF >> tmp_Y;

				/*7*/std::cout << " OUTPUT D " << tmp_Dimension << " X " << tmp_X << " Y " << tmp_Y;

				create_Output(tmp_Dimension);
				Outputs[Output_Count - 1].X = tmp_X;
				Outputs[Output_Count - 1].Y = tmp_Y;
			}
			if (Register_Type[cou_Index] == NT3CONSTRUCT1D)
			{
				//Save the name.
				SF >> tmp_Input_Name;

				//Coordinates.
				SF >> tmp_Input;
				SF >> tmp_X;

				SF >> tmp_Input;
				SF >> tmp_Y;

				//Forge Coordinates.
				SF >> tmp_Input;
				SF >> tmp_nX;

				SF >> tmp_Input;
				SF >> tmp_nY;

				SF >> tmp_Input;
				SF >> tmp_Type;

				/*7*/std::cout << " CONSTRUCT " << tmp_Input_Name << " X " << tmp_X << " Y " << tmp_Y << " T " << tmp_Type;

				//create_Construct(tmp_Input_Name, tmp_Type);
				load_Construct(tmp_Input_Name, tmp_Type);
				Constructs[Construct_Count - 1]->X = tmp_X;
				Constructs[Construct_Count - 1]->Y = tmp_Y;
				Construct_Positions[Construct_Count - 1].X = tmp_nX;
				Construct_Positions[Construct_Count - 1].Y = tmp_nY;
			}
		}

		//This was added IIRC to initialize the constructs, however, it adds .con to everything and I don't think it's needed.
		/*
		for (int cou_Index = 0; cou_Index < Construct_Count; cou_Index++)
		{
			tmp_FName = Construct_Names[cou_Index] + ".con";
			Constructs[cou_Index]->Load(DIR, tmp_FName);
		}*/

		SF >> tmp_Input;
		SF >> tmp_Data_Pipe_Count;
		std::cout << "\n Data_Pipes " << tmp_Data_Pipe_Count;
		for (int cou_Index = 0; cou_Index < tmp_Data_Pipe_Count; cou_Index++)
		{
			//From
			SF >> tmp_Input;
			SF >> tmp_Data_Pipe_From_Register;
			SF >> tmp_Input;
			SF >> tmp_Data_Pipe_From_Index;

			//To
			SF >> tmp_Input;
			SF >> tmp_Data_Pipe_To_Register;
			SF >> tmp_Input;
			SF >> tmp_Data_Pipe_To_Index;

			/*7*/std::cout << "\n From->" << tmp_Data_Pipe_From_Register << "." << tmp_Data_Pipe_From_Index << " To->" << tmp_Data_Pipe_To_Register << "." << tmp_Data_Pipe_To_Index;
			create_Connection(tmp_Data_Pipe_From_Register, tmp_Data_Pipe_From_Index, tmp_Data_Pipe_To_Register, tmp_Data_Pipe_To_Index);
		}

		SF.close();
		flg_Initialized = true;
	}

};

int c_NT3_SANDBOX::stat_MAX_CONSTRUCTS = 1000;




