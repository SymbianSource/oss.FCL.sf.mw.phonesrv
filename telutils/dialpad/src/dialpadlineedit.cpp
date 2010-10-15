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

// System includes
#include <hbevent.h>
#include <hbapplication.h>

// User includes
#include "dialpadlineedit.h"


// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========
/*!
     Constructor.
 */
DialpadLineEdit::DialpadLineEdit (QGraphicsItem *parent) :
    HbLineEdit(parent)
{
}

/*!
     Destructor.
 */
DialpadLineEdit::~DialpadLineEdit ()
{
}

/*!
    \reimp
 */
void DialpadLineEdit::focusOutEvent(QFocusEvent * event)
{
    HbWidget::focusOutEvent(event);
    event->accept();
}

/*!
    \reimp
 */
void DialpadLineEdit::focusInEvent(QFocusEvent * event)
{
    HbWidget::focusInEvent(event);
    event->accept();
    HbEvent focusEvent(HbEvent::InputMethodFocusIn);
    HbApplication::sendEvent(this, &focusEvent);
}
