#ifndef SENSORTCRT_H
#define SENSORTCRT_H

#include <Arduino.h>

/**
 * @file sensorctrl.h
 * @brief Header file for sensor control in an Arduino project.
 */

#define SENSOR_COUNT 8 /**< Number of sensors connected */
#define WHITE_LINE 1   /**< Value representing a white line */
#define BLACK_LINE 0   /**< Value representing a black line */

/*______________________________________SENSORS______________________________________________________*/

/**
 * @brief Array storing sensor positions.
 * 
 * - 0: Leftmost sensor
 * - 7: Rightmost sensor
 */
extern const byte posSensor[];

/**
 * @brief Array storing sensor states.
 * 
 * - 0: Black
 * - 1: White
 */
extern bool s[];

/**
 * @brief Integer storing sensor states.
 * - Bitwise representation of sensor states.
 * - Updated by readSensors() function.
 * @note Example usage of dataSensor:
 * - All white: dataSensor = 0b00000000
 * - All black: dataSensor = 0b11111111
 * - Line in center: dataSensor = 0b00011000
 * @see readSensors()
 */
extern int dataSensor;

/**
 * @brief Number of sensors detecting line color.
 * - Updated by readSensors() function.
 * 
 * @see readSensors()
 */
extern int cnt;

/**
 * @brief Sum representing line position.
 * 
 * - Ranges from 0 (left) to 7 (right).
 * - Updated by readSensors() function.
 * 
 * @see readSensors()
 */
extern float somme;

/**
 * @brief Last sum of readings when line was detected.
 * 
 * - Updated by readSensors() function.
 * 
 * @see readSensors()
 */
extern float lastOnLineSomme;

/**
 * @brief Flag indicating line detection.
 * 
 * - Updated by readSensors() function.
 * 
 * @see readSensors()
 */
extern bool onLine;

/**
 * @brief Flag indicating line color.
 * 
 * - 0: Black
 * - 1: White
 */
extern bool lineColor;

/**
 * @brief Initializes sensors.
 * 
 * - Call once at program start.
 */
void setupSensors();

/**
 * @brief Reads sensor data.
 * 
 * - Updates relevant variables.
 * - Call periodically.
 */
void readSensors();

/**
 * @brief Prints sensor data.
 * 
 * - For debugging.
 */
void printSensors();

#endif
