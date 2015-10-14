void readSensors(){
  digitalWrite(sensorTriggerPin, HIGH);
  delay(50);
  digitalWrite(sensorTriggerPin, LOW);
  delay(10);
  
  for(int n=0; n<sensorNum; n++){  
    sensorValues[n] = analogRead(n);
  }
}

String getSensorArray(){
   String sensorData = "";
   for(int n=0; n<sensorNum; n++){  
    sensorData += "0,";
  }
  return sensorData;
  
}
