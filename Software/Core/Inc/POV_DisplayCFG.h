
/*******************************************************************************
 *  [FILE NAME]   :      <POV_DisplayCFG.h>                                    *
 *  [AUTHOR]      :      <David S. Alexander>                                  *
 *  [DATE CREATED]:      <Jan 19, 2024>                                        *
 *  [Description} :      <Header file for POV Display driver Configuration>    *
 *******************************************************************************/

#ifndef INC_POV_DISPLAYCFG_H_
#define INC_POV_DISPLAYCFG_H_


/*******************************************************************************
 *                             Macro Declarations                              *
 *******************************************************************************/

/* Timer used for Input Capture (Period Measuring) */
#define ICUTIM            htim2

/* Timer used for Intervals display */
#define DISPTIM           htim3

#define PIXELS            (8U)
#define RESOLUTION        (200U)

#define COURIER           (7U)
#define FONT8x5           (5U)

#define FONT              FONT8x5
#define FONTSIZE          FONT

#endif /* INC_POV_DISPLAYCFG_H_ */
