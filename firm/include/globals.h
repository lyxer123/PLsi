#ifndef GLOBALS_H
#define GLOBALS_H

#include <arduino.h>
#include <plsi.h>

//--------------------------------------------------------------------------------
// PLC General status
//--------------------------------------------------------------------------------

#define STOPPED                            0 
#define RUNNING                            1 
#define PLCERROR                           2
#define PLCERROR_FFAT_FORMAT_ERROR        10 
#define PLCERROR_INVALID_INSTRUCTION      20

//--------------------------------------------------------------------------------
// PLC Booting status
//--------------------------------------------------------------------------------

#define BOOT_INIT                          0
#define BOOT_DISK_LOADED                  10
#define BOOT_SETTINGS_LOADED              20
#define BOOT_TASK_UNLOCKED                30
#define BOOT_DISK_ERROR                  100

//--------------------------------------------------------------------------------
// PLC Data Types Codes
// Must match with 'MnemonicsTypes' array
//--------------------------------------------------------------------------------

#define TypeM                  0
#define TypeQ                  1
#define TypeI                  2
#define TypeCd                 3
#define TypeCr                 4
#define TypeTd                 5
#define TypeTr                 6
#define TypeIW                 7
#define TypeQW                 8
#define TypeC                  9
#define TypeT                 10
#define TypeD                 11
#define TypeK                 12
#define TypeR                 13
#define TypeKR                14
#define FIRST_INVALID_TYPE    15

//--------------------------------------------------------------------------------
// PLC Ladder Instructions codes 
// Must match with array of funtions in TaskLadder 
//--------------------------------------------------------------------------------

#define NOPP                   0
#define CONN                   1
#define NEG                    2
#define NO                     3
#define NC                     4
#define RE                     5
#define FE                     6
#define COIL                   7
#define COILL                  8
#define COILU                  9
#define TON                   10
#define TOFF                  11
#define TP                    12
#define CTU                   13
#define CTD                   14
#define MOVE                  15
#define SUB                   16
#define ADD                   17
#define MUL                   18
#define DIV                   19
#define MOD                   20
#define SHL                   21
#define SHR                   22
#define ROL                   23
#define ROR                   24
#define AND                   25
#define OR                    26
#define XOR                   27
#define NOT                   28
#define FIRST_INVALID_CODE    29
#define CELL_USED_MASK    0xF000 // Used by an instruction that occupies more than one cell
#define CELL_CODE_MASK    0x0FFF 
#define BAR_MNEMONIC_CODE     99

//--------------------------------------------------------------------------------
// PLC Network Logic matrix Size [units]
//--------------------------------------------------------------------------------

#define NET_COLUMNS            6     
#define NET_ROWS               5

//--------------------------------------------------------------------------------
// SSID and password max length
//     SSID is 32 + 1 for null
//     PASS is 36 + 1 for null
//--------------------------------------------------------------------------------

#define SSID_LENGTH           33
#define PASS_LENGTH           65

//--------------------------------------------------------------------------------
// Type definition for Networks and global declarations
//--------------------------------------------------------------------------------

typedef struct
{
  uint16_t Code;
  int16_t Data;
  uint16_t Type;
} Cell;

typedef struct
{
  Cell Cells[NET_ROWS][NET_COLUMNS];
  uint16_t Bars [NET_COLUMNS - 1];
} Network;

extern Network execNetwork;
extern Network onlineNetwork;
extern Network editingNetwork;
extern Network copyMemoryNetwork;
extern Network emptyNetwork; 

typedef struct
{
  Cell column[NET_ROWS];
  uint16_t bar;
} memoryColumn;

extern memoryColumn copyMemoryColumn;

typedef struct
{
  Cell row[NET_COLUMNS];
  uint16_t bar;
} memoryRow;

extern memoryRow copyMemoryRow;

//--------------------------------------------------------------------------------
// PLC Memory types definitions 
// Byte type is used for "Bits" for increased performance of Scan time
//--------------------------------------------------------------------------------

// Bits declarations
extern byte           M     [QTY_M];   // Regular flags
extern byte           I     [QTY_I];   // Digital Inputs
extern byte           Q     [QTY_Q];   // Digital Outputs
extern byte           Cr    [QTY_C];   // Counter running
extern byte           Cd    [QTY_C];   // Counter done
extern byte           Tr    [QTY_T];   // Timer running
extern byte           Td    [QTY_T];   // Timer done

// Bits Previous Scan Values (history) for FE and RE instructions
extern byte           Mh    [QTY_M];
extern byte           Ih    [QTY_I];
extern byte           Qh    [QTY_Q];
extern byte           Crh   [QTY_C];
extern byte           Cdh   [QTY_C];
extern byte           Trh   [QTY_T];
extern byte           Tdh   [QTY_T];

// Registers declaration
extern uint16_t       IW    [QTY_IW];  // Analog Inputs  (word)
extern uint16_t       QW    [QTY_QW];  // Analog Outputs (word)
extern uint16_t       C     [QTY_C];   // Counter registers (16 bits)
extern int16_t        D     [QTY_D];   // Regular registers (signed 16 bits)
extern float          R     [QTY_R];   // Floaing point registers

// Timers struct
typedef struct {
  unsigned long TimeStamp; // 32bits same as unsigned int
  uint16_t            ACC;
} Timer;

extern Timer       Timers[QTY_T];

//--------------------------------------------------------------------------------
// General PLC Configuration struct
// settings.bin in RAM
//--------------------------------------------------------------------------------
// Main PLC status in PLCstate
//  0  = Stopped 
//  1  = Running 
// >1  = Error Codes
//--------------------------------------------------------------------------------

typedef struct
{
  uint16_t firstRun;
  uint16_t verbosityLevel;
} generalSettings;

typedef struct
{
  uint16_t PLCstate;
  uint16_t NetworksQuantity;
  uint16_t UserProgram;
} ladderSettings;

typedef struct
{
  uint16_t leftX;
  uint16_t rightX;
  uint16_t topY;
  uint16_t bottomY;
} tsCalibration;

typedef struct
{
  tsCalibration ts;
} hmiSettings;

typedef struct
{
  char      ssid[SSID_LENGTH];
  char  password[PASS_LENGTH];
} wifiSettings;

typedef struct
{
  generalSettings    general;
  ladderSettings      ladder;
  hmiSettings            hmi;
  wifiSettings          wifi;
} userSettings;

extern userSettings settings;

//--------------------------------------------------------------------------------
// Manage the boot secuence between tasks.
// Booting state = 0
// User config loaded = 1
// 
//--------------------------------------------------------------------------------

extern uint16_t bootSequence;

//--------------------------------------------------------------------------------
// Sync between taskLadder and others
//--------------------------------------------------------------------------------

extern uint16_t loadSelectedProgram;
extern uint16_t updateSelectedProgramRAM;
extern uint16_t updateSelectedProgramDisk;
extern uint16_t moveNetworksInsert;
extern uint16_t moveNetworksDelete;

extern uint16_t showingNetwork; // move to tskHMI.h lucas

//--------------------------------------------------------------------------------
// Mask for "Bars vs Flags" analisys
//--------------------------------------------------------------------------------

extern const uint16_t FlagsMask[NET_ROWS];
extern const uint16_t FlagsMaskAnd[NET_ROWS];

#endif
