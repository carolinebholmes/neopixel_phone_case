// Open a serial connection and flash LED when input is received

void setup(){
  // Open serial connection.
  Serial.begin(9600);
  Serial.write('0'); 
}

void loop(){ 
  if(Serial.available())
  {      // if data present, blink
    String test_str = Serial.readString();
    Serial.println(test_str);
  }
} 

