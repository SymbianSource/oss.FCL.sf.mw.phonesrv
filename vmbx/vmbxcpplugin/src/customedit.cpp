                         /*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
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

#include <QGraphicsSceneMouseEvent>

#include "customedit.h"
#include "loggerutil.h"

/*!
    Constructor
*/
CustomEdit::CustomEdit(QGraphicsItem *parent)
    : HbLineEdit(parent)
{
    _DBGLOG("CustomEdit::CustomEdit()")
}

/*!
    Distructor
*/
CustomEdit::~CustomEdit()
{
    _DBGLOG("CustomEdit::CustomEdit()")
}

/*!
    mousePressEvent
*/
void CustomEdit::focusInEvent(QFocusEvent *event)
{
    _DBGLOG("CustomEdit::focusInEvent() >")
    if (!event->gotFocus()) {
        event->ignore();
        _DBGLOG("CustomEdit::focusInEvent() ignore")
        return;
    } else {
        emit editItemClicked();
        event->accept();
        _DBGLOG("CustomEdit::focusInEvent() emit clicked")
    }
    _DBGLOG("CustomEdit::focusInEvent() <")
}
    

// End of file
