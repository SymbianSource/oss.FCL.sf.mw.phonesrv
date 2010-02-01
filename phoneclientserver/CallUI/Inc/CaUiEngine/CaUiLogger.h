/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Logger headerfile for CallUi
*
*/



#ifndef CAUILOGGER_H
#define CAUILOGGER_H

/* 
-----------------------------------------------------------------------------
    LOG SELECTION
-----------------------------------------------------------------------------
*/
#ifndef _DEBUG
    // UREL BUILD:
    #define CAUI_LOGGING_METHOD  0   // No logging in UREL builds
#else
    // UDEB BUILD:
    #define CAUI_LOGGING_METHOD  1   // 0 = No logging, 
                                        // 1 = Flogger, 
                                        // 2 = RDebug
#endif //_DEBUG


#if CAUI_LOGGING_METHOD > 0
    #define CAUI_LOGGING_ENABLED     // This is for backward compatibility
#endif //CAUI_LOGGING_METHOD 

/* 
-----------------------------------------------------------------------------
    FLOGGER SETTINGS
-----------------------------------------------------------------------------
*/
#if CAUI_LOGGING_METHOD == 1      // Flogger

    #include <flogger.h>

    _LIT(KCAUILogFolder, "TF");
    _LIT(KCAUILogFile, "TFLOG.TXT");

#elif CAUI_LOGGING_METHOD == 2    // RDebug

    #include <e32svr.h>

#endif //CAUI_LOGGING_METHOD


/*
-----------------------------------------------------------------------------
    LOGGING MACROs

    USE THESE MACROS IN YOUR CODE !
-----------------------------------------------------------------------------
*/


#if CAUI_LOGGING_METHOD == 1      // Flogger

#define CAUILOGTEXT(AAA) /*lint -save -e960 */              { RFileLogger::Write(KCAUILogFolder(),KCAUILogFile(),EFileLoggingModeAppend,AAA); } /*lint -restore */
#define CAUILOGSTRING(AAA) /*lint -save -e960 */           { _LIT(tempLogDes,AAA); RFileLogger::Write(KCAUILogFolder(),KCAUILogFile(),EFileLoggingModeAppend,tempLogDes()); } /*lint -restore */
#define CAUILOGSTRING2(AAA,BBB) /*lint -save -e960 -e437 */       { _LIT(tempLogDes,AAA); RFileLogger::WriteFormat(KCAUILogFolder(),KCAUILogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(tempLogDes()),BBB); } /*lint -restore */
#define CAUILOGSTRING3(AAA,BBB,CCC) /*lint -save -e960 */   { _LIT(tempLogDes,AAA); RFileLogger::WriteFormat(KCAUILogFolder(),KCAUILogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(tempLogDes()),BBB,CCC); } /*lint -restore */

#elif CAUI_LOGGING_METHOD == 2    // RDebug

#define CAUILOGTEXT(AAA)              RDebug::Print(AAA);
#define CAUILOGSTRING(AAA)            RDebug::Print(_L(AAA));
#define CAUILOGSTRING2(AAA,BBB)       RDebug::Print(_L(AAA),BBB);
#define CAUILOGSTRING3(AAA,BBB,CCC)   RDebug::Print(_L(AAA),BBB,CCC);

#else    // CAUI_LOGGING_METHOD == 0 or invalid

// Example: CAUILOGTEXT(own_desc);
#define CAUILOGTEXT(AAA)              
// Example: CAUILOGSTRING("Test");
#define CAUILOGSTRING(AAA)            
// Example: CAUILOGSTRING("Test %i", aValue);
#define CAUILOGSTRING2(AAA,BBB)       
// Example: CAUILOGSTRING("Test %i %i", aValue1, aValue2);
#define CAUILOGSTRING3(AAA,BBB,CCC)   

#endif  // CAUI_LOGGING_METHOD

#endif    // CAUILOGGER_H
            
// End of File
