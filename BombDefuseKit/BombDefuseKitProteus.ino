#include <LiquidCrystal.h>
#include <Arduino.h>
#define  BUZZERTIME time.Current - time.Previous
#define BOMBTIMER BombTimer.Current - BombTimer.Previous
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void DontBounce(int button)
{
  while(!digitalRead(button))
  {

  }

}

struct Time{

    float Current;
    float Previous;

};


struct Button{
    int pin;
    bool value;
};

struct Cable{
    int pin;
    bool value;
    bool IsPassword;
    char PasswordCode;


};

struct Button OkeyButton = {9,true};
struct Button SelectButton = {10,true};
//struct Button DefuseButton = {12,true};
struct Button BuzzerButton = {NULL,false};
struct Time time = {0,0};
struct Time BombTimer = {0,0};
struct Cable Red = {11,false,false,'R'};
struct Cable Green = {2,false,false,'G'};
struct Cable Blue = {12,false,false,'B'};
struct Button Buzzer = {13,true};

struct Cable ColorsValues[] = {Red,Green,Blue};

bool pressed = 0;
bool ChoosingSection = false;
int BuzzerSpeed = 1000;
bool pulled = true;
char PulledCable = NULL;

char Colors[] = {'R','G','B'};
bool IsBuzzerOn = false;
bool IsPLayerPullCable = false;

int index = 0;
int timer = 60; // BombTimer
char Password = NULL;
void setup() {
  // set up the LCD's number of columns and rows:
  //pinMode(DefuseButton.pin, INPUT_PULLUP);
  pinMode(OkeyButton.pin, INPUT_PULLUP);
  pinMode(SelectButton.pin, INPUT_PULLUP);
  pinMode(Buzzer.pin, OUTPUT);

  
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Alps Defuse Kit");
  //Serial.begin(9600);
}
bool temp = true;
void loop() 
{
  
  //BuzzerButton.value = digitalRead(BuzzerButton.pin);
  OkeyButton.value = digitalRead(OkeyButton.pin);  
  SelectButton.value = digitalRead(SelectButton.pin);
  //DefuseButton.value = digitalRead(DefuseButton.pin);
  
  Red.value = digitalRead(Red.pin);
  Green.value = digitalRead(Green.pin);
  Blue.value = digitalRead(Blue.pin);
  for(int i = 0; i <sizeof(Colors) / sizeof(Colors[0]); i++)
  {
    digitalWrite(ColorsValues[i].pin,HIGH);
  }
  ColorsValues[0] = Red;
  ColorsValues[1] = Green;
  ColorsValues[2] = Blue; 
  
  //Serial.println(Red.value);

  time.Current = millis();
  BombTimer.Current = millis();
  
  if(BuzzerButton.value == pressed && Password != NULL && temp)
  {
      IsBuzzerOn = !IsBuzzerOn;
      temp = false;  
      DontBounce(BuzzerButton.pin); 
  }
  if(IsBuzzerOn)
  {
      if(BUZZERTIME >= BuzzerSpeed)
        {
            Buzzer.value = !Buzzer.value;
            //digitalWrite(Buzzer.pin,Buzzer.value);
            switch (Buzzer.value) 
            {
            case true : tone(Buzzer.pin,5000);
            break;
            case false: noTone(Buzzer.pin);
            break;
            }
            time.Previous = time.Current;
        }
          
  }  
      
  else  noTone(Buzzer.pin); 
  
  if(SelectButton.value == pressed && Password == NULL) 
  {
      if(index == sizeof(Colors) / sizeof(Colors[0])) index = 0;
      
      ChoosingSection = true; // kullanıcının seçim menüsü gelmeden ok tuşuna basmasını engellemek için yazdığım bir satır
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Choose Password");
      lcd.setCursor(8,1);
      lcd.print(Colors[index]);
      DontBounce(SelectButton.pin);
      index++;
  }
                    
  else if(OkeyButton.value == pressed && Password == NULL && ChoosingSection)
    {
        Password = Colors[index-1]; 
        lcd.clear();
        DontBounce(OkeyButton.pin);
    }    

  
  
  
  
  if(Password != NULL)
  {
    static bool RunOnce = false;
    if(!RunOnce)
    {
      lcd.setCursor(0,0);
      lcd.print("Password = ");
      lcd.setCursor(11,0);
      lcd.print(Colors[index-1]);
      delay(3000);
      RunOnce = true;
      BombTimer.Current = 0;
      lcd.clear();
    }   
    
    lcd.setCursor(5,0);
    lcd.print("00:");
    lcd.setCursor(9,0);
    if(!(timer < 10)) lcd.print(timer);
    else
    {
      BuzzerSpeed = 500;
      lcd.print("0");
      lcd.setCursor(10,0);
      lcd.print(timer);
    }
    lcd.setCursor(0,1);
    lcd.print("Password = *");
    if(BombTimer.Current - BombTimer.Previous > 1000)
    {
      timer--;
      BombTimer.Previous = BombTimer.Current;
    }    
  }
  
  for(int i = 0; i <sizeof(Colors) / sizeof(Colors[0]) && !IsPLayerPullCable; i++) // if you plug the blue cable , please delete the -1 value or  the code doesnt work right
  {
    if( ColorsValues[i].value == pulled)
    {
      PulledCable = ColorsValues[i].PasswordCode;
      IsPLayerPullCable = true;
      break;
    }
  
  }
  
  if(timer < 0)
  {
    lcd.clear();
        lcd.setCursor(0,0);
        tone(Buzzer.pin,5000);
        lcd.print("Bomb Exploded!!!");
        while(true)
        {

        }        
  }

  
  else if(Password != NULL && PulledCable != NULL)
  {
      if(Password == PulledCable)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Bomb Defused");
        noTone(Buzzer.pin);
        while(true)
        {

        }        
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0,0);
        tone(Buzzer.pin,5000);
        lcd.print("Bomb Exploded !!!");
        while(true)
        {

        }        
      }
      
  }
  

}






