
void odbl(int p_bg, int p_fg, double p_Double);
void oint(int p_bg, int p_fg, int p_Int);
void onr(int p_bg, int p_fg, int p_Int);







//Rounding functions.
int rDbl(double p_Data)
{
     double tmp_Remainder = p_Data - (int (p_Data));
     int tmp_Return = int(p_Data - tmp_Remainder);
     
     if (tmp_Remainder >= .5){ tmp_Return++; }
     if (tmp_Remainder <= -.5){ tmp_Return--; }
     
     return tmp_Return;
}





//Gets the abs value of a given int.
int abs(int p_Int)
{
	if (p_Int < 0) { p_Int *= -1; }
	return p_Int;
}

//Gets the abs value of a given double.
double absd(double p_Double)
{
     if (p_Double < 0){ p_Double *= -1; }
     return p_Double;
}

//Gets the abs value of a given double.
float abs(float p_Float)
{
     if (p_Float < 0){ p_Float *= -1; }
     return p_Float;
}

//Gets the power of an integer.
int pow_Int(int p_Number, int p_Factor, int p_Power)
{
     if (p_Power == 0){ return 1; }
     if (p_Power == 1){ return p_Number; }
     return pow_Int((p_Number * p_Factor), p_Factor, (p_Power -1));
}

int get_Percent_Change(double p_Start, double p_Current)
{
     if (p_Start == 0){ return 0; }
     return int (((p_Current - p_Start) / absd(p_Start)) * 100);
}

int get_Percent_Change(double p_Start, double p_Current, int p_Precision)
{
     if (p_Start == 0){ return 0; }
     return int (((p_Current - p_Start) / absd(p_Start)) * p_Precision);
}

int get_Rounded_Percent_Change(double p_Start, double p_Current, int p_Precision)
{
     if (p_Start == 0){ return 0; }
     
     double tmp_PC = double (((p_Current - p_Start) / absd(p_Start)) * double (p_Precision));
     
     return rDbl(tmp_PC);
}












double get_Percent_Change_F(double p_Start, double p_Current, int p_Precision = 100)
{
     if (p_Start == 0){ return 0.0f; }
     return double (((p_Current - p_Start) / absd(p_Start)) * double (p_Precision));
}


double get_Average(double p_Array[], int p_Depth)
{
     if (p_Depth == 0){ return 0.0f; }
     double tmp_Total = 0.0f;
     for (int cou_Index=0;cou_Index<p_Depth;cou_Index++)
     {
          tmp_Total += p_Array[cou_Index];
     }
     
     return tmp_Total / p_Depth;
}

double get_Rounded_Average(double p_Array[], int p_Depth)
{
     return rDbl(get_Average(p_Array, p_Depth));
}


double get_Average(long long int p_Array[], int p_Depth)
{
     if (p_Depth == 0){ return 0.0f; }
     double tmp_Total = 0.0f;
     for (int cou_Index=0;cou_Index<p_Depth;cou_Index++)
     {
          tmp_Total += p_Array[cou_Index];
     }
     
     return tmp_Total / p_Depth;
}

double get_Rounded_Average(long long int p_Array[], int p_Depth)
{
     return rDbl(get_Average(p_Array, p_Depth));
}


//Gets the fuzzy spread for a given dataset in the form of an array.
double get_FSpread(double p_Data[], int p_Length, int p_Precision = 100)
{
     //Setup the tmp vars.
     double tmp_DAvg = 0.0;
     
     double tmp_STotal = 0.0;
     double tmp_SAvg = 0.0;
     
     //Find the average.
     tmp_DAvg = get_Average(p_Data, p_Length);
     
     //cout << "\n\n\n\n DAvg->"; odbl(0, 14, tmp_DAvg);
     
     //Get the difference between each index and the average, adding it to the total difference.
     for (int cou_Index=0;cou_Index<p_Length;cou_Index++)
     {
          tmp_STotal += absd(get_Percent_Change_F(tmp_DAvg, p_Data[cou_Index], p_Precision));
          /*
          cout << "\n  [" << cou_Index << "] ->";
          cout << "  ";
          for (int cou_D=0;cou_D<p_Data[cou_Index];cou_D++)
          {
               cout << "+";
          }
          odbl(0, 7, p_Data[cou_Index]); cout << " "; odbl(0, 14, tmp_STotal);
          */
     }
     
     //Get the average of the total difference.
     tmp_SAvg = tmp_STotal / p_Length;
     
     //cout << "\n\n Result->"; odbl(0, 7, tmp_SAvg);
     
     //Return this value as it represents the spread.
     return tmp_SAvg;
}

