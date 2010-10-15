/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 *
 */

#ifndef PSUICLINOTEHANDLER_H_
#define PSUICLINOTEHANDLER_H_

#include <QObject>
#include <QList>
#include <psetcliwrapper.h>

class PSetCliWrapper;

#ifdef BUILD_PSUINOTES
#define PSUINOTES_EXPORT Q_DECL_EXPORT
#else
#define PSUINOTES_EXPORT Q_DECL_IMPORT
#endif

class PSUINOTES_EXPORT PsUiCliNoteHandler : public QObject
{
    Q_OBJECT
    
public:
    PsUiCliNoteHandler(PSetCliWrapper& cliWrapper);
    ~PsUiCliNoteHandler();

public slots: // Slots: 

    void handleCliRequesting( bool ongoing, bool interrupted ); 

    void cliInformation( const PsCallLineIdentity& type );

    void handleCliError( int error );
    
    void handleCnapStatus( int status );

private:   // data
   
    // For cli signals
    PSetCliWrapper& m_cliWrapper;
     
    // Current active note id 
    int m_activeNoteId;
    
    // Current progress note id
    int m_activeProgressNoteId;
   
};

#endif /* PSUICLINOTEHANDLER_H_ */
