

class c_MMap
{
private:
     
     //Size of the memory chunk.
     const int CONST_BUFFER_SIZE = 1024;
     
     //The file to open.
     HANDLE Data_File;
     
     //Handle to the mapped file
     HANDLE MMap_File_Mapping;
     
     //Handle to the mapped file view
     HANDLE MMap_View;
     
     //System information. Used to get the system granularity.
     SYSTEM_INFO system_Info;
     DWORD system_Granulation;
     
     //Get the nearest multiple of system grains to the starting offset. Where to start the mapping.
     DWORD MMap_Starting_Offset;
     
     //The size of the file mapping view.
     //MMap_View_Size = (MMap_Starting_Offset % system_Granulation) + BUFFSIZE
     DWORD MMap_View_Size;
     
     //The size of the file mapping object.
     DWORD MMap_File_Map_Size;
     
     //The current position of the data pointer in the file.
     int MMap_Current_Position;
     
public:
     
     string file_Name;
     
     void * MMap_Current_Position_Data;
     
     c_MMap()
     {
          Data_File = NULL;
          MMap_Current_Position = 0;
          MMap_File_Map_Size = 0;
          MMap_File_Mapping = NULL;
          MMap_Starting_Offset = 0;
          MMap_View = NULL;
          MMap_View_Size = 0;
     }
     
     
     void get_System_Granulation()
     {
          // Get the system allocation granularity.
          GetSystemInfo(&system_Info);
          system_Granulation = system_Info.dwAllocationGranularity;
          
     }
     
     void calculate_Mapping_Parameters(int p_File_Map_Start)
     {
          cout << "\n p_File_Map_Start->" << p_File_Map_Start;
          
          //Get the system granulation.
          get_System_Granulation();
          
          cout << "\n system_Granulation->" << system_Granulation;
          
          //Get the MMap_Starting_Offset for where to start the mapping in the file grains.
          MMap_Starting_Offset = (p_File_Map_Start / system_Granulation) * system_Granulation;
          
          cout << "\n MMap_Starting_Offset->" << MMap_Starting_Offset;
          
          //Calculate the size of the mapping view.
          MMap_View_Size = (p_File_Map_Start % system_Granulation) + CONST_BUFFER_SIZE;
          
          cout << "\n MMap_View_Size->" << MMap_View_Size;
          
          //Figure size of the file mapping object.
          MMap_File_Map_Size = p_File_Map_Start + CONST_BUFFER_SIZE;
          
          cout << "\n MMap_File_Map_Size->" << MMap_File_Map_Size;
          
          //Find the current position of the data pointer to use when referencing file contents.
          MMap_Current_Position = p_File_Map_Start - MMap_Starting_Offset;
          
          cout << "\n MMap_Current_Position->" << MMap_Current_Position;
     }
     
     //Maps the files, does not create the view.
     int create_Mapping()
     {
          MMap_File_Mapping = CreateFileMapping(Data_File,          // current file handle
               NULL,           // default security
               PAGE_READWRITE, // read/write permission
               0,              // size of mapping object, high
               MMap_File_Map_Size,  // size of mapping object, low
               NULL);  
          
          if (MMap_File_Mapping == NULL)
          {
               cout << "\n MMap_File_Mapping == NULL LastError():" << GetLastError();
               return 2;
          }
          
          return 1;
     }
     
     //Creates a view at the given offset.
     int create_Mapping_View(int p_File_Map_Start = 5)
     {
          calculate_Mapping_Parameters(p_File_Map_Start);
          
          //Do this in case a view is already mapped.
          if (MMap_View != NULL)
          {
               UnmapViewOfFile(MMap_View);
          }
          
          MMap_View = MapViewOfFile(MMap_File_Mapping,            // handle to
                                // mapping object
               FILE_MAP_ALL_ACCESS, // read/write
               0,                   // high-order 32
                                   // bits of file
                                   // offset
               MMap_Starting_Offset,      // low-order 32
                                   // bits of file
                                   // offset
               MMap_View_Size);      // number of bytes
                                   // to map
                                   
          if (MMap_View == NULL)
          {
               cout << "\n MMap_View == NULL LastError():" << GetLastError();
               return 3;
          }
          
          MMap_Current_Position_Data = ((char*) MMap_View + MMap_Current_Position);
          return 1;
     }
     
     //The p_File_Map_Start is the offset to use in the beginning. 
     //There will be a function for moving the view.
     int open_File(string p_File_Name, int p_File_Map_Start = 0)
     {
          
          Data_File = NULL;
          
          cout << "\n Opening " << p_File_Name;
          
          calculate_Mapping_Parameters(p_File_Map_Start);
          
          Data_File = CreateFile(p_File_Name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
          
          //Map the file
          create_Mapping();
          
          //Create the view.
          create_Mapping_View(p_File_Map_Start);
          
          output_View();
          
          return 1;
     }
     
     void close()
     {
          UnmapViewOfFile(MMap_View);
          CloseHandle(MMap_File_Mapping); 
          CloseHandle(Data_File);   
     }
     
     //Displays the contents of the currently mapped view block.
     void output_View(int p_Mode = 0)
     {
          char * tmp_Data = NULL;
          if (p_Mode == 0)
          {
               cout << "\n -----------------------------------------------------------------------------\n";
               for (int cou_Index=0;cou_Index<CONST_BUFFER_SIZE;cou_Index++)
               {
                    tmp_Data = ((char*) MMap_View + MMap_Current_Position + cou_Index);
                    
                    cout << (*tmp_Data);
               }
               cout << "\n -----------------------------------------------------------------------------\n";
          }
          if (p_Mode == 1)
          {
               for (int cou_Index=0;cou_Index<CONST_BUFFER_SIZE;cou_Index++)
               {
                    tmp_Data = ((char*) MMap_View + MMap_Current_Position + cou_Index);
                    
                    cout << "\n " << cou_Index << "->  -->" << (*tmp_Data) << "<--";
               }
          }
     }
};