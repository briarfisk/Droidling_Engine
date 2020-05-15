//Handles the tables. Allows for one manager for registering, tracking, and manipulating tables.
//Currently only tables with 1 dimensional data are implemented. 

//Handles all dimensions of tables.
class c_Table_Handler
{
protected:

	//The tables to be handled.
	//The reason for the c_Base_Table is that in the future tables that accept more than 1 dimensional data 
	// will be implemented and a polymorphic base class is useful here.
	c_Base_Table** Tables;

	//The current table count.
	int Table_Count;

	//A pointer to the first table in the table array used to track all registered tables.
	c_Table_1D* Registry_Table;

	//The table registry.
	c_Lookup_Tree Table_Registry;

public:

	c_Table_Handler()
	{
		//The first table in the array contains information on all the tables the handler currently has registered.
		//Due to the fact that the default table ID is 0 in the case of nonallocated tables someone may alter the
		//registry, however, it does not actually affect anything other than display data.

		//Allocate the memory for the registry table and create it.
		Tables = new c_Base_Table * [1];
		Tables[0] = new c_Table_1D;

		//Use the table members to assign the ID and name. 
		//Then in the lookup tree set the variable labeled "Table_Registry" to 0.
		Tables[0]->set_Table_ID(0);
		Tables[0]->set_Table_Name("Table_Registry");
		Table_Registry.set_int("Table_Registry", 0);

		//Assign the registry variable to the table just created. Then set the first cell in the first row to represent the registry.
		Registry_Table = (c_Table_1D*)Tables[0];
		Registry_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(0, 0, "Table_Registry");

		//There is now the one table, the registry table.
		Table_Count = 1;
	}

	~c_Table_Handler()
	{
		if (Tables != NULL)
		{
			for (int cou_Index = 0; cou_Index < Table_Count; cou_Index++)
			{
				if (Tables[cou_Index] != NULL)
				{
					delete Tables[cou_Index];
					Tables[cou_Index] = NULL;
				}
			}
		}
		if (Tables != NULL)
		{
			delete[] Tables;
			Tables = NULL;
		}
	}

	//Creates a new table and returns the table address.
	c_Table_1D* register_New_Table_1D(string p_Table_Name)
	{
		//Search for a NULL index, if found register the table there.
		//If a NULL index is found and the table created then the handle is returned.
		for (int cou_Index = 0; cou_Index < Table_Count; cou_Index++)
		{
			if (Tables[cou_Index] == NULL)
			{
				Tables[cou_Index] = new c_Table_1D;
				Tables[cou_Index]->set_Table_ID(cou_Index);
				Tables[cou_Index]->set_Table_Name(p_Table_Name);
				Registry_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S((cou_Index), 0, p_Table_Name);

				Table_Registry.set_int(p_Table_Name, cou_Index);
				return (c_Table_1D*)Tables[cou_Index];
			}
		}
		
		//Make room for the new table.
		increment_Table_Array();

		Tables[Table_Count - 1] = new c_Table_1D;
		Tables[Table_Count - 1]->set_Table_ID(Table_Count - 1);
		Tables[Table_Count - 1]->set_Table_Name(p_Table_Name);

		//Add the table to the lookup tree.
		Table_Registry.set_int(p_Table_Name, (Table_Count - 1));

		//Add the current table to the registry.
		Registry_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S((Table_Count - 1), 0, p_Table_Name);


		return (c_Table_1D*)Tables[Table_Count - 1];

	}


	//Gets the number of tables.
	int get_Table_Count()
	{
		return Table_Count;
	}

	//Removes a table from the registry.
	void delete_Table(int p_TID)
	{
		if (!bounds_TID(p_TID)) { return; }

		//cout << "\n\n\t Deleting Table[" << p_TID << "]";

		//Remove the table from the registry.
		Table_Registry.remove_Node(Tables[p_TID]->get_Table_Name());

		//Delete the table and set to NULL.
		delete Tables[p_TID];
		Tables[p_TID] = NULL;
		Registry_Table->set_string(p_TID, 0, " ");
	}

	//Removes a table using the table name.
	void delete_Table(string p_Table_Name)
	{
		delete_Table(get_Table_ID(p_Table_Name));
	}

