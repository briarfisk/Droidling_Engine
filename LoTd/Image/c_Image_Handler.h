

class c_Image_Handler
{
public:
     
     BITMAP BMP;
     HBITMAP BMP_Source;
     
     BITMAP RGB_BMP[3];
     HBITMAP RGB_BMP_Source[3];
     
     //The RGB input cells.
     c_Raw_Table_Cell_2D RGB_Input_Cell[3];
     
     c_Image_Handler()
     {
          BMP_Source = NULL;
          
          for (int cou_RGB=0;cou_RGB<3;cou_RGB++)
          {
               //RGB_BMP[cou_RGB];
               RGB_BMP_Source[cou_RGB] = NULL;
               RGB_Input_Cell[cou_RGB].reset();
          }
     }
     
     ~c_Image_Handler()
     {
          //Cleanup the Bitmaps.
          if (BMP_Source != NULL){ DeleteObject(BMP_Source); }
     }
     

     bool save_Image(string p_Filename)
     {
          if (BMP_Source == NULL){ return false; }
          
          //Create a new file for writing
          
          //Open with write/binary
          FILE * tmp_File = fopen(p_Filename.c_str(), "wb");
          
          if(tmp_File == NULL)
          {
               return false;
          }

          BITMAPINFOHEADER tmp_Bitmap_Info_Header;

          tmp_Bitmap_Info_Header.biSize = sizeof(BITMAPINFOHEADER);
          tmp_Bitmap_Info_Header.biSizeImage = ((((BMP.bmWidth + 31) & ~31) / 8) * BMP.bmHeight);
          tmp_Bitmap_Info_Header.biSize = sizeof(BITMAPINFOHEADER);
          tmp_Bitmap_Info_Header.biWidth = BMP.bmWidth;
          tmp_Bitmap_Info_Header.biHeight = BMP.bmHeight;
          tmp_Bitmap_Info_Header.biPlanes = BMP.bmPlanes;
          tmp_Bitmap_Info_Header.biBitCount = BMP.bmBitsPixel;
          tmp_Bitmap_Info_Header.biCompression = BI_RGB;

          BITMAPFILEHEADER tmp_Bitmap_File_Header;

          int tmp_Bits_Offset = sizeof(BITMAPFILEHEADER) + tmp_Bitmap_Info_Header.biSize;

          LONG tmp_Image_Size = tmp_Bitmap_Info_Header.biSizeImage;
          LONG tmp_File_Size = tmp_Bits_Offset + tmp_Image_Size;
          tmp_Bitmap_File_Header.bfType = 'B'+('M'<<8);
          tmp_Bitmap_File_Header.bfOffBits = tmp_Bits_Offset;
          tmp_Bitmap_File_Header.bfSize = tmp_File_Size;
          tmp_Bitmap_File_Header.bfReserved1 = tmp_Bitmap_File_Header.bfReserved2 = 0;

          //Write the bitmap file header
          fwrite(&tmp_Bitmap_File_Header, 1, sizeof(BITMAPFILEHEADER), tmp_File);

          //And then the bitmap info header
          fwrite(&tmp_Bitmap_Info_Header, 1, sizeof(BITMAPINFOHEADER), tmp_File);

          //Finally, write the image data itself
          //-- the data represents our drawing
          fwrite(BMP.bmBits, 1, tmp_Image_Size, tmp_File);
          
          fclose(tmp_File);
          
          return true;
     }
     
     
     //Creates a bitmap of the dimensions given.
     void create_Bitmap(int p_W, int p_H, int p_Planes = 1, int p_Bits_Per_Pixel = 32, void * p_Bits = NULL)
     {
          if (BMP_Source != NULL){ DeleteObject(BMP_Source); }
          
          BMP_Source = CreateBitmap(p_W, p_H, p_Planes, p_Bits_Per_Pixel, p_Bits);
     }
          
     //Creates a bitmap of the dimensions given.
     void create_RGB_Bitmaps(int p_W, int p_H, int p_Planes = 1, int p_Bits_Per_Pixel = 32, void * p_Bits = NULL)
     {
          for (int cou_Index=0;cou_Index<3;cou_Index++)
          {
               if (RGB_BMP_Source[cou_Index] != NULL){ DeleteObject(RGB_BMP_Source[cou_Index]); }
               
               RGB_BMP_Source[cou_Index] = CreateBitmap(p_W, p_H, p_Planes, p_Bits_Per_Pixel, p_Bits);
          }
     }
     
