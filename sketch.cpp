#include <Arduino.h>

#define rows 6 //X
#define columns 2 //Y
#define layers 1 //Z

#define lowbit_row 2 //pin of the lowest bit
#define lowbit_column 0 //pin of the lowest bit
#define lowbit_layer 0 //pin of the lowest bit

/* int LED1 = 2; */

/* unsigned char slider_row_start = 1; */
unsigned char slider_row = 1;

/* unsigned char slider_column_start = 1; */
unsigned char slider_column = 1;

/* unsigned char slider_layer_start = 1; */
unsigned char slider_layer = 1;

unsigned char MSG[] = {B00100000, B00110000};
unsigned char buffer[] = {B00000000, B00000000};

unsigned int step = 0;
unsigned int move_speed = 100;

void setup() {
  /* pinMode(LED1,OUTPUT); */
  DDRD = B11111111;
  DDRB = B11111111;

  /* slider_row_start = slider_row_start << lowbit_row; */
  slider_row = slider_row << lowbit_row;

  /* slider_column_start = slider_column_start << lowbit_column; */
  slider_column= slider_column<< lowbit_column;
  
  /* slider_layer_start  = slider_layer_start  << lowbit_layer; */
  slider_layer = slider_layer << lowbit_layer;
  

  for (int i=0; i<columns; i++){
    buffer[i] = MSG[i];
  }
  //Initial setup
}

unsigned char rotate(unsigned char input,int len){
  return (input << 1)|(input >> (len-1));
}


void display_values(unsigned char* input){
  /* slider_layer = slider_layer_start; */
  for (int k=0; k<layers; k++){
    /* slider_column = slider_column_start; */
    for (int j=0; j<columns; j++){
      PORTB = ~slider_column;
      /* slider_row = slider_row_start; */
      for (int i=0; i<rows; i++){
        PORTD = slider_row & input[j];
        /* delay(1000); */
        slider_row = rotate(slider_row,rows);
      }
      slider_column = rotate(slider_column,rows);
    }
    slider_layer = rotate(slider_layer,rows);
  }
}

void shift_message(unsigned char* input,int offset){
  for (int i=0; i<columns; i++){
    input[i] = rotate(input[i],rows);
  }
}

void loop() {

  if ((millis()/move_speed) != step){
    step = millis()/move_speed;
    int offset = step % rows;
    shift_message(buffer,offset);
  }

  display_values(buffer);
  //Main loop
}
