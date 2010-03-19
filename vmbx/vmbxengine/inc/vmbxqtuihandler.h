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
* Description:  Hanlder Ui which implement of qt.
*  Interface   : Private, VmbxQtUiHandler
*
*/


#ifndef VMBXQTUIHANDLER_H
#define VMBXQTUIHANDLER_H

// INCLUDES
#include <QObject>
#include <voicemailboxdefs.h>

// FORWARD DECLARATION

 
// CLASS DECLARATION

/**
*  Dialog manager
*
*  @lib vmbxengine.lib
*  @since  S60 v5.2
*/
NONSHARABLE_CLASS( VmbxQtUiHandler ): public QObject
    {

public: // Constructors and destructor

    /**
    * C++ default constructor.
    *
    * @since S60 v5.2
    */
    VmbxQtUiHandler(QObject* parent = 0 );

    /**
    * Destructor.
    */
     ~VmbxQtUiHandler();

    /**
    * Show mailbox query dialog
    *
    * @since S60 v5.2
    * @param aType in TVmbxType type
    * @param aMode Tin VmbxQueryMode mode
    * @param aNumber the voice mailbox number
    * @param aResult out the result of dialog
    */
    void showVmbxQueryDialog(const TVmbxType& aType,
                                const TVmbxQueryMode& aMode,
                                QString& aNumber, int& aResult);

    /**
     * Show define number in selection dialog
     * Leaves if user cancel selected.
     *
     * @since S60 v5.2
     * @param in aType vmbx type
     * @param out aResult the result of dialog
     */
    void showDefineSelectionDialog(TVmbxType& aType, int& aResult);

    /**
    * Show voice mailbox information number
    *
    * @since S60 v5.2
    * @param aNoteType in TVmbxType type
    */
    void showInformationNote(int aNoteType);

    /**
    * Show save empty number note
    *
    * @since S60 v5.2
    * @param aType in TVmbxType type
    */
    void showSaveEmptyNote(const TVmbxType& aType);
    };

#endif  // VMBXQTUIHANDLER_H
