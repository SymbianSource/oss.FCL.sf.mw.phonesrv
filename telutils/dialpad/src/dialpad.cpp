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
#include "dialpadbackground.h"

static const QString backgroundGraphics("qtg_fr_input_v_bg");
static const QString backgroundGraphicsH("qtg_fr_input_h_bg");
static const QString minimizeIcon("qtg_graf_input_v_swipe");
static const QString minimizeIconH("qtg_graf_input_h_swipe");
static const qreal DialpadCloseSwipeDistanceV = 0.25; //compared to total height
static const qreal DialpadCloseSwipeDistanceH = 0.33; // > button width
static const int DialpadMaxSwipeTime = 300; // ms
static const int DialpadCloseAnimDuration = 200; // ms
static const int DialpadOpenAnimDuration = 200; // ms
static const qreal DialpadComponentMargin = 0.75; // units
static const qreal DialpadCloseHandleHeight = 2.23; // units
static const qreal DialpadCloseHandleWidth = 18.8; // units

Dialpad::Dialpad() :
    mMainWindow(*hbInstance->allMainWindows().at(0)),
    mBackgroundItem(0),
    mMouseButtonPressedDown(false),
    mOpenTimeLine(DialpadOpenAnimDuration),
    mCloseTimeLine(DialpadCloseAnimDuration),
    mAnimationOngoing(false),
    mOrientation(Qt::Vertical),
    mIsOpen(false)
{
    // this constuctor is deprecated
    initialize();
}

Dialpad::Dialpad(const HbMainWindow& mainWindow) :
    mMainWindow(mainWindow),
    mBackgroundItem(0),
    mMouseButtonPressedDown(false),
    mOpenTimeLine(DialpadOpenAnimDuration),
    mCloseTimeLine(DialpadCloseAnimDuration),
    mAnimationOngoing(false),
    mOrientation(Qt::Vertical),
    mIsOpen(false)
{
    initialize();
}

void Dialpad::initialize()
{
    setFocusPolicy(Qt::StrongFocus);
    setFlag(QGraphicsItem::ItemIsFocusable,true);

    // create input field
    mInputField = new DialpadInputField(this);

    // create keypad
    mKeypad = new DialpadKeypad(mMainWindow,*mInputField,this);
    mKeypad->setLongPressDuration(DialpadMaxSwipeTime);

    // layouting params
    qreal unit = HbDeviceProfile::current().unitValue();
    qreal margin = DialpadComponentMargin * unit;
    mCloseHandleHeight = DialpadCloseHandleHeight * unit;
    mCloseHandleWidth = DialpadCloseHandleWidth * unit;

    // create popup background
    mBackgroundDrawer = new HbFrameDrawer();
    mIconDrawer = new HbFrameDrawer();

    // popup layout
    QGraphicsLinearLayout* popupLayout
        = new QGraphicsLinearLayout(mMainWindow.orientation());
    popupLayout->addItem(mInputField);
    popupLayout->addItem(mKeypad);
    popupLayout->setContentsMargins(margin, mCloseHandleHeight, margin, margin);
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
    mMainWindow.scene()->addItem(this);

    // custom button style
    HbStyleLoader::registerFilePath(":/dialpad.css");
    HbStyleLoader::registerFilePath(":/dialpad_color.css");
    HbStyleLoader::registerFilePath(":/dialpad.dialpadbutton.widgetml");

    // grab gestures so that those are not passed to widgets behind dialpad
    grabGesture(Qt::TapGesture);
    grabGesture(Qt::TapAndHoldGesture);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::SwipeGesture);
    grabGesture(Qt::PinchGesture);
}

Dialpad::~Dialpad()
{
    delete mBackgroundDrawer;
    delete mIconDrawer;
    delete mBackgroundItem;
}

