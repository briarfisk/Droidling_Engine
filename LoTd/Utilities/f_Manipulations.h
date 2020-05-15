

//Gets the number of lines in a file.
int get_Number_Of_Lines(ifstream * p_File)
{
     if (!p_File->is_open()){ return 0; }
     string tmp_Data = "";
     
     int tmp_Line_Count = 0;
     p_File->seekg(0);
     while(!p_File->eof())
     {
          getline(*p_File, tmp_Data);
          tmp_Line_Count++;
     }
     p_File->seekg(0);
     return tmp_Line_Count;
}

//Reverses the lines in a file into an output file.
void RFile(string p_File, int p_Show = 0)
{
     int Number_Of_Lines = 0;
     string tmp_Input = "";
     string * Whole_File = NULL;
     int Current_Line = 0;
     
     ifstream DF;
     ofstream OF;
     
     //while(1)
     {
          //system("DIR /a:-d /o:e");
          //cout << "\n\n\t Enter File->";
          //cin >> tmp_Input;
          tmp_Input = p_File;
          
          DF.open(tmp_Input);
          
          if (!DF.is_open()){ cout << "\n\n FILE " << tmp_Input << " NOT OPENED!"; return; }
          
          //Get number of lines.
          Number_Of_Lines = get_Number_Of_Lines(&DF);
          if (p_Show){ cout << "\n\n " << Number_Of_Lines << " Lines Found..."; }
          
          Whole_File = new string[Number_Of_Lines];
          for (int cou_Index=0;cou_Index<Number_Of_Lines;cou_Index++)
          {
               Whole_File[cou_Index] = "";
          }
          
          //Gather the lines
          Current_Line = 0;
          DF.seekg(0);
          if (p_Show){ cout << "\n Gathering File..."; }
          while(!DF.eof())
          {
               getline(DF, Whole_File[Current_Line]);
               if (p_Show){ cout << "\n   [" << Current_Line << "] -- " << Whole_File[Current_Line]; }
               
               Current_Line++;
          }
          
          DF.close();
          
          //Outputs the lines to a file in reverse order.
          tmp_Input = "r." + tmp_Input;
          OF.open(tmp_Input, ios::trunc);
          
          if (!OF.is_open()){ cout << "\n\n FILE " << tmp_Input << " NOT OPENED!"; return; }
          
          if (p_Show){ cout << "\n Reversing File..."; }
          for (int cou_Index=(Number_Of_Lines - 1);cou_Index>=0;cou_Index--)
          {
               if (p_Show){ cout << "\n [" << cou_Index << "] -- " << Whole_File[cou_Index]; }
               OF << Whole_File[cou_Index];
               if (cou_Index != 0){ OF << "\n"; }
          }
          OF.close();
     }
}

//Strips the newlines from a file.
void StripFile(string p_File, int p_Show = 0)
{
     string tmp_Input = p_File;
     string tmp_Line = "";
     int tmp_Current_Line = 0;
     
     ifstream DF;
     ofstream OF;
     
     //while(1)
     {
          //system("DIR /a:-d /o:e");
          //cout << "\n\n\t Enter File->";
          //cin >> tmp_Input;
          
          DF.open(tmp_Input);
          
          if (!DF.is_open()){ cout << "\n\n FILE " << tmp_Input << " NOT OPENED!"; return; }
          tmp_Input = "s." + tmp_Input;
          
          //Outputs the lines to a file in reverse order.
          OF.open(tmp_Input, ios::trunc);
          
          if (!OF.is_open()){ cout << "\n\n FILE " << tmp_Input << " NOT OPENED!"; return; }
          
          //Gather the lines
          tmp_Current_Line = 0;
          DF.seekg(0);
          //if (p_Show){ cout << "\n Stripping File..."; }
          while(!DF.eof())
          {
               getline(DF, tmp_Line);
               if (p_Show){ cout << "\n   [" << tmp_Current_Line << "] -- " << tmp_Line; }
               
               if (tmp_Line == ""){ continue; }
               
               if (tmp_Current_Line != 0){ OF << "\n"; }
               OF << tmp_Line;
               tmp_Current_Line++;
          }
          
          DF.close();
          
          OF.close();
     }
}


//Strips a file for input into MRKT
void MRKTStrip(string p_File, int p_Show = 0)
{
     string tmp_Input = p_File;
     string tmp_Line = "";
     int tmp_Current_Line = 0;
     
     ifstream DF;
     ofstream OF;
     
     DF.open(tmp_Input);
     
     if (!DF.is_open()){ cout << "\n\n FILE " << tmp_Input << " NOT OPENED!"; return; }
     tmp_Input = "s." + tmp_Input;
     
     //Outputs the lines to a file in reverse order.
     OF.open(tmp_Input, ios::trunc);
     
     if (!OF.is_open()){ cout << "\n\n FILE " << tmp_Input << " NOT OPENED!"; return; }
     
     //Gather the lines
     tmp_Current_Line = 0;
     DF.seekg(0);
     if (p_Show){ cout << "\n Stripping File..."; }
     while(!DF.eof())
     {
          getline(DF, tmp_Line);
          if (p_Show){ cout << "\n   [" << tmp_Current_Line << "] -- " << tmp_Line; }
          
          if (tmp_Line == ""){ if (p_Show){ cout << "--<<BLANK LINE>>--"; } continue; }
          if (tmp_Line == "Date,Open,High,Low,Close,Volume,Adj Close"){ if (p_Show){ cout << "--<<DATELINE>>--"; } continue; }
          
          if (tmp_Current_Line != 0){ OF << "\n"; }
          
          //Strip the forbidden characters.
          for (unsigned int cou_Index=0;cou_Index<tmp_Line.size();cou_Index++)
          {
               if (tmp_Line[cou_Index] == '-' || tmp_Line[cou_Index] == ','){ tmp_Line[cou_Index] = ' '; }
          }
          
          if (p_Show){ cout << "\n\t " << tmp_Line; }
          
          OF << tmp_Line;
          tmp_Current_Line++;
     }
          
     DF.close();
          
     OF.close();
}

void main_Loop(int argc, char **argv)
{
     if (argc < 2){ return; }
     
     if (argc == 2){ RFile(argv[1]); }
     if (argc > 2){ RFile(argv[1], int (argv[2] [0])); }
}