//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): ./main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <string.h>

void grayScale(unsigned char input_image_array[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char convertetImage[BMP_WIDTH][BMP_HEIGTH]);
void save_2D_To_3D(unsigned char image[BMP_WIDTH][BMP_HEIGTH] );
void binary_px(unsigned char image[BMP_WIDTH][BMP_HEIGTH], int threshold);
void Erosion(unsigned char image[BMP_WIDTH][BMP_HEIGTH], int* count);
int cellDetection(unsigned char image[BMP_WIDTH][BMP_HEIGTH], int x_current, int y_current);
int calcNewValue(int value, int frameSize, int WithOfImage);



//Function to invert pixels of an image (negative)
void invert(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      for (int c = 0; c < BMP_CHANNELS; c++)
      {
      output_image[x][y][c] = 255 - input_image[x][y][c];
      }
    }
  }
}

  //Declaring the array to store the image (unsigned char = unsigned 8 bit)
  unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  unsigned char convertetImage[BMP_WIDTH][BMP_HEIGTH];

//Main function
int main(int argc, char** argv)
{
  //argc counts how may arguments are passed
  //argv[0] is a string with the name of the program
  //argv[1] is the first command line argument (input image)
  //argv[2] is the second command line argument (output image)

  //Checking that 2 arguments are passed
  if (argc != 3)
  {
      fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
      exit(1);
  }

  printf("Example program - 02132 - A1\n");

  int count = 0;
  

  //Load image from file
  read_bitmap(argv[1], input_image);

  //Run inversion
  invert(input_image,output_image);

  //Save image to file
  write_bitmap(output_image, argv[2]);
  
  grayScale(input_image, convertetImage);
  printf("Hej");
  binary_px(convertetImage,127);



  for(int i = 0; i <= 2;i++){
    Erosion(convertetImage,&count);
  }
  printf("\nCount = %d", count);
  save_2D_To_3D(convertetImage);
  
  
  printf("Done!\n");

  return 0;
}


void grayScale(unsigned char input_image_array[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char convertetImage[BMP_WIDTH][BMP_HEIGTH]){
  for(int x= 0; x < BMP_WIDTH;x++){
      for(int y = 0; y < BMP_HEIGTH; y++){
        convertetImage[x][y] = (input_image_array[x][y][0] + input_image_array[x][y][1] + input_image_array[x][y][2])/3;
      }
    }
}

void save_2D_To_3D(unsigned char image[BMP_WIDTH][BMP_HEIGTH] ){
  static unsigned char returnImage3D[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  for (int c = 0; c < BMP_CHANNELS; c++){ 
    for (int x = 0; x < BMP_WIDTH; x++){
      for (int y = 0; y < BMP_HEIGTH; y++){
      returnImage3D[x][y][c] = image[x][y];
      }
    }
  }
  write_bitmap(returnImage3D,"Outputtet.bmp");

}
void binary_px(unsigned char image[BMP_WIDTH][BMP_HEIGTH], int threshold){
  for(int x = 0; x < BMP_WIDTH; x++){
    for (int y = 0; y < BMP_HEIGTH; y++){
      if (image[x][y]<threshold){
        image[x][y] = 0;
        
      }else{
        image[x][y] = 255;
      }
      
      
    }
    
  }
}

void Erosion(unsigned char image[BMP_WIDTH][BMP_HEIGTH], int* count){
  unsigned char copiedImage[BMP_WIDTH][BMP_HEIGTH];
  memcpy(copiedImage,image,sizeof(copiedImage));//gemmer det oprindelige sort hvid billede, så vi beholder det inden erosion.
  for(int y = 0; y < BMP_HEIGTH; y++){
    for (int x = 0; x < BMP_WIDTH; x++){
      if(copiedImage[x][y] == 255){
        (*count) = (*count) + cellDetection(image,x,y);
        if ((x+1)> 949 || copiedImage[x+1][y] == 0){
          image[x][y] = 0;
        }
        else if ((x-1)<0  || copiedImage[x-1][y] == 0)
        {
          image[x][y] = 0;
        }
        else if ((y+1) > 949|| copiedImage[x][y+1] == 0)
        {
          image[x][y] = 0;
        }
        else if ((y-1) < 0 || copiedImage[x][y-1] == 0)
        {
          image[x][y] = 0;
        }
        else{

        }
        
      }
      }
  }
}

int cellDetection(unsigned char image[BMP_WIDTH][BMP_HEIGTH],int x_current, int y_current){
  int frameSize = 14;
  int x_Start_Fame = calcNewValue(x_current,frameSize,BMP_WIDTH);
  int y_Start_Fame = calcNewValue(y_current,frameSize,BMP_HEIGTH);
  int sum = 0;
  for(int loop = 0; loop < frameSize; loop++){  
    sum = sum + image[x_current + loop][y_current];
    sum = sum + image[x_current][y_current+ loop];
    sum = sum + image[x_current + loop][y_current +14];
    sum = sum + image[x_current + 14][y_current +  loop];  
  }
  if (sum == 0){
  //   for(int y = 0; y < frameSize; y++){
  //   for (int x = 0; x < frameSize; x++){
  //     image[x_Start_Fame + x][y_Start_Fame + y] = 0;
  //   }
  // }
    return 1;
  }
  else{
  return 0;
  }  
   
}



int calcNewValue(int value, int frameSize, int WithOfImage){
  int clacWidth = WithOfImage - (frameSize );
  int clacXDefrence = clacWidth - value;
  int valueUsed = 0;
  if(clacXDefrence < 0){
    valueUsed = value + clacXDefrence - 1;
  }
  else{
    valueUsed = value - 1;
  }
  return valueUsed;
}