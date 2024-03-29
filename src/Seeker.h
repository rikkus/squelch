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

#ifndef SEEKER_H
#define SEEKER_H

#include <qslider.h>

class QTimer;

class Seeker : public QSlider
{
	Q_OBJECT

	public:

		Seeker(QWidget * parent);
		virtual ~Seeker();

	protected:

		void wheelEvent(QWheelEvent *);

	protected slots:

		void slotValueChanged(int i);
		void slotSliderPressed();
		void slotSliderReleased();
		void slotTimeChange(long);
		void slotTrackLengthChange(long);

		void slotDelayedUpdate();

	private:

		bool adjusting_;
		QTimer * delayedUpdateTimer_;
};

#endif
// vim:ts=2:sw=2:tw=78:noet