double get_FSpreadR(double p_Data[], int p_Length, int p_Precision = 100)
{
     //*cout << "\n\n\n\n p_Length->" << p_Length << " Data->";
     /*
     for (int cou_Index=0;cou_Index<p_Length;cou_Index++)
     {
          cout << " [" << cou_Index << "]"; odbl(0, 8, p_Data[cou_Index]);
     }
      * */
     //Setup the tmp vars.
     double tmp_DAvg = 0.0;
     
     double tmp_STotal = 0.0;
     double tmp_SAvg = 0.0;
     
     //Find the average.
     tmp_DAvg = get_Average(p_Data, p_Length);
     
     //*cout << "\n DAvg->"; odbl(0, 14, tmp_DAvg);
     
     //Get the difference between each index and the average, adding it to the total difference.
     for (int cou_Index=0;cou_Index<p_Length;cou_Index++)
     {
          tmp_STotal += absd(get_Rounded_Percent_Change(tmp_DAvg, double(p_Data[cou_Index]), p_Precision));
          /*
          cout << "\n  [" << cou_Index << "] ->";
          cout << "  ";
          for (int cou_D=0;cou_D<p_Data[cou_Index];cou_D++)
          {
               cout << "+";
          }
          odbl(0, 7, p_Data[cou_Index]); cout << " "; odbl(0, 14, tmp_STotal);
          */
     }
     
     //Get the average of the total difference.
     tmp_SAvg = tmp_STotal / p_Length;
     
     //*cout << "\n\n Result->"; odbl(0, 7, tmp_SAvg);
     
     
     double tmp_PC = tmp_SAvg;
     double tmp_Remainder = tmp_PC - (int (tmp_PC));
     int tmp_Return = int(tmp_PC - tmp_Remainder);
     
     if (tmp_Remainder >= .5){ tmp_Return++; }
     if (tmp_Remainder <= -.5){ tmp_Return--; }
     
     /*
     cout << "\n   ";
     cout << " tmp_PC->"; odbl(0, 8, tmp_PC);
     cout << " tmp_Remainder->"; odbl(0, 7, tmp_Remainder);
     cout << " tmp_Return->"; oint(0, 14, tmp_Return);
     */
     
     //Return this value as it represents the spread.
     return tmp_Return;
}



double get_FSpreadR(long long int p_Data[], int p_Length, int p_Precision = 100)
{
     //Setup the tmp vars.
     double tmp_DAvg = 0.0;
     
     double tmp_STotal = 0.0;
     double tmp_SAvg = 0.0;
     
     //Find the average.
     tmp_DAvg = get_Average(p_Data, p_Length);
     
     //cout << "\n\n\n\n DAvg->"; odbl(0, 14, tmp_DAvg);
     
     //Get the difference between each index and the average, adding it to the total difference.
     for (int cou_Index=0;cou_Index<p_Length;cou_Index++)
     {
          tmp_STotal += double(abs(get_Rounded_Percent_Change(tmp_DAvg, double(p_Data[cou_Index]), p_Precision)));
          /*
          cout << "\n  [" << cou_Index << "] ->";
          cout << "  ";
          for (int cou_D=0;cou_D<p_Data[cou_Index];cou_D++)
          {
               cout << "+";
          }
          odbl(0, 7, p_Data[cou_Index]); cout << " "; odbl(0, 14, tmp_STotal);
          */
     }
     
     //Get the average of the total difference.
     tmp_SAvg = tmp_STotal / p_Length;
     
     //cout << "\n\n Result->"; odbl(0, 7, tmp_SAvg);
     
     
     double tmp_PC = tmp_SAvg;
     double tmp_Remainder = tmp_PC - (int (tmp_PC));
     int tmp_Return = tmp_PC - tmp_Remainder;
     
     if (tmp_Remainder >= .5){ tmp_Return++; }
     if (tmp_Remainder <= -.5){ tmp_Return--; }
     
     /*
     cout << "\n   ";
     cout << " tmp_PC->"; odbl(0, 8, tmp_PC);
     cout << " tmp_Remainder->"; odbl(0, 7, tmp_Remainder);
     cout << " tmp_Return->"; oint(0, 14, tmp_Return);
     */
     
     //Return this value as it represents the spread.
     return tmp_Return;
}



int get_Difference(int p_Start, int p_End)
{
     return abs((p_Start - p_End));
}

__int64 get_Difference(long long int p_Start, long long int p_End)
{
	if (p_Start > p_End) { return (p_Start - p_End); }
	if (p_Start < p_End) { return (p_End - p_Start); }
     return 0;
}

double get_Difference(double p_Start, double p_End)
{
     return absd((p_Start - p_End));
}

float get_Difference(float p_Start, float p_End)
{
     return float(abs((p_Start - p_End)));
}
