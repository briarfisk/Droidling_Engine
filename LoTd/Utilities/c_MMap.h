#ifndef c_MMAP_H
#define c_MMAP_H

//This is a chunk of a complete MMap. Each chunk is a predetermined size. This way the base can be broken into pieces and stored on any media available.
class c_MMap
{
private:
     
     //Size of the memory chunk.
     const int CONST_BUFFER_SIZE = 128;
     
     //The temporary character array to hold the current node.
     char Data[128];
     
     //The file to open.
     HANDLE Data_File;
     
     //Handle to the mapped file
     HANDLE MMap_File_Mapping;
     
     //Handle to the mapped file view
     HANDLE MMap_View;
     
     //System information. Used to get the system granularity.
     SYSTEM_INFO system_Info;
     unsigned long long int system_Granulation;
     
     //Get the nearest multiple of system grains to the starting offset. Where to start the mapping.
     unsigned long long int MMap_Starting_Offset;
     
     //The size of the file mapping view.
     //MMap_View_Size = (MMap_Starting_Offset % system_Granulation) + BUFFSIZE
     unsigned long long int MMap_View_Size;
     
     //The size of the file mapping object.
     unsigned long long int MMap_File_Map_Size;
     
     //The current position of the data pointer in the file.
     unsigned long long int MMap_Current_Position;
     
