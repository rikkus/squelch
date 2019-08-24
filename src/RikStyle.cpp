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

#ifndef INCLUDE_MENUITEM_DEF
#define INCLUDE_MENUITEM_DEF
#endif

#include <qmenudata.h>
#include <qpalette.h>
#include <qbitmap.h>
#include <qtabbar.h>
#include <qpointarray.h>
#include <qtabbar.h>
#include <qframe.h>
#include <qwidget.h>
#include <qpushbutton.h>
#include <qscrollbar.h>

#if QT_VERSION < 300

#include "RikStyle.h"

static const int  _indicatorSize = 14;
static QButton *  _highlightedButton = 0;
static const int  _scrollBarExtent = 14;

static QFrame *   _currentFrame = 0;
static int        _savedFrameLineWidth;
static int        _savedFrameMidLineWidth;
static ulong      _savedFrameStyle;

// Gotta keep it separated.

  static void
scrollBarControlsMetrics
(
 const QScrollBar * sb,
 int sliderStart,
 int /* sliderMin */,
 int sliderMax,
 int sliderLength,
 int buttonDim,
 QRect & rSub,
 QRect & rAdd,
 QRect & rSubPage,
 QRect & rAddPage,
 QRect & rSlider
 )
{
  bool horizontal = sb->orientation() == QScrollBar::Horizontal;

  int len     = horizontal ? sb->width()  : sb->height();

  int extent  = horizontal ? sb->height() : sb->width();

  QColorGroup g = sb->colorGroup();

  if (sliderStart > sliderMax)
    sliderStart = sliderMax;

  int sliderEnd = sliderStart + sliderLength;

  int addX, addY;
  int subX, subY;
  int subPageX, subPageY, subPageW, subPageH;
  int addPageX, addPageY, addPageW, addPageH;
  int sliderX, sliderY, sliderW, sliderH;

  if (horizontal)
  {
    subY      = 0;
    addY      = 0;
    subX      = 0;
    addX      = buttonDim;

    subPageX  = buttonDim * 2;
    subPageY  = 0;
    subPageW  = sliderStart - 1;
    subPageH  = extent;

    addPageX  = sliderEnd;
    addPageY  = 0;
    addPageW  = len - sliderEnd;
    addPageH  = extent;

    sliderX   = sliderStart;
    sliderY   = 0;
    sliderW   = sliderLength;
    sliderH   = extent;
  }
  else
  {
    subX    = 0;
    addX    = 0;
    subY    = len - buttonDim * 2;
    addY    = len - buttonDim;

    subPageX = 0;
    subPageY = 0;
    subPageW = extent;
    subPageH = sliderStart;

    addPageX  = 0;
    addPageY  = sliderEnd;
    addPageW  = extent;
    addPageH  = subY - sliderEnd;

    sliderX   = 0;
    sliderY   = sliderStart;
    sliderW   = extent;
    sliderH   = sliderLength;
  }

  rSub      .setRect(    subX,      subY, buttonDim, buttonDim);
  rAdd      .setRect(    addX,      addY, buttonDim, buttonDim);
  rSubPage  .setRect(subPageX,  subPageY,  subPageW,  subPageH);
  rAddPage  .setRect(addPageX,  addPageY,  addPageW,  addPageH);
  rSlider   .setRect( sliderX,   sliderY,   sliderW,   sliderH);
}

// Rounded rects my way.

  static void
