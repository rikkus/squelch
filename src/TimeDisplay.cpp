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

#include "TimeDisplay.h"
#include "TimeChangeEvent.h"
#include "IOController.h"

TimeDisplay::TimeDisplay(QWidget * parent)
	: QLabel(parent),
		lastTime_(-1)
{
	setText("--:--");
	setFixedWidth(fontMetrics().width("88:88"));
	setAlignment(AlignRight);

	connect
		(
		 IOController::instance(), 	SIGNAL(timeChange(long)),
		 this,											SLOT(slotTimeChange(long))
		);
}

TimeDisplay::~TimeDisplay()
{
}

	void
TimeDisplay::slotTimeChange(long t)
{
	if (t == lastTime_)
		return;
	else
		lastTime_ = t;

	int min = t / 60;
	int sec = t - min * 60;

	QString s = QString::number(min) + ':';

	if (sec < 10)
		s += "0";

	s += QString::number(sec);

	setText(s);
}

#include "TimeDisplay.moc"

// vim:ts=2:sw=2:tw=78:noet
