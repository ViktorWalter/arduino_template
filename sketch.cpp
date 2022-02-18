#include <Arduino.h>

#define rows 4 //X
#define columns 2 //Y
#define layers 8 //Z

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
unsigned int move_speed = 500;
/* unsigned int move_speed = 0; */

void setup() {
  /* pinMode(LED1,OUTPUT); */

  /* DDRD = B11111111; */
  DDRD = (((unsigned char)1)<<rows)-1;

  DDRB = B00000111;

  DDRC = B00000001;

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
    PORTC = k;
    /* PORTB = 0; */
    for (unsigned char j=0; j<columns; j++){
      /* PORTB = ~slider_column; */
      /* PORTB = B11111111; */
      /* slider_row = slider_row_start; */
      /* for (int i=0; i<rows; i++){ */
        /* PORTD = slider_row & input[j]; */
        PORTB = j;
        PORTD = input[k][j];
        PORTB = j+1;
        /* slider_row = rotate(slider_row,rows); */
      /* } */
      /* slider_column = rotate(slider_column,columns); */
        /* delay(1); */
        /* PORTB = j+1; */
    }
    /* PORTB = 0; */
    delay(0.1);
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

void loop() {

  if (move_speed > 0){
    if ((millis()/move_speed) != step){
    step = millis()/move_speed;
    int offset = step % rows;
    shift_message(buffer,offset);
    }
  }

  display_values(buffer);
  //Main loop
}
