void readSensors() {
  digitalWrite(sensorTriggerPin, HIGH);
  delay(50);
  digitalWrite(sensorTriggerPin, LOW);
  delay(10);

  for (int n = 0; n < sensorNum; n++) {
    sensorValues[n] = analogRead(n);
  }
}

String getSensorArray() {
  String sensorData = "";
  for (int n = 0; n < sensorNum; n++) {
    sensorData += "0,";
  }
  return sensorData;

}

//first = number of arrays in array
//second = number of elements in each sub array


String sensorAverage() {
  int sensorSamples[sensorNum][sampleSize];
  String returnValue;
  long sensorSum = 0;

  for (int j = 0; j < sampleSize; j++) {
    digitalWrite(sensorTriggerPin, HIGH);
    delay(50);
    digitalWrite(sensorTriggerPin, LOW);
    delay(10);

    for (int n = 0; n < sensorNum; n++) {
      sensorSamples[j][n] = analogRead(n);
    }

  }


  for (int o = 0; o < sensorNum; o++) {
    for (int i = 0; i < sampleSize; i++) {
      sensorSum += int(sensorSamples[o][i]);
    }

    returnValue += String((sensorSum / sampleSize));
  }

  return returnValue;
}