void Dialpad::paint(
    QPainter* painter,
    const QStyleOptionGraphicsItem* option,
    QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // paint popup background
    if ( mOrientation == Qt::Vertical ) {
        mBackgroundDrawer->setFrameGraphicsName(backgroundGraphics);
        mIconDrawer->setFrameGraphicsName(minimizeIcon);
        mBackgroundDrawer->setFrameType(HbFrameDrawer::ThreePiecesVertical);
        mBackgroundDrawer->setBorderWidths(0.0, mCloseHandleHeight, 0.0, 0.0);
    } else {
        mBackgroundDrawer->setFrameGraphicsName(backgroundGraphicsH);
        mIconDrawer->setFrameGraphicsName(minimizeIconH);
        mBackgroundDrawer->setFrameType(HbFrameDrawer::ThreePiecesHorizontal);
        mBackgroundDrawer->setBorderWidths(mCloseHandleHeight, 0.0, 0.0, 0.0);
    }

    // draw background
    QRectF rect = boundingRect();
    mBackgroundDrawer->setFillWholeRect(true);
    mBackgroundDrawer->paint(painter, rect);

    // adjust rectangle to close bar position
    if ( mOrientation == Qt::Vertical ) {
        rect.setLeft((rect.width()- mCloseHandleWidth)/2);
        rect.setWidth(mCloseHandleWidth);
        rect.setHeight(mCloseHandleHeight);
    } else {
        if (layoutDirection()==Qt::LeftToRight) {
            rect.setTop((rect.height() - mCloseHandleWidth)/2);
            rect.setWidth(mCloseHandleHeight);
            rect.setHeight(mCloseHandleWidth);
        } else {
            rect.setLeft((rect.width() - mCloseHandleHeight));
            rect.setTop((rect.height() - mCloseHandleWidth)/2);
            rect.setWidth(mCloseHandleHeight);
            rect.setHeight(mCloseHandleWidth);
        }
    }

    mIconDrawer->setFrameType(HbFrameDrawer::OnePiece);
    mIconDrawer->paint(painter, rect);
}

bool Dialpad::isOpen() const
{
    return mIsOpen;
}

void Dialpad::openDialpad()
{
    mKeypad->resetButtons();
    
    if (mIsOpen) {
        return;
    }

    connect(&mMainWindow,SIGNAL(aboutToChangeOrientation()),
            SLOT(orientationChangeStarted()));
    connect(&mMainWindow,SIGNAL(orientationChanged(Qt::Orientation)),
            SLOT(orientationChangeFinished(Qt::Orientation)));

    // set offset for open animation
    int previousOrientation = mOrientation;
    mOrientation = mMainWindow.orientation();

    mPosition = pos();
    if (mOrientation==Qt::Vertical) {
        qreal height = geometry().height();
        setPos(mPosition.x(),mPosition.y()+height);
    } else {
        qreal width = geometry().width();
        if (layoutDirection()==Qt::LeftToRight) {
            setPos(mPosition.x()+width,mPosition.y());
        } else {
            setPos(mPosition.x()-width,mPosition.y());
        }
    }

    if (mOrientation!=previousOrientation) {
        mKeypad->createButtonGrid();
        updateLayout((Qt::Orientation)mOrientation);
    }

    show();

    setFocusProxy(&mInputField->editor());
    mInputField->editor().setFocus();

    mOpenTimeLine.start();
    mAnimationOngoing = true;
    if (!mInputField->editor().text().isEmpty()) {
        setCallButtonEnabled(true);
    }
    mIsOpen = true;

    emit aboutToOpen();
}

void Dialpad::closeDialpad()
{
    close();

    disconnect(&mMainWindow,SIGNAL(aboutToChangeOrientation()),
               this, SLOT(orientationChangeStarted()));
    disconnect(&mMainWindow,SIGNAL(orientationChanged(Qt::Orientation)),
               this, SLOT(orientationChangeFinished(Qt::Orientation)));

    if (mBackgroundItem) {
        mBackgroundItem->hide();
    }
}    

HbLineEdit& Dialpad::editor() const
{
    return mInputField->editor();
}

void Dialpad::setCallButtonEnabled(bool enabled)
{
    mKeypad->setCallButtonEnabled(enabled);
}

