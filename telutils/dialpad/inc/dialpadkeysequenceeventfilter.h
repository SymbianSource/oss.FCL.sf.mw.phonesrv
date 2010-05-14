/*!
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
* Description: Implements key sequence recognition filter for Dialpad.
*
*/

#ifndef DIALPADKEYSEQUENCEEVENTFILTER_H
#define DIALPADKEYSEQUENCEEVENTFILTER_H

#include <qobject>

class Dialpad;

/*!
    DialpadKeySequenceEventFilter
    Class provides key sequence recognition and handling.
*/
class DialpadKeySequenceEventFilter : public QObject
{
    Q_OBJECT

public:
    explicit DialpadKeySequenceEventFilter(
        Dialpad* dialpad, 
        QObject* parent = 0);
    virtual ~DialpadKeySequenceEventFilter();

public:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    /*!
       \fn bool preValidateKeySequence()

       Checks if the given sequence conforms key sequence command syntax.
       
       \return true if sequence conforms key sequence command syntax, 
       false otherwise.  
    */
    bool preValidateKeySequence(const QString &sequence);
    
private:
    /*! Dialpad. Not own. */
    Dialpad* mDialpad;
};

#endif // DIALPADKEYSEQUENCEEVENTFILTER_H
