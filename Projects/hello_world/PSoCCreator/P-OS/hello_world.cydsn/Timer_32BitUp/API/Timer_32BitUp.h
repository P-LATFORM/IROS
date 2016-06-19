/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <cytypes.h>

#if !defined(`$INSTANCE_NAME`_H)
#define `$INSTANCE_NAME`_H

void `$INSTANCE_NAME`_Start(void);
void `$INSTANCE_NAME`_Stop(void);
void `$INSTANCE_NAME`_Reset();
uint32 `$INSTANCE_NAME`_ReadCounter();
void `$INSTANCE_NAME`_SetCompareValue(uint32 value);

#endif
/* [] END OF FILE */
