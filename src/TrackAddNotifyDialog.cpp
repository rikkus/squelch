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

#include <qlayout.h>

#include "Static.h"
#include "PlayList.h"
#include "Track.h"
#include "TrackAddNotifyDialog.h"

TrackAddNotifyDialog::TrackAddNotifyDialog()
	: QWidget(0, "Squelch track add notify dialog")
{
	setCaption("Scanning - squelch");
	QLabel * l = new QLabel(this);
	l->setPixmap(Static::instance()->pixmap("track"));

	label_ = new QLabel("Hello, world!", this);

	QHBoxLayout * layout = new QHBoxLayout(this);

	layout->addWidget(l);
	layout->addWidget(label_);

	l->setFixedWidth(l->sizeHint().width() + 4);
	setFixedWidth(400);
	setFixedHeight(label_->sizeHint().height());
	resize(400, height());

	hide();

	PlayList::instance();

	connect
		(
		 PlayList::instance(), 	SIGNAL(trackAdded(Track *)),
		 this,									SLOT(slotNewTrack(Track *))
		);
}

	void
TrackAddNotifyDialog::slotNewTrack(Track * t)
{
	killTimers();
	label_->setText(t->artist() + " - " + t->title());
	show();
	startTimer(2000);
}

	void
TrackAddNotifyDialog::timerEvent(QTimerEvent *)
{
	hide();
	killTimers();
}

#include "TrackAddNotifyDialog.moc"

// vim:ts=2:sw=2:tw=78:noet
