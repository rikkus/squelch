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

#include <qpainter.h>
#include <qimage.h>

#include "ScrollingLabel.h"

ScrollingLabel::ScrollingLabel(const QString & initialText, QWidget * parent)
	: QWidget(parent),
		scrollSize_(0),
		pos_(0),
		add_(false)
{
	scrollTimer_	= new QTimer(this);
	resetTimer_		= new QTimer(this);

	connect(scrollTimer_, SIGNAL(timeout()), this, SLOT(scroll()));
	connect(resetTimer_, 	SIGNAL(timeout()), this, SLOT(restoreText()));

	setText(initialText);
}

	void
ScrollingLabel::setText(const QString & t, int time)
{
	if (-1 != time)
	{
		resetTimer_->stop();
		restoreText();
		originalText_	= text_;
		text_= t;
		resetTimer_->start(time, true);
		_update();
	}
	else
	{
		resetTimer_->stop();
		text_ = originalText_ = t;
		_update();
	}
}

	void
ScrollingLabel::restoreText()
{
	text_ = originalText_;
	_update();
}

	void
ScrollingLabel::_update()
{
	scrollTimer_->stop();

	pos_ = 0;
	add_ = false;

	int w = fontMetrics().width(text_);
	int h = fontMetrics().height();

	setFixedHeight(h);

	scrollSize_ = QMAX(0, w - width());

	buf_.resize(w, h);
	buf_.fill(colorGroup().background());

	QPainter p(&buf_);
	p.setFont(font());
	p.drawText(0, fontMetrics().ascent(), text_);

	if (scrollSize_ > 0)
		scrollTimer_->start(100, true);

	repaint();
}

	void
ScrollingLabel::paintEvent(QPaintEvent *)
{
	bitBlt(this, 0, 0, &buf_, pos_, 0, pos_ + width(), height(), Qt::CopyROP);
}

	void
ScrollingLabel::resizeEvent(QResizeEvent *)
{
	_update();
}

	void
ScrollingLabel::scroll()
{
	scrollTimer_->stop();

	repaint(false);

	int scrollTime = 100;

	if (pos_ == scrollSize_ || pos_ == 0)
	{
		add_ = !add_;
		scrollTime = 800;
	}

	pos_ += (add_ ? 1 : -1);

	scrollTimer_->start(scrollTime, true);
}

#include "ScrollingLabel.moc"

// vim:ts=2:sw=2:tw=78:noet
