//*****************************************************************************
//
// ringbuf.h - Defines and Macros for the ring buffer utilities.
//
// Copyright (c) 2008 Luminary Micro, Inc.  All rights reserved.
// 
// Software License Agreement
// 
// Luminary Micro, Inc. (LMI) is supplying this software for use solely and
// exclusively on LMI's microcontroller products.
// 
// The software is owned by LMI and/or its suppliers, and is protected under
// applicable copyright laws.  All rights are reserved.  You may not combine
// this software with "viral" open-source software in order to form a larger
// program.  Any use in violation of the foregoing restrictions may subject
// the user to criminal sanctions under applicable laws, as well as to civil
// liability for the breach of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
// CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2692 of the Stellaris Peripheral Driver Library.
//
//*****************************************************************************

#ifndef __RINGBUF_H__
#define __RINGBUF_H__

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
    
    
//*****************************************************************************
//
// The ASSERT macro, which does the actual assertion checking.  Typically, this
// will be for procedure arguments.
//
//*****************************************************************************
#ifdef USE_FULL_ASSERT
extern void assert_failed(uint8_t* file, uint32_t line);
#define ASSERT(expr) {                                      \
                         if(!(expr))                        \
                         {                                  \
                             assert_failed(__FILE__, __LINE__); \
                         }                                  \
                     }
#else
#define ASSERT(expr)
#endif
	
	
#define tBoolean		bool	

//*****************************************************************************
//
//! \addtogroup ringbuf_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! The structure used for encapsulating all the items associated with a
//! ring buffer.
//
//*****************************************************************************
typedef struct
{
    //
    //! The ring buffer size.
    //
    uint16_t ulSize;

    //
    //! The ring buffer write index.
    //
    uint16_t ulWriteIndex;

    //
    //! The ring buffer read index.
    //
    uint16_t ulReadIndex;
    
    //
    //! The ring buffer single size.
    //
    uint16_t singleSize;

    //
    //! The ring buffer.
    //
    uint8_t *pucBuf;
}
tRingBufObject;

//*****************************************************************************
//
// API Function prototypes
//
//*****************************************************************************
extern tBoolean RingBufFull(tRingBufObject *ptRingBuf);
extern tBoolean RingBufEmpty(tRingBufObject *ptRingBuf);
extern void RingBufFlush(tRingBufObject *ptRingBuf);
extern uint16_t RingBufUsed(tRingBufObject *ptRingBuf);
extern uint16_t RingBufFree(tRingBufObject *ptRingBuf);
extern uint16_t RingBufSize(tRingBufObject *ptRingBuf);
extern uint16_t RingBufSingleSize(tRingBufObject *ptRingBuf);
extern void RingBufReadOne(tRingBufObject *ptRingBuf,void *pucData);
extern void RingBufRead(tRingBufObject *ptRingBuf, void *pucData,
                        uint16_t ulLength);
extern void RingBufWriteOne(tRingBufObject *ptRingBuf, void * pucData);
extern void RingBufWrite(tRingBufObject *ptRingBuf, void *pucData,
                         uint16_t ulLength);
extern void RingBufInit(tRingBufObject *ptRingBuf, void *pucBuf,
                        uint16_t singleSize,uint16_t ulSize);

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

#ifdef __cplusplus
}
#endif

#endif //  __RINGBUF_H__
