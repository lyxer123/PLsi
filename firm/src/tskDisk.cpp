#include <globals.h>
#include <plsi.h>
#include <tskDisk.h>
#include <disk.h>

#include <TFT_eSPI.h> // Lucas borrar
#include <hmi.h> // Lucas borrar

#include "FS.h"
#include "SD.h"
#include "SPIFFS.h"

//--------------------------------------------------------------------------------
// Disk Task 
// Controls SPIFSS memory and SD Card files
// Running on Core 0
//--------------------------------------------------------------------------------

void TaskDisk(void *pvParameters)
{
  (void) pvParameters;

  //------------------------------------------------------
  // Regular boot: Mount SPIFFS Disk
  // First boot: Format disk (SPIFFS)
  //------------------------------------------------------
  
  loadDisk();         

  //------------------------------------------------------
  // Regular boot: Load saved settings from SPIFFS Disk
  // First boot: Load default settings and Save to Disk
  //------------------------------------------------------

  loadSettings();
  
  //----------------------------------------------------
  // Task Main loop 
  //----------------------------------------------------

  while(1){

    if(updateSelectedProgramDisk){
      Serial.println("TaskDisk - Network Saved to Disk ");

      SPIFFS.begin();

      File userProgramFile = SPIFFS.open(FILENAME_USER_PROGRAMS[settings.ladder.UserProgram],"r+");
      userProgramFile.seek(showingNetwork * sizeof(onlineNetwork));

      Serial.print("Position in File: ");
      Serial.println(userProgramFile.position());
        
      userProgramFile.write((uint8_t *)&onlineNetwork, sizeof(onlineNetwork));
      userProgramFile.close();
      SPIFFS.end();

      updateSelectedProgramDisk = 0;
    }


    if (I[0]){
      SPIFFS.begin();
      File root = SPIFFS.open("/");
      File file = root.openNextFile();
      while(file){
        Serial.print("FILE: ");
        Serial.print(file.name());
        Serial.print("       SIZE: ");
        Serial.println(file.size());
        file = root.openNextFile();
      }
      SPIFFS.end();
      delay(4000);
    }

    if (I[1]){ // Lucas delete
        SPIFFS.begin();
        Serial.print("Info SPIFFS Total Bytes: ");
        Serial.println(SPIFFS.totalBytes());
        Serial.print("Info SPIFFS Used Bytes: ");
        Serial.println(SPIFFS.usedBytes());
        SPIFFS.end();
      delay(2000);
    }
    
    if (I[2]){
        Serial.println("Formatting SPIFFS...");
        SPIFFS.begin();
        SPIFFS.format();
        SPIFFS.end();
        Serial.print("Info SPIFFS Total Bytes: ");
        Serial.println(SPIFFS.totalBytes());
        Serial.print("Info SPIFFS Used Bytes: ");
        Serial.println(SPIFFS.usedBytes());
      delay(4000);
    }

    if (I[3]){
      settings.ladder.UserProgram ++;
      if (settings.ladder.UserProgram > 7){
        settings.ladder.UserProgram = 0;
      }
      saveSettings();
      loadSelectedProgram = 1;
      delay(4000);
      Serial.print("Program changed to: ");
      Serial.println(settings.ladder.UserProgram);
    }

    if (I[4] && editionMode){
      onlineNetwork.Cells[0][0].Data++;

      Serial.println("TaskDisk - Program changed to test edition: ");
      delay(4000);
    }


























    delay(5);
 }
}