	//Gets a reference to the current table.
	void* get_Table_Reference(int p_TID = 0)
	{
		if (p_TID < 0) { return NULL; }

		if (!bounds_TID(p_TID)) { return Tables[p_TID]; }

		return Tables[p_TID];
	}
	void* get_Table_Reference(string p_TName)
	{
		return get_Table_Reference(get_Table_ID(p_TName));
	}

	//Gets the ID of a given table from the given table name.
	int get_Table_ID(string p_Table_Name)
	{
		return Table_Registry.get_int(p_Table_Name);
	}

	//Gets the name of a given table.
	string get_Table_Name(int p_TID)
	{
		if (bounds_TID(p_TID)) { return Tables[p_TID]->get_Table_Name(); }
		return "Table_ID_Not_Found";
	}

	//Updates the name of a table in the registry.
	void set_Table_Name(int p_TID, string p_Table_Name)
	{
		if (!bounds_TID(p_TID)) { return; }

		//Update the registry lookup tree.
		Table_Registry.change_name(Tables[p_TID]->get_Table_Name(), p_Table_Name);

		Tables[p_TID]->set_Table_Name(p_Table_Name);
		Registry_Table->set_string(p_TID, 0, p_Table_Name);
	}
	void set_Table_Name(string p_TName, string p_Table_Name)
	{
		set_Table_Name(get_Table_ID(p_TName), p_Table_Name);
	}

	//Gets a reference to a given row in a given table.
	void* get_Table_Row_Reference(int p_TID, int p_Row)
	{
		if (!bounds_TID(p_TID)) { return NULL; }

		return Tables[p_TID]->get_Row_Reference(p_Row);
	}
	void* get_Table_Row_Reference(string p_TName, int p_Row)
	{
		return get_Table_Row_Reference(get_Table_ID(p_TName), p_Row);
	}


	//Gets a reference to a given cell in a given row in a given table.
	void* get_Table_Cell_Reference(int p_TID, int p_Row, int p_Cell)
	{
		if (!bounds_TID(p_TID)) { return NULL; }

		return Tables[p_TID]->get_Cell_Reference(p_Row, p_Cell);
	}
	void* get_Table_Cell_Reference(string p_TName, int p_Row, int p_Cell)
	{
		return get_Table_Cell_Reference(get_Table_ID(p_TName), p_Row, p_Cell);
	}

	//Gets the number of rows in the given table.
	int get_Row_Count(int p_TID)
	{
		if (!bounds_TID(p_TID)) { return 0; }

		return Tables[p_TID]->get_Row_Count();
	}
	int get_Row_Count(string p_Table)
	{
		return get_Row_Count(get_Table_ID(p_Table));
	}

	//Gets the cell count of the given row.
	int get_Row_Cell_Count(int p_TID, int p_Row)
	{
		if (!bounds_TID(p_TID)) { return 0; }

		return Tables[p_TID]->get_Row_Cell_Count(p_Row);
	}
	int get_Row_Cell_Count(string p_Table, int p_Row)
	{
		return get_Row_Cell_Count(get_Table_ID(p_Table), p_Row);
	}

	//Gets whether or not the table is empty.
	int table_Is_Empty(int p_TID)
	{
		if (!bounds_TID(p_TID)) { return 0; }

		return Tables[p_TID]->table_Is_Empty();
	}
	int table_Is_Empty(string p_Table)
	{
		return table_Is_Empty(get_Table_ID(p_Table));
	}

	//Resets a table.
	void reset_Table(int p_TID)
	{
		if (!bounds_TID(p_TID)) { return; }

		Tables[p_TID]->reset_Table();
	}
	void reset_Table(string p_Table)
	{
		reset_Table(get_Table_ID(p_Table));
	}

	//--     TABLE MANIPULATIONS     --//

	//==--     Table ID Submission

