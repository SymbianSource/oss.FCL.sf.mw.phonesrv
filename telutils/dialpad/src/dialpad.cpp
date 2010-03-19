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

#include <QtGui>

#include <hbframedrawer.h>
#include <hbinstance.h>
#include <hbeffect.h>
#include <hbinstance.h>
#include <hbmainwindow.h>
#include <hbstyleloader.h>
#include <hblineedit.h>
#include <hbapplication.h>

#include "dialpad.h"
#include "dialpadinputfield.h"
#include "dialpadkeypad.h"
#include "dialpadbutton.h"
#include "dialpadmultitaphandler.h"

static const QString backgroundGraphics(":/inputmethods/qtg_fr_input_bg");
static const QString minimizeIcon(":/inputmethods/qtg_mono_input_down");
static const qreal DialpadCloseSwipeDistance = 0.25; //compared to total height
static const int DialpadMaxSwipeTime = 300; // ms
static const int DialpadCloseAnimDuration = 400; // ms
static const int DialpadOpenAnimDuration = 200; // ms
static const qreal DialpadComponentMargin = 0.75; // units
static const qreal DialpadCloseHandleHeight = 2.23; // units

Dialpad::Dialpad() : mMouseButtonPressedDown(false),
                     mOpenTimeLine(DialpadOpenAnimDuration),
                     mCloseTimeLine(DialpadCloseAnimDuration),
                     mAnimationOngoing(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setFlag(QGraphicsItem::ItemIsFocusable,true);

    // create input field
    mInputField = new DialpadInputField(this);

    // create keypad
    mKeypad = new DialpadKeypad(*mInputField,this);
    mKeypad->setLongPressDuration(DialpadMaxSwipeTime);

    // layouting params
    qreal unit = HbDeviceProfile::current().unitValue();
    qreal margin = DialpadComponentMargin * unit;
    mCloseHandleHeight = DialpadCloseHandleHeight * unit;

    // create popup close handler and background
    QGraphicsWidget* closeHandle = new QGraphicsWidget(this);
    closeHandle->setPreferredHeight(mCloseHandleHeight);
    closeHandle->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    mBackgroundDrawer = new HbFrameDrawer();
    mIconDrawer = new HbFrameDrawer();

    // popup layout
    QGraphicsLinearLayout* popupLayout
        = new QGraphicsLinearLayout(Qt::Vertical);
    popupLayout->addItem(closeHandle);
    popupLayout->addItem(mInputField);
    popupLayout->addItem(mKeypad);    
    popupLayout->setContentsMargins(margin, 0, margin, margin);
    popupLayout->setSpacing(margin);
    setLayout(popupLayout);

    // asterisk multitap handler
    mMultitap = new DialpadMultitapHandler(mInputField->editor(),this);
    mInputField->editor().installEventFilter(mMultitap);

    // close animation
    mCloseTimeLine.setEasingCurve(QEasingCurve::InQuad);
    connect(&mCloseTimeLine, SIGNAL(finished()),
            SLOT(closeAnimFinished()));
    connect(&mCloseTimeLine, SIGNAL(valueChanged(qreal)),
            SLOT(closeAnimValueChanged(qreal)));

    // open animation
    mOpenTimeLine.setEasingCurve(QEasingCurve::OutQuad);
    connect(&mOpenTimeLine, SIGNAL(finished()),
            SLOT(openAnimFinished()));
    connect(&mOpenTimeLine, SIGNAL(valueChanged(qreal)),
            SLOT(openAnimValueChanged(qreal)));

    // default values
    setPos(0,0);
    setPreferredSize(360,360);
    setZValue(0x00004000); // Hb PopupZValueRangeStart
    hide();

    // custom button style
    HbStyleLoader::registerFilePath(":/dialpad.css");
    HbStyleLoader::registerFilePath(":/dialpad.dialpadbutton.widgetml");
}

Dialpad::~Dialpad()
{
    delete mBackgroundDrawer;
    delete mIconDrawer;
}


void Dialpad::paint(
    QPainter* painter,
    const QStyleOptionGraphicsItem* option,
    QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // paint popup background
    mBackgroundDrawer->setFrameGraphicsName(backgroundGraphics);
    mBackgroundDrawer->setFrameType(HbFrameDrawer::ThreePiecesVertical);
    mBackgroundDrawer->setFillWholeRect(true);

    QRectF rect = boundingRect();
    mBackgroundDrawer->setBorderWidths(0.0, mCloseHandleHeight, 0.0,
                                       (rect.height()-mCloseHandleHeight)/2.0);
    mBackgroundDrawer->paint(painter, rect);

    mIconDrawer->setFrameType(HbFrameDrawer::OnePiece);
    rect.setLeft((rect.width() - mCloseHandleHeight)/2);
    rect.setWidth(mCloseHandleHeight);
    rect.setHeight(mCloseHandleHeight);
    mIconDrawer->setFrameGraphicsName(minimizeIcon);
    mIconDrawer->paint(painter, rect);
}

