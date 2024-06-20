#include "rfid.h"
#include "wavtrigger.h"

struct rfidStatus  // to keep the details of each card in
{
  unsigned long ID = 0;
  int state = 0;
};

rfidStatus readers[NR_OF_READERS]; //create an array of currentRFID objects

void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
  delay(1000);
  init_rfid();
  wTrig.start();
  delay(10);
  wTrig.stopAllTracks();
  wTrig.samplerateOffset(0);
  wTrig.masterGain(-12);                  // Reset the master gain to 0dB
  wTrig.setReporting(true);
  delay(100); 
}

const int numberOfRfids = 15;
unsigned long rfids[numberOfRfids] = {3618, 30577, -16606, 1570, 13425, 13169, -2270, -3550, 30065, 28529, -222, -2014, -15838, -3294, -10974 };
bool playTheTrack[numberOfRfids] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
bool trackPlaying[numberOfRfids] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };


void checkTrackStatus() {\
  int checkAllOff = 0;
  for (int i = 0; i < numberOfRfids; i++) {
    // bool status = wTrig.isTrackPlaying(i+1);
    // Serial.print(status);
    // Serial.print(" ");
    // Serial.println(trackPlaying[i]);
    if (playTheTrack[i] && !trackPlaying[i]) {
      Serial.print(playTheTrack[i]);
      Serial.println(trackPlaying[i]);
      wTrig.trackPlaySolo(i+1);
      delay(50);
      wTrig.trackPlaySolo(i+1);
      trackPlaying[i] = true;
    } else if (!playTheTrack[i] && trackPlaying[i]){
      Serial.println("are we here?");
      wTrig.trackStop(i+1);
      delay(50);
      wTrig.trackStop(i+1);
      delay(50);
      wTrig.trackStop(i+1);
      trackPlaying[i] = false;
    }

    if (!playTheTrack[i]) {
      checkAllOff = checkAllOff + 1;
    }

    if (checkAllOff == numberOfRfids-1) {
      wTrig.stopAllTracks(); 
    }
  }
}

void loop() {

  wTrig.update(); 

  checkTrackStatus();

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)
  {
    int rfid_removed[NR_OF_READERS];    //create an array to store values in

    rfid_removed[reader] = checkRFIDremoved(reader); // call the check RFID function and store returned value in the array

    if (rfid_removed[reader] != -1)   // if the above function doesn't have a card removed then it will return -1 and we won't do the rest
    {
      #ifdef DEBUG
      Serial.print("Reader ");
      Serial.print(reader);
      Serial.print(":");
      Serial.print(rfid_removed[reader]);
      Serial.println(" REMOVED");

      for (int i = 0; i < numberOfRfids; i++){
        if (rfid_removed[reader] == rfids[i]) {
          Serial.print("stop ");
          Serial.println(i+1);
          playTheTrack[i] = false;
        }
      }
      
      #endif
      readerState[reader]= false;
      readers[reader].state = 0;
    }

    int rfid_added[NR_OF_READERS];  //create an array to store values in

    rfid_added[reader] = checkRFIDadded(reader); // call the check RFID function and store returned value in the array

    if (rfid_added[reader] != -1) // if the above function doesn't have a card added then it will return -1 and we won't do the rest
    {
      #ifdef DEBUG
      Serial.print("Reader ");
      Serial.print(reader);
      Serial.print(":");
      Serial.print(rfid_added[reader]);
      Serial.println(" ADDED");

      // if (rfid_added[reader] == rfids) {
      //   Serial.println("play");
      // }

      for (int i = 0; i < numberOfRfids; i++){
        if (rfid_added[reader] == rfids[i]) {
          Serial.print("play ");
          Serial.println(i+1);
          playTheTrack[i] = true;
        }
      }


      #endif
      readerState[reader] = true;
      readers[reader].state = 1;
      readers[reader].ID = rfid_added[reader];
    }
    }
}









// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
// void requestEvent() {
//   for (uint8_t reader = 0; reader < NR_OF_READERS; reader++){
//     if(readers[reader].state == true){
//       String cardNo = String(readers[reader].ID); //convert the card number to a string so that we can then convert it to a char*
//       Wire.write(cardNo.c_str()); // Wire.write only accepts char* so we need to cast the string so that Wire.write will accept it and send it
//     }else{
//       Wire.write("no card");
//     }
//   }
// }
