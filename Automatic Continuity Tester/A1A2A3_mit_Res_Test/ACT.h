void(receiveRe)
void saveResultRes(int R1,char* S1,int R2,char* S2, int R3,char*S3){
  File dataFile=SD.open(fileName,FILE_WRITE);
  if(dataFile){
    dataFile.print(R1);
    dataFile.print(",");
    dataFile.print(S1);
    dataFile.print(",");
    dataFile.print(R2);
    dataFile.print(",");
    dataFile.print(S2);
    dataFile.print(",");
    dataFile.print(R3);
    dataFile.print(",");
    dataFile.println(S3);
    dataFile.close();
  }
  else{
    Serial.println("Error Opening Res File");
  }
  }
  
float ResCal(int anlg){
  float Vpv=3.3/4096;
  float Vval=anlg*Vpv;
  float Rval=((3.3*10)-(Vval*10))/Vval;

  return Rval;
  }
void ResTest(){
    // Res 1:Pin 8 to Pin 6
    // Res 2:Pin 2 to 17
    // Res 3:Pin 13 to 19
    digitalWrite(16,HIGH); //end 2 of 21
    int ReadCount=0;
    int Status;
  while(ReadCount<31){
    if(digitalRead(17)==HIGH && ReadCount<30 && Status==0){
      digitalWrite(0,HIGH);
      digitalWrite(2,HIGH);
      digitalWrite(15,HIGH);
      int Status=1;
      int anlg1=analogRead(A13);
      char* RStatus1;
      float Res1=ResCal(anlg1);
      int anlg2=analogRead(A14);
      char* RStatus2;
      float Res2=ResCal(anlg2);
      int anlg3=analogRead(A14);
      char* RStatus3;
      float Res3=ResCal(anlg3);
        if(Res1>8000 && Res1<15000){
          Serial.print("Pass");
          RStatus1="Pass";
            }
        else{
          Serial.print("Fail");
          RStatus1="Fail";
            }
        if(Res2<10){
          Serial.print("Pass");
          RStatus2="Pass";
            }
        else{
          Serial.print("Fail");
          RStatus2="Fail";
        } 
        if(Res3<10){
          Serial.println("Pass");
          RStatus3="Pass";
        }
        else{
          Serial.println("Fail");
          RStatus3="Fail";
        }
        saveResultRes(Res1,RStatus1,Res2,RStatus2,Res3,RStatus3);
       
        delay(200);
      ReadCount++;
    }
      //Write Into Res Test Result;
        if(digitalRead(16)==HIGH){
      Status=1;  
      Serial.println("Disconnect");
        }
        else{
      Serial.println("Reconnect");
      Status=0;   
        }
      if(ReadCount==30 && Status==0){
        Serial.println("30 Test Done");
      delay(100000);
      }
      else{
      Serial.println("Cable Disconnect");
      delay(1000);
      }
    }
  }


void WriteHead(){
    File dataFile=SD.open(fileName,FILE_WRITE);
    if(dataFile){
      dataFile.print("Res1");
      dataFile.print(",");
      dataFile.print("Status 1");
      dataFile.print(",");
      dataFile.print("Res2");
      dataFile.print(",");
      dataFile.print("Status 2");
      dataFile.print(",");
      dataFile.print("Res3");
      dataFile.print(",");
      dataFile.print("Status 3");
  }
  }

void startContinuityTest() {
  // Your continuity test code goes here...

  // This part should trigger the test when the function is called
  // This is a simplified example of how you might start the test
  // Wire.beginTransmission(8); // Address of the slave ESP32
  // Wire.write('T'); // Command the slave to perform a continuity test
  // Wire.endTransmission();
  Serial.println("Doing Something");
  //tester();
  delay(1000);
  // Perform other steps of the continuity test...
  }
