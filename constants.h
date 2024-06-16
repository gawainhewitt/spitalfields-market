//DEBUG
#define DEBUG // comment this out to disable debugging on the serial bus

//RFID

#define NR_OF_READERS   1           // can only reliably use two on each arduino
//for more readers you have to set ss pins in rfid.h
#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_1_PIN        10         // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 2
#define SS_2_PIN        8          // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 1

//readable stuff
#define ON_READER       50
