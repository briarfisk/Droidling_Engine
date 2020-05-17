


//--      CALLBACKS FOR THE NT ENGINE      --//


//This handles the callbacks for the Sandbox.
std::string c_NT3_Command_Parser::cmde_Main_Sandbox(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
{
    std::string tmp_Token[100];
    int tmp_Token_Count = 0;
    //Gather all the tokens up to the limit of 10.
    for (int cou_Index = 0; cou_Index < p_Tokens->get_Row_Count(); cou_Index++)
    {
        if (cou_Index >= 100) { break; }
        p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
        tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
        //std::cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
        tmp_Token_Count++;
    }

    //Creates a new new_Output of the given type.
    //rename new_Name
    //  0       1
    if (tmp_Token[0] == "set_name")
    {
        Sandbox.set_Name(tmp_Token[1]);
        return "Sandbox__set_name";
        return "Sandbox__";
    }


    //===-----------------------------+
    //==--  Creation and Destruction
    //===-----------------------------+


    //Creates a connection between two objects.
    //sand    new_Connection   -FROM    -TO
    //   0            1          2       3 
    if (tmp_Token[0] == "create_connection")
    {
        Sandbox.create_Connection(str2int(tmp_Token[1]), str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]));
        return "Sandbox__create_connection";
    }

    //Creates a new construct of the given name and type.
    //sand    new_Construct   -NAME    -TYPE
    //   0            1          2       3 
    if (tmp_Token[0] == "create_construct")
    {
        if (p_Tokens->get_Row_Count() == 2)
        {
            Sandbox.create_Construct(tmp_Token[1]);
            return "Sandbox__construct_created";
        }
        if (p_Tokens->get_Row_Count() == 3)
        {
            Sandbox.create_Construct(tmp_Token[1], str2int(tmp_Token[2]));
            return "Sandbox__construct_created";
        }
        std::cout << "\n   ERROR: Invalid Number of Arguments: create_construct [string:NAME] [int:TYPE]";
        return "Sandbox__create_construct__ERROR: Invalid Number of Arguments: create_construct [string:NAME] [int:TYPE]";
    }

    //Creates a new new_Input of the given type.
    //sand    new_Input   -TYPE  
    //   0        1          2    
    if (tmp_Token[0] == "create_input")
    {
        Sandbox.create_Input(str2int(tmp_Token[1]));
        return "Sandbox__create_input";
    }

    //Creates a new new_Output of the given type.
    //sand    new_Output   -TYPE  
    //   0        1          2    
    if (tmp_Token[0] == "create_output")
    {
        Sandbox.create_Output(str2int(tmp_Token[1]));
        return "Sandbox__create_output";
    }



    //===------------------+
    //==--  Manipulations
    //===------------------+


    if (tmp_Token[0] == "move_Construct")
    {
        std::cout << "\n moving construct ";
        if (tmp_Token_Count < 3)
        {
            return "Sandbox__move_Construct__FAIL:NOTENOUGHARGS";
        }

        Sandbox.move_Construct(str2int(tmp_Token[1]), str2int(tmp_Token[2]), str2int(tmp_Token[3]));
        std::cout << "\n " << tmp_Token[1] << " to (" << tmp_Token[2] << ", " << tmp_Token[3] << ")";
        return "Sandbox__move_Construct";
    }


    //The sandbox has 2 different groups of command tables that are hooked into the eval and build members.
    //These two groups are arrays with 10 tables each. This allows for the creation of several different approaches
    //to input for each function.
    //--Outdated, now these are used to just hook into the Sandbox.
    //build [CONSTRUCT_ID]
    if (tmp_Token[0] == "build")
    {
        if (tmp_Token_Count == 1)
        {
            Sandbox.Build(0);
            return "Sandbox__build";
        }
        if (tmp_Token_Count == 2)
        {
            Sandbox.Build(str2int(tmp_Token[1]));
            return "Sandbox__build";
        }
    }

    if (tmp_Token[0] == "eval")
    {
        //*---std::cout << "\n Eval()";
        if (tmp_Token_Count == 1)
        {
            Sandbox.Eval(0);
            return "Sandbox__eval";
        }
        if (tmp_Token_Count == 2)
        {
            Sandbox.Eval(str2int(tmp_Token[1]));
            return "Sandbox__eval";
        }
        if (tmp_Token_Count == 3)
        {
            Sandbox.Eval(str2int(tmp_Token[1]), str2int(tmp_Token[2]));
            return "Sandbox__eval";
        }
    }

    if (tmp_Token[0] == "discharge")
    {
        if (tmp_Token_Count == 1)
        {
            Sandbox.Discharge(0);
            return "Sandbox__discharge";
        }
        if (tmp_Token_Count == 2)
        {
            Sandbox.Discharge(str2int(tmp_Token[1]));
            return "Sandbox__discharge";
        }
    }

    //Resets a constructs input and output tables for new evaluation.
    //Using this negates and pipe transfers done previously.
    if (tmp_Token[0] == "rio")
    {
        Sandbox.reset_IO(str2int(tmp_Token[1]));
        return "Sandbox__rio";
    }

    //Tells a pipe to transfer data from its input to its output. Does reset and cell copy. One row only.
    //sand    pipe_cut    pipe_ID
    //   0     1             2        
    if (tmp_Token[0] == "po")
    {
        if (tmp_Token_Count == 2) { Sandbox.pipe_Overwrite(str2int(tmp_Token[1]), 0); }
        if (tmp_Token_Count == 3) { Sandbox.pipe_Overwrite(str2int(tmp_Token[1]), str2int(tmp_Token[2])); }
        return "Sandbox__po";
    }

    //Tells a pipe to transfer data from its input to its output. Appends input row to destination row.
    //sand    pipe_cut    pipe_ID
    //   0     1             2        
    if (tmp_Token[0] == "pa")
    {
        Sandbox.pipe_Append(str2int(tmp_Token[1]));
        return "Sandbox__pa";
    }

    //Tells a pipe to drain.
    //sand    pipe_cut    pipe_ID
    //   0     1             2        
    if (tmp_Token[0] == "pd")
    {
        Sandbox.pipe_Overwrite(str2int(tmp_Token[1]), str2int(tmp_Token[2]));
        return "Sandbox__pd";
    }

    //Tells a pipe to hotlink the connections.
    if (tmp_Token[0] == "pipe_hotlink")
    {
        Sandbox.pipe_Hotlink(str2int(tmp_Token[1]));
        return "Sandbox__pipe_hotlink";
    }

    //Tells a pipe to transfer data from its output to its input. Does reset and cell copy. One row only.
    //Relies on the salt table to determine which nodes to gather.
    //sand    pipe_cut    pipe_ID
    //   0     1             2        
    if (tmp_Token[0] == "pd")
    {
        Sandbox.pipe_Overwrite(str2int(tmp_Token[1]), str2int(tmp_Token[2]));
        return "Sandbox__pd";
    }


    //Sets the input of a given input to the given string.
    //sand    set_input    input_ID    input_String
    //   0     1             2              3
    if (tmp_Token[0] == "set_input")
    {
        Sandbox.set_Input_String(str2int(tmp_Token[1]), tmp_Token[2]);
        return "Sandbox__set_input";
    }



    //===-----------+
    //==--  Output
    //===-----------+

    //Displays the crude graphical output to console of the objects and connections.
    //sand    output_raw
    //   0        1     
    if (tmp_Token[0] == "output_raw")
    {
        Sandbox.output_Raw();
        return "Sandbox__output_raw";
    }

    //Displays the registered objects.
    //sand    output_registry
    //   0        1     
    if (tmp_Token[0] == "output_registry")
    {
        Sandbox.output_Registry();
        return "Sandbox__output_registry";
    }

    //Displays the data pipes and their information.
    //sand    output_pipes
    //   0        1     
    if (tmp_Token[0] == "output_pipes")
    {
        Sandbox.output_Pipe_Data();
        return "Sandbox__output_pipes";
    }

    //Outputs the inputs.
    if (tmp_Token[0] == "output_inputs")
    {
        Sandbox.output_Input_Data();
        return "Sandbox__output_inputs";
    }

    //Outputs the outputs.
    if (tmp_Token[0] == "output_outputs")
    {
        Sandbox.output_Output_Data();
        return "Sandbox_output_outputs_";
    }

    //Outputs the IO tables for a given construct.
    if (tmp_Token[0] == "output_IO")
    {
        if (tmp_Token_Count <= 1) { return "Sandbox__output_IO__FAILED_ARGUMENTS_INSUFFICIENT"; }
        Sandbox.output_Construct_IO(str2int(tmp_Token[1]));
        return "Sandbox__output_IO";
    }

    //Outputs the IO tables for every construct
    if (tmp_Token[0] == "output_IO_all")
    {
        Sandbox.output_Construct_IO_All();
        return "Sandbox__output_IO_all";
    }

    //Outputs the CAN for a given construct
    if (tmp_Token[0] == "output_CAN")
    {
        if (tmp_Token_Count <= 1) { return "Sandbox__output_CAN__FAILED_ARGUMENTS_INSUFFICIENT"; }
        Sandbox.output_CAN(str2int(tmp_Token[1]));
        return "Sandbox__output_CAN";
    }

    //Outputs the CAN for every Construct.
    if (tmp_Token[0] == "output_CAN_all")
    {
        Sandbox.output_CAN_All();
        return "Sandbox__output_CAN_all";
    }

    //Outputs the network to an HTML file.
    if (tmp_Token[0] == "output_HTML")
    {
        //Outputs the node network for a given construct to an HTML file that uses javascript to display the nodes.
        // [1]: Construct, [2]: Directory, [3]: Filename, [4]: X padding, [5]: Y padding
        if (tmp_Token_Count <= 5) { std::cout << "Sandbox__output_HTML__FAILED_ARGUMENTS_INSUFFICIENT"; return "Sandbox__output_HTML__FAILED_ARGUMENTS_INSUFFICIENT"; }

        Sandbox.output_Nodes_As_HTML(str2int(tmp_Token[1]), tmp_Token[2], tmp_Token[3], str2int(tmp_Token[4]), str2int(tmp_Token[5]));
        return "Sandbox__output_HTML";
    }

    //Submits a std::string to the given input.
    if (tmp_Token[0] == "in")
    {
        Sandbox.set_Input_String(str2int(tmp_Token[1]), tmp_Token[2]);
        return "Sandbox__in";
    }

    //Submits a std::string to the given input.
    if (tmp_Token[0] == "add")
    {
        Sandbox.add_Input_String(str2int(tmp_Token[1]), tmp_Token[2]);
        return "Sandbox__add";
    }

    //Hotlinks an input to a construct.
    if (tmp_Token[0] == "hotlink_in")
    {
        //Hotlink the given input to the given construct. The row is linked to the constructs input table at the given index.
        // [1]: Input_ID, [2]: Construct_ID, [3]: Construct_Input_Index.
        Sandbox.hotlink_Input(str2int(tmp_Token[1]), str2int(tmp_Token[2]), str2int(tmp_Token[3]));
        return "Sandbox__hotlink_in";
    }


    //Submits a std::string to the given input.
    if (tmp_Token[0] == "forge")
    {
        std::string tmp_UInput;
        std::string tmp_Name;
        int tmp_From = 0;
        int tmp_To = 0;
        //int tmp_From_Index = 0;
        //int tmp_To_Index = 0;
        int tmp_Dimension = 0;
        int tmp_Tier = 0;
        int tmp_X = 0;
        int tmp_Y = 0;

        while (!(Sandbox.flg_Initialized))
        {
            std::cout << "\n --Create a new network or load an old one.";
            std::cout << "\n Create New Network: new";
            std::cout << "\n Load: load";
            std::cout << "\n ->";
            std::cin >> tmp_UInput;

            if (tmp_UInput == "new")
            {
                std::cout << "\n Name->";
                std::cin >> tmp_Name;

                Sandbox.create_Sandbox(tmp_Name);
                break;
            }
            if (tmp_UInput == "load")
            {
                std::cout << "\n Name->";
                std::cin >> tmp_Name;

                //Sandbox.Load("NT3.Save/SANDBOX/", tmp_Name);
                std::string tmp_CMD_Text = "lrealm " + tmp_Name;
                eval_Command_String(tmp_CMD_Text);
                break;
            }
        }

        while (1)
        {
            Sandbox.output_Raw();
            std::cout << "\n --After selecting the command you will be requested to give parameters. This is a menu.";
            std::cout << "\n Create Input: mkinput";
            std::cout << "\n Create Output: mkoutput";
            std::cout << "\n Create Connection: mklink";
            std::cout << "\n Create Raw Construct: mkraw";
            std::cout << "\n Create MSC Construct: mkMSC";
            std::cout << "\n Move Construct: move";
            std::cout << "\n Save: save";
            std::cout << "\n Load: load";
            std::cout << "\n ->";
            std::cin >> tmp_UInput;

            if (tmp_UInput == "mkinput")
            {
                std::cout << "\n Dimension->";
                std::cin >> tmp_Dimension;
                Sandbox.create_Input(tmp_Dimension);
                continue;
            }
            if (tmp_UInput == "mkoutput")
            {
                std::cout << "\n Dimension->";
                std::cin >> tmp_Dimension;
                Sandbox.create_Output(tmp_Dimension);
                continue;
            }
            if (tmp_UInput == "mklink")
            {
                std::cout << "\n From->";
                std::cin >> tmp_From;
                //std::cout << "\n From_Index->";
                //std::cin >> tmp_From_Index;
                std::cout << "\n To->";
                std::cin >> tmp_To;
                //std::cout << "\n To_Index->";
                //std::cin >> tmp_To_Index;

                //Sandbox.create_Connection(tmp_From, tmp_From_Index, tmp_To, tmp_To_Index);
                Sandbox.create_Connection(tmp_From, 0, tmp_To, 0);
                continue;
            }
            if (tmp_UInput == "mkraw")
            {
                std::cout << "\n Name->";
                std::cin >> tmp_Name;

                Sandbox.create_Construct(tmp_Name, 1, 1);
                continue;
            }
            if (tmp_UInput == "mkMSC")
            {
                std::cout << "\n Name->";
                std::cin >> tmp_Name;

                std::cout << "\n Tier->";
                std::cin >> tmp_Tier;

                Sandbox.create_Construct(tmp_Name, 0, tmp_Tier);
                continue;
            }
            if (tmp_UInput == "move")
            {
                std::cout << "\n Construct ID->";
                std::cin >> tmp_From;

                std::cout << "\n X->";
                std::cin >> tmp_X;

                std::cout << "\n Y->";
                std::cin >> tmp_Y;

                Sandbox.move_Construct(tmp_From, tmp_X, tmp_Y);
                continue;
            }
            if (tmp_UInput == "save")
            {
                std::cout << "\n Name->";
                std::cin >> tmp_Name;

                //Sandbox.Save("NT3.Save/SANDBOX/", tmp_Name);
                std::string tmp_CMD_Text = "srealm " + tmp_Name;
                eval_Command_String(tmp_CMD_Text);
                continue;
            }
            if (tmp_UInput == "load")
            {
                std::cout << "\n Name->";
                std::cin >> tmp_Name;

                //Sandbox.Load("NT3.Save/SANDBOX/", tmp_Name);
                std::string tmp_CMD_Text = "lrealm " + tmp_Name;
                eval_Command_String(tmp_CMD_Text);
                continue;
            }
            if (tmp_UInput == "OUT")
            {
                Sandbox.output_Raw();
                continue;
            }
            if (tmp_UInput == "exit")
            {
                Sandbox.init_Constructs();
                break;
            }

        }

        return "Sandbox__forge";
    }


    return "NO_COMMAND";
}


















