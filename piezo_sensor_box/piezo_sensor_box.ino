

//constants

int PWM_pin=3; //Trigger treshold PWM pin
int trigger_reset=2; //trigger reset signal, active LOW
int treshold=135; //trigger treshold value, min practical value is about 135, max about 210
int piezo1=4; //piezo1 input pin
int rel1_SET=7;
int rel1_RESET=8;
int timer=0; //timer that counts 0.1s
int timer_limit=600; //time that relay is ON given as 10xseconds, i.e 100 means 10 seconds
bool rel1_status=false; //relay 1 status, True means relay is on

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
 
  
  pinMode(rel1_SET, OUTPUT);//relay 1 set signal
  pinMode(rel1_RESET, OUTPUT);//relay 1 resetset signal

  digitalWrite(rel1_SET,LOW);
  digitalWrite(rel1_RESET,LOW);
  
  pinMode(13, OUTPUT);//internal led, use for debug
  
  pinMode(piezo1, INPUT);//relay 1 reset signal (with single coil relay set)
  digitalWrite(piezo1,LOW);//pull down
  
  pinMode(trigger_reset,OUTPUT);
  digitalWrite(trigger_reset,HIGH);; //reset is active low
  
 
  pinMode(PWM_pin, OUTPUT);  // sets the pin as output
  analogWrite(PWM_pin,treshold);
  
  rel1_OFF(); //just to be sure, set relay to OFF state
  
}

// the loop function runs over and over again forever
void loop() {

   check_piezo1();
   check_timer();
   delay(100);
  
}

void check_timer(){
  if ((rel1_status==true) and (timer>timer_limit)){  //If relay is ON and timer limit is bypassed then do
    rel1_OFF(); //close relay
    timer=0; //reset timer
    rel1_status=false;
  }
  if ((rel1_status==true) and (timer<=timer_limit)){  //If relay is ON and timer limit is bypassed then do
    timer++;
  }
}

void check_piezo1(){
  int p=0;
  p=digitalRead(piezo1); //piezo1 pin is 4
  if (p==HIGH){
    rel1_ON();
    rel1_status=true;
    
    timer=0; //reset timer allways when bang detected
    reset_trigger();
  }
}

void rel1_ON(){
  digitalWrite(rel1_RESET,LOW);
  digitalWrite(rel1_SET,HIGH);
  delay(100);
  digitalWrite(rel1_SET,LOW); //relay control pins are kept low
  delay(1000);//there must be delay, otherwise relay noise will trigger again
  reset_trigger();
}

void rel1_OFF(){
  digitalWrite(rel1_RESET,HIGH);
  digitalWrite(rel1_SET,LOW);
  delay(100);
  digitalWrite(rel1_RESET,LOW);
  delay(1000);
  reset_trigger();
}

void reset_trigger(){
  digitalWrite(trigger_reset,LOW); //flip flop reset is active LOW
  delay(10);
  digitalWrite(trigger_reset,HIGH);
}