	//Sets a given rows cell to the given data.
	void set_string(int p_TID, int p_Row, int p_Cell, string p_String) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->set_string(p_Row, p_Cell, p_String); }
	void set_int(int p_TID, int p_Row, int p_Cell, int p_Int) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->set_int(p_Row, p_Cell, p_Int); }
	void set_float(int p_TID, int p_Row, int p_Cell, float p_Float) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->set_float(p_Row, p_Cell, p_Float); }
	void set_reference(int p_TID, int p_Row, int p_Cell, void* p_Void) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->set_reference(p_Row, p_Cell, p_Void); }
	void set_bool(int p_TID, int p_Row, int p_Cell, bool p_Bool) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->set_bool(p_Row, p_Cell, p_Bool); }

	//Pushes a given rows cell to the given data.
	void push_string(int p_TID, int p_Row, string p_String) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_string(p_Row, p_String); }
	void push_int(int p_TID, int p_Row, int p_Int) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_int(p_Row, p_Int); }
	void push_float(int p_TID, int p_Row, float p_Float) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_float(p_Row, p_Float); }
	void push_reference(int p_TID, int p_Row, void* p_Void) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_reference(p_Row, p_Void); }
	void push_bool(int p_TID, int p_Row, bool p_Bool) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_bool(p_Row, p_Bool); }


	//Pushes a data onto the curren row.
	void pushc_string(int p_TID, string p_String) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pushc_string(p_String); }
	void pushc_int(int p_TID, int p_Int) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pushc_int(p_Int); }
	void pushc_float(int p_TID, float p_Float) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pushc_float(p_Float); }
	void pushc_reference(int p_TID, void* p_Void) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pushc_reference(p_Void); }
	void pushc_bool(int p_TID, bool p_Bool) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pushc_bool(p_Bool); }


	//Pushes a given rows cell to the given data.
	void pop_push_string(int p_TID, string p_String) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pop_push_string(p_String); }
	void pop_push_int(int p_TID, int p_Int) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pop_push_int(p_Int); }
	void pop_push_float(int p_TID, float p_Float) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pop_push_float(p_Float); }
	void pop_push_reference(int p_TID, void* p_Void) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pop_push_reference(p_Void); }
	void pop_push_bool(int p_TID, bool p_Bool) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pop_push_bool(p_Bool); }


	//Pushes a given rows cell to the given data.
	void push_pop_string(int p_TID, string p_String) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_pop_string(p_String); }
	void push_pop_int(int p_TID, int p_Int) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_pop_int(p_Int); }
	void push_pop_float(int p_TID, float p_Float) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_pop_float(p_Float); }
	void push_pop_reference(int p_TID, void* p_Void) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_pop_reference(p_Void); }
	void push_pop_bool(int p_TID, bool p_Bool) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_pop_bool(p_Bool); }

	//Gets a given cells data in a given row.
	string get_string(int p_TID, int p_Row, int p_Cell) { if (!bounds_TID(p_TID)) { return "Foul"; } return Tables[p_TID]->get_string(p_Row, p_Cell); }
	int    get_int(int p_TID, int p_Row, int p_Cell, int p_Index = 0) { if (!bounds_TID(p_TID)) { return 0; }    return Tables[p_TID]->get_int(p_Row, p_Cell, p_Index); }
	float  get_float(int p_TID, int p_Row, int p_Cell, int p_Index = 0) { if (!bounds_TID(p_TID)) { return 0.0; }  return Tables[p_TID]->get_float(p_Row, p_Cell, p_Index); }
	void* get_reference(int p_TID, int p_Row, int p_Cell, int p_Index = 0) { if (!bounds_TID(p_TID)) { return NULL; } return Tables[p_TID]->get_reference(p_Row, p_Cell, p_Index); }
	bool   get_bool(int p_TID, int p_Row, int p_Cell, int p_Index = 0) { if (!bounds_TID(p_TID)) { return 0; }    return Tables[p_TID]->get_bool(p_Row, p_Cell, p_Index); }

	//Gets a given cells data in a given row.
	string getc_string(int p_TID) { if (!bounds_TID(p_TID)) { return "Foul"; } return Tables[p_TID]->getc_string(); }
	int    getc_int(int p_TID) { if (!bounds_TID(p_TID)) { return 0; }      return Tables[p_TID]->getc_int(); }
	float  getc_float(int p_TID) { if (!bounds_TID(p_TID)) { return 0.0; }    return Tables[p_TID]->getc_float(); }
	void* getc_reference(int p_TID) { if (!bounds_TID(p_TID)) { return NULL; }   return Tables[p_TID]->getc_reference(); }
	bool   getc_bool(int p_TID) { if (!bounds_TID(p_TID)) { return 0; }      return Tables[p_TID]->getc_bool(); }


	//==--    Table Name Submission

	//Sets a given rows cell to the given data.
	void set_string(string p_TName, int p_Row, int p_Cell, string p_String) { set_string(get_Table_ID(p_TName), p_Row, p_Cell, p_String); }
	void set_int(string p_TName, int p_Row, int p_Cell, int p_Int) { set_int(get_Table_ID(p_TName), p_Row, p_Cell, p_Int); }
	void set_float(string p_TName, int p_Row, int p_Cell, float p_Float) { set_float(get_Table_ID(p_TName), p_Row, p_Cell, p_Float); }
	void set_reference(string p_TName, int p_Row, int p_Cell, void* p_Void) { set_reference(get_Table_ID(p_TName), p_Row, p_Cell, p_Void); }
	void set_bool(string p_TName, int p_Row, int p_Cell, bool p_Bool) { set_bool(get_Table_ID(p_TName), p_Row, p_Cell, p_Bool); }

	//Sets a given rows cell to the given data.
	void push_string(string p_TName, int p_Row, string p_String) { push_string(get_Table_ID(p_TName), p_Row, p_String); }
	void push_int(string p_TName, int p_Row, int p_Int) { push_int(get_Table_ID(p_TName), p_Row, p_Int); }
	void push_float(string p_TName, int p_Row, float p_Float) { push_float(get_Table_ID(p_TName), p_Row, p_Float); }
	void push_reference(string p_TName, int p_Row, void* p_Void) { push_reference(get_Table_ID(p_TName), p_Row, p_Void); }
	void push_bool(string p_TName, int p_Row, bool p_Bool) { push_bool(get_Table_ID(p_TName), p_Row, p_Bool); }

	//Pushes a given rows cell to the given data.
	void pop_push_string(string p_TName, string p_String) { pop_push_string(get_Table_ID(p_TName), p_String); }
	void pop_push_int(string p_TName, int p_Int) { pop_push_int(get_Table_ID(p_TName), p_Int); }
	void pop_push_float(string p_TName, float p_Float) { pop_push_float(get_Table_ID(p_TName), p_Float); }
	void pop_push_reference(string p_TName, void* p_Void) { pop_push_reference(get_Table_ID(p_TName), p_Void); }
	void pop_push_bool(string p_TName, bool p_Bool) { pop_push_bool(get_Table_ID(p_TName), p_Bool); }


	//Pushes a given rows cell to the given data.
	void push_pop_string(string p_TName, string p_String) { push_pop_string(get_Table_ID(p_TName), p_String); }
	void push_pop_int(string p_TName, int p_Int) { push_pop_int(get_Table_ID(p_TName), p_Int); }
	void push_pop_float(string p_TName, float p_Float) { push_pop_float(get_Table_ID(p_TName), p_Float); }
	void push_pop_reference(string p_TName, void* p_Void) { push_pop_reference(get_Table_ID(p_TName), p_Void); }
	void push_pop_bool(string p_TName, bool p_Bool) { push_pop_bool(get_Table_ID(p_TName), p_Bool); }

	//Gets a given cells data in a given row.
	string get_string(string p_TName, int p_Row, int p_Cell) { return get_string(get_Table_ID(p_TName), p_Row, p_Cell); }
	int    get_int(string p_TName, int p_Row, int p_Cell, int p_Index = 0) { return get_int(get_Table_ID(p_TName), p_Row, p_Cell, p_Index); }
	float  get_float(string p_TName, int p_Row, int p_Cell, int p_Index = 0) { return get_float(get_Table_ID(p_TName), p_Row, p_Cell, p_Index); }
	void* get_reference(string p_TName, int p_Row, int p_Cell, int p_Index = 0) { return get_reference(get_Table_ID(p_TName), p_Row, p_Cell, p_Index); }
	bool   get_bool(string p_TName, int p_Row, int p_Cell, int p_Index = 0) { return get_bool(get_Table_ID(p_TName), p_Row, p_Cell, p_Index); }

	//Gets a the current cells first data bit.
	string getc_string(string p_TName) { return getc_string(get_Table_ID(p_TName)); }
	int    getc_int(string p_TName) { return getc_int(get_Table_ID(p_TName)); }
	float  getc_float(string p_TName) { return getc_float(get_Table_ID(p_TName)); }
	void* getc_reference(string p_TName) { return getc_reference(get_Table_ID(p_TName)); }
	bool   getc_bool(string p_TName) { return getc_bool(get_Table_ID(p_TName)); }


	//Copies the data from the submitted cell into itself.
	string copy(int p_Table, int p_Row, int p_Cell, void* p_Cell_To_Copy)
	{
		if (!bounds_TID(p_Table)) { cout << "\n\n\t Table[" << p_Table << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		Tables[p_Table]->copy_Cell(p_Row, p_Cell, p_Cell_To_Copy);

		return "CODE 4";
	}

	string copy(string p_TName, int p_Row, int p_Cell, void* p_Cell_To_Copy)
	{
		return copy(get_Table_ID(p_TName), p_Row, p_Cell, p_Cell_To_Copy);
	}

	//Copies a row.
	string copy_Row(int p_Table, int p_Row, void* p_Row_To_Copy)
	{
		if (!bounds_TID(p_Table)) { cout << "\n\n\t Table[" << p_Table << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		Tables[p_Table]->copy_Row(p_Row, p_Row_To_Copy);

		return "CODE 4";
	}
	string copy_Row(string p_TName, int p_Row, void* p_Row_To_Copy)
	{
		return copy_Row(get_Table_ID(p_TName), p_Row, p_Row_To_Copy);
	}

	//Copies a row from given table IDs
	string copy_Row(int p_Table, int p_Row, int p_Table_To, int p_Row_To)
	{
		if (!bounds_TID(p_Table) || !bounds_TID(p_Table_To)) { return "TABLE_NO_EXISTY"; }

		return copy_Row(p_Table_To, p_Row_To, Tables[p_Table]->get_Row_Reference(p_Row));
	}

	//Copies a whole damn table.
	string copy_Table(int p_Table, int p_Table_To_Copy)
	{
		if (!bounds_TID(p_Table)) { cout << "\n\n\t Table[" << p_Table << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		Tables[p_Table]->copy_Table(Tables[p_Table_To_Copy]);

		return "CODE 4";
	}
	string copy_Table(string p_TName, string p_TTCName)
	{
		return copy_Table(get_Table_ID(p_TName), get_Table_ID(p_TTCName));
	}

	//Copies a whole damn table.
	string rotate_Table(int p_Table)
	{
		if (!bounds_TID(p_Table)) { cout << "\n\n\t Table[" << p_Table << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		Tables[p_Table]->rotate_Table();

		return "CODE 4";
	}
	string rotate_Table(string p_TName)
	{
		return rotate_Table(get_Table_ID(p_TName));
	}


	string translate_Row(string p_From_Table, int p_Row, string p_To_Table, int p_Offset, int p_Column = 0)
	{
		int tmp_Table_ID = get_Table_ID(p_To_Table);

		if (!bounds_TID(tmp_Table_ID)) { cout << "\n\n\t Table[" << tmp_Table_ID << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		return Tables[tmp_Table_ID]->translate_Row(get_Table_Reference(p_From_Table), p_Row, p_Offset, p_Column);
	}
	string translate_Row(int p_From_Table, int p_Row, int p_To_Table, int p_Offset, int p_Column = 0)
	{
		if (!bounds_TID(p_To_Table)) { cout << "\n\n\t Table[" << p_To_Table << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		return Tables[p_To_Table]->translate_Row(get_Table_Reference(p_From_Table), p_Row, p_Offset, p_Column);
	}

	//Overlays a row onto another row at the given index.
	string overlay_Row(int p_Table, int p_Row, int p_Index, void* p_Row_To_Copy)
	{
		if (!bounds_TID(p_Table)) { cout << "\n\n\t Table[" << p_Table << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		Tables[p_Table]->overlay_Row(p_Row, p_Index, p_Row_To_Copy);

		return "CODE 4";
	}
	string overlay_Row(string p_TName, int p_Row, int p_Index, void* p_Row_To_Copy)
	{
		return overlay_Row(get_Table_ID(p_TName), p_Row, p_Index, p_Row_To_Copy);
	}

	string shift_Column(int p_Table, int p_Column_To_Shift)
	{
		if (!bounds_TID(p_Table)) { cout << "\n\n\t Table[" << p_Table << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		return Tables[p_Table]->shift_Column(p_Column_To_Shift);
	}
	string shift_Column(string p_TName, int p_Column_To_Shift)
	{
		return shift_Column(get_Table_ID(p_TName), p_Column_To_Shift);
	}

	string shift_All_Rows(int p_Table, int p_Index)
	{
		if (!bounds_TID(p_Table)) { cout << "\n\n\t Table[" << p_Table << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		return Tables[p_Table]->shift_All_Rows(p_Index);
	}
	string shift_All_Rows(string p_TName, int p_Index)
	{
		return shift_All_Rows(get_Table_ID(p_TName), p_Index);
	}

	//Expands the table array by one.
	void increment_Table_Array()
	{
		expand_Table_Array(Table_Count + 1);
	}

	//Expands the table array.
	void expand_Table_Array(int p_Depth)
	{
		if (p_Depth <= Table_Count) { return; }

		//Setup the tmp_tables
		c_Base_Table** tmp_Tables = new c_Base_Table * [Table_Count];

		//Copy the current tables into the temporary tables.
		for (int cou_Table = 0; cou_Table < Table_Count; cou_Table++)
		{
			tmp_Tables[cou_Table] = Tables[cou_Table];
			Tables[cou_Table] = NULL;
		}

		//Reallocate the Table array.
		delete[] Tables;
		Tables = NULL;

		Tables = new c_Base_Table * [p_Depth];

		//Read the original tables back into the reallocated array.
		for (int cou_Index = 0; cou_Index < Table_Count; cou_Index++)
		{
			Tables[cou_Index] = tmp_Tables[cou_Index];
			tmp_Tables[cou_Index] = NULL;
		}

		//NULL the newly minted indexes.
		for (int cou_Index = Table_Count; cou_Index < p_Depth; cou_Index++)
		{
			Tables[cou_Index] = NULL;
		}

		//Set the table count.
		Table_Count = p_Depth;

		//Cleanup the tmp array.
		delete[] tmp_Tables;

	}

	//Checks for bounding errors.
	bool bounds_TID(int p_TID)
	{
		if (p_TID >= Table_Count) { return 0; }
		if (p_TID < 0) { return 0; }
		if (Tables[p_TID] == NULL) { return 0; }
		return 1;
	}

	//Outputs a given table.
	void output_Table(int p_TID)
	{
		if (!bounds_TID(p_TID)) { return; }
		ostr(0, 12, "\n\n\t __________");
		for (unsigned int cou_Index = 0; cou_Index < ((Tables[p_TID]->get_Table_Name()).size()); cou_Index++)
		{
			ostr(0, 12, "_");
		}
		Tables[p_TID]->output_Table();
	}
	void output_Table(string p_TName)
	{
		output_Table(get_Table_ID(p_TName));
	}

	//Outputs all of the tables.
	void output_All_Tables()
	{
		for (int cou_Index = 0; cou_Index < Table_Count; cou_Index++)
		{
			output_Table(cou_Index);
		}
	}

	//Outputs a tables header.
	void output_Table_Header(int p_TID)
	{
		if (!bounds_TID(p_TID)) { return; }

		cout << "\n\t TID[" << p_TID << "]";
		Tables[p_TID]->output_Table_Header();
	}
	void output_Table_Header(string p_TName)
	{
		output_Table_Header(get_Table_ID(p_TName));
	}

	//Outputs every table header.
	void output_All_Table_Headers()
	{
		for (int cou_Index = 0; cou_Index < Table_Count; cou_Index++)
		{
			output_Table_Header(cou_Index);
		}
	}

	//Outputs a tables header.
	void output_Table_Verbose(int p_TID)
	{
		if (!bounds_TID(p_TID)) { return; }

		cout << "\n\t TID[" << p_TID << "]";
		Tables[p_TID]->output_Table_Verbose();
	}
	void output_Table_Verbose(string p_TName)
	{
		output_Table_Verbose(get_Table_ID(p_TName));
	}

	//Outputs every table header.
	void output_All_Tables_Verbose()
	{
		for (int cou_Index = 0; cou_Index < Table_Count; cou_Index++)
		{
			output_Table_Verbose(cou_Index);
		}
	}

	//Outputs the headers for registered tables.
	void output_Table_Registry()
	{
		output_Table(0);
	}

	//Outputs the registry tree.
	void output_Table_Registry_Tree()
	{
		Table_Registry.output_Tree();
	}
	
	//Saves all the tables.
	void save_Tables()
	{
		
	}
};



