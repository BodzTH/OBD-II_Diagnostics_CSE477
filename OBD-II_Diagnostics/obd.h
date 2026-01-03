/******************************************************************************
 *
 * Module: OBD - On-Board Diagnostics II Driver
 *
 * File Name: obd.h
 *
 * Description: Header file for OBD-II Protocol Driver
 *              Implements SAE J1979 (ISO 15031-5) standard
 *              For automotive diagnostics via CAN bus
 *
 * Author: BodzTH
 *
 *******************************************************************************/

#ifndef OBD_H_
#define OBD_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                   *
 *******************************************************************************/

/* OBD-II CAN Message IDs */
#define OBD_REQUEST_ID              0x7DF   /* Functional (broadcast) request ID */
#define OBD_RESPONSE_ID_MIN         0x7E8   /* ECU response ID range start */
#define OBD_RESPONSE_ID_MAX         0x7EF   /* ECU response ID range end */

/* OBD-II Service Modes (PIDs) */
#define OBD_MODE_CURRENT_DATA           0x01    /* Show current data */
#define OBD_MODE_FREEZE_FRAME           0x02    /* Show freeze frame data */
#define OBD_MODE_DTC_CODES              0x03    /* Show stored DTCs */
#define OBD_MODE_CLEAR_DTC              0x04    /* Clear DTCs and stored values */
#define OBD_MODE_O2_TEST_RESULTS        0x05    /* Oxygen sensor test results */
#define OBD_MODE_TEST_RESULTS           0x06    /* Other test results */
#define OBD_MODE_PENDING_DTC            0x07    /* Show pending DTCs */
#define OBD_MODE_CONTROL_OPERATION      0x08    /* Control on-board system */
#define OBD_MODE_VEHICLE_INFO           0x09    /* Request vehicle information */
#define OBD_MODE_PERMANENT_DTC          0x0A    /* Permanent DTCs */

/* Common Mode 01 PIDs (Current Data) */
#define OBD_PID_SUPPORTED_PIDS_01_20    0x00    /* Supported PIDs [01-20] */
#define OBD_PID_MONITOR_STATUS          0x01    /* Monitor status since DTCs cleared */
#define OBD_PID_FREEZE_DTC              0x02    /* Freeze DTC */
#define OBD_PID_FUEL_SYSTEM_STATUS      0x03    /* Fuel system status */
#define OBD_PID_ENGINE_LOAD             0x04    /* Calculated engine load */
#define OBD_PID_COOLANT_TEMP            0x05    /* Engine coolant temperature */
#define OBD_PID_SHORT_FUEL_TRIM_1       0x06    /* Short term fuel trim - Bank 1 */
#define OBD_PID_LONG_FUEL_TRIM_1        0x07    /* Long term fuel trim - Bank 1 */
#define OBD_PID_SHORT_FUEL_TRIM_2       0x08    /* Short term fuel trim - Bank 2 */
#define OBD_PID_LONG_FUEL_TRIM_2        0x09    /* Long term fuel trim - Bank 2 */
#define OBD_PID_FUEL_PRESSURE           0x0A    /* Fuel pressure */
#define OBD_PID_INTAKE_MAP              0x0B    /* Intake manifold absolute pressure */
#define OBD_PID_ENGINE_RPM              0x0C    /* Engine RPM */
#define OBD_PID_VEHICLE_SPEED           0x0D    /* Vehicle speed */
#define OBD_PID_TIMING_ADVANCE          0x0E    /* Timing advance */
#define OBD_PID_INTAKE_TEMP             0x0F    /* Intake air temperature */
#define OBD_PID_MAF_RATE                0x10    /* MAF air flow rate */
#define OBD_PID_THROTTLE_POS            0x11    /* Throttle position */
#define OBD_PID_O2_SENSORS_PRESENT      0x13    /* Oxygen sensors present */
#define OBD_PID_O2_SENSOR_1             0x14    /* Oxygen sensor 1 */
#define OBD_PID_OBD_STANDARDS           0x1C    /* OBD standards compliance */
#define OBD_PID_RUN_TIME                0x1F    /* Run time since engine start */
#define OBD_PID_SUPPORTED_PIDS_21_40    0x20    /* Supported PIDs [21-40] */
#define OBD_PID_DISTANCE_MIL            0x21    /* Distance traveled with MIL on */
#define OBD_PID_FUEL_RAIL_PRESSURE      0x22    /* Fuel rail pressure */
#define OBD_PID_FUEL_RAIL_GAUGE         0x23    /* Fuel rail gauge pressure */
#define OBD_PID_COMMANDED_EGR           0x2C    /* Commanded EGR */
#define OBD_PID_EGR_ERROR               0x2D    /* EGR error */
#define OBD_PID_FUEL_LEVEL              0x2F    /* Fuel level input */
#define OBD_PID_DISTANCE_CODES_CLR      0x31    /* Distance since codes cleared */
#define OBD_PID_BAROMETRIC_PRESSURE     0x33    /* Barometric pressure */
#define OBD_PID_CATALYST_TEMP_B1S1      0x3C    /* Catalyst temperature Bank 1, Sensor 1 */
#define OBD_PID_SUPPORTED_PIDS_41_60    0x40    /* Supported PIDs [41-60] */
#define OBD_PID_CONTROL_MODULE_VOLT     0x42    /* Control module voltage */
#define OBD_PID_ABSOLUTE_LOAD           0x43    /* Absolute load value */
#define OBD_PID_COMMANDED_EQUIV_RATIO   0x44    /* Commanded equivalence ratio */
#define OBD_PID_RELATIVE_THROTTLE       0x45    /* Relative throttle position */
#define OBD_PID_AMBIENT_TEMP            0x46    /* Ambient air temperature */
#define OBD_PID_ENGINE_OIL_TEMP         0x5C    /* Engine oil temperature */
#define OBD_PID_FUEL_INJECTION_TIMING   0x5D    /* Fuel injection timing */
#define OBD_PID_ENGINE_FUEL_RATE        0x5E    /* Engine fuel rate */

