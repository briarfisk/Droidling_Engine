//This file contains the command parser whose job it is to accept input in the form of command text and parse it accordingly.
//Tables can be submitted for parsing on a row by row basis with each cell containing an argument. Or perhaps on a row by row basis with comments being in the secondary cells.


class c_NT3_Command_Parser
{
protected:

    //The whole std::string to collect during rcon.
    std::string Command_String;

    //The table used to build scripts before saving them to a dedicated table.
    c_Table_1D* CMD_Sandbox;

    //The flag for the current variable when reading in tokens.
    int flg_Var_Pos;

    //This flag represents whether or not there are undefined variables being used resulting in NULL values.
    int flg_Undefined;

    //The flag allows for std::string inputs multiple tokens long.
    int flg_String;

    //Holds all of the variables and their values.
    c_Lookup_Tree Var_Lookup_Tree;

    //Holds the CMD tables.
    c_Table_1D* CMD_Script_Registry;
    c_Lookup_Tree CMD_Script_Registry_Lookup_Tree;

    //The table that handles the script tokens.
    c_Table_1D* Tokens;

    //The table handler linking this to the tables.
    c_Table_Handler* Tables;

public:

    //The SANDBOX to work with.
    //-This is public for now until all of the API functions are written and integrated into the command parser.
    c_NT3_SANDBOX Sandbox;

    c_NT3_Command_Parser()
    {
        init();
    }

    ~c_NT3_Command_Parser()
    {

        //Cleanup the tables.
        if (Tables != NULL)
        {
            delete Tables;
            Tables = NULL;
        }
    }

    void init()
    {
        std::cout << "\n c_NT3_Command_Parser()"; std::cout.flush();

        //Set the table handler.
        Tables = new c_Table_Handler;

        CMD_Sandbox = Tables->register_New_Table_1D("CMD_Sandbox");
        CMD_Script_Registry = Tables->register_New_Table_1D("CMD_Script_Registry");

        Command_String = "";

        Tokens = Tables->register_New_Table_1D("Token_Table");

        flg_String = 0;
        flg_Undefined = 0;
        flg_Var_Pos = 0;

        std::cout << " <>";

        //May use this feature in the future. For now it is commented out.
        //eval_Command_String("load_CMD_script autoexec");
        //eval_Command_String("eval_CMD_table autoexec");
    }

    //Allows for live input into the parser for debug and root control.
    void rcon()
    {
        //Until the std::string "return" is found do not exit the command loop.
        std::cout << "\n\n\t\t NeuroTox rcon";
        std::cout << "\n\t\t   ->help -outputs information on available commands.";
        std::cout << "\n\t\t   ->otables -shows all the tables.";
        std::cout << "\n\t\t   ->return -exits the rcon.";


        while (1)
        {
            std::cout << "\n\n";

            std::cout << "\n\n\t ->";

            getline(std::cin, Command_String);

            if (Command_String == "exit") { return; }
            if (Command_String == "otables") { Tables->output_Table(0); }
            if (Command_String == "help" || Command_String == "?") { system("type NT3_Command_Text_Reference.dat"); }
            eval_Command_String(Command_String);

            //Tables->output_Table(0);
        }
    }

    //This is used to search the table for labels and and submit them with their index to the Label_Tree.
    //Each instance of the table parser has its own labels.
    std::string labelize_Command_Table(c_Table_1D* p_Command_Table, c_Lookup_Tree* p_Labels)
    {
        //This keeps track of the current position in the table, it is passed to the parser for modification with goto statements.
        int EIP = 0;

        //The current command string.
        std::string CMD_Line = "";

        //Loop through each line in the command table evaluating them as it goes.
        for (EIP = 0; EIP < p_Command_Table->get_Row_Count(); EIP++)
        {
            //Get the current command string.
            p_Command_Table->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(EIP, 0, &CMD_Line);

            //Labelize the current command text string.
            labelize_Command_Text(CMD_Line, p_Labels, &EIP);
        }

        //Debug
        p_Labels->output_Tree();

        return "LABELIZER_COMPLETED_TASK";
    }

    //If the submitted std::string is a label then submit it to the p_Labels tree.
    std::string labelize_Command_Text(std::string p_Command_Text, c_Lookup_Tree* p_Labels, int* p_EIP)
    {
        if (!p_Command_Text.size()) { return "ERR_NO_TEXT_SUBMITTED->LABELIZE_COMMAND_STRING->p_Command_Text"; }

        //Labels are identified by the character ':'.
        if (p_Command_Text[0] == ':')
        {
            p_Labels->set_int(p_Command_Text, *p_EIP);
            return ("LABEL_FOUND->" + p_Command_Text);
        }
        return "NO_LABEL_FOUND";
    }

    //Evaluates a given table.
    //This is done by feeding the table row by row into the eval_Command_String().
    //A for loop controls the reading of the table, however, different commands will be able to modify the counter.
    //By modifying the counter we can have jumps and loops within the table.
    //To avoid variable conflicts this will have its own Binary_Tree, however, it will be used to store labels and their positions.
    //Command tables will be one dimensional.
    std::string eval_Command_Table(c_Table_1D* p_Command_Table)
    {
        //This keeps track of the current position in the table, it is passed to the parser for modification with goto statements.
        int EIP = 0;

        //The current command string.
        std::string CMD_Line = "";

        //The labels for the current table.
        c_Lookup_Tree Labels;

        //Find the labels in the table and store them in the lookup tree for the goto commands to use.
        labelize_Command_Table(p_Command_Table, &Labels);

        //Loop through each line in the command table evaluating them as it goes.
        for (EIP = 0; EIP < p_Command_Table->get_Row_Count(); EIP++)
        {
            //Get the current command string.
            p_Command_Table->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(EIP, 0, &CMD_Line);
            eval_Command_String(CMD_Line, &EIP, &Labels);
        }

        return "COMMAND_PARSER->EVAL_COMMAND_TABLE_SUCCESS";
    }

    //Overloaded for single std::string submission.
    std::string eval_Command_String(std::string p_Command_Text)
    {
        std::string tmp_Return = "";

        int tmp_EIP = 0;
        c_Lookup_Tree tmp_Labels;
        //c_Table_1D * Tokens = Tables->register_New_Table_1D("Token_Table");
        tmp_Return = eval_Command_String(p_Command_Text, &tmp_EIP, &tmp_Labels, Tokens);
        //Tables->delete_Table(Tokens->get_Table_ID());

        return tmp_Return;
    }

    //Same as eval_Command_String but shortened for the API.
    std::string cmd(std::string p_Command_Text)
    {
        std::string tmp_Return = "";

        int tmp_EIP = 0;
        c_Lookup_Tree tmp_Labels;
        //c_Table_1D * Tokens = Tables->register_New_Table_1D("Token_Table");
        tmp_Return = eval_Command_String(p_Command_Text, &tmp_EIP, &tmp_Labels, Tokens);
        //Tables->delete_Table(Tokens->get_Table_ID());

        return tmp_Return;
    }

    //Overloaded for non-recursive submission.
    std::string eval_Command_String(std::string p_Command_Text, int* p_EIP, c_Lookup_Tree* p_Labels)
    {
        std::string tmp_Return = "";

        c_Table_1D* Tokens = Tables->register_New_Table_1D("Token_Table");
        tmp_Return = eval_Command_String(p_Command_Text, p_EIP, p_Labels, Tokens);
        Tables->delete_Table(Tokens->get_Table_ID());

        return tmp_Return;
    }



    //Evaluates a submitted string.
    //It does this by breaking the std::string down into componets.
    //Each componet is stored in a separate string, in an array of strings.
    //Spaces act as deliminators between command componets, the semicolon acts as an eol character.
    std::string eval_Command_String(std::string p_Command_Text, int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {
        //Break the command text down into tokens.
        tokenize_Command_Text(p_Command_Text, p_Tokens);

        //std::cout << "\n\n\t Tokens:";
        //p_Tokens->output_Table();

        //Replace the variables with their values.
        convert_Variables_To_Values(p_Tokens);

        //Evaluates the tokens.
        return eval_Tokens(p_EIP, p_Labels, p_Tokens);


        //std::cout << "\n\n\t Var_Lookup_Tree:";
        //Var_Lookup_Tree.output_Tree();
    }

    //Breaks the command std::string down into tokens.
    void tokenize_Command_Text(std::string p_Command_Text, c_Table_1D* p_Tokens)
    {
        //Prep for new submission.
        p_Tokens->reset_Table();
        flg_Var_Pos = 0;
        flg_String = 0;

        //Loop through the command text finding variable identifiers and extracting them.
        for (unsigned int cou_Index = 0; cou_Index < p_Command_Text.size(); cou_Index++)
        {
            //std::cout << "\n\t p_Command_Text[" << cou_Index << "]->" << p_Command_Text[cou_Index];


            //This allows for special characters to be read in, useful for setting table entries where a variable is needed.
            if (p_Command_Text[cou_Index] == '^' && (cou_Index + 1) < p_Command_Text.size())
            {
                p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_C(p_Command_Text[cou_Index + 1]);
                cou_Index++;
                continue;
            }

            //Check for strings.
            if (p_Command_Text[cou_Index] == '"')
            {

                //If a std::string is being read in then stop the reading.
                if (flg_String)
                {
                    flg_String = 0;

                    //End the current string.
                    p_Tokens->pop_Cell_In_Current_Row();
                    p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_I(0);
                    p_Tokens->pop_Row();
                }
                else
                {
                    flg_String = 1;
                    //If the current token does not contain whitespace then increment the current token.
                    if (p_Tokens->get_Set_State_For_Given_Cell_In_Current_Row(0))
                    {
                        p_Tokens->pop_Cell_In_Current_Row();
                        p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_I(0);
                        p_Tokens->pop_Row();
                    }
                }

                //Do not read in the '"' character.
                continue;
            }

            //If there is currently a std::string being read in then add to the current token irregardless of special characters, whitespace, or variables.
            if (flg_String)
            {
                p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_C(p_Command_Text[cou_Index]);
                continue;
            }



            //If a space is encountered then start on the next token.
            if (p_Command_Text[cou_Index] == ' ')
            {
                //If the current token does not contain whitespace then increment the current token.
                if (p_Tokens->get_Set_State_For_Given_Cell_In_Current_Row(0))
                {
                    p_Tokens->pop_Cell_In_Current_Row();
                    p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_I(0);
                    p_Tokens->pop_Row();
                }
                continue;
            }

            //If a '%' is encountered then check the flg_Var_Pos to determine if a new token is needed, or if the current token is ending.
            if (p_Command_Text[cou_Index] == '%' && flg_Var_Pos == 0)
            {
                //std::cout << "\n\t Found a variable...";

                //If the current token does not contain whitespace then increment the current token.
                if (p_Tokens->get_Set_State_For_Given_Cell_In_Current_Row(0))
                {
                    p_Tokens->pop_Cell_In_Current_Row();
                    p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_I(0);
                    p_Tokens->pop_Row();
                }
                flg_Var_Pos = 1;
                continue;
            }

            if (p_Command_Text[cou_Index] == '%' && flg_Var_Pos == 1)
            {
                //std::cout << "\n\t\t End of variable string, setting flag on cell...";
                p_Tokens->pop_Cell_In_Current_Row(); p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_I(1);
                if (cou_Index < (p_Command_Text.size() - 1)) { p_Tokens->pop_Row(); }
                flg_Var_Pos = 0;
                continue;
            }

            //Add the current character to the current token.
            p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_C(p_Command_Text[cou_Index]);
            //std::cout << "\n\t\t p_Command_Text[" << cou_Index << "]->" << int (p_Command_Text[cou_Index]);

            if (cou_Index == (p_Command_Text.size() - 1) && p_Tokens->get_Set_State_For_Given_Cell_In_Current_Row(0)) { p_Tokens->pop_Cell_In_Current_Row(); p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_I(0); }


        }

        //Debug information.
        //p_Tokens->output_Table();
    }

    //Replaces any token that represents a variable with its value.
    void convert_Variables_To_Values(c_Table_1D* p_Tokens)
    {
        //Reset the undefined flag.
        flg_Undefined = 0;

        //Temp for current var type.
        int tmp_Var_Type = 0;

        //Temps std::string for the variable name.
        std::string tmp_Var_Name = "";

        //Stores the temporary data. 
        std::string tmp_Var_Data = "";

        //Loop through the tokens querying the lookup tree for the values. If no variable yet exists with that name then it is added to the table and a null value returned. The undefined error is thrown.
        for (int cou_Index = 0; cou_Index < p_Tokens->get_Row_Count(); cou_Index++)
        {
            //std::cout << "\n\t p_Tokens->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(cou_Index, 1, 0)->" << p_Tokens->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(cou_Index, 1, 0); 

            //std::cout << "\n\n\n\tconvert_Variables_To_Values();";
            //p_Tokens->output_Table();

            if (p_Tokens->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(cou_Index, 1, 0) == 1)
            {
                //std::cout << "\n\n\t Variable Detected, Token Number " << cou_Index << "...";
                //Get the variable name.
                p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Var_Name);

                //std::cout << "\n\t Variable Name->" << tmp_Var_Name;

                //Google it.
                tmp_Var_Type = Var_Lookup_Tree.get_Var_Type(tmp_Var_Name);

                //std::cout << "\n\t Variable Type->" << tmp_Var_Type;


                //Replace the variable with the data found.
                if (tmp_Var_Type == 0) { tmp_Var_Data = Var_Lookup_Tree.get_string(tmp_Var_Name); }
                if (tmp_Var_Type == 1) { tmp_Var_Data = int2str(Var_Lookup_Tree.get_int(tmp_Var_Name)); }
                if (tmp_Var_Type == 2) { tmp_Var_Data = float2str(Var_Lookup_Tree.get_float(tmp_Var_Name)); }
                //if (tmp_Var_Type == 3){ p_Tokens->set_Data_Bit_For_Given_Cell_In_Given_Row_NR(cou_Index, 0, Var_Lookup_Tree.get_reference(tmp_Var_Name)); }
                if (tmp_Var_Type == 4) { tmp_Var_Data = bool2str(Var_Lookup_Tree.get_bool(tmp_Var_Name)); }

                //std::cout << "\n\t Variable Data->" << tmp_Var_Data; 

                //std::cout << "\n\t Setting Cell[0] in Row[" << cou_Index << "] to Data->" << tmp_Var_Data;
                p_Tokens->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(cou_Index, 0, tmp_Var_Data);
            }
        }

    }

