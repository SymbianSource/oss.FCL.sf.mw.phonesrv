/*!
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
* Description: Dialpad popup
*
*/

#ifndef DIALPAD_H
#define DIALPAD_H

#include <hbwidget.h>
#include <QTime>
#include <QTimeLine>

class HbLineEdit;
class HbFrameDrawer;
class DialpadInputField;
class DialpadKeypad;
class DialpadMultitapHandler;

#ifdef BUILD_DIALPAD
#define DIALPAD_EXPORT Q_DECL_EXPORT
#else
#define DIALPAD_EXPORT Q_DECL_IMPORT
#endif

class DIALPAD_EXPORT Dialpad : public HbWidget
{
    Q_OBJECT

public:
    explicit Dialpad();
    virtual ~Dialpad();

    HbLineEdit& editor() const;

public slots:
    void openDialpad();

    void closeDialpad();

    void setCallButtonEnabled(bool enabled);

protected:
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget);
               
    bool sceneEvent(QEvent *event);

    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);

    bool handleSceneEvent(QEvent *event);

    void showEvent(QShowEvent *event);

    void hideEvent(QHideEvent *event);

    void closeEvent(QCloseEvent * event);

protected slots:
    void closeAnimValueChanged(qreal value);
    void closeAnimFinished();
    void openAnimValueChanged(qreal value);
    void openAnimFinished();
    void orientationChangeStarted();
    void orientationChangeFinished();

signals:
    void aboutToClose();

private:
    HbFrameDrawer *mBackgroundDrawer;
    HbFrameDrawer *mIconDrawer;
    DialpadInputField* mInputField;
    DialpadKeypad* mKeypad;
    DialpadMultitapHandler* mMultitap;
    int mMouseButtonPressedDown;
    QTime mSwipeTimer;
    QTimeLine mOpenTimeLine;
    QTimeLine mCloseTimeLine;
    bool mAnimationOngoing;
    QPointF mPosition;
    int mCloseHandleHeight;
};

#endif // DIALPAD_H