void performContinuityTestM62497_003(){
 
  String WireEnds[] =
  //  ArrayNumber[] = 
  {"P2_01", "P2_02", "P2_03", "P2_04", "P2_05", "P2_06", "P2_07", 
  //  0   ,    1   ,    2   ,    3   ,    4   ,    5   ,    6   , 
  "P2_14", "P2_08", "P2_09", "P2_10", "P2_11", "P2_12", "P2_13", 
  //  7   ,    8   ,    9   ,   10   ,   11   ,   12   ,   13   , 
  "P1_17", "P1_06", "P1_19", "P1_08", "P1_21", "P1_10", "P2_15",
  //  14  ,   15   ,   16   ,   17   ,   18   ,   19   ,   20   ,                    
  "P1_04", "P1_02", "P1_13", "P1_05", "P1_16", "P1_18", "P1_07",
  //  21  ,   22   ,   23   ,   24   ,   25   ,   26   ,   27   ,
  "P1_20", "P1_09", "P1_22", "P1_11", "P1_03", "P1_14", "P1_01",
  //  28  ,   29   ,   30   ,   31   ,   32   ,   33   ,   34   ,                         
  "P1_12", "P3_01", "P3_02", "P4_01", "P5_01", "P4_02", "P5_02"};
  //  35  ,   36   ,   37   ,   38   ,   39   ,   40   ,   41   } 
  const int NumWires = sizeof(WireEnds) / sizeof(WireEnds[0]);
  bool continuityMatrix[NumWires][NumWires];
  int connections[][2] = {  // Fill in WireEnd[] array number (not wire number) that are connected to each other
    {7, 32},{32, 7}, // Example {13, 2},{2, 13}
    {12, 21},{21, 12},
    {10, 24},{24, 10},
    {8, 15},{15, 8},{8, 41},{41, 8},{41, 15},{15, 41},
    {13, 29},{29, 13},
    {11, 19},{19, 11},
    {9, 31},{31, 9},
    {6, 33},{33, 6},
    {4, 20},{20, 4},
    {2, 25},{25, 2},
    {0, 14},{14, 0},{0, 37},{37, 0},{14, 37},{37, 14},
    {5, 28},{28, 5},
    {3, 18},{18, 3},
    {38, 22},{22, 38},
    {40, 23},{23, 40},
    {16, 36},{36, 16},
    {17, 39},{39, 17},
    {1, 30},{30, 1}
  };
  const int NumConnections = sizeof(connections) / sizeof(connections[0]);

    Serial.println("Start M62497_003 continuity test");

    for (int i = 0; i < NumWires; i++) {
      for (int j = 0; j < NumWires; j++) {
        continuityMatrix[i][j] = false;
      }
    }
 
    for (int i = 0; i < NumWires; i++) {
      for (int j = 0; j < NumWires; j++) {
        if (i == j) continue;

        Wire.beginTransmission(8);  // Address of the slave ESP32
        Wire.write('T');  // Command the slave to perform a single continuity test
        Wire.write(i);    // Tell the salve which is the first wire end
        Wire.write(j);    // Tell the salve which is the second wire end
        Wire.endTransmission();
        //delay(0);
       
        Wire.requestFrom(8, 1);
        if (Wire.available() >= 1) {
          int reading = Wire.read();
        /// Serial.println("Here");
        continuityMatrix[i][j] = (reading == LOW);
        String output = WireEnds[i] + " & " + WireEnds[j] + " is ";
        output += continuityMatrix[i][j] ? "Connected" : "Not Connected";
        Serial.println(output);  // Print the formatted message  // Can be change to write into csv
        }
      }
    }

    bool pass = true;  // Assume pass is true unless proven otherwise
    bool connected09 = true;  // Assume connected09 is true unless proven otherwise

    // Check for unexpected connections
    for (int i = 0; i < NumWires; i++) {
      for (int j = 0; j < NumWires; j++) {
        if (i == j) continue;

        bool isExpectedConnection = false;
        for (int k = 0; k < NumConnections; k++) {
          if ((i == connections[k][0] && j == connections[k][1]) ||
              (i == connections[k][1] && j == connections[k][0])) {
            isExpectedConnection = true;
            break;
          }
        }

        if (!isExpectedConnection && continuityMatrix[i][j]) {
          pass = false;
          Serial.print("Unexpected connection: Point ");
          Serial.print(WireEnds[i]);
          Serial.print(" to Point ");
          Serial.println(WireEnds[j]);
        }
      }
    }

    // Check for unexpected disconnections
    for (int i = 0; i < NumConnections; i++) {
      int wire1 = connections[i][0];
      int wire2 = connections[i][1];

      if (!continuityMatrix[wire1][wire2] && !continuityMatrix[wire2][wire1]) {
        pass = false;
        Serial.print("Unexpected disconnection: Point ");
        Serial.print(WireEnds[wire1]);
        Serial.print(" to Point ");
        Serial.println(WireEnds[wire2]);
      }
    }

    if (pass && connected09) {
      Serial.println("Continuity test passed");
    } else {
      Serial.println("Continuity test FAILED");
    }

  //  while (true) {}
  //  delay(1000);  // Delay between tests
  }
