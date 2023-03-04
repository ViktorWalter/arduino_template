#include <Arduino.h>

#define ROW_COUNT 8 //X
#define COLUMN_COUNT 8 //Y
#define LAYER_COUNT 8 //Z

#define CELL_COUNT 4 //CELLS

/* #define lowbit_row 0 //pin of the lowest bit */
/* #define lowbit_column 0 //pin of the lowest bit */
/* #define lowbit_layer 0 //pin of the lowest bit */

/* #define split_demux true */
#define split_demux false

#define OUTPUT_CONFIRM_PIN 8
#define ADDRESS_RESET_PIN 13

#define CELL_PIN_0 9
#define CELL_PIN_1 10
#define CELL_PIN_2 11
#define CELL_PIN_3 12

#define DATA_BUS PORTD
/* #define split_demux false */

/* int LED1 = 2; */

/* unsigned char slider_row_start = 1; */
/* unsigned char slider_row = 1; */

/* unsigned char slider_column_start = 1; */
/* unsigned char slider_column = 1; */

/* unsigned char slider_layer_start = 1; */
/* unsigned char slider_layer = 1; */

typedef unsigned char matrix3D[LAYER_COUNT][COLUMN_COUNT];

/* matrix3D MSG = {{B10100101}, {B00110110}}; */
/* matrix3D buffer = {{B00000000}, {B00000000}}; */
matrix3D MSG;
volatile matrix3D buffer;
/* matrix3D MSG = {B10100101, B00110110}; */
/* matrix3D buffer = {{B00000000}, {B00000000}}; */
#define SIN_BUFFER_SIZE 16
int sinbuffer[SIN_BUFFER_SIZE];

unsigned int step = 0;
unsigned int offset = 0;
/* unsigned int move_speed = 500; */
/* unsigned long move_speed = 100; */
/* unsigned long move_speed = 10; */
/* unsigned int move_speed = 0; */
unsigned long move_speed = 1;
/* unsigned long move_speed = 5; */

/* int potenciometer_value; */

void reset_address_counter(){
  digitalWrite(ADDRESS_RESET_PIN,false);
  digitalWrite(ADDRESS_RESET_PIN,true);
}

void setup() {
  cli();                      //stop interrupts for till we make the settings */

  reset_address_counter();

  for (int x =0; x<SIN_BUFFER_SIZE; x++){
    sinbuffer[x] = round(1.5*(sin(2.0*M_PI*((float)x/(float)SIN_BUFFER_SIZE)))+1.5);
  }

  pinMode(OUTPUT_CONFIRM_PIN,OUTPUT);
  pinMode(ADDRESS_RESET_PIN,OUTPUT);

  /* DDRD = B11111111; */
  if (split_demux){
    DDRD = B11111111;
  }
  else {
    DDRD = (((unsigned char)1)<<ROW_COUNT)-1;
  }

  DDRB = B00000011;

  DDRC = B00001111;

  /* slider_row_start = slider_row_start << lowbit_row; */
  /* slider_row = slider_row << lowbit_row; */

  /* slider_column_start = slider_column_start << lowbit_column; */
  /* slider_column= slider_column<< lowbit_column; */
  
  /* slider_layer_start  = slider_layer_start  << lowbit_layer; */
  /* slider_layer = slider_layer << lowbit_layer; */
  

  for (int i=0; i<LAYER_COUNT; i++){
    for (int j=0; j<COLUMN_COUNT; j++){
      MSG[i][j]    = B00000000;
      buffer[i][j] = B00000000;
    }
  }
  //Initial setup
  /*1. First we reset the control register to amke sure we start with everything disabled.*/
  TCCR1A = 0;                 // Reset entire TCCR1A to 0 
  TCCR1B = 0;                 // Reset entire TCCR1B to 0
 
  /*2. We set the prescalar to the desired value by changing the CS10 CS12 and CS12 bits. */  
  TCCR1B |= B00000100;        //Set CS12 to 1 so we get prescaler 256  
  
  /*3. We enable compare match mode on register A*/
  TIMSK1 |= B00000010;        //Set OCIE1A to 1 so we enable compare match A 
  
  //16MHz / 256 = 62.5 kHz
  //0.1s * 62.5 kHz = 6250 pulses
  OCR1A = 6250;             //Finally we set compare register A to this value  

  /* pinMode(pwm_pin, OUTPUT); */

  sei();
}

unsigned char rotate(unsigned char input,int len){
  return (input << 1)|(input >> (len-1));
}


