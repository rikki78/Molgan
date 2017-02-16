//#define MY_DEBUG    // Enables debug messages in the serial log
//#define DEBUG

#define OVERLOOP

#ifdef WOONKAMER
  // Woonkamer 
  #define MY_NODE_ID 12 // Sets a static id for a node
  #define SN "Molgan woonkamer"
  #define USE_TEMP_HUM
  #define LONG_SLEEP_LIGHT_LEVEL_LO  10 // if the light is below this value we will sleep longer
  #define LONG_SLEEP_LIGHT_LEVEL_HI  30 // if the light is above this value we will sleep longer
  #define LONG_SLEEP_TIME   300000L
  #define SHORT_SLEEP_TIME   60000L
  #define MAX_LIGHT_VAL   800
  #define BATT_4_5V
#endif
#ifdef HAL
// Hal 
  #define MY_NODE_ID 15 // Sets a static id for a node
  #define SN "Molgan hal"
  #define LONG_SLEEP_LIGHT_LEVEL_LO  10 // if the light is below this value we will sleep longer
  #define LONG_SLEEP_LIGHT_LEVEL_HI  30 // if the light is above this value we will sleep longer
  #define LONG_SLEEP_TIME   300000L
  #define SHORT_SLEEP_TIME   60000L
//  #define LOW_LIGHT_AREA
  #define MAX_LIGHT_VAL   500
  #define BATT_4_5V
#endif
#ifdef EETKAMER
  // Eetkamer 
  #define MY_NODE_ID 13 // Sets a static id for a node
  #define SN "Molgan eetkamer"
  #define LONG_SLEEP_LIGHT_LEVEL_LO  10 // if the light is below this value we will sleep longer
  #define LONG_SLEEP_LIGHT_LEVEL_HI  30 // if the light is above this value we will sleep longer
  #define LONG_SLEEP_TIME   300000L
  #define SHORT_SLEEP_TIME   60000L
  #define MAX_LIGHT_VAL   800
  #define BATT_4_5V
#endif
#ifdef GARAGE
  // Garage 
  #define MY_NODE_ID 14 // Sets a static id for a node
  #define SN "Molgan garage"
  #define USE_TEMP_HUM
  #define LONG_SLEEP_LIGHT_LEVEL_LO  10 // if the light is below this value we will sleep longer
  #define LONG_SLEEP_LIGHT_LEVEL_HI  30 // if the light is above this value we will sleep longer
  #define LONG_SLEEP_TIME   300000L
  #define SHORT_SLEEP_TIME   60000L
  #define MAX_LIGHT_VAL   800
  #define BATT_4_5V
#endif
#ifdef BADKAMER
  // Badkamer 
  #define MY_NODE_ID 16 // Sets a static id for a node
  #define SN "Molgan badkamer"
  #define USE_TEMP_HUM
  #define LONG_SLEEP_LIGHT_LEVEL_LO  10 // if the light is below this value we will sleep longer
  #define LONG_SLEEP_LIGHT_LEVEL_HI  60 // if the light is above this value we will sleep longer
  #define LONG_SLEEP_TIME   300000L
  #define SHORT_SLEEP_TIME   60000L
  #define MAX_LIGHT_VAL   800
  #define BATT_4_5V
#endif
#ifdef ZOLDER
  // Zolder
  #define MY_NODE_ID 17 // Sets a static id for a node
  #define SN "Molgan zolder"
  #define LONG_SLEEP_LIGHT_LEVEL_LO  10 // if the light is below this value we will sleep longer
  #define LONG_SLEEP_LIGHT_LEVEL_HI  30 // if the light is above this value we will sleep longer
  #define LONG_SLEEP_TIME   300000L
  #define SHORT_SLEEP_TIME   60000L
  #define MAX_LIGHT_VAL   800
  #define BATT_4_5V
#endif
#ifdef OVERLOOP
  // Overloop
  #define MY_NODE_ID 11 // Sets a static id for a node
  #define SN "Molgan overloop"
  #define LONG_SLEEP_LIGHT_LEVEL_LO  10 // if the light is below this value we will sleep longer
  #define LONG_SLEEP_LIGHT_LEVEL_HI  30 // if the light is above this value we will sleep longer
  #define LONG_SLEEP_TIME   300000L
  #define SHORT_SLEEP_TIME   60000L
  #define MAX_LIGHT_VAL   800
  #define BATT_3V  
#endif

#define FAST_LIGHT_CHANGE_DIFF 10 

#define BATT_4_5V
// #define BATT_3V

#ifdef BATT_3V
  #define MIN_V   2400
  #define MAX_V   3200
#endif

#ifdef BATT_4_5V
  #define MIN_V   3000
  #define MAX_V   4500
#endif

#ifdef DEBUG
  #define LONG_SLEEP_TIME   3000L
  #define SHORT_SLEEP_TIME  1000L
#endif


