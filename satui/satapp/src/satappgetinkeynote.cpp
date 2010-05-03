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
*
*/

#include "tflogger.h"
#include "satappgetinkeynote.h"

// ======== MEMBER FUNCTIONS ==================================================

// ----------------------------------------------------------------------------
// SatAppGetInkeyNote::SatAppGetInkeyNote
// (Constructor).
// ----------------------------------------------------------------------------
//
SatAppGetInkeyNote::SatAppGetInkeyNote(const QString &text,
        MessageBoxType type, QGraphicsItem *parent):
        HbMessageBox(text, type, parent)
{
    TFLOGSTRING("SATAPP: SatAppGetInkeyNote::SatAppGetInkeyNote call - exit")
}

// ----------------------------------------------------------------------------
// SatAppGetInkeyNote::~SatAppGetInkeyNote
// (Destructor).
// ----------------------------------------------------------------------------
//
SatAppGetInkeyNote::~SatAppGetInkeyNote()
{
    TFLOGSTRING("SATAPP: SatAppGetInkeyNote::~SatAppGetInkeyNote call - exit")
}


// ----------------------------------------------------------------------------
// SatAppGetInkeyNote::keyPressEvent
// (keyPressEvent).
// ----------------------------------------------------------------------------
//
void SatAppGetInkeyNote::keyPressEvent(QKeyEvent *event)
{
    TFLOGSTRING("SATAPP: SatAppGetInkeyNote::keyPressEvent")
    switch (event->key())
    {
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
    case Qt::Key_Plus:
    case Qt::Key_Asterisk:
    case Qt::Key_NumberSign:
        {
        TFLOGSTRING("SATAPP: SatAppGetInkeyNote::keyPressEvent digits key")
        emit digitalKeyPressed(event->key());
        }
        break;
    default :
        {
        HbMessageBox::keyPressEvent(event);
        break;
        }
    }
    TFLOGSTRING2("SATAPP: SatAppGetInkeyNote::keyPressEvent key %d", \
        event->key())
    TFLOGSTRING("SATAPP: SatAppGetInkeyNote::keyPressEvent exit")
}

//End of file
