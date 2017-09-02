#define MOISTURE_SENSOR 0 // Moisture sensor 1
#define PUMP 13 // Pump or relay pump-controller
#define LED_WATER_HIGH 11 // LED indicating good water level in reservoir
#define LED_WATER_LOW 9 //LED indicates low water in reservoir
#define WATER_LEVEL_SENSOR 1 //pin A1 for water level sensor

// INIT
int moistValue = 270;
int waterLevel = 0;
int waterLow  = 150; //low value for water level
int waterHigh = 380; // high value for water level
int dryLimit = 500; // set a lower limit for soil moisture


bool useTimer = true;
int time;
int previousTime = 0;
double timeToWait = 86400000; // 24 hours in milliseconds

// The maximum and minimum values for the sensors 
const int moistValueMAX = 525;
const int moistValueMIN = 270;
const int waterLevelMAX = 400;
const int waterLevelMIN = 0;


const int waterTime = 1000; // set watering time const variable

void setup() {
  pinMode (PUMP, OUTPUT); // set pump/relay to output
  pinMode (LED_WATER_LOW, OUTPUT); 
  pinMode (LED_WATER_HIGH, OUTPUT);
  Serial.begin(9600); // start printing to serial monitor at 9600 baud
}

void loop(){
  if(useTimer == true){
    if(checkTime == true){
      getMoistLevel();
      getWaterLevel();
      pumpAct();
      delay(3000);
    }
  }
  else if(useTimer == false){
    getMoistLevel();
    getWaterLevel();
    pumpAct();
    delay(3000);  
  }
  waterLevelDebug();
  convertToPercentage();
  delay(3000);
}


// FUNCTIONS //

void getMoistLevel(){ // read soil moisture into moistValue variable and print to serial monitor
  moistValue = analogRead(MOISTURE_SENSOR);
  Serial.print("Moisture Level = ");
  Serial.println(moistValue);
}

void getWaterLevel(){ // read  water level into waterLevel variable and print to serial monitor
  waterLevel = analogRead(WATER_LEVEL_SENSOR);
  Serial.print("Water Level = ");
  Serial.println(waterLevel); 
}


void pumpAct(){
  if(analogRead(MOISTURE_SENSOR) > dryLimit && (analogRead (WATER_LEVEL_SENSOR) > waterLow) ){ // If the plant is dry and the reservoir has water activate pump.
    digitalWrite(PUMP, HIGH);
    delay(1000);
  }
  else if (analogRead(MOISTURE_SENSOR) > dryLimit || analogRead(WATER_LEVEL_SENSOR) < waterLow){ // If the plant is wet or ther reservoir does not have enough water dont activate pump.
    digitalWrite (PUMP, LOW);
  }
}

void waterLevelDebug(){
  if (analogRead (WATER_LEVEL_SENSOR) <= waterLow){
    digitalWrite (LED_WATER_LOW, HIGH);
    digitalWrite (LED_WATER_HIGH, LOW);
  }

  else if  (analogRead (WATER_LEVEL_SENSOR) <= waterHigh){
    digitalWrite (LED_WATER_HIGH, HIGH);
    digitalWrite (LED_WATER_LOW, LOW);
  }
}

void convertToPercentage(){ // Print the moisture and water level as a percentage
  int moistPercent = map(moistValue, moistValueMIN, moistValueMAX, 0, 100); // Map all values for moisture sensor to 0-100 to get a percentage
  Serial.print("Moisture Level: ");
  Serial.print(moistPercent);
  Serial.println("%");

  int levelPercent = map(waterLevel, waterLevelMIN, waterLevelMAX, 0, 100); // Map all values for water level sensor to 0-100 to get a percentage
  Serial.print("Water Level: ");
  Serial.print(levelPercent);  
  Serial.println("%");
}


bool checkTime(){ // Check the time and return true if 24 hours have passed.
  time = millis();
  if((time - previousTime) >= timeToWait){
    previousTime = time;
    return true;
    }
  else{
    return false;
    }
}











