//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): ./main.exe example.bmp example_inv.bmp

#include "cbmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define MAX_FILES 100
#define MAX_PATH_LEN 260

void grayScale(unsigned char input_image_array[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char convertetImage[BMP_WIDTH][BMP_HEIGTH]);
void save_2D_To_3D(unsigned char image[BMP_WIDTH][BMP_HEIGTH] );
void binary_px(unsigned char image[BMP_WIDTH][BMP_HEIGTH], int threshold);
void Erosion(unsigned char image[BMP_WIDTH][BMP_HEIGTH], int* sum);
// int cellDetection(unsigned char image[BMP_WIDTH][BMP_HEIGTH], int x_current, int y_current);
// int calcNewValue(int value, int frameSize, int WithOfImage);
int celDetectionBasicVersion(unsigned char image[BMP_WIDTH][BMP_HEIGTH],unsigned char cellMap[BMP_WIDTH][BMP_HEIGTH]);
void generateCross(unsigned char crossMap[BMP_WIDTH][BMP_HEIGTH],int x, int y, int frameSize);
void colorCrossMap(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char crossMap[BMP_WIDTH][BMP_HEIGTH]);
void invert(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]);

// Ekstra Functions
void frameFunction(unsigned char image[BMP_WIDTH][BMP_HEIGTH],int x_start, int y_start,int frameSize);
void dynamicThreshHold(unsigned char image[BMP_WIDTH][BMP_HEIGTH]);
void standardMain(int argc, char** argv);
void getBitmaps();


  //Declaring the array to store the image (unsigned char = unsigned 8 bit)
  unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  unsigned char convertetImage[BMP_WIDTH][BMP_HEIGTH];
  unsigned char cellLocations[BMP_WIDTH][BMP_HEIGTH];
  clock_t start,end;
  double cpu_time_used;
  

//Main function
int main(int argc, char** argv){

  standardMain(argc,argv);
  

  return 0;
}
void getBitmaps(){

}


void standardMain(int argc, char** argv){
  start = clock();
    
  for (int i = 0 ;i < BMP_HEIGTH; i++){
    for (int j = 0; j < BMP_WIDTH; j++)  
  {
    cellLocations[i][j]= 0;
  }
  }
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
  int count_Basic = 0;
  

  //Load image from file
  read_bitmap(argv[1], input_image);

  //Run inversion
  invert(input_image,output_image);

  //Save image to file
  write_bitmap(output_image, argv[2]);
  
  grayScale(input_image, convertetImage);
  //binary_px(convertetImage,220);
  dynamicThreshHold(convertetImage);
  int erostionLoop = 1;

  while(erostionLoop){
    printf("Current count %d\n", count_Basic);
    Erosion(convertetImage, &erostionLoop);
    count_Basic = count_Basic + celDetectionBasicVersion(convertetImage,cellLocations);
  }
  printf("\nTotal Count = %d", count_Basic);
  save_2D_To_3D(convertetImage);
  colorCrossMap(input_image,cellLocations);
  int memoryUsage = (sizeof(input_image) + 
                      sizeof(output_image) + 
                      sizeof(convertetImage) + 
                      sizeof(cellLocations));

  printf("\nMemory usage = %d bytes\n", memoryUsage);

  end = clock();
  cpu_time_used = end - start;
  printf("Total time: %f ms\n",cpu_time_used *1000.0 / CLOCKS_PER_SEC);
}


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

void dynamicThreshHold(unsigned char image[BMP_WIDTH][BMP_HEIGTH]){
  int frameSize = 19; // Must be a factor of 950 or BMP_WIDTH and BMP_HEIGHT
    for(int x = 0; x < (BMP_WIDTH/frameSize); x++){
      for (int y = 0; y <  (BMP_HEIGTH/frameSize); y++){
      frameFunction(image,x*frameSize, y*frameSize,frameSize);
    }  
  }
}
void frameFunction(unsigned char image[BMP_WIDTH][BMP_HEIGTH],int x_start, int y_start,int frameSize){
  int avgGrayScaleValue = 0;
  for (int x = 0; x < frameSize; x++){
    for (int y = 0; y < frameSize; y++){
      avgGrayScaleValue = avgGrayScaleValue + image[x_start + x][y_start + y];
    }
  }
  avgGrayScaleValue = avgGrayScaleValue / (frameSize * frameSize);
  int thresholdGrayScaleValue = 0;
  if (avgGrayScaleValue >= 0 && avgGrayScaleValue < 100){
    thresholdGrayScaleValue = avgGrayScaleValue + 70;
  } else if (avgGrayScaleValue >= 100 && avgGrayScaleValue < 120)
  {
    thresholdGrayScaleValue = avgGrayScaleValue + 135;
  }else if (avgGrayScaleValue >= 120 && avgGrayScaleValue < 160)
  {
    thresholdGrayScaleValue = avgGrayScaleValue + 95;
  }
  
  
  else{
    thresholdGrayScaleValue = avgGrayScaleValue;
}
  
  
  // printf("\n%d",avgGrayScaleValue);
  for (int x = 0; x < frameSize; x++){
    for (int y = 0; y < frameSize; y++){
      if(image[x_start + x][y_start + y] < thresholdGrayScaleValue){
        image[x_start + x][y_start + y] = 0;
      }else{
        image[x_start + x][y_start + y] = 255;
      }
    }
  }
}





