/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*    Provides macros for testing.
*
*
*/


#ifndef CBS_UI_DEBUG_H
#define CBS_UI_DEBUG_H


////////////////////////////
// MEMORY TEST MACRO     //


#ifdef _DEBUG
#define __CBSUI_DEBUG
#endif // _DEBUG

#ifdef __CBSUI_DEBUG
    #define __CBSUI_ASSERT(condition, function) \
            __ASSERT_DEBUG(condition, function)
    #define __CBSUI_ASSERT_E(condition, error) \
            __ASSERT_DEBUG(condition, CbsUiPanic(error) )
#else
    #define __CBSUI_ASSERT(condition, function)
    #define __CBSUI_ASSERT_E(condition, error)
#endif //__CBSUI_DEBUG


#endif // CBS_UI_DEBUG_H

// End of File
