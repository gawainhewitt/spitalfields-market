/*
* Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS 1    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI SS 2    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 */

#include <SPI.h>
#include <MFRC522.h>
#include "constants.h"

unsigned long lastRFID[NR_OF_READERS];                      //Stores whichever RFID was last used whether valid or not

bool readerState[NR_OF_READERS];            // to store whether there is an RFID tag on a reader or not

struct currentRFID
{
  unsigned long ID = 0;
  byte counter = 0;
};

currentRFID currentRFIDs[NR_OF_READERS]; //create an array of currentRFID objects

byte ssPins[] = {SS_1_PIN}; // this needs to be defined to match number of readers

MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instance.

void init_rfid()
{
    SPI.begin();        // Init SPI bus

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)
  {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 reader
    #ifdef DEBUG
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    #endif
    mfrc522[reader].PCD_DumpVersionToSerial();
  }
}

/*
 * Returns the ID from the specified RFID card as an unsigned long
 */
unsigned long getRFID(byte *buffer){

  unsigned long currentBuffer = 0;
  unsigned long ID = 0;

  for (byte i=0; i<4; i++){
    currentBuffer = buffer[i];
    ID += (currentBuffer << (8*(3-i)));
  }
  return ID;
}

unsigned long checkRFIDremoved(int reader)
{
  if ( ! mfrc522[reader].PICC_IsNewCardPresent()) //if there isn't a new card present
    {
    //To detect whether the tag has been removed this has to return for two consecutive program loops
    //It returns false every other program loop even if there is a tag present.
      currentRFIDs[reader].counter++; // start counting the number of times checked

      if (currentRFIDs[reader].counter == 2 && currentRFIDs[reader].ID != 0)
      {
        unsigned long card_number = currentRFIDs[reader].ID;

        currentRFIDs[reader].ID = 0;        // initialise conditions for the above
        lastRFID[reader] = 0;               // initialise conditions for the above

        return card_number;  // we're passing the RFID id through the other function to get it's place in the array so we can use it to define a music track
      }
      else
      {
        return -1;   //useful so we don't do things we don't need to do in the main loop
      }
    }else
      {
        return -1;   //useful so we don't do things we don't need to do in the main loop
      }
}

unsigned long checkRFIDadded(int reader)
{
  if (mfrc522[reader].PICC_ReadCardSerial()) // if it's reading a number
  {
      currentRFIDs[reader].counter = 0;       //if there is a card present then we don't need to count (as above) so initialise
      unsigned long newRFID = getRFID(mfrc522[reader].uid.uidByte); // store RFID number in a variable called newRFID
      if (newRFID != lastRFID[reader])    //if it's not already on the reader
      {
        currentRFIDs[reader].ID = newRFID;
        lastRFID[reader] = newRFID;
        return newRFID;
      }
      currentRFIDs[reader].ID = newRFID;
      lastRFID[reader] = newRFID;
      return -1;
  }
  else
  {
    {
      return -1; //stops us doing things we don't want to do
    }
  }

}
