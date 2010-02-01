/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Provides macros for logging
*
*/



#ifndef __CSSLOGGER_H__
#define __CSSLOGGER_H__

//  INCLUDES
#include <e32def.h>


/* 
 * CodeScanner warnings "codingstandarts: use of #if in .h files
 * (not as main include guards)".
 * Warnings ignored from the logger file.
 */
#ifndef _DEBUG

/***************************
* Logging method variants:
* 0 = No logging
* 1 = Flogger
* 2 = RDebug
***************************/ 

#define CSS_LOGGING_METHOD      0   // UREL BUILD

#else

#ifdef __WINS__
#define CSS_LOGGING_METHOD      2   // UDEB BUILD, WINS
#else
#define CSS_LOGGING_METHOD      2   // UDEB BUILD, HW
#endif // __WINS__

#endif // _DEBUG

/* 
-----------------------------------------------------------------------------

    LOG SETTINGS

-----------------------------------------------------------------------------
*/

#if CSS_LOGGING_METHOD == 1      // Flogger

#include <flogger.h>
_LIT(KTfLogFolder,"CSS");
_LIT(KTfLogFile,"CSSLOG.TXT");

#elif CSS_LOGGING_METHOD == 2    // RDebug

#include <e32svr.h>

#endif


/*
-----------------------------------------------------------------------------

    LOGGING MACROs

    USE THESE MACROS IN YOUR CODE !

-----------------------------------------------------------------------------
*/

#if CSS_LOGGING_METHOD == 1      // Flogger

#define CSSLOGTEXT(AAA)            { \
                                            RFileLogger::Write(KTfLogFolder(),KTfLogFile(), \
                                            EFileLoggingModeAppend, AAA); \
                                        }
#define CSSLOGSTRING(AAA)          { \
                                            _LIT(tempLogDes,AAA); \
                                            RFileLogger::Write(KTfLogFolder(),KTfLogFile(), \
                                            EFileLoggingModeAppend,tempLogDes()); \
                                        }
#define CSSLOGSTRING2(AAA,BBB)     { \
                                            _LIT(tempLogDes,AAA); \
                                            RFileLogger::WriteFormat(KTfLogFolder(),KTfLogFile(), \
                                            EFileLoggingModeAppend, \
                                            TRefByValue<const TDesC>(tempLogDes()),BBB); \
                                        }
#define CSSLOGSTRING3(AAA,BBB,CCC) { \
                                            _LIT(tempLogDes,AAA); \
                                            RFileLogger::WriteFormat(KTfLogFolder(),KTfLogFile(), \
                                            EFileLoggingModeAppend, \
                                            TRefByValue<const TDesC>(tempLogDes()),BBB,CCC); \
                                        }
#define CSSLOGSTRING4(AAA,BBB,CCC,DDD) { \
                                            _LIT(tempLogDes,AAA); \
                                            RFileLogger::WriteFormat(KTfLogFolder(), \
                                            KTfLogFile(),EFileLoggingModeAppend, \
                                            TRefByValue<const TDesC>(tempLogDes()), \
                                            BBB,CCC,DDD); \
                                        } 


#elif CSS_LOGGING_METHOD == 2    // RDebug

#define CSSLOGTEXT(AAA)                     RDebug::Print(AAA);

#define CSSLOGSTRING(AAA)                   RDebug::Print(_L(AAA));

#define CSSLOGSTRING2(AAA,BBB)              RDebug::Print(_L(AAA),BBB); 

#define CSSLOGSTRING3(AAA,BBB,CCC)          RDebug::Print(_L(AAA),BBB,CCC); 

#define CSSLOGSTRING4(AAA,BBB,CCC,DDD)      RDebug::Print(_L(AAA),BBB,CCC,DDD); 



#else   // CSS_LOGGING_METHOD == 0 or invalid

#define CSSLOGTEXT(AAA)              
#define CSSLOGSTRING(AAA)            
#define CSSLOGSTRING2(AAA,BBB)      
#define CSSLOGSTRING3(AAA,BBB,CCC)  
#define CSSLOGSTRING4(AAA,BBB,CCC,DDD)   

#endif  // CSS_LOGGING_METHOD


#endif  // CSSLOGGER_H