drawFunkyRect
(
 QPainter * p,
 int x,
 int y,
 int w,
 int h,
 bool small
)
{
  p->translate(x, y);

  if (small)
  {
    p->drawLine(      2,      0,  w - 3,      0  );
    p->drawLine(  w - 1,      2,  w - 1,  h - 3  );
    p->drawLine(  w - 3,  h - 1,      2,  h - 1  );
    p->drawLine(      0,  h - 3,      0,      2  );

    // Use an array of points so that there's only one round-trip with the
    // X server.

    QCOORD pointList[] =
    {
          1,      1,
      w - 2,      1,
      w - 2,  h - 2,
          1,  h - 2
    };

    p->drawPoints(QPointArray(4, pointList));
  }
  else
  {
    p->drawLine(      3,      0,  w - 4,      0  );
    p->drawLine(  w - 1,      3,  w - 1,  h - 4  );
    p->drawLine(  w - 4,  h - 1,      3,  h - 1  );
    p->drawLine(      0,  h - 4,      0,      3  );

    QCOORD pointList[] =
    {
          1,      2,
          2,      1,
      w - 3,      1,
      w - 2,      2,
      w - 2,  h - 3,
      w - 3,  h - 2,
          2,  h - 2,
          1,  h - 3
    };

    p->drawPoints(QPointArray(8, pointList));
  }

  p->translate(-x, -y);
}

RikStyle::RikStyle()
  : QPlatinumStyle()
{
  setButtonDefaultIndicatorWidth(6);
  setScrollBarExtent(_scrollBarExtent, _scrollBarExtent);
}

RikStyle::~RikStyle()
{
  // Empty.
}

  void
RikStyle::polish(QApplication *)
{
  // Empty.
}

  void
RikStyle::polish(QPalette &)
{
  // Empty.
}

  void
RikStyle::unPolish(QApplication *)
{
  // Empty.
}

  void
RikStyle::polish(QWidget * w)
{
  if (w->inherits("QPushButton"))
    w->installEventFilter(this);

  else if (w->inherits("QGroupBox") || w->inherits("QFrame"))
  {
    QFrame * f(static_cast<QFrame *>(w));

    if (f->frameStyle() != QFrame::NoFrame)
    {
      _currentFrame = f;

      _savedFrameLineWidth = f->lineWidth();
      _savedFrameMidLineWidth = f->midLineWidth();
      _savedFrameStyle = f->frameStyle();

      if (f->frameShape() == QFrame::HLine || f->frameShape() == QFrame::VLine)
      {
        f->setMidLineWidth(1);
        f->setFrameStyle(f->frameShape() | QFrame::Plain);
      }
      else
      {
        f->setLineWidth(1);
        f->setFrameStyle(QFrame::Box | QFrame::Plain);
      }
    }
  }
}

  void
RikStyle::unPolish(QWidget * w)
{
  if (w->inherits("QPushButton"))
    w->removeEventFilter(this);

  else if (w == _currentFrame)
  {
    QFrame * f(static_cast<QFrame *>(w));

    f->setLineWidth(_savedFrameLineWidth);
    f->setMidLineWidth(_savedFrameMidLineWidth);
    f->setFrameStyle(_savedFrameStyle);
  }
}

  bool
RikStyle::eventFilter(QObject * o, QEvent * e)
{
  QPushButton * pb(static_cast<QPushButton *>(o));

  if (e->type() == QEvent::Enter)
  {
    _highlightedButton = pb;
    pb->repaint(false);
  }
  else if (e->type() == QEvent::Leave)
  {
    _highlightedButton = 0;
    pb->repaint(false);
  }

  return false;
}

  void
RikStyle::drawButton
(
 QPainter * p,
 int x,
 int y,
 int w,
 int h,
 const QColorGroup & g,
 bool sunken,
 const QBrush * fill
)
{
  p->save();

  p->setPen(sunken ? g.light() : g.mid());

  p->setBrush(0 == fill ? NoBrush : *fill);

  drawFunkyRect(p, x, y, w, h, true);

  p->restore();
}

  QRect
RikStyle::buttonRect(int x, int y, int w, int h)
{
  return QRect(x + 2, y + 2, w - 4, h - 4);
}

  void
RikStyle::drawBevelButton
(
 QPainter * p,
 int x,
 int y,
 int w,
 int h,
 const QColorGroup & g,
 bool sunken,
 const QBrush * fill
)
{
  drawButton(p, x, y, w, h, g, sunken, fill);
}

  void
