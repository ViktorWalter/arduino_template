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
matrix3D MSG;
volatile matrix3D buffer;
/* matrix3D MSG = {B10100101, B00110110}; */
/* matrix3D buffer = {{B00000000}, {B00000000}}; */

unsigned int step = 0;
unsigned int offset = 0;
/* unsigned int move_speed = 500; */
/* unsigned long move_speed = 100; */
/* unsigned long move_speed = 10; */
/* unsigned int move_speed = 0; */
unsigned long move_speed = 1;

void setup() {
  cli();                      //stop interrupts for till we make the settings */
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
      MSG[i][j]    = B00000000;
      buffer[i][j] = B00000000;
    }
  }
  //Initial setup
  /*1. First we reset the control register to amke sure we start with everything disabled.*/
  TCCR1A = 0;                 // Reset entire TCCR1A to 0 
  TCCR1B = 0;                 // Reset entire TCCR1B to 0
 
  /*2. We set the prescalar to the desired value by changing the CS10 CS12 and CS12 bits. */  
  TCCR1B |= B00000100;        //Set CS12 to 1 so we get prescalar 256  
  
  /*3. We enable compare match mode on register A*/
  TIMSK1 |= B00000010;        //Set OCIE1A to 1 so we enable compare match A 
  
  //16MHz / 256 = 62.5 kHz
  //0.1s * 62.5 kHz = 6250 pulses
  OCR1A = 6250;             //Finally we set compare register A to this value  
  sei();
}

unsigned char rotate(unsigned char input,int len){
  return (input << 1)|(input >> (len-1));
}


void display_values(){
  /* slider_layer = slider_layer_start; */
  unsigned long rand_offset = random(4);
  /* unsigned long rand_offset = 0; */
  for (unsigned char k=0; k<layers; k++){
    unsigned long layer_active = ((k+rand_offset)%layers); //To equalize the apparent increased brigthness on latest layers due to synchronicity with buffer rewriting.
    /* for (unsigned char k=0; k<8; k++){ */
    /* slider_column = slider_column_start; */
    /* PORTC = k; */
    /* PORTB = 0; */
    for (unsigned char j=0; j<columns; j++){
      /* PORTB = ~slider_column; */
      /* PORTB = B11111111; */
      /* slider_row = slider_row_start; */
      /* for (int i=0; i<rows; i++){ */
        /* PORTD = slider_row & input[j]; */
        /* delay(0.5); */
        PORTD = buffer[layer_active][j];
        /* PORTD = input[k][j]; */
        PORTB = j;
        PORTB = j+1;
        /* delay(1.0); */
        /* slider_row = rotate(slider_row,rows); */
      /* } */
      /* slider_column = rotate(slider_column,columns); */
        /* PORTB = j+1; */
    /* delay(1.0); */
    }
    /* PORTB = 0; */
    PORTC = B00000001 << layer_active;
    /* PORTC = B00000001 << k; */
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

void set_led(matrix3D input, unsigned char row, unsigned char column, unsigned char layer, bool state){
  if (state)
    input[layer][column] |= (B00000001 << row);
  else
    input[layer][column] &= (~(B00000001 << row));
}

bool get_led(matrix3D input, unsigned char row, unsigned char column, unsigned char layer){
  return input[layer][column] & (B00000001 << row);
}

void flip_led(matrix3D input, unsigned char row, unsigned char column, unsigned char layer){
  set_led(input, row, column, layer, !get_led(input,row,column,layer));
}

void shift_single(matrix3D input,unsigned long offset){
  unsigned char layer = (offset / (rows*columns)) % layers;
  unsigned char column = (offset % (rows*columns)) / rows;
  unsigned char row = (offset % (rows*columns)) % rows;

  if ((offset % (rows*columns*layers)) == 0) {
  set_full(input,false);
  }
  /* set_full(input,true); */

  /* input[layer][column] = (B00000001 << row); */
  if ((layer % 2) == 0){
    set_led(input,row,column,layer,true);
  }
  else{
    set_led(input,column,row,layer,true);
  }

  /* input[layer][column] = (B00000001 << row); */
  /* input[layer][column] = (B01110111 >> row); */
  /* input[layer][column] = (B00001000 >> row); */
}

void make_sine(matrix3D input, unsigned long offset){
  /* set_full(input,false); */
  for (unsigned int i=0; i<layers; i++){
    for (unsigned int j=0; j<columns; j++){
      for (unsigned int k=0; k<rows; k++){

        /* bool is_sin_surface = false; */
        /* is_sin_surface = (i == round((layers-1)*((sin(((j+offset)/2.0))+1.0)/2.0))); */
        /* is_sin_surface = (i == round((layers-1)*((sin(((j+offset)/2.0))+1.0)/2.0))); */
        set_led(input, k,j,i,(i == round((layers-1)*
                ((sin(((j+offset)/2.0))+1.0)/2.0)*
                ((sin(((k+offset)/2.0))+1.0)/2.0)
                /* (sin(((j+offset)*3.14)/6.0)/2.0+0.5) */
                )
              )
            );
      }
    }
  }
}

void make_edges(matrix3D input, unsigned long offset){
  /* set_full(input,false); */
  for (int i=0; i<layers; i++){
    for (int j=0; j<columns; j++){
      for (int k=0; k<rows; k++){

        unsigned char is_edge = (
            ((i == 0) || (i == layers-1)) +
            ((j == 0) || (j == columns-1)) +
            ((k == 0) || (k == rows-1))
            );
        set_led(input, k,j,i,(is_edge >=2));
      }
    }
  }
}

void flip_bits(matrix3D input){
  /* set_full(input,false); */
  for (int i=0; i<layers; i++){
    for (int j=0; j<columns; j++){
      for (int k=0; k<rows; k++){
        flip_led(input, k,j,i);
      }
    }
  }
}

void copy_array(const matrix3D input, matrix3D output){
  for (int i=0; i<layers; i++){
    for (int j=0; j<columns; j++){
      output[i][j] = input[i][j];
    }
  }
}

void copy_array_to_buffer(const matrix3D input){
  for (int i=0; i<layers; i++){
    for (int j=0; j<columns; j++){
      if (buffer[i][j] != input[i][j])
        buffer[i][j] = input[i][j];
    }
  }
}

void make_contents(unsigned int offset){
  /* make_sine(MSG,offset); */
    /* set_full(MSG,offset%2); */
    set_full(MSG,true);
}

ISR(TIMER1_COMPA_vect){
  TCNT1  = 0;                  //First, set the timer back to 0 so it resets for next interrupt
  unsigned int offset_new = step / move_speed;
  if (offset_new != offset){
    make_contents(offset_new);
    copy_array_to_buffer(MSG);
  }
  offset = offset_new;
  step++;
}


void loop() {

  /* if (move_speed > 0){ */
  /* if ((millis()/move_speed) != step){ */
  /*   if (true){ */
  /*   step = millis()/move_speed; */
  /*   /1* int offset = step % rows; *1/ */
  /*   unsigned long offset = step; */
  /*   /1* shift_message(buffer,offset); *1/ */
  /*   /1* shift_single(MSG,offset); *1/ */
  /*   /1* copy_array(MSG,buffer); *1/ */
  /*   /1* flip_bits(buffer); *1/ */
  /*   /1* make_sine(buffer,offset); *1/ */
  /*   /1* make_edges(buffer,offset); *1/ */
  /*   /1* if ((step % 2) == 0) *1/ */
  /*   } */
  /* } */
  /* set_full(buffer,true); */


  display_values();
  //Main loop
}