    //Checks a token to determine if it is a command or callback. If so it is evaluated and the value returned.
    std::string test_Command_Token(std::string p_Token, int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {
        std::string tmp_Token = "";

        //If the first character is a tilde then extract the rest of the std::string and evaluate_Command_Text(), returning the returned value, otherwise return the token.
        if (p_Token[0] == '~')
        {
            //Gather the token.
            for (unsigned int cou_Index = 1; cou_Index < p_Token.size(); cou_Index++)
            {
                tmp_Token += p_Token[cou_Index];
            }

            //Return the result of the eval.
            return eval_Command_String(tmp_Token, p_EIP, p_Labels);
        }

        //If it is not a command then return the token.
        return p_Token;
    }

    //Evaluates the tokens in the token table.
    std::string eval_Tokens(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {
        //The temporary std::string for the token being evaluated.
        std::string tmp_Token[100];

        //Debug
        //std::cout << "\n\n\n\t eval_Tokens()";
        //p_Tokens->output_Table();

        //Get the current token in std::string format.
        p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(0, 0, &tmp_Token[0]);


        //These are the basic commands for the scripting language.
        if (tmp_Token[0] == "echo") { return CMD_echo(p_EIP, p_Labels, p_Tokens); }
        if (tmp_Token[0] == "table") { return CMD_table(p_EIP, p_Labels, p_Tokens); }
        if (tmp_Token[0] == "if") { return CMD_if(p_EIP, p_Labels, p_Tokens); } //Passing EIP as this calls the eval_Command_String.
        if (tmp_Token[0] == "output") { return CMD_output(p_EIP, p_Labels, p_Tokens); }

        //Data manipulation commands
        if (tmp_Token[0] == "cin") { return CMD_cin(p_EIP, p_Labels, p_Tokens); }
        if (tmp_Token[0] == "set") { return CMD_set(p_EIP, p_Labels, p_Tokens); }
        if (tmp_Token[0] == "gdt") { return CMD_get_datatype(p_EIP, p_Labels, p_Tokens); }
        if (tmp_Token[0] == "com") { return CMD_operation(p_EIP, p_Labels, p_Tokens); }

        //Commands related to the Command tables.
        if (tmp_Token[0] == "eCMD") { return CMD_eval(p_EIP, p_Labels, p_Tokens); } //The EIP and Labels are only for the test_Command_Token.
        if (tmp_Token[0] == "bCMD") { return CMD_beval(p_EIP, p_Labels, p_Tokens); } //The EIP and Labels are only for the test_Command_Token.
        if (tmp_Token[0] == "new_CMD") { return CMD_New(p_EIP, p_Labels, p_Tokens); }
        if (tmp_Token[0] == "goto") { return CMD_goto(p_EIP, p_Labels, p_Tokens); }

        //Commands related to saving and loading command tables.
        if (tmp_Token[0] == "scrs") { return CMD_save_script(p_EIP, p_Labels, p_Tokens); }
        if (tmp_Token[0] == "scrl") { return CMD_load_script(p_EIP, p_Labels, p_Tokens); }
        if (tmp_Token[0] == "scrr") { return CMD_reload_script(p_EIP, p_Labels, p_Tokens); }
        if (tmp_Token[0] == "srealm") { return save_Realm(p_EIP, p_Labels, p_Tokens); }
        if (tmp_Token[0] == "lrealm") { return load_Realm(p_EIP, p_Labels, p_Tokens); }

        //Allows for CMD_tables to be evaluated as commands.
        if (CMD_Script_Registry_Lookup_Tree.exists(tmp_Token[0])) { p_Tokens->shift_All_Rows(0); return CMD_eval(p_EIP, p_Labels, p_Tokens); }

        //These are the callbacks for the engine. 
        //if (tmp_Token[0] == "sand")             { return NT_sand           (p_EIP, p_Labels, p_Tokens); }
        //Now falls through to the NT_Sand member.
        return NT_Sand(p_EIP, p_Labels, p_Tokens);

        ostr(0, 12, "\n Command NOT recognized!");
        return "0";
    }

    //Separates the tokens.
    std::string CMD_Separate_Tokens(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens, std::string* p_Tmp_Tokens)
    {
        //Gather all the tokens up to the limit of 10.
        for (int cou_Index = 0; cou_Index < p_Tokens->get_Row_Count(); cou_Index++)
        {
            if (cou_Index >= 100) { break; }
            p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &(p_Tmp_Tokens[cou_Index]));
            p_Tmp_Tokens[cou_Index] = test_Command_Token(p_Tmp_Tokens[cou_Index], p_EIP, p_Labels, p_Tokens);
            //std::cout << "\n\t p_Tmp_Tokens[" << cou_Index << "]->" << p_Tmp_Tokens[cou_Index];
        }
        return "CMD_Separate_Tokens";
    }

    //--     CMD FUNCTIONS


    std::string CMD_echo(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {
        std::string tmp_echo_Token;

        //p_Tokens->output_Table();

        std::cout << "\n\t\t ->";
        for (int cou_Index = 1; cou_Index < p_Tokens->get_Row_Count(); cou_Index++)
        {
            p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_echo_Token);

            //Checks for command tokens.
            tmp_echo_Token = test_Command_Token(tmp_echo_Token, p_EIP, p_Labels, p_Tokens);

            std::cout << tmp_echo_Token << " ";
        }
        return "1";
    }


    //std::cin Var_Name / = 
    // 0     1     2 3
    std::string CMD_cin(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {
        std::string tmp_Token[10];

        p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(1, 0, &tmp_Token[1]);
        tmp_Token[1] = test_Command_Token(tmp_Token[1], p_EIP, p_Labels, p_Tokens);

        p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(2, 0, &tmp_Token[2]);
        tmp_Token[2] = test_Command_Token(tmp_Token[2], p_EIP, p_Labels, p_Tokens);

        p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(3, 0, &tmp_Token[3]);
        tmp_Token[3] = test_Command_Token(tmp_Token[3], p_EIP, p_Labels, p_Tokens);

        p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(4, 0, &tmp_Token[4]);
        tmp_Token[4] = test_Command_Token(tmp_Token[4], p_EIP, p_Labels, p_Tokens);

        std::cout << "\n\n\t" << tmp_Token[4];
        std::string tmp_Input = "";
        getline(std::cin, tmp_Input);

        //std::cout << "\n\n\t tmp_Token[2]->" << tmp_Token[2] << "  tmp_Token[3]->" << tmp_Token[3] << "  tmp_Token[4]->" << tmp_Token[4];

        //Simple assignment.
        if (tmp_Token[1] == "/s") { if (tmp_Token[3] == "=") { Var_Lookup_Tree.set_string(tmp_Token[2], tmp_Input); } }
        if (tmp_Token[1] == "/i") { if (tmp_Token[3] == "=") { Var_Lookup_Tree.set_int(tmp_Token[2], str2int(tmp_Input)); } }
        if (tmp_Token[1] == "/f") { if (tmp_Token[3] == "=") { Var_Lookup_Tree.set_float(tmp_Token[2], str2float(tmp_Input)); } }
        if (tmp_Token[1] == "/b") { if (tmp_Token[3] == "=") { Var_Lookup_Tree.set_bool(tmp_Token[2], str2bool(tmp_Input)); } }

        //Addition
        if (tmp_Token[1] == "/s") { if (tmp_Token[3] == "+=") { Var_Lookup_Tree.set_string(tmp_Token[2], (Var_Lookup_Tree.get_string(tmp_Token[2]) + tmp_Input)); } }
        if (tmp_Token[1] == "/i") { if (tmp_Token[3] == "+=") { Var_Lookup_Tree.set_int(tmp_Token[2], (Var_Lookup_Tree.get_int(tmp_Token[2]) + str2int(tmp_Input))); } }
        if (tmp_Token[1] == "/f") { if (tmp_Token[3] == "+=") { Var_Lookup_Tree.set_float(tmp_Token[2], (Var_Lookup_Tree.get_float(tmp_Token[2]) + str2float(tmp_Input))); } }

        //Subtraction
        if (tmp_Token[1] == "/i") { if (tmp_Token[3] == "-=") { Var_Lookup_Tree.set_int(tmp_Token[2], (Var_Lookup_Tree.get_int(tmp_Token[2]) - str2int(tmp_Input))); } }
        if (tmp_Token[1] == "/f") { if (tmp_Token[3] == "-=") { Var_Lookup_Tree.set_float(tmp_Token[2], (Var_Lookup_Tree.get_float(tmp_Token[2]) - str2float(tmp_Input))); } }

        //Multiplication
        if (tmp_Token[1] == "/i") { if (tmp_Token[3] == "*=") { Var_Lookup_Tree.set_int(tmp_Token[2], (Var_Lookup_Tree.get_int(tmp_Token[2]) * str2int(tmp_Input))); } }
        if (tmp_Token[1] == "/f") { if (tmp_Token[3] == "*=") { Var_Lookup_Tree.set_float(tmp_Token[2], (Var_Lookup_Tree.get_float(tmp_Token[2]) * str2float(tmp_Input))); } }

        //Division
        if (tmp_Token[1] == "/i") { if (tmp_Token[3] == "/=") { Var_Lookup_Tree.set_int(tmp_Token[2], (Var_Lookup_Tree.get_int(tmp_Token[2]) / str2int(tmp_Input))); } }
        if (tmp_Token[1] == "/f") { if (tmp_Token[3] == "/=") { Var_Lookup_Tree.set_float(tmp_Token[2], (Var_Lookup_Tree.get_float(tmp_Token[2]) / str2float(tmp_Input))); } }

        //References will be treated as integers.
        //if (tmp_Token[1] == "/r"){ Var_Lookup_Tree.set_reference(tmp_Token[1], tmp_Token[3]);
        //std::cout << "\n\n\n\t ";
        //Var_Lookup_Tree.output_Current_Node();
        return "1";
    }


    std::string CMD_set(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {
        std::string tmp_Token[100];
        CMD_Separate_Tokens(p_EIP, p_Labels, p_Tokens, tmp_Token);

        //std::cout << "\n\n\t tmp_Token[2]->" << tmp_Token[2] << "  tmp_Token[3]->" << tmp_Token[3] << "  tmp_Token[4]->" << tmp_Token[4];

        //Simple assignment.
        if (tmp_Token[1] == "/s") { if (tmp_Token[3] == "=") { Var_Lookup_Tree.set_string(tmp_Token[2], tmp_Token[4]); } }
        for (int cou_Index = 5; cou_Index < p_Tokens->get_Row_Count(); cou_Index++)
        {
            if (tmp_Token[1] == "/s") { if (tmp_Token[3] == "=") { Var_Lookup_Tree.set_string(tmp_Token[2], (Var_Lookup_Tree.get_string(tmp_Token[2]) + tmp_Token[cou_Index])); } }
        }
        if (tmp_Token[1] == "/i") { if (tmp_Token[3] == "=") { Var_Lookup_Tree.set_int(tmp_Token[2], str2int(tmp_Token[4])); } }
        if (tmp_Token[1] == "/f") { if (tmp_Token[3] == "=") { Var_Lookup_Tree.set_float(tmp_Token[2], str2float(tmp_Token[4])); } }
        if (tmp_Token[1] == "/b") { if (tmp_Token[3] == "=") { Var_Lookup_Tree.set_bool(tmp_Token[2], str2bool(tmp_Token[4])); } }

        //Addition
        if (tmp_Token[1] == "/s") { if (tmp_Token[3] == "+=") { Var_Lookup_Tree.set_string(tmp_Token[2], (Var_Lookup_Tree.get_string(tmp_Token[2]) + tmp_Token[4])); } }
        if (tmp_Token[1] == "/i") { if (tmp_Token[3] == "+=") { Var_Lookup_Tree.set_int(tmp_Token[2], (Var_Lookup_Tree.get_int(tmp_Token[2]) + str2int(tmp_Token[4]))); } }
        if (tmp_Token[1] == "/f") { if (tmp_Token[3] == "+=") { Var_Lookup_Tree.set_float(tmp_Token[2], (Var_Lookup_Tree.get_float(tmp_Token[2]) + str2float(tmp_Token[4]))); } }

        //Subtraction
        if (tmp_Token[1] == "/i") { if (tmp_Token[3] == "-=") { Var_Lookup_Tree.set_int(tmp_Token[2], (Var_Lookup_Tree.get_int(tmp_Token[2]) - str2int(tmp_Token[4]))); } }
        if (tmp_Token[1] == "/f") { if (tmp_Token[3] == "-=") { Var_Lookup_Tree.set_float(tmp_Token[2], (Var_Lookup_Tree.get_float(tmp_Token[2]) - str2float(tmp_Token[4]))); } }

        //Multiplication
        if (tmp_Token[1] == "/i") { if (tmp_Token[3] == "*=") { Var_Lookup_Tree.set_int(tmp_Token[2], (Var_Lookup_Tree.get_int(tmp_Token[2]) * str2int(tmp_Token[4]))); } }
        if (tmp_Token[1] == "/f") { if (tmp_Token[3] == "*=") { Var_Lookup_Tree.set_float(tmp_Token[2], (Var_Lookup_Tree.get_float(tmp_Token[2]) * str2float(tmp_Token[4]))); } }

        //Division
        if (tmp_Token[1] == "/i") { if (tmp_Token[3] == "/=") { Var_Lookup_Tree.set_int(tmp_Token[2], (Var_Lookup_Tree.get_int(tmp_Token[2]) / str2int(tmp_Token[4]))); } }
        if (tmp_Token[1] == "/f") { if (tmp_Token[3] == "/=") { Var_Lookup_Tree.set_float(tmp_Token[2], (Var_Lookup_Tree.get_float(tmp_Token[2]) / str2float(tmp_Token[4]))); } }

        //References will be treated as integers.
        //if (tmp_Token[1] == "/r"){ Var_Lookup_Tree.set_reference(tmp_Token[1], tmp_Token[3]);
        //std::cout << "\n\n\n\t ";
        //Var_Lookup_Tree.output_Current_Node();
        return "1";
    }

    //The good ol 'if' command. Essential to almost all algorithms.
    std::string CMD_if(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {

        //The result of the comparision being performed.
        bool flg_Result = false;

        //The temporary command std::string to submit for eval.
        std::string tmp_Command_Text = "";

        std::string tmp_Token[100];

        //Gather all the tokens up to the limit of 10.
        for (int cou_Index = 0; cou_Index < p_Tokens->get_Row_Count(); cou_Index++)
        {
            if (cou_Index >= 100) { break; }
            p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
            tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
            //std::cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
        }

        //if TYPE ( F1 Op F2 ) EVAL
        // 0    1 2  3  4  5 6    7
        //Get the result of the comparison based on the datatype of the first factor.
        if (tmp_Token[1] == "s")
        {
            if (tmp_Token[4] == "==") { flg_Result = (tmp_Token[3] == tmp_Token[5]); }
            if (tmp_Token[4] == ">=") { flg_Result = (tmp_Token[3] >= tmp_Token[5]); }
            if (tmp_Token[4] == "<=") { flg_Result = (tmp_Token[3] <= tmp_Token[5]); }
            if (tmp_Token[4] == ">") { flg_Result = (tmp_Token[3] > tmp_Token[5]); }
            if (tmp_Token[4] == "<") { flg_Result = (tmp_Token[3] < tmp_Token[5]); }
        }
        if (tmp_Token[1] == "i")
        {
            int tmp_int[2];
            tmp_int[0] = str2int(tmp_Token[3]);
            tmp_int[1] = str2int(tmp_Token[5]);
            if (tmp_Token[4] == "==") { flg_Result = (tmp_int[0] == tmp_int[1]); }
            if (tmp_Token[4] == ">=") { flg_Result = (tmp_int[0] >= tmp_int[1]); }
            if (tmp_Token[4] == "<=") { flg_Result = (tmp_int[0] <= tmp_int[1]); }
            if (tmp_Token[4] == ">") { flg_Result = (tmp_int[0] > tmp_int[1]); }
            if (tmp_Token[4] == "<") { flg_Result = (tmp_int[0] < tmp_int[1]); }
        }
        if (tmp_Token[1] == "f")
        {
            double tmp_float[2];
            tmp_float[0] = str2float(tmp_Token[3]);
            tmp_float[1] = str2float(tmp_Token[5]);
            if (tmp_Token[4] == "==") { flg_Result = (tmp_float[0] == tmp_float[1]); }
            if (tmp_Token[4] == ">=") { flg_Result = (tmp_float[0] >= tmp_float[1]); }
            if (tmp_Token[4] == "<=") { flg_Result = (tmp_float[0] <= tmp_float[1]); }
            if (tmp_Token[4] == ">") { flg_Result = (tmp_float[0] > tmp_float[1]); }
            if (tmp_Token[4] == "<") { flg_Result = (tmp_float[0] < tmp_float[1]); }
        }
        if (tmp_Token[1] == "b")
        {
            int tmp_bool[2];
            tmp_bool[0] = str2bool(tmp_Token[3]);
            tmp_bool[1] = str2bool(tmp_Token[5]);
            if (tmp_Token[4] == "==") { flg_Result = (tmp_bool[0] == tmp_bool[1]); }
            if (tmp_Token[4] == ">=") { flg_Result = (tmp_bool[0] >= tmp_bool[1]); }
            if (tmp_Token[4] == "<=") { flg_Result = (tmp_bool[0] <= tmp_bool[1]); }
            if (tmp_Token[4] == ">") { flg_Result = (tmp_bool[0] > tmp_bool[1]); }
            if (tmp_Token[4] == "<") { flg_Result = (tmp_bool[0] < tmp_bool[1]); }
        }

        std::cout << "\n\n\t\t (" << tmp_Token[3] << " " << tmp_Token[4] << " " << tmp_Token[5] << ") " << flg_Result;

        //If true then format the remaining tokens into a std::string and evaluate them.
        if (flg_Result)
        {
            for (int cou_Index = 7; cou_Index < p_Tokens->get_Row_Count(); cou_Index++)
            {
                //Gather the tokens and concatinate them onto the temporary command text.
                p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S((cou_Index), 0, &tmp_Token[0]);
                tmp_Command_Text += tmp_Token[0] + " ";
            }

            //Evaluate the gathered command text gathered.
            eval_Command_String(tmp_Command_Text, p_EIP, p_Labels);
        }

        //Return the result of the comparision.
        return bool2str(flg_Result);
    }


    std::string CMD_table(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {
        std::string tmp_Token[100];

        //Gather all the tokens up to the limit of 10.
        for (int cou_Index = 0; cou_Index < p_Tokens->get_Row_Count(); cou_Index++)
        {
            if (cou_Index >= 100) { break; }
            p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
            tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
            //std::cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
        }

        //Do the right thing.

        //Sets a given row and index in a given table to the given data.
        //if (tmp_Token[1] == "set_int"){ Tables->set_int(str2int());

        //==--     BY NAME

        //Sets a given rows cell to the given data.
        //set_X( Table_ID, Row, Cell, Data)
        //Simple assignment.
        std::string tmp_String = "";
        if (tmp_Token[1] == "set_string")
        {
            tmp_String = tmp_Token[5];

            for (int cou_Index = 5; cou_Index < p_Tokens->get_Row_Count(); cou_Index++)
            {
                tmp_String += tmp_Token[cou_Index];
            }
        }

        if (tmp_Token[1] == "push_string")
        {
            tmp_String = tmp_Token[4];

            for (int cou_Index = 5; cou_Index < p_Tokens->get_Row_Count(); cou_Index++)
            {
                tmp_String += tmp_Token[cou_Index];
            }
        }

        if (tmp_Token[1] == "push_pop_string" || tmp_Token[1] == "pop_push_string")
        {
            tmp_String = tmp_Token[3];

            for (int cou_Index = 4; cou_Index < p_Tokens->get_Row_Count(); cou_Index++)
            {
                tmp_String += tmp_Token[cou_Index];
            }
        }

        if (tmp_Token[1] == "set_string") { Tables->set_string(tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]), tmp_String); return "1"; }
        if (tmp_Token[1] == "set_int") { Tables->set_int(tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2int(tmp_Token[5])); return "1"; }
        if (tmp_Token[1] == "set_float") { Tables->set_float(tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2float(tmp_Token[5])); return "1"; }
        if (tmp_Token[1] == "set_bool") { Tables->set_bool(tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2bool(tmp_Token[5])); return "1"; }

        if (tmp_Token[1] == "push_string") { Tables->push_string(tmp_Token[2], str2int(tmp_Token[3]), tmp_String); return "1"; }
        if (tmp_Token[1] == "push_int") { Tables->push_int(tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4])); return "1"; }
        if (tmp_Token[1] == "push_float") { Tables->push_float(tmp_Token[2], str2int(tmp_Token[3]), str2float(tmp_Token[4])); return "1"; }
        if (tmp_Token[1] == "push_bool") { Tables->push_bool(tmp_Token[2], str2int(tmp_Token[3]), str2bool(tmp_Token[4])); return "1"; }

        if (tmp_Token[1] == "pop_push_string") { Tables->pop_push_string(tmp_Token[2], tmp_String); return "1"; }
        if (tmp_Token[1] == "pop_push_int") { Tables->pop_push_int(tmp_Token[2], str2int(tmp_Token[3])); return "1"; }
        if (tmp_Token[1] == "pop_push_float") { Tables->pop_push_float(tmp_Token[2], str2float(tmp_Token[3])); return "1"; }
        if (tmp_Token[1] == "pop_push_bool") { Tables->pop_push_bool(tmp_Token[2], str2bool(tmp_Token[3])); return "1"; }

        if (tmp_Token[1] == "push_pop_string") { Tables->push_pop_string(tmp_Token[2], tmp_String); return "1"; }
        if (tmp_Token[1] == "push_pop_int") { Tables->push_pop_int(tmp_Token[2], str2int(tmp_Token[3])); return "1"; }
        if (tmp_Token[1] == "push_pop_float") { Tables->push_pop_float(tmp_Token[2], str2float(tmp_Token[3])); return "1"; }
        if (tmp_Token[1] == "push_pop_bool") { Tables->push_pop_bool(tmp_Token[2], str2bool(tmp_Token[3])); return "1"; }


        //Gets a given cells data in a given row.
        //get_X( Table_ID, Row, Cell)
        if (tmp_Token[1] == "get_string") { return          (Tables->get_string(tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]))); }
        if (tmp_Token[1] == "get_int") { return int2str(Tables->get_int(tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2int(tmp_Token[5]))); }
        if (tmp_Token[1] == "get_float") { return float2str(Tables->get_float(tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2int(tmp_Token[5]))); }
        if (tmp_Token[1] == "get_bool") { return bool2str(Tables->get_bool(tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2int(tmp_Token[5]))); }

