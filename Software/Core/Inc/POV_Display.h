
/*******************************************************************************
 *  [FILE NAME]   :      <POV_Display.h>                                       *
 *  [AUTHOR]      :      <David S. Alexander>                                  *
 *  [DATE CREATED]:      <Jan 19, 2024>                                        *
 *  [Description} :      <Header file for POV Display driver>                  *
 *******************************************************************************/

#ifndef INC_POV_DISPLAY_H_
#define INC_POV_DISPLAY_H_

/*******************************************************************************
 *                                  Includes                                   *
 *******************************************************************************/
#include "stm32f1xx_hal.h"
#include "POV_DisplayCFG.h"

/*******************************************************************************
 *                             Macro Declarations                              *
 *******************************************************************************/
#define ON      (0x01)
#define OFF     (0x00)

/*******************************************************************************
 *                            Data Types Declaration                           *
 *******************************************************************************/
typedef struct
{
	GPIO_TypeDef *POV_Ports[PIXELS];
	uint16_t      POV_Pins [PIXELS];
}POV_Pins_t;

/*******************************************************************************
 *                              External Variables                             *
 *******************************************************************************/
extern const POV_Pins_t   POV_Pins;
extern const uint8_t      POV_Font[][FONTSIZE];
extern TIM_HandleTypeDef  ICUTIM;
extern TIM_HandleTypeDef  DISPTIM;
extern uint16_t           ICU_TIM_OVC;
extern uint8_t            POVDigits;

/*******************************************************************************
 *                             Functions Declaration                           *
 *******************************************************************************/

void POV_Init(void);
void POV_WriteChar(uint8_t Chr);
void POV_WriteCharInPos(uint8_t Chr, uint8_t Pos);
void POV_SetCursor(uint8_t Pos);
void POV_Clear(void);
void POV_WritePixel(uint8_t Row, uint8_t Column, uint8_t State);
void POV_InvertDisplay(void);
void POV_WriteString(const uint8_t *Str);
void POV_WriteStringInPos(const uint8_t *Str, uint8_t Pos);
void POV_DrawBitmap(const uint8_t *MyBitmap, uint8_t BitmapSize);
void POV_DrawFrame(uint8_t Column1, uint8_t Row1, uint8_t Row2, uint8_t Column2);
void POV_DrawLine(uint8_t Column1, uint8_t Row1, uint8_t Column2, uint8_t Row2);
void POV_DrawTriangle(uint8_t Column1, uint8_t Row1, uint8_t Column2, uint8_t Row2, uint8_t Column3, uint8_t Row3);
void POV_WriteColumn(uint8_t Column, uint8_t Value);
void POV_MeasurePeriod(void);
void POV_ControlFrames(void);
void POV_WriteInteger(int32_t Num);
void POV_WriteIntegerInPos(int32_t Num, uint8_t Pos);

uint16_t POV_MeasureRPM(void);
uint8_t  POV_ReadColumn(uint8_t Column);
uint8_t  POV_ReadPixel(uint8_t Row, uint8_t Column);

#endif /* INC_POV_DISPLAY_H_ */