     //Sets a pixel.
     void set_Pixel(int p_X, int p_Y, int p_R, int p_G, int p_B)
     {
          
          //Create the DCs needed for each of the four bitmaps.
          HDC tmp_DC_This = CreateCompatibleDC(GetDC(0));
          //HBITMAP tmp_DC_This_Orig = (HBITMAP) SelectObject(tmp_DC_This, BMP_Source);
          
          SetPixel(tmp_DC_This, p_X, p_Y, RGB(p_R, p_G, p_B));
          //tmp_Color = GetPixel(tmp_DC_This, cou_X, cou_Y);
     }
     
     //Copies a given HBITMAP into the BMP_Source.
     void set_HBITMAP(c_Image_Handler * p_IMG)
     {
          if (p_IMG == NULL){ ostr(0, 12, "\n\nPassed p_IMG* is NULL"); return; }
          cout << "\n\n\n set_HBITMAP->" << this;
          
          cout << "\n creating_Bitmaps...";
          //Create the device contexts to work with the HBITMAPS.
          //Creating with the NULL parameter creates one compatable with the current display.
          HDC tmp_HDC_This = CreateCompatibleDC(NULL);
          HDC tmp_HDC_Given = CreateCompatibleDC(NULL);
          
          
          //If BMP_Source exists then delete it and CreateCompatableBitmap to copy to.
          if (BMP_Source != NULL){ DeleteObject(BMP_Source); }
          BMP_Source = CreateBitmap(p_IMG->BMP.bmWidth, p_IMG->BMP.bmHeight, p_IMG->BMP.bmPlanes, p_IMG->BMP.bmBitsPixel, NULL);
          
          
          //Select the given HBITMAP into the temporary HDC.
          HBITMAP tmp_HDC_This_Orig = (HBITMAP)SelectObject(tmp_HDC_This, BMP_Source);
          HBITMAP tmp_HDC_Given_Orig = (HBITMAP)SelectObject(tmp_HDC_Given, p_IMG->BMP_Source);
          
          cout << "\n BitBlt...";
          
          //BitBlt the given HBITMAP into the BMP_Source
          BitBlt(tmp_HDC_This, 0, 0, p_IMG->BMP.bmWidth, p_IMG->BMP.bmHeight, tmp_HDC_Given, 0, 0, SRCCOPY);
          
          
          //Cleanup the HDCs we created.
          SelectObject(tmp_HDC_This, tmp_HDC_This_Orig);
          SelectObject(tmp_HDC_Given, tmp_HDC_Given_Orig);
          DeleteDC(tmp_HDC_This);
          DeleteDC(tmp_HDC_Given);
          
          
          //Setup the bitmap with the information on the new HBITMAP.
          GetObject(BMP_Source, sizeof(BITMAP), &BMP);
          
          cout << "\n set_HBITMAP";
          cout << "\n  Given:";
          cout << "\n   int bmType->" << p_IMG->BMP.bmType;  
          cout << "\n   int bmWidth->" << p_IMG->BMP.bmWidth;  
          cout << "\n   int bmHeight->" << p_IMG->BMP.bmHeight;  
          cout << "\n   int bmWidthBytes->" << p_IMG->BMP.bmWidthBytes;  
          cout << "\n   BYTE bmPlanes->" << p_IMG->BMP.bmPlanes;  
          cout << "\n   BYTE bmBitsPixel->" << p_IMG->BMP.bmBitsPixel;  
          cout << "\n   LPVOID bmBits->" << p_IMG->BMP.bmBits;  
          
          cout << "\n  Gathered:";
          cout << "\n   int bmType->" << BMP.bmType;  
          cout << "\n   int bmWidth->" << BMP.bmWidth;  
          cout << "\n   int bmHeight->" << BMP.bmHeight;  
          cout << "\n   int bmWidthBytes->" << BMP.bmWidthBytes;  
          cout << "\n   BYTE bmPlanes->" << BMP.bmPlanes;  
          cout << "\n   BYTE bmBitsPixel->" << BMP.bmBitsPixel;  
          cout << "\n   LPVOID bmBits->" << BMP.bmBits;  
     }
     