RikStyle::drawPushButton(QPushButton * b, QPainter * p)
{
  // Note: painter is already translated for us.

  bool sunken(b->isDown() || b->isOn());
  bool hl(_highlightedButton == b);

  p->save();
  p->fillRect(b->rect(), b->colorGroup().brush(QColorGroup::Background));

  if (b->isDefault())
  {
    p->setPen(hl ? b->colorGroup().highlight() : b->colorGroup().mid());
    drawFunkyRect(p, 0, 0, b->width(), b->height(), false);
  }

  if (b->isEnabled())
  {
    if (sunken)
    {
      p->setPen(b->colorGroup().light());
    }
    else
    {
      if (hl)
        p->setPen(b->colorGroup().highlight());
      else
        p->setPen(b->colorGroup().mid());
    }
  }
  else
  {
    p->setPen(b->colorGroup().button());
  }

  p->fillRect
    (
     4,
     4,
     b->width() - 8,
     b->height() - 8,
     b->colorGroup().brush(QColorGroup::Button)
    );

  drawFunkyRect(p, 3, 3, b->width() - 6, b->height() - 6, true);

  p->restore();
}

  void
RikStyle::drawPushButtonLabel(QPushButton * b, QPainter * p)
{
  // This is complicated stuff and we don't really want to mess with it.

  QPlatinumStyle::drawPushButtonLabel(b, p);
}

  void
RikStyle::drawScrollBarControls
(
 QPainter * p,
 const QScrollBar * sb,
 int sliderStart,
 uint controls,
 uint activeControl
)
{
  p->save();

  int sliderMin, sliderMax, sliderLength, buttonDim;

  scrollBarMetrics(sb, sliderMin, sliderMax, sliderLength, buttonDim);

  QRect rSub, rAdd, rSubPage, rAddPage, rSlider;

  scrollBarControlsMetrics
    (
     sb,
     sliderStart,
     sliderMin,
     sliderMax,
     sliderLength,
     buttonDim,
     rSub,
     rAdd,
     rSubPage,
     rAddPage,
     rSlider
    );

  QColorGroup g(sb->colorGroup());

  if (controls & AddLine && rAdd.isValid())
  {
    bool active(activeControl & AddLine);

    QColor c(active ? g.highlight() : g.dark());

    p->setPen(c);
    p->setBrush(g.button());
    p->drawRect(rAdd);

    Qt::ArrowType t =
      sb->orientation() == Horizontal ? Qt::RightArrow : Qt::DownArrow;

    drawArrow
      (
       p,
       t,
       true, // FIXME - down ?
       rAdd.x(),
       rAdd.y(),
       rAdd.width(),
       rAdd.height(),
       g,
       true // FIXME - enabled ?
      );
  }

  if (controls & SubLine && rSub.isValid())
  {
    bool active(activeControl & SubLine);

    QColor c(active ? g.highlight() : g.dark());

    p->setPen(c);
    p->setBrush(g.button());
    p->drawRect(rSub);

    Qt::ArrowType t =
      sb->orientation() == Horizontal ? Qt::LeftArrow : Qt::UpArrow;

    drawArrow
      (
       p,
       t,
       true, // FIXME - down ?
       rSub.x(),
       rSub.y(),
       rSub.width(),
       rSub.height(),
       g,
       true // FIXME - enabled ?
      );
  }

  if (controls & SubPage && rSubPage.isValid())
  {
    p->setPen(g.mid());
    p->setBrush(g.base());
    p->drawRect(rSubPage);
  }

  if (controls & AddPage && rAddPage.isValid())
  {
    p->setPen(g.mid());
    p->setBrush(g.base());
    p->drawRect(rAddPage);
  }

  if (controls & Slider && rSlider.isValid())
  {
    p->setPen(activeControl & Slider ? g.highlight() : g.dark());

    p->setBrush(g.button());
    p->drawRect(rSlider);

    p->setBrush(g.light());
    p->setPen(g.dark());

    if (sliderLength > _scrollBarExtent * 2)
    {
      int ellipseSize = 
        Horizontal == sb->orientation()
        ?
        rSlider.height() - 4
        :
        rSlider.width()  - 4
        ;

      QPoint center(rSlider.center());

      if (Horizontal == sb->orientation())
      {
        p->drawEllipse
          (
           center.x() - ellipseSize / 2, rSlider.y() + 2,
           ellipseSize, ellipseSize
          );
      }
      else
      { 
        p->drawEllipse
          (
           rSlider.x() + 2, center.y() - ellipseSize / 2,
           ellipseSize, ellipseSize
          );
      }
    }
  }

  p->restore();
}

  QStyle::ScrollControl
