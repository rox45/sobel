/************************************************************************
 **
 ** SOEBEL Filter
 **
 ** Created: June 14, 2013
 **
 ** Version 1.0 : Anushree
 ** Version 1.1 : BCS
 **
 ***********************************************************************/


/* Parameter declarations */


#ifdef RESHD

  #define BYTES_PER_PIXEL 3

  #define COLS 1920 * BYTES_PER_PIXEL 
  #define ROWS 1080 * BYTES_PER_PIXEL 


  #define IMAGE_IN "batmanjoker.bmp"
  #define IMAGE_OUT "batmanjoker_out.bmp"
  #define IMAGE_OUT_HW "batmanjokerout_hw.bmp"

#else

  #define BYTES_PER_PIXEL 1

   #define COLS 512 * BYTES_PER_PIXEL 
   #define ROWS 512 * BYTES_PER_PIXEL 


   #define IMAGE_IN "lena512.bmp"
   #define IMAGE_OUT "lena512_out.bmp" 
   #define IMAGE_OUT_HW "lena512_out_hw.bmp"


#endif
 

#define SIZE_BUFFER 3


/* Global variables */
unsigned char line_buffer[SIZE_BUFFER][SIZE_BUFFER];


/******************
 **
 ** Soebel filter main function
 **
 ***************/

unsigned char input_row[SIZE_BUFFER];
unsigned char output_row;


void sobel()
{


  unsigned int X, Y;
  int sumX, sumY;
  int SUM, rowOffset, colOffset;

  char Gx[3][3] ={{-1 ,-2 ,-1},
		  { 0, 0, 0},
		  { 1, 2, 1}};

  
  char Gy[3][3] ={{-1, 0, 1},
		  {-2, 0, 2},
		  {-1, 0, 1}};


  /* Shifting line buffer */

  for(Y=SIZE_BUFFER-1;Y>0;Y--)
  {
    for(X=0;X< SIZE_BUFFER;X++)
    {
      line_buffer[Y][X]=line_buffer[Y-1][X];
    }
  }


  for(X=0; X<SIZE_BUFFER; X++)
    line_buffer[0][X] = input_row[X];

 


  Y=1;
  X=1;
  sumX = 0;
  sumY = 0;

  // Convolution starts here
  //-------X GRADIENT APPROXIMATION------
  //-------Y GRADIENT APPROXIMATION------
  for(rowOffset = -1; rowOffset <= 1; rowOffset++)
  {
    for(colOffset = -1; colOffset <=1; colOffset++)
    {
      sumX = sumX + line_buffer[Y -rowOffset][X+colOffset] * Gx[1+rowOffset][1+colOffset];
      sumY = sumY + line_buffer[Y -rowOffset][X+colOffset] * Gy[1+rowOffset][1+colOffset]; 
    }
  }

      if(sumX > 255)    sumX = 255;
      else if(sumX < 0) sumX = 0;

      if(sumY > 255)    sumY = 255;
      else if(sumY < 0) sumY = 0;

      SUM = sumX + sumY;
      
      if(SUM > 255)    SUM = 255;
         else if(SUM < 0) SUM = 0;     
   
      //line_buffer[1][X] = 255  - (unsigned char)(SUM);
      //line_buffer[1][X] = (unsigned char)(SUM);


      /* Write filter result  to output port */
      output_row =  255  - (unsigned char)(SUM);

}



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char outputmem[ROWS][COLS]; //the array before filtering
unsigned char filtermem[ROWS][COLS]; // The array after filtering
unsigned char biColourPalette[1024];

typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef char BYTE;

#pragma pack(push,1)
typedef struct {
  WORD  bfType;                       // The type of the image
 
  DWORD bfSize;                       //size of the file
  WORD  bfReserved;                 // reserved type
  WORD  bfReserved2;               
  DWORD bfOffBits;                   //offset bytes from the file header to the image data
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;                   //the size of the header
  LONG  biWidth;                 //the width in pixels
  LONG  biHeight;                //the height in pixels
  WORD  biPlanes;                //the no. of planes in the bitmap
  WORD  biBitCount;              //bits per pixel
  DWORD biCompression;           //compression specifications
  DWORD biSizeImage;            //size of the bitmap data
  LONG  biXPelsPerMeter;        //horizontal res(pixels per meter)
  LONG  biYPelsPerMeter;        //vertical res(pixels per meter) 
  DWORD biClrUsed;              //colours used in the image
  DWORD biClrImportant;        //num of important colours used
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
#pragma pack(pop)

/***********************
 **
 ** Load BMP file into memory
 **
 **********************/
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader,BITMAPFILEHEADER *bitmapFileHeader)
{

  /* Variables declaration */

   FILE *filePtr;               // out file pointer
   unsigned char *bitmapImage;  // store image data



    //open filename in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL)
        return NULL;
   
    //read the bitmap file header
    fread(bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,filePtr);
    printf("%c bitmap identifies\n",bitmapFileHeader->bfType);
    printf("%d bitmap identifies the size of image\n",bitmapFileHeader->bfSize);
   
    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader->bfType !=0x4D42)
    {
        fclose(filePtr);
        return NULL;
    }

    //read the bitmap info header
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr);

    //read colour palette
    fread(&biColourPalette,1,bitmapInfoHeader->biClrUsed*4,filePtr);

    printf("\n%d the size of info header",bitmapInfoHeader->biSize);
    printf("\n%x the horizontal width",bitmapInfoHeader->biWidth);
    printf("\n%x the vertical height",bitmapInfoHeader->biHeight);
    printf("\n%x num of planes",bitmapInfoHeader->biPlanes);
    printf("\n%d bits per pixel",bitmapInfoHeader->biBitCount);
    printf("\n%d compression specs",bitmapInfoHeader->biCompression);
    printf("\n%d the size of the image",bitmapInfoHeader->biSizeImage);
    printf("\n%x the num of colours used",bitmapInfoHeader->biClrUsed);
    // bitmapInfoHeader->biClrUsed = 64;

    //move file point to the begging of bitmap data
    fseek(filePtr, bitmapFileHeader->bfOffBits, SEEK_SET);

      //allocate enough memory for the bitmap image data
      bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

      //verify memory allocation
      if (!bitmapImage)
	{
	  free(bitmapImage);
	  fclose(filePtr);
	  return NULL;
	}

      //read in the bitmap image data
      fread(bitmapImage,1, bitmapInfoHeader->biSizeImage,filePtr);
   

      //make sure bitmap image data was read
      if (bitmapImage == NULL)
	{
	  printf("Data could not be read");
	  fclose(filePtr);
	  return NULL;
	}
      fclose(filePtr);
      return bitmapImage;
}


