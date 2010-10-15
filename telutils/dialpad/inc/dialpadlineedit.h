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


#ifndef DIALPADLINEEDIT_H
#define DIALPADLINEEDIT_H

// System includes
#include <hblineedit.h>

// Class declaration
class DialpadLineEdit : public HbLineEdit
{
    Q_OBJECT

public:
    explicit DialpadLineEdit(QGraphicsItem *parent = 0);

    ~DialpadLineEdit();

protected:
    void focusOutEvent(QFocusEvent * event);
    void focusInEvent(QFocusEvent * event);

};

#endif // DIALPADLINEEDIT_H