void Erosion(unsigned char image[BMP_WIDTH][BMP_HEIGTH], int* sum){
  *sum = 0;
  static unsigned char copiedImage[BMP_WIDTH][BMP_HEIGTH];
  memcpy(copiedImage,image,sizeof(copiedImage));//gemmer det oprindelige sort hvid billede, så vi beholder det inden erosion.
  for(int y = 0; y < BMP_HEIGTH; y++){
    for (int x = 0; x < BMP_WIDTH; x++){
      *sum  = *sum + copiedImage[x][y];
      if(copiedImage[x][y] == 255){
        //(*count) = (*count) + cellDetection(image,x,y);
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

int celDetectionBasicVersion(unsigned char image[BMP_WIDTH][BMP_HEIGTH],unsigned char cellMap[BMP_WIDTH][BMP_HEIGTH]){
  int frameSize = 18;
  int sum = 0;
  int count = 0;
  for(int y = 0; y < (BMP_HEIGTH - frameSize); y++){
    for (int x = 0; x < (BMP_WIDTH - frameSize); x++){
      int sum = 0;
      for(int loop = 0; loop < frameSize; loop++){  
        sum = sum + image[x + loop][y];
        sum = sum + image[x][y + loop];
        sum = sum + image[x + loop][y + (frameSize - 1)];
        sum = sum + image[x + (frameSize - 1)][y +  loop];  
      }
      if(sum == 0){
        int whiteExists = 0;
        for(int y_s = 0; y_s < frameSize; y_s++){
          for (int x_s = 0; x_s < frameSize; x_s++){
            whiteExists = whiteExists + image[x+x_s][y+y_s];
          }
        }
        if(whiteExists > 0 ){
        for(int y_i = 1; y_i < (frameSize -1); y_i++){
          for (int x_i = 1 ; x_i < (frameSize-1 ); x_i++){
            image[x + x_i][y + y_i] = 0;
          }
        }
        count++;
        generateCross(cellMap,x,y,frameSize);
      }
      }
    }
  }
  return count;
}

void generateCross(unsigned char crossMap[BMP_WIDTH][BMP_HEIGTH],int x_start, int y_start, int frameSize){
  for(int x = 0; x <= frameSize ; x++){
    for(int y = 0; y <= frameSize ; y++){
      if(x == frameSize/2){
        crossMap[x_start + x][y_start + y] = 1;
      }else if(y == frameSize/2){
        crossMap[x_start + x][y_start + y] = 1;
      }
    }
  }
};

void colorCrossMap(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char crossMap[BMP_WIDTH][BMP_HEIGTH]){
  for (int z = 0; z < BMP_CHANNELS; z++){
    for (int x = 0; x < BMP_WIDTH; x++)
      for (int y = 0; y < BMP_HEIGTH; y++){
        if(crossMap[x][y] == 1){
          if(z == 0){
            image[x][y][z] = 255;

          } else{
            image[x][y][z] = 0;
          }
        }
      }
  }
  write_bitmap(image,"OutputtedCross.bmp");
}





// int cellDetection(unsigned char image[BMP_WIDTH][BMP_HEIGTH],int x_current, int y_current){
//   int frameSize = 14;
//   int x_Start_Fame = calcNewValue(x_current,frameSize,BMP_WIDTH);
//   int y_Start_Fame = calcNewValue(y_current,frameSize,BMP_HEIGTH);
//   int sum = 0;
//   for(int loop = 0; loop < frameSize; loop++){  
//     sum = sum + image[x_Start_Fame + loop][y_Start_Fame];
//     sum = sum + image[x_Start_Fame][y_Start_Fame + loop];
//     sum = sum + image[x_Start_Fame + loop][y_Start_Fame +14];
//     sum = sum + image[x_Start_Fame + 14][y_Start_Fame +  loop];  
//   }
//   if (sum == 0){
//     for(int y = 0; y < frameSize; y++){
//     for (int x = 0; x < frameSize; x++){
//       image[x_Start_Fame + x][y_Start_Fame + y] = 0;
//     }
//   }
//     return 1;
//   }
//   else{
//   return 0;
//   }  
   
// }



// int calcNewValue(int value, int frameSize, int WithOfImage){
//   int clacWidth = WithOfImage - (frameSize );
//   int clacXDefrence = clacWidth - value;
//   int valueUsed = 0;
//   if(clacXDefrence < 0){
//     valueUsed = value + clacXDefrence - 1;
//   }
//   else{
//     valueUsed = value - 1;
//   }
//   return valueUsed;
// }