/**************************
**************************
 **
 **   MAIN 
 **
 *************************
 ************************/
int main()
{
  int bytesperline;
  FILE *fptr;
  int l,k;
  int i,n,j,x;

  printf("Sobel Filter : %s\n", IMAGE_IN);
  unsigned char *tk;

	
  BITMAPINFOHEADER bitmapInfoHeader;
  BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
  unsigned char *bitmapData;
  unsigned char *bitmapFinalImage;
 
 bitmapFinalImage = (unsigned char*)malloc(ROWS*COLS);
 bitmapData = LoadBitmapFile(IMAGE_IN,&bitmapInfoHeader, &bitmapFileHeader);


 /*
 ** Main Sobel filter function
 ** Filter each line of the image
 */
 for(i=0; i < COLS; i ++){

     for(n=1;n < ROWS ; n++){
       for(x=i-1; x<=i+1;x++){


	 if(x== i-1)
	   input_row[0] = bitmapData[(n*COLS)+i+1*BYTES_PER_PIXEL];
	 else if(x == i)
	   input_row[1] = bitmapData[(n*COLS)+i];	   
	 else
	   input_row[2] = bitmapData[(n*COLS)+i+1*BYTES_PER_PIXEL];
	 
       }
	 /*
	 ** Sobel function
	 */
	 sobel();
     
	 bitmapFinalImage[(n*ROWS)+i] = output_row; 


     }


}



 k=sizeof(BITMAPFILEHEADER);
 l=sizeof(BITMAPINFOHEADER);

 printf("\nDize of file header object and info header object and final image%d %d\n",k,l);

 bytesperline = bitmapInfoHeader.biWidth * (bitmapInfoHeader.biBitCount/8); // multiply by 3 only for 24 bit images
 if( bytesperline & 0x0003)
   {  bytesperline |= 0x0003;
     ++bytesperline;}
 tk = (unsigned char *)calloc(1,bytesperline);
 bitmapFileHeader.bfSize= bitmapInfoHeader.biSize + (long)bytesperline* bitmapInfoHeader.biHeight;

 bitmapFileHeader.bfOffBits = k+l+ 4* bitmapInfoHeader.biClrUsed;
 bitmapFileHeader.bfSize = k+l+bitmapInfoHeader.biSizeImage;

  printf("\nElement %d\n",bitmapFinalImage[1]);
 printf("\nBytes per line and k %d %d\n",bytesperline,k);




 if((fptr=fopen(IMAGE_OUT,"wb"))!=NULL){

   fwrite(&bitmapFileHeader,1,sizeof(BITMAPFILEHEADER),fptr);
   fwrite(&bitmapInfoHeader,1,sizeof(BITMAPINFOHEADER),fptr);
   fwrite(biColourPalette,bitmapInfoHeader.biClrUsed*4,1,fptr);


    printf("\n%d the size of info header",bitmapInfoHeader.biSize);
    printf("\n%x the horizontal width",bitmapInfoHeader.biWidth);
    printf("\n%x the vertical height",bitmapInfoHeader.biHeight);
    printf("\n%x num of planes",bitmapInfoHeader.biPlanes);
    printf("\n%d bits per pixel",bitmapInfoHeader.biBitCount);
    printf("\n%d compression specs",bitmapInfoHeader.biCompression);
    printf("\n%d the size of the image",bitmapInfoHeader.biSizeImage);
    printf("\n%x the num of colours used",bitmapInfoHeader.biClrUsed);
    n=0;
   for(x=0; x<  BYTES_PER_PIXEL ; x++){
    for(i=0; i<= (bitmapInfoHeader.biHeight-1); i++){ 
          for(j=0;j<=bytesperline-1;j++){
	    tk[j] = bitmapFinalImage[n++];

	   }

       fwrite(tk,1,bytesperline,fptr);
    }
   }
 }

 fclose(fptr);

 return 0;
}


