#include "rfid.h"

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
  init_rfid();
}

void loop() {
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
      Serial.print(rfid_removed[reader], HEX);
      Serial.println(" REMOVED");
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
      Serial.print(rfid_added[reader], HEX);
      Serial.println(" ADDED");
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