/* Mode 09 PIDs (Vehicle Information) */
#define OBD_PID_VIN_COUNT               0x01    /* VIN message count */
#define OBD_PID_VIN                     0x02    /* Vehicle Identification Number */
#define OBD_PID_CALIBRATION_ID          0x04    /* Calibration ID */
#define OBD_PID_ECU_NAME                0x0A    /* ECU name */

/* OBD Status Codes */
#define OBD_STATUS_OK                   0
#define OBD_STATUS_ERROR                1
#define OBD_STATUS_TIMEOUT              2
#define OBD_STATUS_NO_RESPONSE          3
#define OBD_STATUS_INVALID_DATA         4

/* OBD-II Response Timeout (ms) */
#define OBD_RESPONSE_TIMEOUT            100

/* Maximum DTC count */
#define OBD_MAX_DTC_COUNT               16

/*******************************************************************************
 *                              Type Definitions                                *
 *******************************************************************************/

/* OBD-II Response Structure */
typedef struct {
    uint8  mode;            /* Response mode (request mode + 0x40) */
    uint8  pid;             /* PID */
    uint8  dataLength;      /* Number of data bytes */
    uint8  data[5];         /* Response data (max 5 bytes for single frame) */
} OBD_Response;

/* Diagnostic Trouble Code Structure */
typedef struct {
    uint8  prefix;          /* P/C/B/U (Powertrain/Chassis/Body/Network) */
    uint8  digit1;          /* First digit (0-3) */
    uint8  digit2;          /* Second digit */
    uint8  digit3;          /* Third digit */
    uint8  digit4;          /* Fourth digit */
} OBD_DTC;

/* Vehicle Data Structure */
typedef struct {
    uint16 engineRPM;           /* Engine RPM */
    uint8  vehicleSpeed;        /* Vehicle speed (km/h) */
    sint8  coolantTemp;         /* Coolant temperature (°C) */
    sint8  intakeTemp;          /* Intake air temperature (°C) */
    uint8  engineLoad;          /* Engine load (%) */
    uint8  throttlePosition;    /* Throttle position (%) */
    uint8  fuelLevel;           /* Fuel level (%) */
    float32 batteryVoltage;     /* Control module voltage (V) */
    uint16 mafRate;             /* MAF rate (g/s * 100) */
} OBD_VehicleData;

/*******************************************************************************
 *                              Function Prototypes                             *
 *******************************************************************************/