void Dialpad::setTapOutsideDismiss(bool dismiss)
{
    // set dismiss policy before opening dialpad
    Q_ASSERT(!isVisible());

    if (dismiss) {
        mBackgroundItem = new DialpadBackground(*this);
        mBackgroundItem->setZValue(zValue()-1);
        mMainWindow.scene()->addItem(mBackgroundItem);
        qreal chromeHeight = 0;
        hbInstance->style()->parameter("hb-param-widget-chrome-height",
                                       chromeHeight);
        mTitleBarHeight = chromeHeight;
    } else {
        delete mBackgroundItem;
        mBackgroundItem = 0;
    }
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

        if (mOrientation==Qt::Vertical) {
            qreal height = geometry().height() * DialpadCloseSwipeDistanceV;

            if ( (delta.y() >= height) &&
                 (mSwipeTimer.elapsed() <= DialpadMaxSwipeTime)) {
                startCloseAnimation();
            }
        } else {
            qreal width = geometry().width() * DialpadCloseSwipeDistanceH;
            bool swipe = (layoutDirection()==Qt::LeftToRight &&
                          delta.x() >= width) ||
                         (layoutDirection()==Qt::RightToLeft &&
                          -delta.x() >= width);

            if ( swipe &&
                 (mSwipeTimer.elapsed() <= DialpadMaxSwipeTime)) {
                startCloseAnimation();
            }
        }
    }

    return false;
}

void Dialpad::startCloseAnimation()
{
    if (!mAnimationOngoing) {
        mCloseTimeLine.start();
        mAnimationOngoing = true;
   }
}

void Dialpad::showEvent(QShowEvent *event)
{
    HbWidget::showEvent(event);

    // for closing swipe gesture
    installSceneEventFilter(this);
    setFiltersChildEvents(true);

    if (mBackgroundItem) {
        layoutBackgroundItem();
        mBackgroundItem->show();
    }
}

void Dialpad::hideEvent(QHideEvent *event)
{
    HbWidget::hideEvent(event);
    setFiltersChildEvents(false);
    removeSceneEventFilter(this);
}

void Dialpad::closeEvent(QCloseEvent * event)
{
    mIsOpen = false;
    HbWidget::closeEvent(event);
    emit aboutToClose();
}

void Dialpad::closeAnimValueChanged(qreal value)
{
    QPointF currentPos = pos();
    QPointF newPos;

    if (mOrientation==Qt::Vertical) {
        qreal height = geometry().height();

        newPos.setX(currentPos.x());
        newPos.setY(mPosition.y()+(height*value));
    } else {
        qreal width = geometry().width();

        if (layoutDirection()==Qt::LeftToRight) {
            newPos.setY(currentPos.y());
            newPos.setX(mPosition.x()+(width*value));
        } else {
            newPos.setY(currentPos.y());
            newPos.setX(mPosition.x()-(width*value));
        }
    }

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

    if (mOrientation==Qt::Vertical) {
        qreal height = geometry().height();

        newPos.setX(currentPos.x());
        newPos.setY(mPosition.y()+(height*(1-value)));
    } else {
        qreal width = geometry().width();

        if (layoutDirection()==Qt::LeftToRight) {
            newPos.setY(currentPos.y());
            newPos.setX(mPosition.x()+(width*(1-value)));
        } else {
            newPos.setY(currentPos.y());
            newPos.setX(mPosition.x()-(width*(1-value)));
        }
    }

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

void Dialpad::orientationChangeFinished(Qt::Orientation current)
{
    updateLayout(current);

    show();

    mOrientation = current;

    mPosition = pos();

    if (mBackgroundItem) {
        layoutBackgroundItem();
    }
}

void Dialpad::layoutBackgroundItem()
{
    Q_ASSERT(mBackgroundItem);
    QRectF backgroundRect(mMainWindow.layoutRect());
    backgroundRect.adjust(0,mTitleBarHeight,0,0);
    mBackgroundItem->setRect(backgroundRect);
}

void Dialpad::updateLayout(Qt::Orientation orientation)
{
    Q_ASSERT(layout());

    qreal unit = HbDeviceProfile::current().unitValue();
    qreal margin = DialpadComponentMargin * unit;

    QGraphicsLinearLayout* mainLayout =
        static_cast<QGraphicsLinearLayout*>(layout());

    // close handle location changes, update margin values
    if (orientation==Qt::Vertical) {
        mainLayout->setContentsMargins(margin,
                                       mCloseHandleHeight,
                                       margin,
                                       margin);
    } else {
        mainLayout->setContentsMargins(mCloseHandleHeight,
                                       margin,
                                       margin,
                                       margin);
    }
}