RikStyle::scrollBarPointOver
(
 const QScrollBar * sb,
 int sliderStart,
 const QPoint & point
)
{
  if (!sb->rect().contains(point))
    return NoScroll;

  int sliderMin, sliderMax, sliderLength, buttonDim;

  scrollBarMetrics(sb, sliderMin, sliderMax, sliderLength, buttonDim);

  if (sb->orientation() == QScrollBar::Horizontal)
  {
    int x = point.x();

    if (x <= buttonDim)
      return SubLine;

    else if (x <= buttonDim * 2)
      return AddLine;

    else if (x < sliderStart)
      return SubPage;

    else if (x < sliderStart+sliderLength)
      return Slider;

    return AddPage;
  }
  else
  {
    int y = point.y();

    if (y < sliderStart)
      return SubPage;

    else if (y < sliderStart + sliderLength)
      return Slider;

    else if (y < sliderMax + sliderLength)
      return AddPage;

    else if (y < sliderMax + sliderLength + buttonDim)
      return SubLine;

    return AddLine;
  }
}

  void
RikStyle::scrollBarMetrics
(
 const QScrollBar * sb,
 int & sliderMin,
 int & sliderMax,
 int & sliderLength,
 int & buttonDim
)
{
  int maxlen;

  bool horizontal = sb->orientation() == QScrollBar::Horizontal;

  int len = (horizontal) ? sb->width() : sb->height();

  int extent = (horizontal) ? sb->height() : sb->width();

  if (len > (extent - 1) * 2)
    buttonDim = extent;
  else
    buttonDim = len / 2 - 1;

  if (horizontal)
    sliderMin = buttonDim * 2;
  else
    sliderMin = 1;

  maxlen = len - buttonDim * 2 - 1;

  sliderLength =
    (sb->pageStep() * maxlen) /
    (sb->maxValue() - sb->minValue() + sb->pageStep());

  if (sliderLength < _scrollBarExtent)
    sliderLength = _scrollBarExtent;

  if (sliderLength > maxlen)
    sliderLength = maxlen;

  sliderMax = sliderMin + maxlen - sliderLength;
}

  QSize
RikStyle::indicatorSize() const
{
  return QSize(_indicatorSize, _indicatorSize);
}

  void
RikStyle::drawIndicator
(
 QPainter * p,
 int x,
 int y,
 int w,
 int h,
 const QColorGroup & g,
 int state,
 bool down,
 bool enabled
)
{
  p->save();

  p->fillRect(x, y, w, h, g.background());

  p->setPen(enabled ? (down ? g.light() : g.dark()) : g.mid());

  p->drawRect(x, y, w, h);

  if (state != QButton::Off)
  {
    p->fillRect(x + 2, y + 2, w - 4, h - 4, g.light());

    if (state == QButton::NoChange)
    {
      p->fillRect(x + 4, y + 4, w - 8, h - 8, g.background());
    }
  }

  p->restore();
}

  QSize
RikStyle::exclusiveIndicatorSize() const
{
  return QSize(_indicatorSize, _indicatorSize);
}

  void
