/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class for dealing with Emergency Call
*  Interface   : Private, CVmbxEmergencyCall
*
*/


#ifndef C_VMBXEMERGENCYCALL_H
#define C_VMBXEMERGENCYCALL_H

// INCLUDES
#include <e32base.h>
#include <PhCltTypes.h>         // from PhoneClient
#include <MPhCltEmergencyCallObserver.h>

// CLASS DECLARATION

/**
*  CVmbxEmergencyCall declaration
*
*  @lib vmbxengine.lib
*  @since S60 v5.2
*/
NONSHARABLE_CLASS( CVmbxEmergencyCall ) : public CBase,
                        public MPhCltEmergencyCallObserver
    {
 public:

    /**
     * Two-phased constructor
     * @since S60 v5.2
     */
    static CVmbxEmergencyCall* NewL();
    
    /**
     * Destructor.
     * @since S60 v5.2
     */
    ~CVmbxEmergencyCall();

    /**
     * Verifies that the given number is not an emergency number.
     *
     * @since S60 v5.2
     * @param aNumber Mailbox number
     * @return ETrue if emergency number
     */
    TBool IsEmergencyNumber( const TDesC& aNumber );
    
private:  // New Functions

    /**
     * C++ default constructor.
     *
     * @since S60 v5.2
     */
    CVmbxEmergencyCall();

    /**
     * C++  Two-phased constructor.
     * @since S60 v5.2
     */
    void ConstructL();

private: // Functions from base classes

    /**
     * Handle dial request completion.
     *
     * @see MPhCltExtPhoneObserver::HandleDialL().
     *
     * @since S60 v5.2
     * @param aEntry Reference to store handler
     */
    void HandleDialL( const TInt aStatus );

    /**
     * Handling of emergency dial result.
     *
     * @since S60 v5.2
     * @param aEntry Reference to store handler
     */
    void HandleEmergencyDialL( const TInt aStatus );
 
    };

#endif  // C_VMBXEMERGENCYCALL_H
