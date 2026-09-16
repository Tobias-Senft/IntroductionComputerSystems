//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): ./main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

void grayScale(unsigned char input_image_array[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char convertetImage[BMP_WIDTH][BMP_HEIGTH]);
void save_2D_To_3D(unsigned char image[BMP_WIDTH][BMP_HEIGTH] );
void binary_px(unsigned char image[BMP_WIDTH][BMP_HEIGTH], int threshold);
void Erosion(unsigned char image[BMP_WIDTH][BMP_HEIGTH]);


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


  

  //Load image from file
  read_bitmap(argv[1], input_image);

  //Run inversion
  invert(input_image,output_image);

  //Save image to file
  write_bitmap(output_image, argv[2]);
  
  grayScale(input_image, convertetImage);
  printf("Hej");
  binary_px(convertetImage,90);
  Erosion(convertetImage);
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
  printf("2D\n");
  unsigned char returnImage3D[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  for (int c = 0; c < BMP_CHANNELS; c++) 
    for (int x = 0; x < BMP_WIDTH; x++)
      for (int y = 0; y < BMP_HEIGTH; y++)
      returnImage3D[x][y][c] = image[x][y];
  write_bitmap(returnImage3D,"Outputtet.bmp");

}
void binary_px(unsigned char image[BMP_WIDTH][BMP_HEIGTH], int threshold){
  printf("Binary Px\n");
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

void Erosion(unsigned char image[BMP_WIDTH][BMP_HEIGTH]){
  printf("Erosion\n");
  for(int x = 0; x < BMP_WIDTH; x++){
    for (int y = 0; y < BMP_HEIGTH; y++){
      if(image[x][y] == 255){
        if (&image[x+1][y] == NULL || &image[x+1][y] == 0){
          image[x][y] = 0;
        }
        else if (&image[x-1][y] == NULL || &image[x-1][y] == 0)
        {
          image[x][y] = 0;
        }
        else if (&image[x][y+1] == NULL || &image[x][y+1] == 0)
        {
          image[x][y] = 0;
        }
        else if (&image[x][y-1] == NULL || &image[x][y-1] == 0)
        {
          image[x][y] = 0;
        }
        // else{

        // }
      }
    }
  }
}