RikStyle::drawExclusiveIndicator
(
 QPainter * p,
 int x,
 int y,
 int w,
 int h,
 const QColorGroup & g,
 bool on,
 bool down,
 bool enabled
)
{
  p->save();

  p->fillRect(x, y, w, h, g.background());

  p->setPen(enabled ? (down ? g.light() : g.dark()) : g.mid());

  p->setBrush(g.brush(QColorGroup::Background));

  p->drawEllipse(x, y, w, h);

  if (on)
  {
    p->setPen(g.light());
    p->setBrush(g.light());
    p->drawEllipse(x + 2, y + 2, w - 4, h - 4);
  }

  p->restore();
}

  void
RikStyle::drawIndicatorMask
(
 QPainter * p,
 int x,
 int y,
 int w,
 int h,
 int /* state */
)
{
  p->fillRect(x, y, w, h, Qt::color1);
}

  void
RikStyle::drawExclusiveIndicatorMask
(
 QPainter * p,
 int x,
 int y,
 int w,
 int h,
 bool /* on */
)
{
  p->setPen(Qt::color1);
  p->setBrush(Qt::color1);
  p->drawEllipse(x, y, w, h);
}

  void
RikStyle::drawComboButton
(
 QPainter * p,
 int x,
 int y,
 int w,
 int h,
 const QColorGroup & g,
 bool sunken,
 bool editable,
 bool enabled,
 const QBrush * fill
)
{
  p->save();

  if (enabled)
  {
    if (sunken)
      p->setPen(g.highlight());
    else
      p->setPen(g.dark());
  }
  else
  {
    p->setPen(g.mid());
  }

  p->setBrush(0 == fill ? NoBrush : *fill);

  drawFunkyRect(p, x, y, w, h, true);

  p->drawPoint(w - 10, h - 6);
  p->drawPoint(w - 9, h - 6);
  p->drawPoint(w - 8, h - 6);
  p->drawPoint(w - 7, h - 6);
  p->drawPoint(w - 6, h - 6);

  p->drawPoint(w - 9, h - 7);
  p->drawPoint(w - 8, h - 7);
  p->drawPoint(w - 7, h - 7);
  p->drawPoint(w - 6, h - 7);

  p->drawPoint(w - 8, h - 8);
  p->drawPoint(w - 7, h - 8);
  p->drawPoint(w - 6, h - 8);

  p->drawPoint(w - 7, h - 9);
  p->drawPoint(w - 6, h - 9);

  p->drawPoint(w - 6, h - 10);

  if (editable)
    p->fillRect(comboButtonFocusRect(x, y, w, h), Qt::red);

  p->restore();
}

  QRect
RikStyle::comboButtonRect(int x, int y, int w, int h)
{
  return QRect(x, y, w, h);
}

  QRect
RikStyle::comboButtonFocusRect(int x, int y, int w, int h)
{
  return QRect(x + 2, y + 2, w - 20, h - 4);
}

  int
RikStyle::sliderLength() const
{
  return _scrollBarExtent;
}

  void
RikStyle::drawSliderGroove
(
 QPainter * p,
 int x,
 int y,
 int w,
 int h,
 const QColorGroup & g,
 QCOORD /* c */,
 Orientation /* o */
)
{
  p->save();

  p->setPen(QPen(g.dark(), 0, Qt::DotLine));
  p->drawLine(x, y + h / 2, w, y + h / 2);

  p->restore();
}

  void
RikStyle::drawArrow
(
 QPainter * p,
 Qt::ArrowType type,
 bool down,
 int x,
 int y,
 int w,
 int h,
 const QColorGroup & g,
 bool enabled,
 const QBrush * fill
)
{
  QPlatinumStyle::drawArrow(p, type, down, x, y, w, h, g, enabled, fill);
}

  void
RikStyle::drawSlider
(
 QPainter * p,
 int x,
 int y,
 int w,
 int h,
 const QColorGroup & g,
 Orientation /* o */,
 bool /* tickAbove */,
 bool /* tickBelow */
)
{
  p->save();

  p->fillRect(x, y, w, h, g.background());
  p->setPen(g.dark());
  p->setBrush(g.light());
  int sl = sliderLength();

  int sz = 8;

  p->drawEllipse(x + (w / 2) - 6, y + (h / 2) - 6, 12, 12);

  p->restore();
}

  void
