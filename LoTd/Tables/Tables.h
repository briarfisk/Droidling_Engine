template <class t_Data>
class c_2D_Matrix
{
public:
     
	t_Data** DATA;
     int Width;
     int Height;
     
	 c_2D_Matrix()
     {
          Width = 0;
          Height = 0;
          DATA = NULL;
          
          initialize_Data();
     }
     
     ~c_2D_Matrix()
     {
          reset();
     }
     
     void resize(int p_X, int p_Y)
     {
          reset();
          
          Width = p_X;
          Height = p_Y;
          
          DATA = new t_Data*[Width];
          
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               DATA[cou_X] = new t_Data[Height];
               for (int cou_Y=0;cou_Y<Height;cou_Y++)
               {
                    DATA[cou_X] [cou_Y] = NULL;
               }
          }
     }
     
     void reset()
     {
          if (DATA != NULL)
          {
               for (int cou_X=0;cou_X<Width;cou_X++)
               {
                    delete [] DATA[cou_X]; DATA[cou_X] = NULL;
               }
               delete [] DATA; DATA = NULL;
          }
     }
     
     void initialize_Data()
     {
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               for (int cou_Y=0;cou_Y<Height;cou_Y++)
               {
                    DATA[cou_X] [cou_Y] = 0.0f;
               }
          }
     }
          
     void set_Pixel(int p_X, int p_Y, t_Data p_Bit)
     {
          DATA[p_X] [p_Y] = p_Bit;
     }
     
     void output_IMG()
     {
          int tmp_X = get_Console_Cursor_X();
          int tmp_Y = get_Console_Cursor_Y();
          
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               for (int cou_Y=0;cou_Y<Height;cou_Y++)
               {
                    xy((cou_X + tmp_X), (cou_Y + tmp_Y));
                    ochr(1, (int (DATA[cou_X] [cou_Y].F)), char (176)); continue;
               }
          }
          
     }
     
     void output_IMG_Grayscale()
     {
          int tmp_X = get_Console_Cursor_X();
          int tmp_Y = get_Console_Cursor_Y();
          
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               for (int cou_Y=0;cou_Y<Height;cou_Y++)
               {
                    xy((cou_X + tmp_X), (cou_Y + tmp_Y));
                    //ochr(1, (int (DATA[cou_X] [cou_Y].F)), char (176)); continue;
                    
                    //Is the data over the range? If yes then continue so that it does not fall through.
                    if (DATA[cou_X] [cou_Y].F < 1 ){ ochr(0, 0, char(176)); continue; }
                    if (DATA[cou_X] [cou_Y].F < 2 ){ ochr(0, 8, char(176)); continue; }
                    if (DATA[cou_X] [cou_Y].F < 3 ){ ochr(0, 8, char(177)); continue; }
                    if (DATA[cou_X] [cou_Y].F < 4 ){ ochr(0, 8, char(178)); continue; }
                    if (DATA[cou_X] [cou_Y].F < 5 ){ ochr(0, 7, char(177)); continue; }
                    if (DATA[cou_X] [cou_Y].F < 6 ){ ochr(0, 7, char(178)); continue; }
                    if (DATA[cou_X] [cou_Y].F < 7 ){ ochr(0, 15, char(177)); continue; }
                    if (DATA[cou_X] [cou_Y].F < 8 ){ ochr(0, 15, char(178)); continue; }
                    //Fall through.
                    ochr(0, 15, char(178)); continue;
               }
          }
     }
};



//Includes for the tables.
#include "c_Cell.h"
#include "c_Row.h"
#include "c_Table.h"
#include "c_Table_Handler.h"
#include "c_Raw_Table_1D.h"
#include "c_Raw_Table_2D.h"
#include "c_Raw_Table_3D.h"
