/*
* Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef HBLINEEDIT_H
#define HBLINEEDIT_H

#include <hbglobal.h>
#include <hbnamespace.h>
#include <hbabstractedit.h>
#include <QFont>

class HbLineEditPrivate;

class HB_WIDGETS_EXPORT HbLineEdit: public QObject
{
    Q_OBJECT

    Q_ENUMS(EchoMode)

    Q_PROPERTY(QString text READ text WRITE setText USER true)
    Q_PROPERTY(QString displayText READ displayText)
    Q_PROPERTY(int maxLength READ maxLength WRITE setMaxLength)
    Q_PROPERTY(bool hasSelectedText READ hasSelectedText)
    Q_PROPERTY(QString selectedText READ selectedText)
    Q_PROPERTY(int selectionStart READ selectionStart)
    Q_PROPERTY(int maxRows READ maxRows WRITE setMaxRows)
    Q_PROPERTY(int minRows READ minRows WRITE setMinRows)
    Q_PROPERTY(bool expandable READ isExpandable)
    Q_PROPERTY(EchoMode echoMode READ echoMode WRITE setEchoMode)

public:

    explicit HbLineEdit(QGraphicsItem *parent = 0);
    explicit HbLineEdit(const QString &text, QGraphicsItem *parent = 0);
    virtual ~HbLineEdit();

    enum {
        Type = Hb::ItemType_LineEdit
    };

    int type() const;

    int maxLength() const;
    void setMaxLength(int length);

    void setMinRows(int rows);
    int minRows() const;

    void setMaxRows(int rows);
    int maxRows() const;

    bool isExpandable() const;

    QString text() const;
    QString displayText() const;

    bool hasSelectedText() const;
    QString selectedText() const;
    int selectionStart() const;
    void setSelection(int start, int length);

    int cursorPosition() const;
    void setCursorPosition(int pos);

    enum EchoMode { Normal, NoEcho, Password, PasswordEchoOnEdit };
    EchoMode echoMode() const;
    void setEchoMode(EchoMode);

    void setCapitalization ( QFont::Capitalization caps );
    QFont::Capitalization capitalization () const;

public slots:
    void setText(const QString &text);

signals:
    void editingFinished();
    void textChanged(const QString &text);
    void selectionChanged();

protected:
    HbLineEdit(HbLineEditPrivate &dd, QGraphicsItem *parent);

    void inputMethodEvent(QInputMethodEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void resizeEvent(QGraphicsSceneResizeEvent *event);

    bool canInsertFromMimeData(const QMimeData *source) const;
    void insertFromMimeData(const QMimeData *source);

    void focusOutEvent ( QFocusEvent * event );
    void focusInEvent ( QFocusEvent * event );
private:
    Q_DISABLE_COPY(HbLineEdit)
    //Q_DECLARE_PRIVATE_D(d_ptr, HbLineEdit)
    Q_PRIVATE_SLOT(d_func(), void _q_textChanged())
};

#endif // HBLINEEDIT_H
