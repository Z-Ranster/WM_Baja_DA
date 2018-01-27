void setup() {
  // This is to power on the shield
  pinMode(MEM_PW, OUTPUT);
  digitalWrite(MEM_PW, HIGH);
}

void loop() {
  // This will append the text "Data" to the end of
  // file "data.log"
  byte buffer[] = "Data";
  if(FileLogger::append("data.log", buffer,4) != 0){
    // handle error
  }
}
