/*
  Copyright (C) 2001 Rik Hemsley (rikkus) <rik@kde.org>
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to
  deal in the Software without restriction, including without limitation the
  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
 
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
  AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef RIK_STYLE_H
#define RIK_STYLE_H

#include <qplatinumstyle.h>

class QPainter;
class QPalette;
class QWidget;
class QPushButton;
class QScrollBar;

class RikStyle : public QPlatinumStyle
{
  public:

    RikStyle();

    ~RikStyle();

    void polish(QApplication *);

    void unPolish(QWidget *);

    void polish(QWidget *);

    void polish(QPalette &);

    void unPolish(QApplication *);

    void drawButton
      (
       QPainter * p,
       int x,
       int y,
       int w,
       int h,
       const QColorGroup & g,
       bool sunken = false,
       const QBrush * fill = 0
      );

    QRect buttonRect(int x, int y, int w, int h);

    void drawBevelButton
      (
       QPainter *,
       int x,
       int y,
       int w,
       int h,
       const QColorGroup &,
       bool sunken = false,
       const QBrush * fill = 0
      );

    void drawPushButton(QPushButton *, QPainter *);

    virtual void drawPushButtonLabel(QPushButton *, QPainter *);

    void drawScrollBarControls
      (
       QPainter *,
       const QScrollBar *,
       int sliderStart,
       uint controls,
       uint activeControl
      );

    QStyle::ScrollControl scrollBarPointOver
      (
       const QScrollBar *,
       int sliderStart,
       const QPoint &
      );

    void scrollBarMetrics
      (
       const QScrollBar *,
       int & sliderMin,
       int & sliderMax,
       int & sliderLength,
       int & buttonDim
      );

    QSize indicatorSize() const;

    void drawIndicator
      (
       QPainter *,
       int x,
       int y,
       int w,
       int h,
       const QColorGroup &,
       int state,
       bool down = false,
       bool enabled = true
      );

    QSize exclusiveIndicatorSize() const;

    void drawExclusiveIndicator
      (
       QPainter *,
       int x,
       int y,
       int w,
       int h,
       const QColorGroup &,
       bool on,
       bool down = false,
       bool enabled = true
      );

    void drawIndicatorMask
      (
       QPainter *,
       int x,
       int y,
       int w,
       int h,
       int state
      );

    void drawExclusiveIndicatorMask
      (
       QPainter *,
       int x, 
       int y, 
       int w,
       int h, 
       bool on
      );

    void drawComboButton
      (
       QPainter *, 
       int x, 
       int y, 
       int w, 
       int h,
       const QColorGroup &, 
       bool sunken = false,
       bool editable = false, 
       bool enabled = true,
       const QBrush * fill = 0
      );

    QRect comboButtonRect(int x, int y, int w, int h);

    QRect comboButtonFocusRect(int x, int y, int w, int h);

    int sliderLength() const;

    void drawSliderGroove
      (
       QPainter *, 
       int x, 
       int y, 
       int w, 
       int h,
       const QColorGroup &,
       QCOORD, 
       Orientation
      );

    void drawArrow
      (
       QPainter *,
       Qt::ArrowType, 
       bool down,
       int x, 
       int y, 
       int w, 
       int h, 
       const QColorGroup &,
       bool enabled = true, 
       const QBrush * fill = 0
      );

    void drawSlider
      (
       QPainter *, 
       int x, 
       int y, 
       int w, 
       int h,
       const QColorGroup &, 
       Orientation,
       bool tickAbove, 
       bool tickBelow
      );

    void drawPopupMenuItem
      (
       QPainter *, 
       bool checkable, 
       int maxpmw,
       int tab,
       QMenuItem *, 
       const QPalette &,
       bool act, 
       bool enabled, 
       int x, 
       int y, 
       int w,
       int h
      );

    void drawFocusRect
      (
       QPainter *, 
       const QRect &, 
       const QColorGroup &,
       const QColor * pen, 
       bool atBorder
      );

    void drawPanel
      (
       QPainter *,
       int x,
       int y,
       int w,
       int h,
       const QColorGroup &,
       bool sunken,
       int lineWidth = 1,
       const QBrush * = 0
      );

    void drawPopupPanel
      (
       QPainter *,
       int x,
       int y,
       int w,
       int h,
       const QColorGroup &,
       int lineWidth = 2,
       const QBrush * = 0
      );

    void drawSeparator
      (
       QPainter *,
       int x,
       int y,
       int w,
       int h,
       const QColorGroup &,
       bool sunken = true,
       int lineWidth = 1,
       int midLineWidth = 0
      );

    void drawTab
      (
       QPainter * p,
       const QTabBar * tabBar,
       QTab * tab,
       bool selected
      );

    void drawTabMask
      (
       QPainter * p,
       const QTabBar *,
       QTab * tab,
       bool
      );

    GUIStyle guiStyle() const { return Qt::MotifStyle; }

    bool eventFilter(QObject *, QEvent *);
};

#endif
