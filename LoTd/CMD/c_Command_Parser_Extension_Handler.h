#define c_COMMAND_PARSER_EXTENSION_HANDLER_H
#define c_COMMAND_PARSER_EXTENSION_HANDLER_H
//This file is meant to handle the extended command hooked into the command parser. 
//The command parser passes control here if it falls through, from here control is passed 
//to custom extensions.


/*
	--==  How to add an extension  ==--

	-Include your file.
	--"c_CMDE_example.h"
	---for inclusion in the main library preface with c_CMDE_

	-Create a function to handle the tokens.
	--cmde_Main_Example(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
	---For inclusion in main library:
	----Preface callback with cmde_Main_
	----Return the name of the callback with "CALLBACK" as a prefix, "CALLBACK_Example"
	----To provide additional information append after a double underscore, "CALLBACK_Example__test_Function"
	----Provide a fall through return of "NO_COMMAND" at the end of your main.

	-Add your function by including it in the check block of eval_Tokens_Extended(p_EIP, p_Labels, p_Tokens).
*/


std::string c_NT3_Command_Parser::eval_Tokens_Extended(int* p_EIP, c_Lookup_Tree* p_Labels, c_Table_1D* p_Tokens)
{
	//The temporary std::string for the token being evaluated.
	std::string tmp_Token[100];

	//Get the current token in std::string format.
	p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(0, 0, &tmp_Token[0]);

	//Hold the temporary return string from the extensions. 
	std::string tmp_Return_String = "NO_COMMAND";

	//The if block that handles extension callbacks.
	tmp_Return_String = cmde_Main_Sandbox(p_EIP, p_Labels, p_Tokens);
	if (tmp_Return_String != "NO_COMMAND") { return tmp_Return_String; }
	
	//if (tmp_Token[0] == "sand")             { return NT_sand           (p_EIP, p_Labels, p_Tokens); }
	//Now falls through to the NT_Sand member.
	return "NO_COMMAND";
}

#endif // !c_COMMAND_PARSER_EXTENSION_HANDLER_H