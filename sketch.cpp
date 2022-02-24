#include <Arduino.h>

#define rows 4 //X
#define columns 4 //Y
#define layers 4 //Z

#define lowbit_row 0 //pin of the lowest bit
#define lowbit_column 0 //pin of the lowest bit
#define lowbit_layer 0 //pin of the lowest bit

/* int LED1 = 2; */

/* unsigned char slider_row_start = 1; */
/* unsigned char slider_row = 1; */

/* unsigned char slider_column_start = 1; */
/* unsigned char slider_column = 1; */

/* unsigned char slider_layer_start = 1; */
/* unsigned char slider_layer = 1; */

typedef unsigned char matrix3D[layers][columns];

/* matrix3D MSG = {{B10100101}, {B00110110}}; */
/* matrix3D buffer = {{B00000000}, {B00000000}}; */
matrix3D MSG = {{B00001010, B00000011}, {B00001110, B00001001}};
matrix3D buffer = {{B00000000, B00000000}, {B00000000, B00000000}};
/* matrix3D MSG = {B10100101, B00110110}; */
/* matrix3D buffer = {{B00000000}, {B00000000}}; */

unsigned int step = 0;
/* unsigned int move_speed = 500; */
/* unsigned long move_speed = 100; */
/* unsigned long move_speed = 10; */
unsigned int move_speed = 0;

void setup() {
  /* pinMode(LED1,OUTPUT); */

  /* DDRD = B11111111; */
  DDRD = (((unsigned char)1)<<rows)-1;

  DDRB = B00000011;

  DDRC = B00001111;

  /* slider_row_start = slider_row_start << lowbit_row; */
  /* slider_row = slider_row << lowbit_row; */

  /* slider_column_start = slider_column_start << lowbit_column; */
  /* slider_column= slider_column<< lowbit_column; */
  
  /* slider_layer_start  = slider_layer_start  << lowbit_layer; */
  /* slider_layer = slider_layer << lowbit_layer; */
  

  for (int i=0; i<layers; i++){
    for (int j=0; j<columns; j++){
      buffer[i][j] = MSG[i][j];
    }
  }
  //Initial setup
}

unsigned char rotate(unsigned char input,int len){
  return (input << 1)|(input >> (len-1));
}


void display_values(matrix3D input){
  /* slider_layer = slider_layer_start; */
  for (unsigned char k=0; k<layers; k++){
    /* slider_column = slider_column_start; */
    /* PORTC = k; */
    PORTC = B00000001 << k;
    /* PORTB = 0; */
    for (unsigned char j=0; j<columns; j++){
      /* PORTB = ~slider_column; */
      /* PORTB = B11111111; */
      /* slider_row = slider_row_start; */
      /* for (int i=0; i<rows; i++){ */
        /* PORTD = slider_row & input[j]; */
        PORTB = j;
        /* delay(1); */
        PORTD = input[k][j];
        /* delay(1); */
        PORTB = j+1;
        /* slider_row = rotate(slider_row,rows); */
      /* } */
      /* slider_column = rotate(slider_column,columns); */
        /* PORTB = j+1; */
    /* delay(1.0); */
    }
    /* PORTB = 0; */
    delay(1.0);
    /* PORTD = B00000000; */
    // The RMS value of PWM is its amplitude times the square root of its duty cycle.
    // Each layer has duty cycle of 1/8 by design
    // My LEDs have peak forward current of 100mA, specified at 1/10 duty cycle with 0.1 pulse width
    // This means that it is at T=10*0.1=1ms.

    /* slider_layer = rotate(slider_layer,layers); */
  }
}

void shift_message(matrix3D input,int offset){
  for (int i=0; i<layers; i++){
    for (int j=0; j<columns; j++){
      input[i][j] = rotate(input[i][j],rows);
    }
  }
}

void set_full(matrix3D input, bool state){
  for (int i=0; i<layers; i++){
    for (int j=0; j<columns; j++){
      if (state){
      input[i][j] = B11111111;
      }
      else {
      input[i][j] = B00000000;
      }
    }
  }
}

void shift_single(matrix3D input,unsigned long offset){
  unsigned char layer = (offset / (rows*columns)) % layers;
  unsigned char column = (offset % (rows*columns)) / rows;
  unsigned char row = (offset % (rows*columns)) % rows;

  set_full(input,false);
  /* set_full(input,true); */

  input[layer][column] = (B00000001 << row);
  /* input[layer][column] = (B01110111 >> row); */
  /* input[layer][column] = (B00001000 >> row); */
}

void make_sine(matrix3D input){
  set_full(input,false);
  for (int i=0; i<layers; i++){
    for (int j=0; j<columns; j++){
      bool is_sin_surface = false;
      is_sin_surface = (i == round((layers-1)*((sin(((j+1.0)/2.0)*(2.0*8.0))+1.0)/2.0)));
      if (is_sin_surface){
      input[i][j] = B11111111;
      }
      else {
      input[i][j] = B00000000;
      }
    }
  }
}

void loop() {

  if (move_speed > 0){
    /* if ((millis()/move_speed) != step){ */
    if (true){
    step = millis()/move_speed;
    /* int offset = step % rows; */
    unsigned long offset = step;
    /* shift_message(buffer,offset); */
    shift_single(buffer,offset);
    }
  }
  /* set_full(buffer,true); */

  make_sine(buffer);

  display_values(buffer);
  //Main loop
}