     //Loads an image into the BMP_Source.
     void load_Image(string p_Filename)
     {
          //Cleanup the BMP_Source if it already is set.
          if (BMP_Source != NULL){ DeleteObject(BMP_Source); }
          
          BMP_Source = (HBITMAP)LoadImage(NULL, p_Filename.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
          
          //Setup the bitmap with the information on the new HBITMAP.
          GetObject(BMP_Source, sizeof(BITMAP), &BMP);
          
          cout << "\n\n load_Image(" << p_Filename << ")";
          cout << "\n   int bmType->" << BMP.bmType;  
          cout << "\n   int bmWidth->" << BMP.bmWidth;  
          cout << "\n   int bmHeight->" << BMP.bmHeight;  
          cout << "\n   int bmWidthBytes->" << BMP.bmWidthBytes;  
          cout << "\n   BYTE bmPlanes->" << BMP.bmPlanes;  
          cout << "\n   BYTE bmBitsPixel->" << BMP.bmBitsPixel;  
          cout << "\n   LPVOID bmBits->" << BMP.bmBits;  
     }

     //Breaks the bitmap into the color layers.
     void separate_Bitmap()
     {
          if (BMP_Source == NULL){ return; }
          
          cout << "\n\n separate_Bitmap()->" << this;
          
          //Create the DCs needed for each of the four bitmaps.
          HDC tmp_DC_This = CreateCompatibleDC(GetDC(0));
          HBITMAP tmp_DC_This_Orig = (HBITMAP) SelectObject(tmp_DC_This, BMP_Source);
          
          HDC tmp_DC_RGB[3];
          HBITMAP tmp_DC_RGB_Orig[3];
          
          COLORREF tmp_Color;
          BYTE R = 0;
          BYTE G = 0;
          BYTE B = 0;
          
          cout << "\n Setting up...";
          
          //Reset the bitmaps in preparation for the RGB.
          create_RGB_Bitmaps(BMP.bmWidth, BMP.bmHeight, BMP.bmPlanes, BMP.bmBitsPixel, NULL);
          
          for (int cou_RGB=0;cou_RGB<3;cou_RGB++)
          {
               tmp_DC_RGB[cou_RGB] = CreateCompatibleDC(GetDC(0));
               tmp_DC_RGB_Orig[cou_RGB] = (HBITMAP) SelectObject(tmp_DC_RGB[cou_RGB], RGB_BMP_Source[cou_RGB]);
               GetObject(RGB_BMP_Source[cou_RGB], sizeof(BITMAP), &RGB_BMP[cou_RGB]);
               
               //Reset the input cells.
               RGB_Input_Cell[cou_RGB].reset();
               RGB_Input_Cell[cou_RGB].resize(BMP.bmWidth, BMP.bmHeight);
          }
          
          cout << "\n gathering_RGB...";
          
          for (int cou_X=0;cou_X<BMP.bmWidth;cou_X++)
          {
               for (int cou_Y=0;cou_Y<BMP.bmHeight;cou_Y++)
               {
                    tmp_Color = GetPixel(tmp_DC_This, cou_X, cou_Y);
                    R = GetRValue(tmp_Color);
                    G = GetGValue(tmp_Color);
                    B = GetBValue(tmp_Color);
                    //cout << "\n"; oint(0, 12, R); cout << " "; oint(0, 10, G); cout << " "; oint(0, 9, B);
                    
                    SetPixel(tmp_DC_RGB[0], cou_X, cou_Y, RGB(R, 0, 0));
                    SetPixel(tmp_DC_RGB[1], cou_X, cou_Y, RGB(0, G, 0));
                    SetPixel(tmp_DC_RGB[2], cou_X, cou_Y, RGB(0, 0, B));
                    
                    RGB_Input_Cell[0].set_IMG_Pixel(int (R), cou_X, cou_Y);
                    RGB_Input_Cell[1].set_IMG_Pixel(int (G), cou_X, cou_Y);
                    RGB_Input_Cell[2].set_IMG_Pixel(int (B), cou_X, cou_Y);
               }
          }
          
          cout << "\n Cleanup...";
          
          //Cleanup the DCs.
          SelectObject(tmp_DC_This, tmp_DC_This_Orig);
          DeleteDC(tmp_DC_This);
          
          for (int cou_RGB=0;cou_RGB<3;cou_RGB++)
          {
               SelectObject(tmp_DC_RGB[cou_RGB], tmp_DC_RGB_Orig[cou_RGB]);
               DeleteDC(tmp_DC_RGB[cou_RGB]);
          }
     }
     
     
     //Grayscales the bitmap.
     void grayscale_Bitmap()
     {
          if (BMP_Source == NULL){ return; }
          
          cout << "\n\n separate_Bitmap()->" << this;
          
          //Create the DCs needed for each of the four bitmaps.
          HDC tmp_DC_This = CreateCompatibleDC(GetDC(0));
          HBITMAP tmp_DC_This_Orig = (HBITMAP) SelectObject(tmp_DC_This, BMP_Source);
          
          COLORREF tmp_Color;
          BYTE R = 0;
          BYTE G = 0;
          BYTE B = 0;
          BYTE tmp_Gray = 0;
          
          cout << "\n Setting up...";
          
          cout << "\n gathering_Grayscale...";
          
          for (int cou_X=0;cou_X<BMP.bmWidth;cou_X++)
          {
               for (int cou_Y=0;cou_Y<BMP.bmHeight;cou_Y++)
               {
                    tmp_Color = GetPixel(tmp_DC_This, cou_X, cou_Y);
                    R = GetRValue(tmp_Color);
                    G = GetGValue(tmp_Color);
                    B = GetBValue(tmp_Color);
                    //cout << "\n"; oint(0, 12, R); cout << " "; oint(0, 10, G); cout << " "; oint(0, 9, B);
                    
                    tmp_Gray = (R + G) / 2;
                    tmp_Gray = (tmp_Gray + B) / 2;
                    
                    SetPixel(tmp_DC_This, cou_X, cou_Y, RGB(tmp_Gray, tmp_Gray, tmp_Gray));
               }
          }
          
          cout << "\n Cleanup...";
          
          //Cleanup the DCs.
          SelectObject(tmp_DC_This, tmp_DC_This_Orig);
          DeleteDC(tmp_DC_This);
     }
     
     void average_2D_Cell_Into_BMP_Source_RGB(c_Raw_Table_Cell_2D p_Cell[3], double p_Diffusion_Modifier = 0.5f)
     {
          
          //Get the current pixel color value.
          
          //Get the current output color value
          
          //Average them.
          
          //Resubmit them to the output image.
          
          cout << "\n\n average_2D_Cell_Into_BMP_Source()->" << this;
          
          //Create the DCs needed for each of the four bitmaps.
          HDC tmp_DC_This = CreateCompatibleDC(GetDC(0));
          HBITMAP tmp_DC_This_Orig = (HBITMAP) SelectObject(tmp_DC_This, BMP_Source);
          
          COLORREF tmp_Color;
          BYTE NR = 0;
          BYTE NG = 0;
          BYTE NB = 0;
          BYTE R = 0;
          BYTE G = 0;
          BYTE B = 0;
          BYTE FR = 0;
          BYTE FG = 0;
          BYTE FB = 0;
          double DR = 0;
          double DG = 0;
          double DB = 0;
          
          cout << "\n Setting up...";
          
          
          cout << "\n gathering_RGB...";
          
          for (int cou_X=0;cou_X<BMP.bmWidth;cou_X++)
          {
               for (int cou_Y=0;cou_Y<BMP.bmHeight;cou_Y++)
               {
                    tmp_Color = GetPixel(tmp_DC_This, cou_X, cou_Y);
                    
                    R = GetRValue(tmp_Color);
                    G = GetGValue(tmp_Color);
                    B = GetBValue(tmp_Color);
                    
                    //       tmp_Color = (tmp_Current_Color - tmp_Get_Pixel_Color) * Diffusion_Modifier;
                    NR = (p_Cell[0].Data[cou_X] [cou_Y]).I;
                    NG = (p_Cell[1].Data[cou_X] [cou_Y]).I;
                    NB = (p_Cell[2].Data[cou_X] [cou_Y]).I;
                    
                    //**cout << "\n\n\n"; oint(0, 12, NR); cout << " "; oint(0, 10, NG); cout << " "; oint(0, 9, NB);
                    //**cout << "\n"; oint(0, 12, R); cout << " "; oint(0, 10, G); cout << " "; oint(0, 9, B);
                    
                    //**cout << "\n  R += BYTE (double (double (" << R << ") - double (" << NR << ")) * " << p_Diffusion_Modifier << ")";
                    //**cout << "\n  R += BYTE (" << (double (double ( R ) - double ( NR ))) << " * " << p_Diffusion_Modifier << ")";
                    DR = (double (double (NR) - double (R)));
                    //cout << "         " << (R - NR) << "  d " << (double (R) - double (NR));
                    //cout << "         " << (NR - R) << "  d " << (double (NR) - double (R));
                    DG = (double (double (NG) - double (G)));
                    DB = (double (double (NB) - double (B)));
                    
                    FR = (((1 - (absoluted(DR) / 255)) * DR) * p_Diffusion_Modifier) + R;
                    FG = (((1 - (absoluted(DG) / 255)) * DG) * p_Diffusion_Modifier) + G;
                    FB = (((1 - (absoluted(DB) / 255)) * DB) * p_Diffusion_Modifier) + B;
                    
                    //cout << "\n\n\n"; oint(0, 12, R); cout << " "; oint(0, 10, G); cout << " "; oint(0, 9, B);
                    //cout << "\nN "; oint(0, 12, NR); cout << " "; oint(0, 10, NG); cout << " "; oint(0, 9, NB);
                    //cout << "\nD "; odbl(0, 12, DR); cout << " "; odbl(0, 10, DG); cout << " "; odbl(0, 9, DB);
                    //cout << "\nF "; oint(0, 12, FR); cout << " "; oint(0, 10, FG); cout << " "; oint(0, 9, FB);
                    
                    
                    SetPixel(tmp_DC_This, cou_X, cou_Y, RGB(FR, FG, FB));
               }
          }
          
          cout << "\n Cleanup...";
          
          //Cleanup the DCs.
          SelectObject(tmp_DC_This, tmp_DC_This_Orig);
          DeleteDC(tmp_DC_This);
     }
     
     
     void average_2D_Cell_Into_BMP_Source_Grayscale(c_Raw_Table_Cell_2D * p_Cell, double p_Diffusion_Modifier = 0.5f)
     {
          
          //Get the current pixel color value.
          
          //Get the current output color value
          
          //Average them.
          
          //Resubmit them to the output image.
          
          cout << "\n\n average_2D_Cell_Into_BMP_Source_Grayscale()->" << this;
          
          //Create the DCs needed for each of the four bitmaps.
          HDC tmp_DC_This = CreateCompatibleDC(GetDC(0));
          HBITMAP tmp_DC_This_Orig = (HBITMAP) SelectObject(tmp_DC_This, BMP_Source);
          
          COLORREF tmp_Color;
          BYTE NGr = 0;
          BYTE Gr = 0;
          BYTE FGr = 0;
          double DGr = 0;
          
          cout << "\n Setting up...";
          
          
          cout << "\n gathering_Gr...";
          
          for (int cou_X=0;cou_X<BMP.bmWidth;cou_X++)
          {
               for (int cou_Y=0;cou_Y<BMP.bmHeight;cou_Y++)
               {
                    tmp_Color = GetPixel(tmp_DC_This, cou_X, cou_Y);
                    
                    Gr = GetGValue(tmp_Color);
                    
                    //       tmp_Color = (tmp_Current_Color - tmp_Get_Pixel_Color) * Diffusion_Modifier;
                    NGr = (p_Cell[1].Data[cou_X] [cou_Y]).I;
                    
                    //**cout << "\n\n\n"; oint(0, 12, NR); cout << " "; oint(0, 10, NG); cout << " "; oint(0, 9, NB);
                    //**cout << "\n"; oint(0, 12, R); cout << " "; oint(0, 10, G); cout << " "; oint(0, 9, B);
                    
                    //**cout << "\n  R += BYTE (double (double (" << R << ") - double (" << NR << ")) * " << p_Diffusion_Modifier << ")";
                    //**cout << "\n  R += BYTE (" << (double (double ( R ) - double ( NR ))) << " * " << p_Diffusion_Modifier << ")";
                    
                    //cout << "         " << (R - NR) << "  d " << (double (R) - double (NR));
                    //cout << "         " << (NR - R) << "  d " << (double (NR) - double (R));
                    DGr = (double (double (NGr) - double (Gr)));
                    
                    FGr = (((1 - (absoluted(DGr) / 255)) * DGr) * p_Diffusion_Modifier) + Gr;
                    
                    //cout << "\n\n\n"; oint(0, 12, R); cout << " "; oint(0, 10, G); cout << " "; oint(0, 9, B);
                    //cout << "\nN "; oint(0, 12, NR); cout << " "; oint(0, 10, NG); cout << " "; oint(0, 9, NB);
                    //cout << "\nD "; odbl(0, 12, DR); cout << " "; odbl(0, 10, DG); cout << " "; odbl(0, 9, DB);
                    //cout << "\nF "; oint(0, 12, FR); cout << " "; oint(0, 10, FG); cout << " "; oint(0, 9, FB);
                    
                    
                    SetPixel(tmp_DC_This, cou_X, cou_Y, RGB(FGr, FGr, FGr));
               }
          }
          
          cout << "\n Cleanup...";
          
          //Cleanup the DCs.
          SelectObject(tmp_DC_This, tmp_DC_This_Orig);
          DeleteDC(tmp_DC_This);
     }
     
     /*
     
          for (int cou_RGB=0;cou_RGB<3;cou_RGB++)
          {
               //RGB_BMP[cou_RGB];
               RGB_BMP_Source[cou_RGB] = NULL;
          }
           * */
     
     
     //void load_Network()
     /*
          
          
     */

     //void save_Network()
     /*
          
          
     */

     //void move_Input_Image_To_Network_Input()
     /*
          
          
     */

     //load_Input_Image()
     /*
          load_Image();
          move_Input_Image_To_Network_Input();
          
     */

     //get_Output_Image()
     /*
          
          
     */
};








