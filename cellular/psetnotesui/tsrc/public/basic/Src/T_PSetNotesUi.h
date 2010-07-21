/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Declaration of T_PSetNotesUi class.
*
*/


#ifndef __T_PSETNOTESUI_H__
#define __T_PSETNOTESUI_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>


//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CPsuiContainer;

//  CLASS DEFINITION
/**
 * TODO Auto-generated EUnit test suite
 *
 */
NONSHARABLE_CLASS( T_PSetNotesUi )
	: public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static T_PSetNotesUi* NewL();
        static T_PSetNotesUi* NewLC();
        /**
         * Destructor
         */
        ~T_PSetNotesUi();

    private:    // Constructors and destructors

        T_PSetNotesUi();
        void ConstructL();

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void T_TestL();
         void T_Test2L();
         void T_Test3L();
         void T_Test4L();
         void T_Test5L();
         void T_Test6L();
         void T_Test7L();
        

    private:    // Data
		
        CPsuiContainer* iContainer;
        
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __T_PSETNOTESUI_H__

// End of file