        //==--     BY ID

        //Sets a given rows cell to the given data.
        //set_X( Table_ID, Row, Cell, Data)
        if (tmp_Token[1] == "bset_string") { Tables->set_string(str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]), tmp_Token[5]); return "1"; }
        if (tmp_Token[1] == "bset_int") { Tables->set_int(str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2int(tmp_Token[5])); return "1"; }
        if (tmp_Token[1] == "bset_float") { Tables->set_float(str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2float(tmp_Token[5])); return "1"; }
        if (tmp_Token[1] == "bset_bool") { Tables->set_bool(str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2bool(tmp_Token[5])); return "1"; }


        //Gets a given cells data in a given row.
        //get_X( Table_ID, Row, Cell)
        if (tmp_Token[1] == "bget_string") { return          (Tables->get_string(str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]))); }
        if (tmp_Token[1] == "bget_int") { return int2str(Tables->get_int(str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]))); }
        if (tmp_Token[1] == "bget_float") { return float2str(Tables->get_float(str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]))); }
        if (tmp_Token[1] == "bget_bool") { return bool2str(Tables->get_bool(str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]))); }

        //Copies the data from the submitted cell into itself.
        //copy(int p_Table, int p_Row, int p_Cell, c_Cell_1D * p_Cell_To_Copy)
        //table-0 copy-1 from_Tbl-2 from_Row-3 from_Cell-4 To_Tbl-5 To_Row-6 To_Cell-7
        if (tmp_Token[1] == "copy_cell")
        {
            if (p_Tokens->get_Row_Count() < 7) { std::cout << "\n\n\n\t SYN_ERR_TABLE_COPY: #tokens = " << p_Tokens->get_Row_Count(); return "SYN_ERR_TABLE_COPY"; }

            //Holds the cell to help with reading the code.
            void* tmp_Cell = Tables->get_Table_Cell_Reference(tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]));

            Tables->copy(tmp_Token[5], str2int(tmp_Token[6]), str2int(tmp_Token[7]), tmp_Cell);
            return "TABLE_CELL_COPY";
        }
        if (tmp_Token[1] == "bcopy_cell")
        {
            if (p_Tokens->get_Row_Count() < 7) { std::cout << "\n\n\n\t SYN_ERR_TABLE_COPY: #tokens = " << p_Tokens->get_Row_Count(); return "SYN_ERR_TABLE_COPY"; }

            //Holds the cell to help with reading the code.
            void* tmp_Cell = Tables->get_Table_Cell_Reference(str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]));

            Tables->copy(str2int(tmp_Token[5]), str2int(tmp_Token[6]), str2int(tmp_Token[7]), tmp_Cell);
            return "TABLE_CELL_COPY";
        }

        //table copy_row FROM_TABLE FROM_ROW TO_TABLE TO_ROW
        //  0       1        2          3      4        5
        if (tmp_Token[1] == "copy_row")
        {
            if (p_Tokens->get_Row_Count() < 5) { std::cout << "\n\n\n\t SYN_ERR_TABLE_COPY: #tokens = " << p_Tokens->get_Row_Count(); return "SYN_ERR_TABLE_COPY"; }

            //Holds the row to help with reading the code.
            void* tmp_Row = Tables->get_Table_Row_Reference(tmp_Token[2], str2int(tmp_Token[3]));

            Tables->copy_Row(tmp_Token[4], str2int(tmp_Token[5]), tmp_Row);
            return "TABLE_ROW_COPY";
        }
        if (tmp_Token[1] == "bcopy_row")
        {
            if (p_Tokens->get_Row_Count() < 5) { std::cout << "\n\n\n\t SYN_ERR_TABLE_COPY: #tokens = " << p_Tokens->get_Row_Count(); return "SYN_ERR_TABLE_COPY"; }

            //Holds the row to help with reading the code.
            void* tmp_Row = Tables->get_Table_Row_Reference(str2int(tmp_Token[2]), str2int(tmp_Token[3]));

            Tables->copy_Row(str2int(tmp_Token[4]), str2int(tmp_Token[5]), tmp_Row);
            return "TABLE_ROW_COPY";
        }

        //Copies one table into another.
        //table copy FROM_TABLE TO_TABLE
        //    0    1          2        3
        if (tmp_Token[1] == "copy_table")
        {
            Tables->copy_Table(tmp_Token[3], tmp_Token[2]);
            return "TABLE_ROW_COPY";
        }
        if (tmp_Token[1] == "bcopy_table")
        {
            Tables->copy_Table(str2int(tmp_Token[3]), str2int(tmp_Token[2]));
            return "TABLE_ROW_COPY";
        }



        //Copies a set of rows from one table to another with an offset.
        //table copy_rows FROM_TABLE FROM_ROW_START FROM_ROW_END TO_TABLE TO_OFFSET
        //table copy_rows     2            5              10       8        3 //example
        //would copy rows 5-10 from table 2 into table 8 starting at index 3 and ending at index 8.
        //
        //table copy_rows FROM_TABLE FROM_ROW_START FROM_ROW_END TO_TABLE TO_OFFSET
        //  0       1         2          3              4          5        6 //Tokens
        if (tmp_Token[1] == "copy_row_set")
        {
            //Holds the current row being copied.
            void* tmp_Current_Row = NULL;

            //From table.
            std::string tmp_From_Table = tmp_Token[2];

            //Starting index to copy from.
            int tmp_Starting_Copy_Index = str2int(tmp_Token[3]);

            //Ending index to copy to.
            int tmp_Ending_Copy_Index = str2int(tmp_Token[4]);

            //To Table
            std::string tmp_To_Table = tmp_Token[5];

            //Offset.
            int tmp_To_Offset = str2int(tmp_Token[6]);

            for (int cou_Row = 0; cou_Row <= (tmp_Ending_Copy_Index - tmp_Starting_Copy_Index); cou_Row++)
            {
                //Get the current row.
                tmp_Current_Row = Tables->get_Table_Row_Reference(tmp_From_Table, (cou_Row + tmp_Starting_Copy_Index));

                //Copy it to the appropiate table with the appropriate offset.
                Tables->copy_Row(tmp_To_Table, (cou_Row + tmp_To_Offset), tmp_Current_Row);

            }
            return "TABLE_ROW_SET_COPY";
        }
        if (tmp_Token[1] == "bcopy_row_set")
        {
            //Holds the current row being copied.
            void* tmp_Current_Row = NULL;

            //From table.
            int tmp_From_Table = str2int(tmp_Token[2]);

            //Starting index to copy from.
            int tmp_Starting_Copy_Index = str2int(tmp_Token[3]);

            //Ending index to copy to.
            int tmp_Ending_Copy_Index = str2int(tmp_Token[4]);

            //To Table
            int tmp_To_Table = str2int(tmp_Token[5]);

            //Offset.
            int tmp_To_Offset = str2int(tmp_Token[6]);

            for (int cou_Row = 0; cou_Row <= (tmp_Ending_Copy_Index - tmp_Starting_Copy_Index); cou_Row++)
            {
                //Get the current row.
                tmp_Current_Row = Tables->get_Table_Row_Reference(tmp_From_Table, (cou_Row + tmp_Starting_Copy_Index));

                //Copy it to the appropiate table with the appropriate offset.
                Tables->copy_Row(tmp_To_Table, (cou_Row + tmp_To_Offset), tmp_Current_Row);

            }
            return "TABLE_ROW_SET_COPY";
        }

        //table tanslate_row_set From_Table Row_To_Translate To_Table Offset_Index
        //  0          1              2            3              4       5
        if (tmp_Token[1] == "translate_row")
        {

            //From table.
            std::string tmp_From_Table = tmp_Token[2];

            //Starting index to copy from.
            int tmp_Row_To_Translate = str2int(tmp_Token[3]);

            //To Table
            std::string tmp_To_Table = tmp_Token[4];

            //Offset.
            int tmp_To_Offset = str2int(tmp_Token[5]);

            //The current cell to copy from.
            void* tmp_Cell = NULL;


            for (int cou_Index = 0; cou_Index < Tables->get_Row_Cell_Count(tmp_From_Table, tmp_Row_To_Translate); cou_Index++)
            {
                //Holds the current cell in the row.
                tmp_Cell = Tables->get_Table_Cell_Reference(tmp_From_Table, tmp_Row_To_Translate, cou_Index);
                //std::cout << "\n\n\t Current_Cell->";

                Tables->copy(tmp_To_Table, (cou_Index + tmp_To_Offset), 0, tmp_Cell);
            }
            return "TABLE_TRANSLATE_ROW";
        }
        if (tmp_Token[1] == "btranslate_row")
        {

            //From table.
            int tmp_From_Table = str2int(tmp_Token[2]);

            //Starting index to copy from.
            int tmp_Row_To_Translate = str2int(tmp_Token[3]);

            //To Table
            int tmp_To_Table = str2int(tmp_Token[4]);

            //Offset.
            int tmp_To_Offset = str2int(tmp_Token[5]);

            //The current cell to copy from.
            void* tmp_Cell = NULL;


            for (int cou_Index = 0; cou_Index < Tables->get_Row_Cell_Count(tmp_From_Table, tmp_Row_To_Translate); cou_Index++)
            {
                //Holds the current cell in the row.
                tmp_Cell = Tables->get_Table_Cell_Reference(tmp_From_Table, tmp_Row_To_Translate, cou_Index);
                //std::cout << "\n\n\t Current_Cell->";

                Tables->copy(tmp_To_Table, (cou_Index + tmp_To_Offset), 0, tmp_Cell);
            }
            return "TABLE_TRANSLATE_ROW";
        }

        //This shifts a column to the right, overwriting anything that was there.
        if (tmp_Token[1] == "shift_column")
        {

            //From table.
            std::string tmp_Table = tmp_Token[2];

            //Starting index to copy from.
            int tmp_Column_To_Shift = str2int(tmp_Token[3]);

            Tables->shift_Column(tmp_Table, tmp_Column_To_Shift);

            return "TABLE_TRANSLATE_ROW";
        }
        if (tmp_Token[1] == "bshift_column")
        {

            //From table.
            int tmp_Table = str2int(tmp_Token[2]);

            //Starting index to copy from.
            int tmp_Column_To_Shift = str2int(tmp_Token[3]);

            //The current cell to copy from.
            void* tmp_Cell = NULL;


            for (int cou_Index = 0; cou_Index < Tables->get_Row_Count(tmp_Table); cou_Index++)
            {
                //Holds the current cell in the row.
                tmp_Cell = Tables->get_Table_Cell_Reference(tmp_Table, cou_Index, tmp_Column_To_Shift);

                //std::cout << "\n\n\t Current_Cell->";

                Tables->copy(tmp_Table, cou_Index, (tmp_Column_To_Shift + 1), tmp_Cell);

                Tables->set_string(tmp_Table, cou_Index, tmp_Column_To_Shift, " ");
            }
            return "TABLE_TRANSLATE_ROW";
        }

        //This copies a row onto the given index of another row, only copies the first bit of the first row given.
        //table overlay_row From_Table From_Row To_Table To_Row To_Index
        //  0        1           2         3        4        5     6
        if (tmp_Token[1] == "overlay_row")
        {
            //Holds the row to help with reading the code.
            void* tmp_Row = Tables->get_Table_Row_Reference(tmp_Token[2], str2int(tmp_Token[3]));

            Tables->overlay_Row(tmp_Token[4], str2int(tmp_Token[5]), str2int(tmp_Token[6]), tmp_Row);
            return "TABLE_ROW_COPY";
        }
        if (tmp_Token[1] == "boverlay_row")
        {
            //Holds the row to help with reading the code.
            void* tmp_Row = Tables->get_Table_Row_Reference(str2int(tmp_Token[2]), str2int(tmp_Token[3]));

            Tables->overlay_Row(str2int(tmp_Token[4]), str2int(tmp_Token[5]), str2int(tmp_Token[6]), tmp_Row);
            return "TABLE_ROW_COPY";
        }


        //Shifts the rows starting at the given index towards the end of the table making room for a new row.
        //table shift_all_rows [Table_Name] [Index_To_Start_Shift_At]
        if (tmp_Token[1] == "shift_all_rows")
        {
            Tables->shift_All_Rows(tmp_Token[2], str2int(tmp_Token[3]));
            return "TABLE_SHIFT_ALL_ROWS";
        }
        if (tmp_Token[1] == "bshift_all_rows")
        {
            Tables->shift_All_Rows(str2int(tmp_Token[2]), str2int(tmp_Token[3]));
            return "TABLE_SHIFT_ALL_ROWS";
        }

        //Renames a table.
        //table rename -TABLE_ID -NEW_NAME
        //  0      1         2       3
        if (tmp_Token[1] == "rename")
        {
            Tables->set_Table_Name(tmp_Token[2], tmp_Token[3]);
            return "TABLE_RENAME";
        }
        if (tmp_Token[1] == "brename")
        {
            Tables->set_Table_Name(str2int(tmp_Token[2]), tmp_Token[3]);
            return "TABLE_RENAME";
        }

        //creates a new table.
        //table new -TABLE_NAME
        //  0    1        2    
        if (tmp_Token[1] == "new_1D")
        {
            Tables->register_New_Table_1D(tmp_Token[2]);
            return "TABLE_NEW_1D";
        }

        //Deletes a table.
        if (tmp_Token[1] == "delete")
        {
            Tables->delete_Table(tmp_Token[2]);
        }
        if (tmp_Token[1] == "bdelete")
        {
            Tables->delete_Table(str2int(tmp_Token[2]));
        }

        //Gets a table ID.
        if (tmp_Token[1] == "get_ID")
        {
            return int2str(Tables->get_Table_ID(tmp_Token[2]));
        }

        //Gets a table Name.
        if (tmp_Token[1] == "get_name")
        {
            return Tables->get_Table_Name(str2int(tmp_Token[2]));
        }

        //Resets the given table.
        if (tmp_Token[1] == "reset")
        {
            Tables->reset_Table(tmp_Token[2]);
        }
        if (tmp_Token[1] == "breset")
        {
            Tables->reset_Table(str2int(tmp_Token[2]));
        }


        return "0";
    }


    //creates a new CMD table and registers it with the CMD tables.
    std::string CMD_New(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {
        std::string tmp_Token[100];

        CMD_Separate_Tokens(p_EIP, p_Labels, p_Tokens, tmp_Token);

        //table new -TABLE_NAME
        //  0    1        2    

        Tables->register_New_Table_1D(tmp_Token[1]);

        CMD_Script_Registry->push_pop_string(tmp_Token[1]);
        CMD_Script_Registry->push_int((CMD_Script_Registry->get_Row_Count() - 1), str2int(tmp_Token[1]));
        CMD_Script_Registry_Lookup_Tree.set_int(tmp_Token[1], Tables->get_Table_ID(tmp_Token[1]));

        return "TABLE_NEW_CMD";
    }

    std::string CMD_goto(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {

        //if (p_Tokens->get_Row_Count() < 2){ return "ERR_INVALID_TOKEN_COUNT->CMD_goto"; }

        std::string tmp_Label = "";

        //Check for callbacks.
        tmp_Label = test_Command_Token(tmp_Label, p_EIP, p_Labels, p_Tokens);

        //Gets the name of the label.
        p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(1, 0, &tmp_Label);

        //Insert the : character for the tree search.
        tmp_Label = ":" + tmp_Label;


        //Gets the labels index.
        *p_EIP = p_Labels->get_int(tmp_Label);


        return "1";
    }

    std::string CMD_eval(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {

        std::string tmp_Table = "";

        p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(1, 0, &tmp_Table);

        //Callback checking.
        tmp_Table = test_Command_Token(tmp_Table, p_EIP, p_Labels, p_Tokens);

        void* tmp_Table_Ref = Tables->get_Table_Reference(tmp_Table);

        if (tmp_Table_Ref == NULL) { return "0"; }

        eval_Command_Table((c_Table_1D*)tmp_Table_Ref);

        return "1";
    }

    std::string CMD_beval(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {
        if (p_Tokens->get_Row_Count() < 2) { return "0"; }

        std::string tmp_Table = "";

        p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(1, 0, &tmp_Table);

        //Callback checking.
        tmp_Table = test_Command_Token(tmp_Table, p_EIP, p_Labels, p_Tokens);

        eval_Command_Table((c_Table_1D*)Tables->get_Table_Reference(str2int(tmp_Table)));

        return "1";
    }

    //Handles all the outputs.
    std::string CMD_output(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {
        //The temporary tokens to use.
        std::string tmp_Token[100];

        //Gather all the tokens up to the limit of 10.
        for (int cou_Index = 0; cou_Index < p_Tokens->get_Row_Count(); cou_Index++)
        {
            if (cou_Index >= 100) { break; }
            p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);

            //Callback checking.
            tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
            //std::cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
        }

        //Find the appropriate output and do it.
        if (tmp_Token[1] == "table")
        {

            if (tmp_Token[2] == "header")
            {
                if (tmp_Token[3] == "all") { Tables->output_All_Table_Headers(); return "1"; }

                Tables->output_Table_Header(tmp_Token[3]); return "1";
            }
            if (tmp_Token[2] == "bheader")
            {
                Tables->output_Table_Header(str2int(tmp_Token[3])); return "1";
            }

            if (tmp_Token[2] == "data")
            {
                if (tmp_Token[3] == "all") { Tables->output_All_Tables(); return "1"; }

                Tables->output_Table(tmp_Token[3]); return "1";
            }
            if (tmp_Token[2] == "bdata")
            {
                Tables->output_Table(str2int(tmp_Token[3])); return "1";
            }

            if (tmp_Token[2] == "verbose")
            {
                if (tmp_Token[3] == "all") { Tables->output_All_Tables_Verbose(); return "1"; }

                Tables->output_Table_Verbose(tmp_Token[3]); return "1";
            }
            if (tmp_Token[2] == "bverbose")
            {
                Tables->output_Table_Verbose(str2int(tmp_Token[3])); return "1";
            }

        }
        //Find the appropriate output and do it.
        if (tmp_Token[1] == "CMD_tables")
        {

            if (tmp_Token[2] == "header")
            {
                for (int cou_S = 0; cou_S < CMD_Script_Registry->get_Row_Count(); cou_S++)
                {
                    Tables->output_Table_Header(CMD_Script_Registry->get_Table_Name());
                }
            }

            if (tmp_Token[2] == "data")
            {
                for (int cou_S = 0; cou_S < CMD_Script_Registry->get_Row_Count(); cou_S++)
                {
                    Tables->output_Table(CMD_Script_Registry->get_Table_Name());
                }
            }

            if (tmp_Token[2] == "verbose")
            {
                for (int cou_S = 0; cou_S < CMD_Script_Registry->get_Row_Count(); cou_S++)
                {
                    Tables->output_Table_Verbose(CMD_Script_Registry->get_Table_Name());
                }
            }

        }



        if (tmp_Token[1] == "Var_Lookup_Tree") { Var_Lookup_Tree.output_Tree(); }
        if (tmp_Token[1] == "Label_Lookup_Tree") { p_Labels->output_Tree(); }
        if (tmp_Token[1] == "Table_Registry") { Tables->output_Table_Registry(); }
        if (tmp_Token[1] == "Table_Registry_Tree") { Tables->output_Table_Registry(); }

        return "1";
    }



    //==--      LOADING FUNCTION FOR COMMAND SCRIPTS
    //Loads a command script.
    //It reads each line of the script into a CMD_Table labeled the same as the file.
    std::string CMD_load_script(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {
        std::string tmp_Token[100];

        //Gather all the tokens up to the limit of 10.
        for (int cou_Index = 0; cou_Index < p_Tokens->get_Row_Count(); cou_Index++)
        {
            if (cou_Index >= 100) { break; }
            p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
            tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
            //std::cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
        }

        //The temporary filename.
        std::string tmp_DIR = tmp_Token[1] + "/";
        std::string tmp_File = tmp_Token[2] + ".ntxs";

        return CMD_load_script(tmp_DIR, tmp_File);
    }

    std::string CMD_load_script(std::string p_Dir, std::string p_File)
    {
        p_File += ".ntxs";
        std::cout << "\n CMD_load_script(" << p_Dir << ", " << p_File << ")";

        //The stream for reading in the file.
        std::ifstream Script_File;

        //The current line.
        std::string Command_Text = "";

        std::string tmp_Path = p_Dir + p_File;

        Script_File.open(tmp_Path.c_str());

        //This std::string is to extract the filename only for the table.
        std::string tmp_Table_Name = "";
        int tmp_Table_Name_Offset = 0;

        if (Script_File.is_open())
        {
            //Get the filename only, no directories.
            for (int cou_Index = p_File.size(); cou_Index >= 0; cou_Index--)
            {
                if (p_File[cou_Index] == char(92))
                {
                    tmp_Table_Name_Offset = cou_Index + 1;
                    break;
                }
            }

            for (unsigned int cou_Index = (tmp_Table_Name_Offset); (cou_Index < p_File.size() - 5); cou_Index++)
            {
                tmp_Table_Name += p_File[cou_Index];
            }


            //If the file exists, create a table with the same name, sans the .ntxs extension.
            c_Table_1D* tmp_Table = Tables->register_New_Table_1D(tmp_Table_Name);

            CMD_Script_Registry->pushc_string(tmp_Table_Name);
            CMD_Script_Registry->push_pop_int(Tables->get_Table_ID(tmp_Table_Name));
            CMD_Script_Registry_Lookup_Tree.set_int(tmp_Table_Name, Tables->get_Table_ID(tmp_Table_Name));

            while (!Script_File.eof())
            {
                getline(Script_File, Command_Text);
                std::cout << Command_Text << std::endl;
                if (Command_Text.size())
                {
                    tmp_Table->push_Data_Set_S(Command_Text);
                }
            }
        }
        Script_File.close();

        //std::cout << "\n\n\t Done Loading scriptfile(" << p_File << ") into Table->" << tmp_Table_Name;
        return "Done";
    }

    //Reloads a command script.
    //It reads each line of the script into a CMD_Table labeled the same as the file.
    std::string CMD_reload_script(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {
        std::string tmp_Token[100];

        //Gather all the tokens up to the limit of 10.
        for (int cou_Index = 0; cou_Index < p_Tokens->get_Row_Count(); cou_Index++)
        {
            if (cou_Index >= 100) { break; }
            p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
            tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
            //std::cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
        }

        //The temporary filename.
        //std::string tmp_DIR = tmp_Token[1] + "/";
        std::string tmp_File = tmp_Token[1];

        return CMD_reload_script(tmp_File);
    }

    std::string CMD_reload_script(std::string p_File)
    {
        p_File += ".ntxs";
        std::cout << "\n CMD_load_script(" << Sandbox.DIR << p_File << ")";

        //The stream for reading in the file.
        std::ifstream Script_File;

        //The current line.
        std::string Command_Text = "";

        std::string tmp_Path = Sandbox.DIR + p_File;

        Script_File.open(tmp_Path.c_str());

        //This std::string is to extract the filename only for the table.
        std::string tmp_Table_Name = "";
        int tmp_Table_Name_Offset = 0;

        if (Script_File.is_open())
        {
            //Get the filename only, no directories.
            for (int cou_Index = p_File.size(); cou_Index >= 0; cou_Index--)
            {
                if (p_File[cou_Index] == char(92))
                {
                    tmp_Table_Name_Offset = cou_Index + 1;
                    break;
                }
            }

            for (unsigned int cou_Index = (tmp_Table_Name_Offset); (cou_Index < p_File.size() - 5); cou_Index++)
            {
                tmp_Table_Name += p_File[cou_Index];
            }

            c_Table_1D* tmp_Table_Ref = ((c_Table_1D*)(Tables->get_Table_Reference(tmp_Table_Name)));

            if (tmp_Table_Ref == NULL) { return "0"; }

            //May be dangerous.
            tmp_Table_Ref->reset_Table();

            while (!Script_File.eof())
            {
                getline(Script_File, Command_Text);
                std::cout << Command_Text << std::endl;
                if (Command_Text.size())
                {
                    tmp_Table_Ref->push_Data_Set_S(Command_Text);
                }
            }
        }
        Script_File.close();

        //std::cout << "\n\n\t Done Loading scriptfile(" << p_File << ") into Table->" << tmp_Table_Name;
        return "Done";
    }

    //This saves the command table, it is standalone so that a function can be made to save all tables in a single folder.
    std::string CMD_save_script(std::string p_DIR, std::string p_Table)
    {

        c_Table_1D* tmp_Table_Ref = (c_Table_1D*)Tables->get_Table_Reference(p_Table);

        if (tmp_Table_Ref == NULL) { return "0"; }

        std::string tmp_DIR = "md \"" + p_DIR + "\"";

        std::cout << "\n Making Dir: " << tmp_DIR;

        system(tmp_DIR.c_str());

        std::ofstream tmp_SF;


        //eval_Command_Table((c_Table_1D*) tmp_Table_Ref);

        //This keeps track of the current position in the table, it is passed to the parser for modification with goto statements.
        int EIP = 0;

        //The current command string.
        std::string CMD_Line = "";

        std::string tmp_FName = p_DIR + tmp_Table_Ref->get_Table_Name() + ".ntxs";
        //std::string tmp_FName = p_DIR + "/" + tmp_Table_Ref->get_Table_Name() + ".ntxs";
        std::cout << "\n saving file " << tmp_FName;
        tmp_SF.open(tmp_FName, std::ios::trunc);

        if (!tmp_SF.is_open()) { ostr(0, 12, "\n\n ERROR FAILE SAVING FILE "); std::cout << tmp_FName; }

        //Loop through each line in the command table evaluating them as it goes.
        for (EIP = 0; EIP < tmp_Table_Ref->get_Row_Count(); EIP++)
        {
            CMD_Line = "";
            //Get the current command string.
            tmp_Table_Ref->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(EIP, 0, &CMD_Line);
            //eval_Command_String(CMD_Line, &EIP, &Labels);
            if (EIP) { tmp_SF << "\n"; }
            std::cout << "\n CMD_Line: \"" << CMD_Line << "\" " << (CMD_Line != " ");
            if ((EIP != 0) && (CMD_Line != " ")) { tmp_SF << CMD_Line; }

        }

        tmp_SF.close();

        return "COMMAND_PARSER->EVAL_COMMAND_TABLE_SUCCESS";;
    }

    //This function saves a command table.
    //It reads each line of the script into a CMD_Table labeled the same as the file.
    std::string CMD_save_script(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {
        std::string tmp_Token[100];

        //Gather all the tokens up to the limit of 10.
        for (int cou_Index = 0; cou_Index < p_Tokens->get_Row_Count(); cou_Index++)
        {
            if (cou_Index >= 100) { break; }
            p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
            tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
            //std::cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
        }

        std::string tmp_DIRName = tmp_Token[1];


        std::string tmp_Table = tmp_Token[2];

        //Callback checking.
        tmp_Table = test_Command_Token(tmp_Table, p_EIP, p_Labels, p_Tokens);

        CMD_save_script(tmp_DIRName, tmp_Table);

        return "COMMAND_PARSER->SAVE_COMMAND_TABLE_SUCCESS";
    }

    //Saves all of the scripts in the script registry under a single title.
    //The exception is the autoexec file.
    std::string save_Realm(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {

        std::string tmp_Token[100];
        c_Table_1D* tmp_Scripts = ((c_Table_1D*)(Tables->get_Table_Reference(0)));

        //Gather all the tokens up to the limit of 10.
        for (int cou_Index = 0; cou_Index < p_Tokens->get_Row_Count(); cou_Index++)
        {
            if (cou_Index >= 100) { break; }
            p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
            tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
            //std::cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
        }

        if (!Sandbox.flg_Initialized)
        {
            Sandbox.create_Sandbox(tmp_Token[1]);
        }

        std::string tmp_DIR = "NT3.Save/" + tmp_Token[1] + "/";

        std::string tmp_MD = "md \"" + tmp_DIR + "\"";
        std::cout << "\n system:"; ostr(0, 12, tmp_MD);
        system(tmp_MD.c_str());

        Sandbox.Save(tmp_Token[1]);
        std::cout << "\n\n\n CMD_Script_Registry->get_Row_Count() ->" << tmp_Scripts->get_Row_Count();

        //Save the registry table to refer to when loading.
        CMD_save_script(tmp_DIR, tmp_Scripts->get_Table_Name());

        //We start on 4 because tables 0-3 are system tables.
        for (int cou_Index = 4; cou_Index < Tables->get_Table_Count(); cou_Index++)
        {
            std::cout << "\n " << tmp_DIR << " " << Tables->get_Table_Name(cou_Index);
            CMD_save_script(tmp_DIR, Tables->get_Table_Name(cou_Index));
        }

        return "COMMAND_PARSER->SAVE_ALL_COMMAND_TABLES_SUCCESS";
    }


    //This function saves a command table.
    //It reads each line of the script into a CMD_Table labeled the same as the file.
    std::string load_Realm(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {
        std::string tmp_Token[100];
        std::ifstream Script_Registry;
        std::string tmp_Table_Name;

        //Gather all the tokens up to the limit of 10.
        for (int cou_Index = 0; cou_Index < p_Tokens->get_Row_Count(); cou_Index++)
        {
            if (cou_Index >= 100) { break; }
            p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
            tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
            //std::cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
        }

        if (tmp_Token[1] == "list") { std::cout << "\n\n\nDIR"; system("DIR \"NT3.Save\\\""); return "COMMAND_PARSER->SAVE_COMMAND_DIR_SAVES"; }

        std::string tmp_DIRName = "NT3.Save/" + tmp_Token[1] + "/";

        std::string tmp_SR = tmp_DIRName + "Table_Registry.ntxs";

        Script_Registry.open(tmp_SR);

        int tmp_Tracker = -1;
        while (!Script_Registry.eof())
        {
            tmp_Tracker++;
            getline(Script_Registry, tmp_Table_Name);
            std::cout << "\n " << tmp_Table_Name;
            if (tmp_Tracker < 4) { std::cout << ": Skipped"; continue; }
            CMD_load_script(tmp_DIRName, tmp_Table_Name);
        }
        Script_Registry.close();

        std::string tmp_Realm = tmp_Token[1];

        std::cout << "\n\n";
        ostr(0, 12, "\n Loading Sandbox:");
        Sandbox.Load(tmp_Realm);

        //system("PAUSE");
        return "COMMAND_PARSER->SAVE_COMMAND_TABLE_SUCCESS";
    }

    std::string CMD_operation(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {
        std::string tmp_Token[100];
        bool tmp_Token_Done[100];

        p_Tokens->output_Table();

        //This flag is set when searching for assignment operators, when nothing is left to evaluate the result is returned.
        int flg_Done = 0;
        std::string flg_Datatype = "int";
        std::string tmp_Datatype = "int";
        std::string tmp_Data = "";
        int tmp_Number_Of_Tokens = p_Tokens->get_Row_Count();
        int tmp_Token_Offset = 0;

        //Gather all the tokens up to the limit of 10.
        for (int cou_Index = 0; cou_Index < tmp_Number_Of_Tokens; cou_Index++)
        {
            if (cou_Index >= 100) { break; }
            p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
            tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);

        }

        //This is so it does not try to operate on the com token.
        tmp_Token_Done[0] = 1;

        //Gather all the tokens up to the limit of 10.
        for (int cou_Index = 1; cou_Index < tmp_Number_Of_Tokens; cou_Index++)
        {
            if (cou_Index >= 100) { break; }

            //Set the done flags.
            tmp_Token_Done[cou_Index] = 0;

            //If the operation is found then continue.
            if (tmp_Token[cou_Index] == "+" || tmp_Token[cou_Index] == "-" || tmp_Token[cou_Index] == "*" || tmp_Token[cou_Index] == "/") { tmp_Token_Done[cou_Index] = 1; continue; }

            //Test the datatypes and set to higher type if found.
            tmp_Datatype = CMD_get_datatype(tmp_Token[cou_Index]);

            ////std::cout << "\n\t\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index] << " tmp_Data_Type->" << tmp_Datatype;

            if (tmp_Datatype == "string") { flg_Datatype = "string"; }
            if (tmp_Datatype == "float" && flg_Datatype == "int") { flg_Datatype = "float"; }
        }

        ////std::cout << "\n\n\t DataType->" << flg_Datatype;

        if (flg_Datatype == "string")
        {
            for (int cou_Index = 1; cou_Index < tmp_Number_Of_Tokens; cou_Index++)
            {
                if (tmp_Token[cou_Index] == "+" || tmp_Token[cou_Index] == "-" || tmp_Token[cou_Index] == "*" || tmp_Token[cou_Index] == "/") { continue; }

                tmp_Data += tmp_Token[cou_Index];
            }
            return tmp_Data;

        }

        if (flg_Datatype == "float")
        {
            //Get the highest priority items.
            flg_Done = 4;
            while (flg_Done)
            {
                flg_Done = 0;
                for (int cou_Index = 1; cou_Index < tmp_Number_Of_Tokens; cou_Index++)
                {
                    if (tmp_Token[cou_Index] == "*") { flg_Done = 4;  break; }
                    if (tmp_Token[cou_Index] == "/") { flg_Done = 3;  continue; }
                    if (tmp_Token[cou_Index] == "+" && flg_Done < 3) { flg_Done = 2; continue; }
                    if (tmp_Token[cou_Index] == "-" && flg_Done < 2) { flg_Done = 1; }
                }



                if (flg_Done == 4)
                {
                    //Loop through looking for *, when found multiply the tokens on either side.
                    for (int cou_Index = 1; cou_Index < tmp_Number_Of_Tokens; cou_Index++)
                    {
                        if (tmp_Token[cou_Index] == "*")
                        {
                            //Finds a good token on the left to use.
                            tmp_Token_Offset = find_good_token(tmp_Token_Done, cou_Index);

                            tmp_Token[cou_Index - tmp_Token_Offset] = float2str(str2float(tmp_Token[cou_Index - tmp_Token_Offset]) * str2float(tmp_Token[cou_Index + 1]));
                            tmp_Token_Done[cou_Index - tmp_Token_Offset] = 0;

                            tmp_Token_Done[cou_Index] = 1;
                            tmp_Token[cou_Index] = "";
                            tmp_Token_Done[cou_Index + 1] = 1;
                            tmp_Token[cou_Index + 1] = "";
                        }
                    }
                }

                if (flg_Done == 3)
                {
                    //Loop through looking for *, when found multiply the tokens on either side.
                    for (int cou_Index = 1; cou_Index < tmp_Number_Of_Tokens; cou_Index++)
                    {
                        if (tmp_Token[cou_Index] == "/")
                        {
                            //Finds a good token on the left to use.
                            tmp_Token_Offset = find_good_token(tmp_Token_Done, cou_Index);
                            tmp_Token[cou_Index - tmp_Token_Offset] = float2str(str2float(tmp_Token[cou_Index - tmp_Token_Offset]) / str2float(tmp_Token[cou_Index + 1]));
                            tmp_Token_Done[cou_Index - tmp_Token_Offset] = 0;

                            tmp_Token_Done[cou_Index] = 1;
                            tmp_Token[cou_Index] = "";
                            tmp_Token_Done[cou_Index + 1] = 1;
                            tmp_Token[cou_Index + 1] = "";
                        }
                    }

                }

                if (flg_Done == 2)
                {
                    //Loop through looking for *, when found multiply the tokens on either side.
                    for (int cou_Index = 1; cou_Index < tmp_Number_Of_Tokens; cou_Index++)
                    {
                        if (tmp_Token[cou_Index] == "+")
                        {
                            //Finds a good token on the left to use.
                            tmp_Token_Offset = find_good_token(tmp_Token_Done, cou_Index);

                            tmp_Token[cou_Index - tmp_Token_Offset] = float2str(str2float(tmp_Token[cou_Index - tmp_Token_Offset]) + str2float(tmp_Token[cou_Index + 1]));
                            tmp_Token_Done[cou_Index - tmp_Token_Offset] = 0;

                            tmp_Token_Done[cou_Index] = 1;
                            tmp_Token[cou_Index] = "";
                            tmp_Token_Done[cou_Index + 1] = 1;
                            tmp_Token[cou_Index + 1] = "";
                        }
                    }
                }

                if (flg_Done == 1)
                {
                    //Loop through looking for *, when found multiply the tokens on either side.
                    for (int cou_Index = 1; cou_Index < tmp_Number_Of_Tokens; cou_Index++)
                    {
                        if (tmp_Token[cou_Index] == "-")
                        {
                            //Finds a good token on the left to use.
                            tmp_Token_Offset = find_good_token(tmp_Token_Done, cou_Index);
                            tmp_Token[cou_Index - tmp_Token_Offset] = float2str(str2float(tmp_Token[cou_Index - tmp_Token_Offset]) - str2float(tmp_Token[cou_Index + 1]));
                            tmp_Token_Done[cou_Index - tmp_Token_Offset] = 0;

                            tmp_Token_Done[cou_Index] = 1;
                            tmp_Token[cou_Index] = "";
                            tmp_Token_Done[cou_Index + 1] = 1;
                            tmp_Token[cou_Index + 1] = "";
                        }
                    }
                }
            }
        }


        if (flg_Datatype == "int")
        {
            //Get the highest priority items.
            flg_Done = 4;
            while (flg_Done)
            {
                flg_Done = 0;
                for (int cou_Index = 1; cou_Index < tmp_Number_Of_Tokens; cou_Index++)
                {
                    if (tmp_Token[cou_Index] == "*") { flg_Done = 4;  break; }
                    if (tmp_Token[cou_Index] == "/") { flg_Done = 3;  continue; }
                    if (tmp_Token[cou_Index] == "+" && flg_Done < 3) { flg_Done = 2; continue; }
                    if (tmp_Token[cou_Index] == "-" && flg_Done < 2) { flg_Done = 1; }
                }



                if (flg_Done == 4)
                {
                    //Loop through looking for *, when found multiply the tokens on either side.
                    for (int cou_Index = 1; cou_Index < tmp_Number_Of_Tokens; cou_Index++)
                    {
                        if (tmp_Token[cou_Index] == "*")
                        {
                            //Finds a good token on the left to use.
                            tmp_Token_Offset = find_good_token(tmp_Token_Done, cou_Index);

                            tmp_Token[cou_Index - tmp_Token_Offset] = int2str(str2int(tmp_Token[cou_Index - tmp_Token_Offset]) * str2int(tmp_Token[cou_Index + 1]));
                            tmp_Token_Done[cou_Index - tmp_Token_Offset] = 0;

                            tmp_Token_Done[cou_Index] = 1;
                            tmp_Token[cou_Index] = "";
                            tmp_Token_Done[cou_Index + 1] = 1;
                            tmp_Token[cou_Index + 1] = "";
                        }
                    }
                }

                if (flg_Done == 3)
                {
                    //Loop through looking for *, when found multiply the tokens on either side.
                    for (int cou_Index = 1; cou_Index < tmp_Number_Of_Tokens; cou_Index++)
                    {
                        if (tmp_Token[cou_Index] == "/")
                        {
                            //Finds a good token on the left to use.
                            tmp_Token_Offset = find_good_token(tmp_Token_Done, cou_Index);
                            tmp_Token[cou_Index - tmp_Token_Offset] = int2str(str2int(tmp_Token[cou_Index - tmp_Token_Offset]) / str2int(tmp_Token[cou_Index + 1]));
                            tmp_Token_Done[cou_Index - tmp_Token_Offset] = 0;

                            tmp_Token_Done[cou_Index] = 1;
                            tmp_Token[cou_Index] = "";
                            tmp_Token_Done[cou_Index + 1] = 1;
                            tmp_Token[cou_Index + 1] = "";
                        }
                    }

                }

                if (flg_Done == 2)
                {
                    //Loop through looking for *, when found multiply the tokens on either side.
                    for (int cou_Index = 1; cou_Index < tmp_Number_Of_Tokens; cou_Index++)
                    {
                        if (tmp_Token[cou_Index] == "+")
                        {
                            //Finds a good token on the left to use.
                            tmp_Token_Offset = find_good_token(tmp_Token_Done, cou_Index);

                            tmp_Token[cou_Index - tmp_Token_Offset] = int2str(str2int(tmp_Token[cou_Index - tmp_Token_Offset]) + str2int(tmp_Token[cou_Index + 1]));
                            tmp_Token_Done[cou_Index - tmp_Token_Offset] = 0;

                            tmp_Token_Done[cou_Index] = 1;
                            tmp_Token[cou_Index] = "";
                            tmp_Token_Done[cou_Index + 1] = 1;
                            tmp_Token[cou_Index + 1] = "";
                        }
                    }
                }

                if (flg_Done == 1)
                {
                    //Loop through looking for *, when found multiply the tokens on either side.
                    for (int cou_Index = 1; cou_Index < tmp_Number_Of_Tokens; cou_Index++)
                    {
                        if (tmp_Token[cou_Index] == "-")
                        {
                            //Finds a good token on the left to use.
                            tmp_Token_Offset = find_good_token(tmp_Token_Done, cou_Index);
                            tmp_Token[cou_Index - tmp_Token_Offset] = int2str(str2int(tmp_Token[cou_Index - tmp_Token_Offset]) - str2int(tmp_Token[cou_Index + 1]));
                            tmp_Token_Done[cou_Index - tmp_Token_Offset] = 0;

                            tmp_Token_Done[cou_Index] = 1;
                            tmp_Token[cou_Index] = "";
                            tmp_Token_Done[cou_Index + 1] = 1;
                            tmp_Token[cou_Index + 1] = "";
                        }
                    }
                }
            }
        }

        return tmp_Token[1];

    }

    //When setting the tokens they will always be set to the left of the operation. In this way I will only need to search to the lft for good tokens to use.
    int find_good_token(bool p_Tokens[], int p_Token_Length)
    {
        int tmp_Shift = -1;
        for (int cou_Index = p_Token_Length; cou_Index > 0; cou_Index--)
        {
            //If a good token is found then return the offset to the current index.
            tmp_Shift++;
            if (p_Tokens[cou_Index] == 0) { return tmp_Shift; }
        }
        return -1;
    }

    std::string CMD_get_datatype(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
    {
        std::string tmp_Data;
        p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(1, 0, &tmp_Data);
        return CMD_get_datatype(tmp_Data);
    }

    std::string CMD_get_datatype(std::string p_Data)
    {
        // > 57 = char
        // < 48 && !46 = char
        // = 46 = float
        bool flg_Int = true;
        bool flg_Float = false;

        for (unsigned int cou_Index = 0; cou_Index < (p_Data.size()); cou_Index++)
        {
            if (int(p_Data[cou_Index]) > 57) { return "string"; }
            if (int(p_Data[cou_Index]) < 48 && int(p_Data[cou_Index]) != 46 && int(p_Data[cou_Index]) != 45) { return "string"; }
            if (int(p_Data[cou_Index]) == 45 && cou_Index > 0) { return "string"; }
            if (int(p_Data[cou_Index]) == 46) { flg_Float = true; flg_Int = false; }
        }
        if (flg_Float) { return "float"; }
        if (flg_Int) { return "int"; }
        return "string";
    }



    //--      API Members      --//


    //Allows for setting an input to a given string.
    void set_Input_String(int p_Input, std::string p_Data)
    {
        Sandbox.set_Input_String(p_Input, p_Data);
    }



    //Allows for setting an input to an array
    void set_Input_Array_Double(int p_Input, double p_Data[], int p_Length)
    {
        //Sandbox.
    }

    //Gets a given outputs data as a string.
    std::string get_Output_String(int p_Output)
    {
        return Sandbox.get_Output_String(p_Output);
    }


    //--      Externally Defined Members      --//


    std::string NT_Sand(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens);

    std::string NT_Callback(std::string p_Message);
};





























































std::string c_NT3_Command_Parser::NT_Callback(std::string p_Message)
{
    std::cout << "\n CALLBACK RECEIVED: " << p_Message;
    return p_Message;
}






//--      CALLBACKS FOR THE NT ENGINE      --//


//This handles the callbacks for the Sandbox.
std::string c_NT3_Command_Parser::NT_Sand(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
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
    }

    //Creates a new construct of the given name and type.
    //sand    new_Construct   -NAME    -TYPE
    //   0            1          2       3 
    if (tmp_Token[0] == "create_construct")
    {
        if (p_Tokens->get_Row_Count() == 2)
        {
            Sandbox.create_Construct(tmp_Token[1]);
            return "construct_created";
        }
        if (p_Tokens->get_Row_Count() == 3)
        {
            Sandbox.create_Construct(tmp_Token[1], str2int(tmp_Token[2]));
            return "construct_created";
        }
        std::cout << "\n   ERROR: Invalid Number of Arguments: create_construct [string:NAME] [int:TYPE]";
    }

    //Creates a new new_Input of the given type.
    //sand    new_Input   -TYPE  
    //   0        1          2    
    if (tmp_Token[0] == "create_input")
    {
        Sandbox.create_Input(str2int(tmp_Token[1]));
    }

    //Creates a new new_Output of the given type.
    //sand    new_Output   -TYPE  
    //   0        1          2    
    if (tmp_Token[0] == "create_output")
    {
        Sandbox.create_Output(str2int(tmp_Token[1]));
    }



    //===------------------+
    //==--  Manipulations
    //===------------------+


    if (tmp_Token[0] == "move_Construct")
    {
        std::cout << "\n moving construct ";
        if (tmp_Token_Count < 3)
        {
            return "Move:FAIL:NOTENOUGHARGS";
        }

        Sandbox.move_Construct(str2int(tmp_Token[1]), str2int(tmp_Token[2]), str2int(tmp_Token[3]));
        std::cout << "\n " << tmp_Token[1] << " to (" << tmp_Token[2] << ", " << tmp_Token[3] << ")";
        return "Move";
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
            return "Build";
        }
        if (tmp_Token_Count == 2)
        {
            Sandbox.Build(str2int(tmp_Token[1]));
            //std::cout << "\n Build(" << tmp_Token[1] << ")";
            return "BUILD";
        }
    }

    if (tmp_Token[0] == "eval")
    {
        //*---std::cout << "\n Eval()";
        if (tmp_Token_Count == 1)
        {
            Sandbox.Eval(0);
            return "Eval";
        }
        if (tmp_Token_Count == 2)
        {
            Sandbox.Eval(str2int(tmp_Token[1]));
            //std::cout << "\n Eval(" << tmp_Token[1] << ")";
            return "EVAL";
        }
        if (tmp_Token_Count == 3)
        {
            Sandbox.Eval(str2int(tmp_Token[1]), str2int(tmp_Token[2]));
            //std::cout << "\n Eval(" << tmp_Token[1] << ", " << tmp_Token[2] << ")";
            return "EVAL";
        }
    }

    if (tmp_Token[0] == "discharge")
    {
        //*---std::cout << "\n Discharge()";
        if (tmp_Token_Count == 1)
        {
            Sandbox.Discharge(0);
            return "Discharge";
        }
        if (tmp_Token_Count == 2)
        {
            Sandbox.Discharge(str2int(tmp_Token[1]));
            //std::cout << "\n Discharge(" << tmp_Token[1] << ")";
            return "EVAL";
        }
    }

    //Resets a constructs input and output tables for new evaluation.
    //Using this negates and pipe transfers done previously.
    if (tmp_Token[0] == "rio")
    {
        Sandbox.reset_IO(str2int(tmp_Token[1]));
    }

    //Tells a pipe to transfer data from its input to its output. Does reset and cell copy. One row only.
    //sand    pipe_cut    pipe_ID
    //   0     1             2        
    if (tmp_Token[0] == "po")
    {
        if (tmp_Token_Count == 2) { Sandbox.pipe_Overwrite(str2int(tmp_Token[1]), 0); }
        if (tmp_Token_Count == 3) { Sandbox.pipe_Overwrite(str2int(tmp_Token[1]), str2int(tmp_Token[2])); }
    }

    //Tells a pipe to transfer data from its input to its output. Appends input row to destination row.
    //sand    pipe_cut    pipe_ID
    //   0     1             2        
    if (tmp_Token[0] == "pa")
    {
        Sandbox.pipe_Append(str2int(tmp_Token[1]));
    }

    //Tells a pipe to drain.
    //sand    pipe_cut    pipe_ID
    //   0     1             2        
    if (tmp_Token[0] == "pd")
    {
        Sandbox.pipe_Overwrite(str2int(tmp_Token[1]), str2int(tmp_Token[2]));
    }

    //Tells a pipe to hotlink the connections.
    if (tmp_Token[0] == "pipe_hotlink")
    {
        Sandbox.pipe_Hotlink(str2int(tmp_Token[1]));
    }

    //Tells a pipe to transfer data from its output to its input. Does reset and cell copy. One row only.
    //Relies on the salt table to determine which nodes to gather.
    //sand    pipe_cut    pipe_ID
    //   0     1             2        
    if (tmp_Token[0] == "pd")
    {
        Sandbox.pipe_Overwrite(str2int(tmp_Token[1]), str2int(tmp_Token[2]));
    }


    //Sets the input of a given input to the given string.
    //sand    set_input    input_ID    input_String
    //   0     1             2              3
    if (tmp_Token[0] == "set_input")
    {
        Sandbox.set_Input_String(str2int(tmp_Token[1]), tmp_Token[2]);
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
    }

    //Displays the registered objects.
    //sand    output_registry
    //   0        1     
    if (tmp_Token[0] == "output_registry")
    {
        Sandbox.output_Registry();
    }

    //Displays the data pipes and their information.
    //sand    output_pipes
    //   0        1     
    if (tmp_Token[0] == "output_pipes")
    {
        Sandbox.output_Pipe_Data();
    }

    //Outputs the inputs.
    if (tmp_Token[0] == "output_inputs")
    {
        Sandbox.output_Input_Data();
    }

    //Outputs the outputs.
    if (tmp_Token[0] == "output_outputs")
    {
        Sandbox.output_Output_Data();
    }

    //Outputs the IO tables for a given construct.
    if (tmp_Token[0] == "output_IO")
    {
        if (tmp_Token_Count <= 1) { return "output_IO::FAILED_ARGUMENTS_INSUFFICIENT"; }
        Sandbox.output_Construct_IO(str2int(tmp_Token[1]));
    }

    //Outputs the IO tables for every construct
    if (tmp_Token[0] == "output_IO_all")
    {
        Sandbox.output_Construct_IO_All();
    }

    //Outputs the CAN for a given construct
    if (tmp_Token[0] == "output_CAN")
    {
        if (tmp_Token_Count <= 1) { return "output_CAN::FAILED_ARGUMENTS_INSUFFICIENT"; }
        Sandbox.output_CAN(str2int(tmp_Token[1]));
    }

    //Outputs the CAN for every Construct.
    if (tmp_Token[0] == "output_CAN_all")
    {
        Sandbox.output_CAN_All();
    }

    //Outputs the network to an HTML file.
    if (tmp_Token[0] == "output_HTML")
    {
        //Outputs the node network for a given construct to an HTML file that uses javascript to display the nodes.
        // [1]: Construct, [2]: Directory, [3]: Filename, [4]: X padding, [5]: Y padding
        if (tmp_Token_Count <= 5) { std::cout << "output_HTML::FAILED_ARGUMENTS_INSUFFICIENT"; return "output_HTML::FAILED_ARGUMENTS_INSUFFICIENT"; }

        Sandbox.output_Nodes_As_HTML(str2int(tmp_Token[1]), tmp_Token[2], tmp_Token[3], str2int(tmp_Token[4]), str2int(tmp_Token[5]));
    }

    //Submits a std::string to the given input.
    if (tmp_Token[0] == "in")
    {
        Sandbox.set_Input_String(str2int(tmp_Token[1]), tmp_Token[2]);
    }

    //Submits a std::string to the given input.
    if (tmp_Token[0] == "add")
    {
        Sandbox.add_Input_String(str2int(tmp_Token[1]), tmp_Token[2]);
    }

    //Hotlinks an input to a construct.
    if (tmp_Token[0] == "hotlink_in")
    {
        //Hotlink the given input to the given construct. The row is linked to the constructs input table at the given index.
        // [1]: Input_ID, [2]: Construct_ID, [3]: Construct_Input_Index.
        Sandbox.hotlink_Input(str2int(tmp_Token[1]), str2int(tmp_Token[2]), str2int(tmp_Token[3]));
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

    }


    return "0";
}


















