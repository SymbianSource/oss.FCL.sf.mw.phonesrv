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
*    Observes settings monitor.
*
*
*/


#ifndef CBS_UI_SETTINGS_OBSERVER_H
#define CBS_UI_SETTINGS_OBSERVER_H



// CLASS DECLARATION


/**
*  Observes settings monitor.
*  
*/
class MCbsUiSettingsObserver
    {
    public:

        /**
        * Settings monitor calls this function when settings has been
        * changed.
        *
        * @param aEvent What kind of event caused the settings change
        */
        virtual void SettingsChangedL( TCbsSettingsEvent aEvent ) = 0;

    };

#endif      // CBS_UI_SETTINGS_OBSERVER_H
            
// End of File

