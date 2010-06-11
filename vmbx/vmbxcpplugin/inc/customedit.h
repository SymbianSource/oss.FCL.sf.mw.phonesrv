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

#ifndef CUSTOMEDIT_H
#define CUSTOMEDIT_H

#include <hblineedit.h>

class CustomEdit : public HbLineEdit
{
    Q_OBJECT
public:

    /*!
        Constructor
    */
    explicit CustomEdit(QGraphicsItem *parent = 0);

    /*!
        Distructor
    */
    virtual ~CustomEdit();

    /*!
        Rewriten,from base class HbLineEdit
    */    
    void focusInEvent ( QFocusEvent * event );

signals:

    /*!
        Click line edit
    */
    void editItemClicked();
};

#endif //CUSTOMEDIT_H

