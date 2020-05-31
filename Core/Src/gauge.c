/*
 * gauge.c
 *
 *  Created on: May 30, 2020
 *  Author: Martin Záruba - Mutusoft.com - 05/2020
 */

#define		PI 		3.14159265

#include 	"gauge.h"


int8_t to_int( float value)
{
	value += 0.5;
	return (int8_t)value;
}

uint8_t to_uint( float value)
{
	value += 0.5;
	return (uint8_t)value;
}

void Gauge_DrawInnerArc(Gauge_type* theGauge)
{
	float Y;
	float X;
	float Alfa;
	float to_deg = PI / 180;
	int8_t	prevY;
	int8_t	prevX;

	for ( Alfa = 0; Alfa <= 180; Alfa += 1)
	{
		Y = (theGauge->RadiusIn - 2) * sin(Alfa * to_deg);
		X = (theGauge->RadiusIn - 2) * cos(Alfa * to_deg);

		if ( !(prevX == to_int(X) && prevY == to_int(Y)) ) // ** skip drawing same pixels
		{
			SSD1306_DrawPixel(theGauge->Center_X - to_int(X), theGauge->Center_Y - to_int(Y), 0x01);
			prevX = to_int(X);
			prevY = to_int(Y);
		}
	}
}


void Gauge_ValuePct(uint8_t Value, uint8_t DrawNow, Gauge_type* theGauge)
{
	float Y;
	float X;
	float startX;
	float startY;
	float Alfa;
	float to_deg = PI / 180;

	if (Value > 100) return;

	if (Value > 0) // ** hide previous position of the pointer
	{
		Alfa = (theGauge->Alfa_end - theGauge->Alfa_start) * theGauge->Previous_pct_value / 100 + theGauge->Alfa_start;
		Y = (theGauge->RadiusOut - 3) * sin(Alfa * to_deg);
		X = (theGauge->RadiusOut - 3) * cos(Alfa * to_deg);
		startY = theGauge->RadiusIn * sin((Alfa - 10) * to_deg);
		startX = theGauge->RadiusIn * cos((Alfa - 10) * to_deg);
		SSD1306_DrawLine(theGauge->Center_X - (int8_t)startX, theGauge->Center_Y - (int8_t)startY, theGauge->Center_X - to_int(X), theGauge->Center_Y - to_int(Y), 0x00);
		startY = theGauge->RadiusIn * sin((Alfa + 10)  * to_deg);
		startX = theGauge->RadiusIn * cos((Alfa + 10) * to_deg);
		SSD1306_DrawLine(theGauge->Center_X - (int8_t)startX, theGauge->Center_Y - (int8_t)startY, theGauge->Center_X - to_int(X), theGauge->Center_Y - to_int(Y), 0x00);
	}

	Alfa = (theGauge->Alfa_end - theGauge->Alfa_start) * Value / 100 + theGauge->Alfa_start;
	Y = (theGauge->RadiusOut - 3) * sin(Alfa * to_deg);
	X = (theGauge->RadiusOut - 3) * cos(Alfa * to_deg);
	startY = theGauge->RadiusIn * sin((Alfa - 10) * to_deg);
	startX = theGauge->RadiusIn * cos((Alfa - 10) * to_deg);
	SSD1306_DrawLine(theGauge->Center_X - (int8_t)startX, theGauge->Center_Y - (int8_t)startY, theGauge->Center_X - to_int(X), theGauge->Center_Y - to_int(Y), 0x01);
	startY = theGauge->RadiusIn * sin((Alfa + 10)  * to_deg);
	startX = theGauge->RadiusIn * cos((Alfa + 10) * to_deg);
	SSD1306_DrawLine(theGauge->Center_X - (int8_t)startX, theGauge->Center_Y - (int8_t)startY, theGauge->Center_X - to_int(X), theGauge->Center_Y - to_int(Y), 0x01);

	theGauge->Previous_pct_value = Value;

	if (DrawNow)
		SSD1306_UpdateScreen();
}


void Gauge_Init(uint8_t startX, uint8_t startY,  uint8_t Rout, uint8_t Rin, uint8_t alfaStart, uint8_t alfaEnd, uint8_t tickInterval, uint8_t tickSize, Gauge_type* theGauge)
{
	float dAlfa = 1;
	float Y;
	float X;
	float Ytick;
	float Xtick;
	float to_deg = PI / 180;
	int8_t	prevY;
	int8_t	prevX;

	theGauge->Center_X = startX;
	theGauge->Center_Y = startY;
	theGauge->RadiusOut = Rout;
	theGauge->RadiusIn = Rin;
	theGauge->Alfa_start = alfaStart;
	theGauge->Alfa_end = alfaEnd;
	theGauge->Tick_interval = tickInterval;
	theGauge->Tick_size = tickSize;

	for (float Alfa = alfaStart; Alfa <= alfaEnd; Alfa += dAlfa)
	{
		Y = Rout * sin(Alfa * to_deg);
		X = Rout * cos(Alfa * to_deg);

		if ( !(prevX == to_int(X) && prevY == to_int(Y)) ) // ** skip drawing same pixels
		{
			SSD1306_DrawFilledCircle(startX - to_int(X), startY - to_int(Y), 0x01, 0x01);
			//SSD1306_DrawPixel(startX - to_int(X), startY - to_int(Y), 0x01);
			prevX = to_int(X);
			prevY = to_int(Y);
		}

		if ( !(to_uint(Alfa - theGauge->Alfa_start) % theGauge->Tick_interval) )
		{
			Ytick = ( theGauge->RadiusOut + theGauge->Tick_size) * sin(Alfa * to_deg);
			Xtick = ( theGauge->RadiusOut + theGauge->Tick_size) * cos(Alfa * to_deg);
			SSD1306_DrawLine(theGauge->Center_X - to_int(X), theGauge->Center_Y - to_int(Y), theGauge->Center_X - to_int(Xtick), theGauge->Center_Y - to_int(Ytick), 0x01);
		}
	}
	Gauge_DrawInnerArc(theGauge);
	Gauge_ValuePct(0, 1, theGauge);
}


