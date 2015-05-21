// Open a serial connection and flash LED when input is received

void setup(){
  // Open serial connection.
  Serial.begin(9600);
  Serial.write('0'); 
}

void loop(){ 
  if(Serial.available())
  {      // if data present, blink
    String test_str;
    float test_flt;
    test_str = Serial.readString();
    
    char carray[test_str.length() + 1]; //determine size of the array
    test_str.toCharArray(carray, sizeof(carray)); //put readStringinto an array
    float f = atof(carray); //convert the array into a float
    Serial.println();
    Serial.println(f);
  }
} 