RikStyle::drawPopupMenuItem
(
 QPainter * p,
 bool checkable,
 int maxpmw,
 int tab,
 QMenuItem * mi,
 const QPalette & pal,
 bool act,
 bool enabled,
 int x,
 int y,
 int w,
 int h
)
{
  // TODO
  QPlatinumStyle::drawPopupMenuItem
    (p, checkable, maxpmw, tab, mi, pal, act, enabled, x, y, w, h);
}

  void
RikStyle::drawFocusRect
(
 QPainter * p,
 const QRect & r,
 const QColorGroup & g,
 const QColor * c,
 bool atBorder
)
{
  p->save();

  p->setPen(0 == c ? g.foreground() : *c);

  p->setBrush(NoBrush);

  if (atBorder)
  {
    p->drawRect(QRect(r.x() + 1, r.y() + 1, r.width() - 2, r.height() - 2));
  }
  else
  {
    p->drawRect(r);
  }

  p->restore();
}

  void
RikStyle::drawPanel
(
 QPainter * p,
 int x,
 int y,
 int w,
 int h,
 const QColorGroup & g,
 bool /* sunken */,
 int /* lineWidth */,
 const QBrush * fill
)
{
  p->save();

  p->setPen(g.dark());

  p->setBrush(0 == fill ? NoBrush : *fill);

  p->drawRect(x, y, w, h);

  p->restore();
}

  void
RikStyle::drawPopupPanel
(
 QPainter * p,
 int x,
 int y,
 int w,
 int h,
 const QColorGroup & g,
 int /* lineWidth */,
 const QBrush * fill
)
{
  p->save();

  p->setPen(g.dark());

  p->setBrush(0 == fill ? NoBrush : *fill);

  p->drawRect(x, y, w, h);

  p->restore();
}

  void
RikStyle::drawSeparator
(
 QPainter * p,
 int x,
 int y,
 int w,
 int h,
 const QColorGroup & g,
 bool /* sunken */,
 int /* lineWidth */,
 int /* midLineWidth */
)
{
  p->save();

  p->setPen(g.dark());

  if (w > h)
  {
    p->drawLine(x, y + h / 2, x, y + h / 2);
  }
  else
  {
    p->drawLine(x + w / 2, y, x + w / 2, y);
  }

  p->restore();
}

  void
RikStyle::drawTab
(
 QPainter * p,
 const QTabBar * tabBar,
 QTab * tab,
 bool selected
)
{
  QRect r(tab->rect());

  QColorGroup g(tabBar->colorGroup());

  p->save();

  p->setPen(selected ? g.dark() : g.mid());
  p->fillRect(r, g.brush(QColorGroup::Background));

  switch (tabBar->shape())
  {
    case QTabBar::RoundedAbove:
    case QTabBar::TriangularAbove:
      p->drawLine(r.left(), r.top(), r.left(), r.bottom() - 1);
      p->drawLine(r.left(), r.top(), r.right(), r.top());
      p->drawLine(r.right(), r.top(), r.right(), r.bottom() - 1);
      if (!selected)
      {
        p->setPen(g.dark());
        p->drawLine(r.left(), r.bottom() - 1, r.right(), r.bottom() - 1);
      }
      break;
    case QTabBar::RoundedBelow:
    case QTabBar::TriangularBelow:
      if (!selected)
      {
        p->setPen(g.dark());
        p->drawLine(r.left(), r.top(), r.right(), r.top());
      }
      p->drawLine(r.left(), r.top(), r.right(), r.bottom());
      p->drawLine(r.right(), r.top(), r.right(), r.bottom());
      p->drawLine(r.left(), r.bottom(), r.top(), r.bottom());
      break;
  }

  p->restore();
}

  void
RikStyle::drawTabMask
(
 QPainter * p,
 const QTabBar *,
 QTab * tab,
 bool
)
{
  p->fillRect(tab->rect(), Qt::color1);
}

#endif /* QT_VERSION */