     //The current position of the offset into the view the data is at.
     unsigned long long int MMap_View_Current_Position;
     
     
     //The current filename opened
     std::string File_Name;
     
public:
     
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
          MMap_View_Current_Position = 0;
          File_Name = "NOFILE";
     }
     
     
     unsigned long long int get_System_Granulation()
     {
          // Get the system allocation granularity.
          GetSystemInfo(&system_Info);
          system_Granulation = system_Info.dwAllocationGranularity;
          return system_Granulation;
     }
     
     //p_Map_Size is MB
     void calculate_Mapping_Parameters(unsigned long long int p_Map_Size = 512)
     {
          std::cout << "\n p_Map_Size->" << p_Map_Size;
          
          //Get the system granulation.
          get_System_Granulation();
          
          std::cout << "\n system_Granulation->" << system_Granulation;
          
          //Get the MMap_Starting_Offset for where to start the mapping in the file grains.
          MMap_Starting_Offset = 0;//(p_File_Map_Start / system_Granulation) * system_Granulation;
          
          std::cout << "\n MMap_Starting_Offset->" << MMap_Starting_Offset;
          
          //Figure size of the file mapping object.
          MMap_File_Map_Size = (p_Map_Size * (1024 * 1024)) + CONST_BUFFER_SIZE;
          
          if (p_Map_Size == 0)
          {
               std::cout << "\n MMap_File_Map_Size->FILE_DEPENDENT";
               MMap_File_Map_Size = 0;
          }
          std::cout << "\n MMap_File_Map_Size->" << MMap_File_Map_Size;
          
          //Find the current position of the data pointer to use when referencing file contents.
          MMap_Current_Position = MMap_Starting_Offset;
          
          std::cout << "\n MMap_Current_Position->" << MMap_Current_Position;
     }
     
     //Maps the files, does not create the view.
     int create_Mapping()
     {
          MMap_File_Mapping = CreateFileMapping(Data_File,          // current file handle
			   NULL,           // default security
               PAGE_READWRITE, // read/write permission
               0,              // size of mapping object, high
               DWORD(MMap_File_Map_Size),  // size of mapping object, low
               NULL);  
          
          if (MMap_File_Mapping == NULL)
          {
               std::cout << "\n MMap_File_Mapping == NULL LastError():" << GetLastError();
               return 2;
          }
          
          return 1;
     }
     
     
     //unsigned long long int tmp_MMap_Stating_Offset;
     void calculate_View_Parameters(unsigned long long int p_Position)
     {
          //Get the MMap_Starting_Offset for where to start the mapping in the file grains.
          
          //MMap_Starting_Offset = tmp_MMap_Stating_Offset * (unsigned long long int) system_Granulation;
          MMap_Starting_Offset = ((p_Position / ((unsigned long long int) system_Granulation))) * (unsigned long long int) system_Granulation;
          
          //cout << "\n MMap_Starting_Offset->" << MMap_Starting_Offset;
          
          //Calculate the size of the mapping view.
          //MMap_View_Size = (p_Position - ((unsigned long long int) (p_Position / system_Granulation))) + CONST_BUFFER_SIZE;
          MMap_View_Size = CONST_BUFFER_SIZE + (p_Position - MMap_Starting_Offset);
          //MMap_View_Size = system_Granulation;
          
          //cout << "\n MMap_View_Size->" << MMap_View_Size;
          
          MMap_Current_Position = p_Position;
          
          //cout << "\n MMap_Current_Position->" << MMap_Current_Position;
          
          MMap_View_Current_Position = MMap_View_Size - CONST_BUFFER_SIZE;
          
          //cout << "\n MMap_View_Current_Position->" << MMap_View_Current_Position;
     }
     
     //Creates a view at the given offset.
     int create_Mapping_View(unsigned long long int p_Position = 0)
     {
          calculate_View_Parameters(p_Position);
          
          //Do this in case a view is already mapped.
          if (MMap_View != NULL)
          {
               UnmapViewOfFile(MMap_View);
               MMap_View = NULL;
          }
          MMap_View = MapViewOfFile(MMap_File_Mapping,            // handle to
                                // mapping object
               FILE_MAP_ALL_ACCESS, // read/write
               0,                   // high-order 32
                              // bits of file
                              // offset
               DWORD(MMap_Starting_Offset),      // low-order 32
                                   // bits of file
                                   // offset
               SIZE_T(MMap_View_Size));      // number of bytes
                                   // to map
                                   
          if (MMap_View == NULL)
          {
               std::cout << "\n MMap_View == NULL LastError():" << GetLastError();
               return 3;
          }
          
          MMap_Current_Position_Data = ((char*) MMap_View + MMap_View_Current_Position);
          return 1;
     }
     
     //The p_File_Map_Start is the offset to use in the beginning. 
     //There will be a function for moving the view.
     int open_File(std::string p_File_Name, unsigned long long int p_Map_Size_MB = 512, bool p_Resize = 0)
     {
          
          Data_File = NULL;
          
          File_Name = p_File_Name;
          
          std::cout << "\n Opening " << p_File_Name;
          
          //After the file iloaded 
          Data_File = CreateFileA(p_File_Name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
          
          if ((GetLastError() == 183) && (!p_Resize))
          {
               calculate_Mapping_Parameters(0);
          }
          else
          {
               calculate_Mapping_Parameters(p_Map_Size_MB);
          }
          
          //Map the file
          create_Mapping();
          
          //Create the view.
          create_Mapping_View(0);
          
          //Determine if the mapping was a success.
          if (MMap_Current_Position_Data == NULL){ return 0; }
          
          return 1;
     }
     
     void close()
     {
          save_Data();
          UnmapViewOfFile(MMap_View);
          CloseHandle(MMap_File_Mapping); 
          CloseHandle(Data_File);   
     }
     
     void save_Data()
     {
          if (MMap_View != NULL)
          {
               FlushViewOfFile(MMap_Current_Position_Data, CONST_BUFFER_SIZE);
          }
     }
     
     //Gets the current datas.
     void get_Data(char * p_Data)
     {                  
          if (MMap_View == NULL)
          {
               std::cout << "\n MMap_View == NULL LastError():" << GetLastError();
               return;
          }
          for (int cou_Index=0;cou_Index<CONST_BUFFER_SIZE;cou_Index++)
          {
               p_Data[cou_Index] = ((char*)MMap_Current_Position_Data)[cou_Index];
          }
     }
     
     //Gets a CONST_BUFFER_SIZE byte chunk.
     void get_Chunk(unsigned long long int p_Position, char * p_Data)
     {
          //Multiply by CONST_BUFFER_SIZE to get the chunk position, not byte position.
          create_Mapping_View(p_Position * CONST_BUFFER_SIZE);
          std::cout << "\n create_Mapping_View :: LastError():" << GetLastError() << " MMap_View:" << MMap_View << " MMap_Current_Position_Data:" << MMap_Current_Position_Data;
          get_Data(p_Data);
     }
     
     int expand(unsigned long long int p_Map_Size_MB)
     {
          close();
          open_File(File_Name, p_Map_Size_MB, 1);
          return 1;
     }
     
     //Sets the current MMap_View to the given data.
     void set_Data(char * p_Data, int p_Len)
     {
          //cout << "\n"; std::cout.flush();
          //cout << " p_Len:" << p_Len; std::cout.flush();
          //cout << " &p_Data:" << p_Data; std::cout.flush();
          for (int cou_Index=0;cou_Index<p_Len;cou_Index++)
          {
               //cout << " p_Data:" << p_Data[cou_Index]; std::cout.flush();
               ((char*)MMap_Current_Position_Data)[cou_Index] = p_Data[cou_Index];
          }
     }
     
     void output_View(int p_Type)
     {
          
     }
};

#endif // !c_MMAP_H