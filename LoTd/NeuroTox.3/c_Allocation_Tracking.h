int ga_Cell_Alloc;
int ga_Row_Alloc;
int ga_Table_Alloc;

int ga_Buffer_Alloc;


void init_Alloc()
{
     ga_Cell_Alloc = 0;
     ga_Row_Alloc = 0;
     ga_Table_Alloc = 0;
}

void output_Alloc()
{
     cout << "\n Cell->" << ga_Cell_Alloc;
     cout << "\n Row->" << ga_Row_Alloc;
     cout << "\n Table->" << ga_Table_Alloc;
}