void display_values(){
  /* slider_layer = slider_layer_start; */
  unsigned long rand_offset = random(4);
  /* unsigned long rand_offset = 0; */
  for (unsigned char k=0; k<LAYER_COUNT; k++){
  /* for (unsigned char k=0; k<8; k++){ */
    /* unsigned long layer_active = ((k+rand_offset)%LAYER_COUNT); //To equalize the apparent increased brigthness on latest LAYER_COUNT due to synchronicity with buffer rewriting. */
    /* unsigned char layer_active = ((k+rand_offset)%8); //To equalize the apparent increased brigthness on latest LAYER_COUNT due to synchronicity with buffer rewriting. */
    /* for (unsigned char k=0; k<8; k++){ */
    /* slider_column = slider_column_start; */
    /* PORTC = k; */
    /* PORTB = 0; */
    /* PORTC = B00000001 << layer_active; */
    unsigned long layer_active = (k); //To equalize the apparent increased brigthness on latest LAYER_COUNT due to synchronicity with buffer rewriting.
    PORTC = layer_active;
    int skip_column = (split_demux?2:1);
    for (unsigned char j=0; j<(COLUMN_COUNT); j += (skip_column)){
      /* PORTB = ~slider_column; */
      /* PORTB = B11111111; */
      /* slider_row = slider_row_start; */
      /* for (int i=0; i<ROW_COUNT; i++){ */
        /* PORTD = slider_row & input[j]; */
        /* delay(0.5); */

      unsigned char subbuffer_curr;
      if (split_demux){
        subbuffer_curr = buffer[layer_active][j] | buffer[layer_active][j+1] << 4;
      }
      else {
        subbuffer_curr = buffer[layer_active][j];
      }
        
      if (j >= 2) {
        PORTD = ~subbuffer_curr;
      }
      else{
        PORTD = subbuffer_curr;
      }
      /* PORTD = input[k][j]; */
      PORTB = j;
      PORTB = j+1;
        /* delay(1.0); */
        /* slider_row = rotate(slider_row,ROW_COUNT); */
      /* } */
      /* slider_column = rotate(slider_column,COLUMN_COUNT); */
        /* PORTB = j+1; */
    /* delay(1.0); */
    }
    /* PORTB = 0; */
    /* delay(1.0); */
    delay(1.0);
    /* PORTC = B00000001 << k; */
    /* delay(1.0); */
    /* PORTD = B00000000; */
    // The RMS value of PWM is its amplitude times the square root of its duty cycle.
    // Each layer has duty cycle of 1/8 by design
    // My LEDs have peak forward current of 100mA, specified at 1/10 duty cycle with 0.1 pulse width
    // This means that it is at T=10*0.1=1ms.

    /* slider_layer = rotate(slider_layer,LAYER_COUNT); */
  }
}

void shift_message(matrix3D input,int offset){
  for (int i=0; i<LAYER_COUNT; i++){
    for (int j=0; j<COLUMN_COUNT; j++){
      input[i][j] = rotate(input[i][j],ROW_COUNT);
    }
  }
}

