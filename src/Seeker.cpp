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

#include <qtimer.h>

#include "Seeker.h"
#include "IOController.h"

Seeker::Seeker(QWidget * parent)
	: QSlider(0, 60, 10, 0, Horizontal, parent),
		adjusting_(false)
{
	delayedUpdateTimer_ = new QTimer(this);

	connect(this, SIGNAL(sliderPressed()), SLOT(slotSliderPressed()));
	connect(this, SIGNAL(sliderReleased()), SLOT(slotSliderReleased()));
	connect(this, SIGNAL(valueChanged(int)), SLOT(slotValueChanged(int)));

	connect
		(
		 IOController::instance(),	SIGNAL(timeChange(long)),
		 this,											SLOT(slotTimeChange(long))
		);

	connect
		(
		 IOController::instance(), 	SIGNAL(trackLengthChange(long)),
		 this,											SLOT(slotTrackLengthChange(long))
		);

	connect
		(
		 delayedUpdateTimer_,				SIGNAL(timeout()),
		 this,											SLOT(slotDelayedUpdate())
		);
}

Seeker::~Seeker()
{
}

	void
Seeker::slotSliderPressed()
{
	adjusting_ = true;
}

	void
Seeker::slotValueChanged(int i)
{
	if (adjusting_)
		delayedUpdateTimer_->start(100, true);
}

	void
Seeker::slotDelayedUpdate()
{
	IOController::instance()->slotSeek(value());
}

	void
Seeker::slotSliderReleased()
{
	delayedUpdateTimer_->stop();
	adjusting_ = false;
	IOController::instance()->slotSeek(value());
}

	void
Seeker::wheelEvent(QWheelEvent *)
{
	// Ignore.
}

	void
Seeker::slotTimeChange(long t)
{
	blockSignals(true);

	if (!adjusting_)
		setValue(t);

	blockSignals(false);
}

	void
Seeker::slotTrackLengthChange(long t)
{
	if (t < 0)
		return;

	delayedUpdateTimer_->stop();

	blockSignals(true);
	setMaxValue(t);
	QSlider::setValue(0);
	blockSignals(false);
	repaint(); // Qt bug.
}

#include "Seeker.moc"

// vim:ts=2:sw=2:tw=78:noet