void Dialpad::openDialpad()
{
    HbMainWindow* window = hbInstance->allMainWindows().at(0);
    window->scene()->addItem(this);
    connect(window,SIGNAL(aboutToChangeOrientation()),
            SLOT(orientationChangeStarted()));
    connect(window,SIGNAL(orientationChanged(Qt::Orientation)),
            SLOT(orientationChangeFinished()));

    // set offset for open animation
    mPosition = pos();
    qreal height = geometry().height();
    setPos(mPosition.x(),mPosition.y()+height);

    show();

    setFocusProxy(&mInputField->editor());
    mInputField->editor().setFocus();

    mOpenTimeLine.start();
    mAnimationOngoing = true;
}

void Dialpad::closeDialpad()
{
    close();

    HbMainWindow* window = hbInstance->allMainWindows().at(0);
    window->scene()->removeItem(this);
    disconnect(window,SIGNAL(aboutToChangeOrientation()));
    disconnect(window,SIGNAL(orientationChanged(Qt::Orientation)));
}

HbLineEdit& Dialpad::editor() const
{
    return mInputField->editor();
}

void Dialpad::setCallButtonEnabled(bool enabled)
{
    mKeypad->setCallButtonEnabled(enabled);
}

bool Dialpad::sceneEvent(QEvent *event)
{
    return handleSceneEvent(event);
}

bool Dialpad::sceneEventFilter(QGraphicsItem *watched, QEvent *event)        
{
    Q_UNUSED(watched);

    return handleSceneEvent(event);
}

bool Dialpad::handleSceneEvent(QEvent *event)
{
    // handle close swipe gesture
    if (event->type() == QEvent::GraphicsSceneMousePress) {
        mMouseButtonPressedDown = true;
        mSwipeTimer.start();
    } else if (event->type() == QEvent::GraphicsSceneMouseRelease &&
               mMouseButtonPressedDown) {
        mMouseButtonPressedDown = false;
        QGraphicsSceneMouseEvent *mouseEvent =
            static_cast<QGraphicsSceneMouseEvent*> (event);

        // check if mouse has moved DialpadCloseSwipeDistance vertically
        QPointF delta = mouseEvent->scenePos() -
                        mouseEvent->buttonDownScenePos(Qt::LeftButton);

        qreal height = geometry().height() * DialpadCloseSwipeDistance;

        if ( (delta.y() >= height) &&
             (mSwipeTimer.elapsed() <= DialpadMaxSwipeTime) &&
             !mAnimationOngoing) {
            mCloseTimeLine.start();
            mAnimationOngoing = true;
        }
    }

    return false;
}

void Dialpad::showEvent(QShowEvent *event)
{
    HbWidget::showEvent(event);

    // for closing swipe gesture
    installSceneEventFilter(this);
    setFiltersChildEvents(true);
}

void Dialpad::hideEvent(QHideEvent *event)
{
    HbWidget::hideEvent(event);
    setFiltersChildEvents(false);
    removeSceneEventFilter(this);
}

void Dialpad::closeEvent(QCloseEvent * event)
{
    HbWidget::closeEvent(event);
    emit aboutToClose();
}

void Dialpad::closeAnimValueChanged(qreal value)
{
    QPointF currentPos = pos();
    QPointF newPos;
    qreal height = geometry().height();

    newPos.setX(currentPos.x());
    newPos.setY(mPosition.y()+(height*value));

    setPos(newPos);
}


void Dialpad::closeAnimFinished()
{
    mAnimationOngoing = false;
    closeDialpad();
    setPos(mPosition);
}

void Dialpad::openAnimValueChanged(qreal value)
{
    QPointF currentPos = pos();
    QPointF newPos;
    qreal height = geometry().height();

    newPos.setX(currentPos.x());
    newPos.setY(mPosition.y()+(height*(1-value)));

    setPos(newPos);
}

void Dialpad::openAnimFinished()
{
    mAnimationOngoing = false;    
    setPos(mPosition);
}

void Dialpad::orientationChangeStarted()
{
    hide();
    // needs to re-create grid layout to get
    // scale with uniform item sizes
    mKeypad->createButtonGrid();
}

void Dialpad::orientationChangeFinished()
{
    show();
}