void set_full(matrix3D input, bool state){
  for (int i=0; i<LAYER_COUNT; i++){
    for (int j=0; j<COLUMN_COUNT; j++){
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
  unsigned char layer = (offset / (ROW_COUNT*COLUMN_COUNT)) % LAYER_COUNT;
  unsigned char column = (offset % (ROW_COUNT*COLUMN_COUNT)) / ROW_COUNT;
  unsigned char row = (offset % (ROW_COUNT*COLUMN_COUNT)) % ROW_COUNT;

  if ((offset % (ROW_COUNT*COLUMN_COUNT*LAYER_COUNT)) == 0) {
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

bool sin_fast(unsigned long offset, unsigned int i, unsigned int j, unsigned int k){
  return i == sinbuffer[((offset+j+k)>>-2)%SIN_BUFFER_SIZE];


  /* round((LAYER_COUNT-1)* */ 
  /*     ((sin(((j+offset)/2.0))+1.0)/2.0)* */
  /*     ((sin(((k+offset)/2.0))+1.0)/2.0) */
  /*                ) */
}

void make_sine(matrix3D input, unsigned long offset){
  /* set_full(input,false); */
  for (unsigned int i=0; i<LAYER_COUNT; i++){
    for (unsigned int j=0; j<COLUMN_COUNT; j++){
      for (unsigned int k=0; k<ROW_COUNT; k++){

        /* bool is_sin_surface = false; */
        /* is_sin_surface = (i == round((LAYER_COUNT-1)*((sin(((j+offset)/2.0))+1.0)/2.0))); */
        /* is_sin_surface = (i == round((LAYER_COUNT-1)*((sin(((j+offset)/2.0))+1.0)/2.0))); */
        /* set_led(input, k,j,i,(i == round((LAYER_COUNT-1)* */
        /*         ((sin(((j+offset)/2.0))+1.0)/2.0)* */
        /*         ((sin(((k+offset)/2.0))+1.0)/2.0) */
        /*         /1* (sin(((j+offset)*3.14)/6.0)/2.0+0.5) *1/ */
        /*         ) */
        /*       ) */
        /*     ); */
        set_led(input, k,j,i,sin_fast(offset, i,j,k));

      }
    }
  }
}




void make_edges(matrix3D input, unsigned long offset){
  /* set_full(input,false); */
  for (int i=0; i<LAYER_COUNT; i++){
    for (int j=0; j<COLUMN_COUNT; j++){
      for (int k=0; k<ROW_COUNT; k++){

        /* unsigned char is_edge = ( */
        /*     ((i == 0) || (i == LAYER_COUNT-1)) + */
        /*     ((j == 0) || (j == COLUMN_COUNT-1)) + */
        /*     ((k == 0) || (k == ROW_COUNT-1)) */
        /*     ); */
        unsigned char is_edge = (
            ((i == 0) || (i == 3)) +
            ((j == 0) || (j == 3)) +
            ((k == 0) || (k == 3))
            );
        set_led(input, k,j,i,(is_edge >=2));
      }
    }
  }
}

void flip_bits(matrix3D input){
  /* set_full(input,false); */
  for (int i=0; i<LAYER_COUNT; i++){
    for (int j=0; j<COLUMN_COUNT; j++){
      for (int k=0; k<ROW_COUNT; k++){
        flip_led(input, k,j,i);
      }
    }
  }
}

void copy_array(const matrix3D input, matrix3D output){
  for (int i=0; i<LAYER_COUNT; i++){
    for (int j=0; j<COLUMN_COUNT; j++){
      output[i][j] = input[i][j];
    }
  }
}

void copy_array_to_buffer(const matrix3D input){
  for (int i=0; i<LAYER_COUNT; i++){
    for (int j=0; j<COLUMN_COUNT; j++){
      if (buffer[i][j] != input[i][j])
        buffer[i][j] = input[i][j];
    }
  }
}

void make_contents(unsigned int offset){
  make_sine(MSG,offset);
  /* make_edges(MSG,offset); */
    /* set_full(MSG,offset%2); */
    /* set_full(MSG,true); */
    /* set_full(MSG,false); */
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

void select_cell(unsigned char cell){
  digitalWrite(CELL_PIN_0,cell^B00000001);
  digitalWrite(CELL_PIN_1,cell^B00000010);
  digitalWrite(CELL_PIN_2,cell^B00000100);
  digitalWrite(CELL_PIN_3,cell^B00001000);
}

void write_signal(){
  digitalWrite(OUTPUT_CONFIRM_PIN,true);
  digitalWrite(OUTPUT_CONFIRM_PIN,false);
}

int write_octet(unsigned char data, unsigned char cell){
  if ((cell < 0) || (cell >= CELL_COUNT)){
    return -1;
  }

  select_cell(cell);

  DATA_BUS = data;

  write_signal();

  return 0;
}

int write_buffer_EM_cell(unsigned char cell) { // external memory cell 

  reset_address_counter();
  if ((cell < 0) || (cell >= CELL_COUNT)){
    return -1;
  }

  for (int y=0; y<LAYER_COUNT; y++){
    for (int x=0; x<COLUMN_COUNT; x++){
      write_octet(buffer[y][x], cell);
      delay(0.01);
    }
  }

  return 0;

} 

void loop() {

  /* if (move_speed > 0){ */
  /* if ((millis()/move_speed) != step){ */ /*   if (true){ */
  /*   step = millis()/move_speed; */
  /*   /1* int offset = step % ROW_COUNT; *1/ */
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

  /* unsigned int offset_new = step / move_speed; */
  //Main loop
  /* display_values(); */
  write_buffer_EM_cell(0);
  delay(10);
  
  /* potenciometer_value = analogRead(A5); */
  /* analogWrite(pwm_pin,potenciometer_value/4); */

}
