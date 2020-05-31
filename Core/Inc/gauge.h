/*
 * gauge.h
 *
 *  Created on: May 30, 2020
 *      Author: Martin Záruba - Mutusoft.com - 05/2020
 */

#ifndef INC_GAUGE_H_
#define INC_GAUGE_H_

#include 	"stdlib.h"
#include 	"ssd1306.h"
#include	"math.h"


typedef struct Gauge_struct {

	uint8_t Center_X;
	uint8_t Center_Y;
	uint8_t RadiusIn;
	uint8_t RadiusOut;
	uint8_t Alfa_start;
	uint8_t Alfa_end;
	uint8_t Previous_pct_value;
	uint8_t Tick_interval;
	uint8_t Tick_size;

} Gauge_type;


void Gauge_ValuePct(uint8_t Value, uint8_t DrawNow, Gauge_type* theGauge);
void Gauge_Init(uint8_t startX, uint8_t startY,  uint8_t Rout, uint8_t Rin, uint8_t alfaStart, uint8_t alfaEnd, uint8_t tickInterval, uint8_t tickSize, Gauge_type* theGauge);

#endif /* INC_GAUGE_H_ */