/*
 * Description: Initialize OBD-II communication
 * Returns:      OBD_STATUS_OK on success
 */
uint8 OBD_Init(void);

/*
 * Description: Send OBD-II request and receive response
 * Parameters:  mode - OBD service mode
 *              pid - Parameter ID
 *              response - Pointer to store response
 * Returns:     OBD_STATUS_OK on success
 */
uint8 OBD_Request(uint8 mode, uint8 pid, OBD_Response *response);

/*
 * Description: Get engine RPM
 * Parameters:  Pointer to store RPM value
 * Returns:     OBD_STATUS_OK on success
 */
uint8 OBD_GetEngineRPM(uint16 *rpm);

/*
 * Description: Get vehicle speed in km/h
 * Parameters:   Pointer to store speed value
 * Returns:     OBD_STATUS_OK on success
 */
uint8 OBD_GetVehicleSpeed(uint8 *speed);

/*
 * Description: Get engine coolant temperature in Celsius
 * Parameters:  Pointer to store temperature value
 * Returns:     OBD_STATUS_OK on success
 */
uint8 OBD_GetCoolantTemp(sint8 *temp);

/*
 * Description: Get intake air temperature in Celsius
 * Parameters:   Pointer to store temperature value
 * Returns:     OBD_STATUS_OK on success
 */
uint8 OBD_GetIntakeTemp(sint8 *temp);

/*
 * Description: Get calculated engine load percentage
 * Parameters:  Pointer to store load value (0-100%)
 * Returns:     OBD_STATUS_OK on success
 */
uint8 OBD_GetEngineLoad(uint8 *load);

/*
 * Description: Get throttle position percentage
 * Parameters:  Pointer to store throttle position (0-100%)
 * Returns:     OBD_STATUS_OK on success
 */
uint8 OBD_GetThrottlePosition(uint8 *position);

/*
 * Description:  Get fuel level percentage
 * Parameters:  Pointer to store fuel level (0-100%)
 * Returns:     OBD_STATUS_OK on success
 */
uint8 OBD_GetFuelLevel(uint8 *level);

/*
 * Description: Get control module voltage
 * Parameters:  Pointer to store voltage value
 * Returns:     OBD_STATUS_OK on success
 */
uint8 OBD_GetBatteryVoltage(float32 *voltage);

/*
 * Description: Get MAF air flow rate
 * Parameters:  Pointer to store MAF rate (grams/sec * 100)
 * Returns:     OBD_STATUS_OK on success
 */
uint8 OBD_GetMAFRate(uint16 *rate);

/*
 * Description:  Get all common vehicle data at once
 * Parameters:  Pointer to vehicle data structure
 * Returns:     OBD_STATUS_OK on success
 */
uint8 OBD_GetAllVehicleData(OBD_VehicleData *data);

/*
 * Description: Read stored Diagnostic Trouble Codes
 * Parameters:  Array to store DTCs
 *              Pointer to store number of DTCs found
 * Returns:     OBD_STATUS_OK on success
 */
uint8 OBD_ReadDTCs(OBD_DTC *dtcArray, uint8 *dtcCount);

/*
 * Description:  Clear all Diagnostic Trouble Codes
 * Returns:     OBD_STATUS_OK on success
 */
uint8 OBD_ClearDTCs(void);

/*
 * Description: Check if specific PID is supported
 * Parameters:   mode - OBD service mode
 *              pid - Parameter ID to check
 * Returns:     TRUE if supported, FALSE otherwise
 */
boolean OBD_IsPIDSupported(uint8 mode, uint8 pid);

/*
 * Description: Get supported PIDs bitmap
 * Parameters:  mode - OBD service mode
 *              startPID - Starting PID (0x00, 0x20, 0x40, etc.)
 *              supportedPIDs - Pointer to store 32-bit bitmap
 * Returns:     OBD_STATUS_OK on success
 */
uint8 OBD_GetSupportedPIDs(uint8 mode, uint8 startPID, uint32 *supportedPIDs);

/*
 * Description: Convert DTC to string format (e.g., "P0301")
 * Parameters:  dtc - DTC structure
 *              buffer - Character buffer (minimum 6 bytes)
 */
void OBD_DTCToString(const OBD_DTC *dtc, char *buffer);

#endif /* OBD_H_ */
