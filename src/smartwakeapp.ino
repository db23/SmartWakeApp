#include <SoftwareSerial.h>
#include <Bridge.h>
#include <Temboo.h>
//#include "TembooAccount.h" // contains Temboo account information

#define TEMBOO_ACCOUNT "****"  // your Temboo account name 
#define TEMBOO_APP_KEY_NAME "*****"  // your Temboo app key name
#define TEMBOO_APP_KEY  "********"  // your Temboo app key


#define NOTE_G3  196    //
#define NOTE_A3  220    //
#define NOTE_B3  247    //
#define NOTE_C4  262   //


SoftwareSerial lcd(2, 3);// This is required, to start an instance of an LCD
int ledPin = 9;
const int buttonPin = 13;

String weatherFlag;

int ledState = LOW;

int interval = 0;
int previousMillis = 0;
int stopClock = 0;
bool ant = false;

int hours = 8;  // Enter the hours here
int minutes = 19;  // Enter the minutes here
int seconds = 50;  // Enter the seconds here

int hoursClock = 8;
int minClock = 50;
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

// the address for which a weather forecast will be retrieved
String ADDRESS_FOR_FORECAST = "Kuala Lumpur";

int numRuns = 1;   // execution count, so that this doesn't run forever
int maxRuns = 10;  // max number of times the Yahoo WeatherByAddress Choreo should be run

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  lcd.begin(9600);  // Start the LCD at 9600 baud
  clearDisplay();  // Clear the display
  setLCDCursor(2);  // Set cursor to the 3rd spot, 1st line
  lcd.print("Hello, world");
  setLCDCursor(16);  // Set the cursor to the beginning of the 2nd line
  lcd.print("Running clock...");
  
  // Flash the backlight:
  lcd.write(0xFE);
  lcd.write(0x7c);  // send the backlight command
  lcd.write(157);  // send the brightness value
  Bridge.begin();
}

void loop()
{ 
  
  if (!(millis() % 1000))  // If it's been 1 second
  {
    if(hours == hoursClock && minutes == minClock - 30 && seconds == 0){
        
      weatherFlag = weather();
      String stringhetta ="28";
     //Serial.print(stringhetta);
     //Serial.print(weatherFlag);
     
      if(weatherFlag.equals(stringhetta)){
          minClock = minClock - 29;
          ant = true;
          Serial.print("piove anticipo!\n"); 
          Serial.print(minClock); }
       
       //Serial.print("controllo meteo!\n");
       //Serial.print(weatherFlag);
      
     }
    
    checkTime();  // this function increases the time and date if necessary
    clearDisplay();  // Clear the display
    setLCDCursor(1);  // set cursor to 2nd spot, 1st row
    
    setLCDCursor(20);  // set the cursor to the 5th spot, 2nd row
    lcd.print(hours);  // print the time:
    lcd.print(":");
    lcd.print(minutes);
    lcd.print(":");
    lcd.print(seconds);
    lcd.print(" :: ");
    lcd.print(stopClock);
    checkDate(); 
  }
  
  setClock();
  
}

String weather()
{
  if (numRuns <= maxRuns) {
      
    // print status
    Serial.println("Running GetWeatherByAddress - Run #" + String(numRuns++) + "...");

    // create a TembooChoreo object to send a Choreo request to Temboo
    TembooChoreo GetWeatherByAddressChoreo;
    
    // invoke the Temboo client
    GetWeatherByAddressChoreo.begin();

    // add your temboo account info
    GetWeatherByAddressChoreo.setAccountName(TEMBOO_ACCOUNT);
    GetWeatherByAddressChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    GetWeatherByAddressChoreo.setAppKey(TEMBOO_APP_KEY);
    
    // set the name of the choreo we want to run
    GetWeatherByAddressChoreo.setChoreo("/Library/Yahoo/Weather/GetWeatherByAddress");
    
    // set choreo inputs; in this case, the address for which to retrieve weather data
    // the Temboo client provides standardized calls to 100+ cloud APIs
    GetWeatherByAddressChoreo.addInput("Address", ADDRESS_FOR_FORECAST);
    GetWeatherByAddressChoreo.addInput("Units", "c");

    // add an output filter to extract the name of the city.
    //GetWeatherByAddressChoreo.addOutputFilter("city", "/rss/channel/yweather:location/@city", "Response");
    
    // add an output filter to extract the current temperature
    //GetWeatherByAddressChoreo.addOutputFilter("temperature", "/rss/channel/item/yweather:condition/@temp", "Response");

    // add an output filter to extract the date and time of the last report.
    //GetWeatherByAddressChoreo.addOutputFilter("date", "/rss/channel/item/yweather:condition/@date", "Response");

    GetWeatherByAddressChoreo.addOutputFilter("condition", "/rss/channel/item/yweather:condition/@code", "Response");
    
    // run the choreo 
    GetWeatherByAddressChoreo.run();
        
        
    String output[5];
    int i = 0;
    
    // when the choreo results are available, print them to the serial monitor
    while(GetWeatherByAddressChoreo.available()) {
          
      char c = GetWeatherByAddressChoreo.read(); 
      if (c == 30 || c == 31) continue;
      if (c != '\n') {
        output[i] += c;
      } else {
        i++;
      }   
      
      
      
    }
    
    Serial.print(output[1]);
    
    
    //weatherFlag = output[1];
    
    if(weatherFlag.equals( output[1]) )
      Serial.print("qui");
      
    
    GetWeatherByAddressChoreo.close();
    
    return output[1];

  }

  //Serial.println("\nWaiting...");
  //Serial.println("");
}

void clearDisplay()
{
  lcd.write(0xFE);  // send the special command
  lcd.write(0x01);  // send the clear screen command
}

void setLCDCursor(byte cursor_position)
{
  lcd.write(0xFE);  // send the special command
  lcd.write(0x80);  // send the set cursor command
  lcd.write(cursor_position);  // send the cursor position
}

void setClock(){
  buttonState = digitalRead(buttonPin);
   // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == LOW) {
    // turn LED on:
    stopClock = 1;
    //digitalWrite(ledPin, HIGH);
    
  }
 
 
}

      
void checkTime()
{
  seconds++;  // increase seconds
  if (seconds == 60)  // If it's been a minute
  {
    seconds = 0;  // start over seconds
    minutes++;  // Increase minutes
    if (minutes == 60)  // If it's been an hour
    {
      minutes = 0;  // start over minutes
      hours++;  // increase hours
      if (hours == 24)  // If it's been a day
      {
        hours = 0;  // start the day over
        
      }
    }
  }
}

void checkDate()
{

      
     if(hours == hoursClock && minutes == minClock && stopClock == 0){
     
     if(ant){
       setLCDCursor(20);
       lcd.print("sveglia anticipata causa pioggia");
     }
      
     
    int thisNote = 0;
    while(thisNote < 8){
        unsigned long currentMillis = millis();
        if(currentMillis - previousMillis > interval ){
          previousMillis = currentMillis;
          
    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(8, melody[thisNote], noteDuration);
      
      thisNote++;
    }
        }

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
  else{
    digitalWrite(ledPin, LOW);
    noTone(8);
  }
  
}
    
  