void performContinuityTestM62498_003(){
  String WireEnds[] =
  //  ArrayNumber[] = 
  {"P1_17", "P1_22", "P1_16", "P1_21", "P1_15", "P2_20", "P2_14", 
  //  0   ,    1   ,    2   ,    3   ,    4   ,    5   ,    6   , 
  "P1_03", "P1_06", "P1_11", "P1_05", "P1_10", "P1_04", "P2_09", 
  //  7   ,    8   ,    9   ,   10   ,   11   ,   12   ,    13   , 
  "P2_11", "P2_22", "P2_10", "P2_21", "P2_09", "P2_20", "P2_08",
  //  14  ,   15   ,   16   ,   17   ,   18   ,   19   ,   20   ,                    
  "P2_19", "P2_07", "P2_18", "P2_06", "P2_17", "P2_05", "P2_16",
  //  21  ,   22   ,   23   ,   24   ,   25   ,   26   ,   27   ,
  "P2_04", "P2_15", "P2_03", "P2_14", "P2_02", "P2_13", "P2_01",
  //  28  ,   29   ,   30   ,   31   ,   32   ,   33   ,   34   ,                         
  "P2_12", "P1_19", "P1_17", "P1_02", "P1_08", "P1_13", "P1_06"};
  //  35  ,   36   ,   37   ,   38   ,   39   ,   40   ,   41   , coming soon  42   ,   43   } 
  const int NumWires = sizeof(WireEnds) / sizeof(WireEnds[0]);
  bool continuityMatrix[NumWires][NumWires];
  int connections[][2] = {  // Fill in WireEnd[] array number (not wire number) that are connected to each other
    {7, 32},{32, 7}, // Example {13, 2},{2, 13}
    {12, 21},{21, 12},
    {10, 24},{24, 10},
    {8, 15},{15, 8},{8, 41},{41, 8},{41, 15},{15, 41},
    {13, 29},{29, 13},
    {11, 19},{19, 11},
    {9, 31},{31, 9},
    {6, 33},{33, 6},
    {4, 20},{20, 4},
    {2, 25},{25, 2},
    {0, 14},{14, 0},{0, 37},{37, 0},{14, 37},{37, 14},
    {5, 28},{28, 5},
    {3, 18},{18, 3},
    {38, 22},{22, 38},
    {40, 23},{23, 40},
    {16, 36},{36, 16},
    {17, 39},{39, 17},
    {1, 30},{30, 1}
  };
  const int NumConnections = sizeof(connections) / sizeof(connections[0]);

    Serial.println("Start M62498_003 continuity test");

    for (int i = 0; i < NumWires; i++) {
      for (int j = 0; j < NumWires; j++) {
        continuityMatrix[i][j] = false;
      }
    }

    for (int i = 0; i < NumWires; i++) {
      for (int j = 0; j < NumWires; j++) {
        if (i == j) continue;

        Wire.beginTransmission(8);  // Address of the slave ESP32
        Wire.write('T');  // Command the slave to perform a single continuity test
        Wire.write(i);    // Tell the salve which is the first wire end
        Wire.write(j);    // Tell the salve which is the second wire end
        Wire.endTransmission();
       // delay(10);

        Wire.requestFrom(8, 1);
        if (Wire.available() >= 1) {
          int reading = Wire.read();

        continuityMatrix[i][j] = (reading == LOW);
        String output = WireEnds[i] + " & " + WireEnds[j] + " is ";
        output += continuityMatrix[i][j] ? "Connected" : "Not Connected";
        Serial.println(output);  // Print the formatted message  // Can be change to write into csv
        }
      }
    }

    bool pass = true;
    bool connected09 = true;  // Assume connected09 is true unless proven otherwise

    // Check for unexpected connections
    for (int i = 0; i < NumWires; i++) {
      for (int j = 0; j < NumWires; j++) {
        if (i == j) continue;

        bool isExpectedConnection = false;
        for (int k = 0; k < NumConnections; k++) {
          if ((i == connections[k][0] && j == connections[k][1]) ||
              (i == connections[k][1] && j == connections[k][0])) {
            isExpectedConnection = true;
            break;
          }
        }

        if (!isExpectedConnection && continuityMatrix[i][j]) {
          pass = false;
          Serial.print("Unexpected connection: Point ");
          Serial.print(WireEnds[i]);
          Serial.print(" to Point ");
          Serial.println(WireEnds[j]);
        }
      }
    }

    // Check for unexpected disconnections
    for (int i = 0; i < NumConnections; i++) {
      int wire1 = connections[i][0];
      int wire2 = connections[i][1];

      if (!continuityMatrix[wire1][wire2] && !continuityMatrix[wire2][wire1]) {
        pass = false;
        Serial.print("Unexpected disconnection: Point ");
        Serial.print(WireEnds[wire1]);
        Serial.print(" to Point ");
        Serial.println(WireEnds[wire2]);
      }
    }

    if (pass && connected09) {
      Serial.println("Continuity test passed");
    } else {
      Serial.println("Continuity test FAILED");
    }

  //  while (true) {}
  //  delay(1000);  // Delay between tests
  }
