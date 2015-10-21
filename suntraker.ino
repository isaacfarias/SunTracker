#include <Servo.h>

//Constants
#define SERIAL_BAUD_RATE 9600
#define DELAY_MILLIS 10
#define PIN_MOTOR_X 5
#define PIN_MOTOR_Y 6
#define MAX_MOTOR_VALUE 1024
#define LIMIT_DOWN_Y 100
#define STEP 3
#define TOLERANCE 3

unsigned int panel[2][2];
//motors objects
Servo motor_x;
Servo motor_y;

//actual motor positions
int motor_x_value;
int motor_y_value;

//to get if move
int motor_x_aux;
int motor_y_aux;

//functins prototypes
void print_data();
void read_data();
int get_delta();
int check_if_move_x();

void setup () {
  motor_x_value = 90;
  motor_y_value = 90;
  motor_x.attach(9);
  motor_y.attach(10);
  
  motor_x.write(motor_x_value);
  motor_y.write(motor_y_value);
  
  Serial.begin(SERIAL_BAUD_RATE);
  while(!Serial) {
    
  };
  //Init input
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(3,OUTPUT);
  //Init Panel
  
  panel[0][0] = 0;
  panel[0][1] = 0;
  panel[1][0] = 0;
  panel[1][1] = 0;
  
  Serial.println("Setup finished");
}


void loop () {
  read_data();
  
  motor_x_aux = check_if_move_x();
  motor_y_aux = -check_if_move_y();
  
  print_data();
  
  
  //for x axis
  if( (motor_x_aux) < -TOLERANCE && motor_x_value >= 0) {
    motor_x_value -= STEP;
  } else if ( motor_x_aux > TOLERANCE && motor_x_value <= 180){
    motor_x_value += STEP;
  }
  motor_x.write(motor_x_value);
  
  //for y axis
  if( (motor_y_aux) < -TOLERANCE && motor_y_value >= 0) {
    motor_y_value -= STEP;
  } else if ( motor_y_aux > TOLERANCE && motor_y_value <= LIMIT_DOWN_Y){
    motor_y_value += STEP;
  }
  motor_y.write(motor_y_value);
  delay(DELAY_MILLIS);
}

void print_data() {
  Serial.println("-----------");
  Serial.print(panel[0][0]);
  Serial.print("\t");
  Serial.println(panel[0][1]);
  Serial.print(panel[1][0]);
  Serial.print("\t");
  Serial.print(panel[1][1]);
  Serial.print("\tdelta: ");
  Serial.print(get_delta());
  Serial.print("\t move X: ");
  Serial.print(check_if_move_x());
  Serial.print("\t move Y: ");
  Serial.println(check_if_move_y());
  
}

void read_data() {
  panel[0][0] = analogRead(A0);
  panel[0][1] = analogRead(A1);
  panel[1][0] = analogRead(A2);
  panel[1][1] = analogRead(A3);
}
int get_delta() {
  int ret = 0;
  int aux;
  
  aux = panel[0][0] - panel[1][1];
  if (aux<0)
    aux*=-1;
  ret+=aux;
  
  aux= panel[0][1] - panel[1][0];
  if (aux<0)
    aux*=-1;
  ret += aux;
  
  return ret;
}

int check_if_move_x() {
  int ret = 0;
  
  ret+= panel[0][0] - panel[0][1];
  ret+= panel[1][0] - panel[1][1];
  
  return ret; 
}

int check_if_move_y() {
  int ret = 0;
  
  ret+= panel[1][0] - panel[0][0];
  ret+= panel[1][1] - panel[0][1];
  
  return ret; 
}
