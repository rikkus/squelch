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

#include <qapplication.h>
#include <qheader.h>

#include "PlayList.h"
#include "IOController.h"
#include "PreviewWidget.h"
#include "PreviewItem.h"
#include "Track.h"
#include "qtimer.h"

PreviewWidget * PreviewWidget::instance_ = 0L;

PreviewWidget::PreviewWidget(QWidget *parent)
	: QListView(parent, "Squelch playlist preview widget")
{
	instance_ = this;

	addColumn(tr("Artist"));
	addColumn(tr("Track"));
	addColumn(tr("Time"));

	setAllColumnsShowFocus(true);

	connect
		(
		this,											SIGNAL(currentChanged(QListViewItem *)),
		this,											SLOT(slotCurrentChanged(QListViewItem *))
		);

	connect
		(
		PlayList::instance(),			SIGNAL(trackAdded(Track *)),
		this,											SLOT(slotTrackAdded(Track *))
		);

	connect
		(
		PlayList::instance(),			SIGNAL(trackChanged(Track *)),
		this,											SLOT(slotTrackChanged(Track *))
		);

	connect
		(
		PlayList::instance(),			SIGNAL(trackListAdded(TrackList &)),
		this,											SLOT(slotTrackListAdded(TrackList &))
		);

	connect
		(
		PlayList::instance(),			SIGNAL(scoreChanged(Track *)),
		this,											SLOT(slotScoreChanged(Track *))
		);

	connect
		(
		PlayList::instance(),			SIGNAL(trackListDisappeared(TrackList &)),
		this,											SLOT(slotTrackListDisappeared(TrackList &))
		);


	connect
		(
		IOController::instance(),	SIGNAL(trackChange(const QString &)),
		this,											SLOT(slotTrackChange(const QString &))
		);

	connect
		(
		this,											SIGNAL(skipToTrack(const QString &)),
		IOController::instance(),	SLOT(slotSkipToTrack(const QString &))
		);

	updateTimer_ = new QTimer(this);

	connect
		(
		 updateTimer_,						SIGNAL(timeout()),
		 this,										SLOT(slotUpdate())
		);
}

PreviewWidget::~PreviewWidget()
{
}

	void
PreviewWidget::_addItem(Track * t)
{
	PreviewItem * i = new PreviewItem(this, t);

	if (t->score() < 1)
		_makeInvisible(i);
}

	void
PreviewWidget::slotTrackAdded(Track * t)
{
	_addItem(t);
}

	void
PreviewWidget::slotTrackChanged(Track * t)
{
	PreviewItem * i(_find(t));

	if (0 != i)
	{
		i->update();
		sort();
	}
}

	void
PreviewWidget::slotTrackListAdded(TrackList & l)
{
	setUpdatesEnabled(false);

	// XXX Should use QTimer.
	QTime time;
	time.start();

	for (TrackListIterator it(l); it.current(); ++it)
	{
		_addItem(it.current());

		if (time.elapsed() > 10)
		{
			time.restart();
			qApp->processEvents();
		}
	}

	setUpdatesEnabled(true);
	triggerUpdate();
}

	void
PreviewWidget::slotTrackListDisappeared(TrackList & l)
{
	setUpdatesEnabled(false);
	
	for (TrackListIterator it(l); it.current(); ++it)
	{
		PreviewItem * i(_find(it.current()));

		if (0 != i)
		{
			if (heldItemList_.contains(i))
				heldItemList_.removeRef(i);

			delete i;
			i = 0;
		}
	}

	setUpdatesEnabled(true);
	triggerUpdate();
}

	void
PreviewWidget::slotTrackChange(const QString & id)
{
	for (QListViewItemIterator it(this); it.current(); ++it)
	{
		PreviewItem * i(static_cast<PreviewItem *>(it.current()));

		if (i->track()->filename() == id)
			i->setActive(true);
		else
			if (i->active())
				i->setActive(false);
	}
}

	void
PreviewWidget::slotCurrentChanged(QListViewItem * item)
{
	for (QListViewItemIterator it(this); it.current(); ++it)
		if (it.current() == item)
		{
			PreviewItem * i(static_cast<PreviewItem *>(it.current()));
			emit(skipToTrack(i->track()->filename()));
			break;
		}
}

	void
PreviewWidget::slotScoreChanged(Track * t)
{
	PreviewItem * i(_find(t));

	if (0 != i)
	{
		bool shouldBeVisible = i->score() > 0;

		if (shouldBeVisible)
			_makeVisible(i);
		else
			_makeInvisible(i);
	}

	_update(100);
}

	void
PreviewWidget::_update(uint t)
{
	updateTimer_->stop();
	updateTimer_->start(t, true);
}

	void
PreviewWidget::slotUpdate()
{
	sort();
}

	void
PreviewWidget::_makeVisible(PreviewItem * i)
{
	if (heldItemList_.contains(i))
	{
		heldItemList_.removeRef(i);
		insertItem(i);
	}
}

	void
PreviewWidget::_makeInvisible(PreviewItem * i)
{
	if (!heldItemList_.contains(i))
	{
		heldItemList_.append(i);
		takeItem(i);
	}
}

	PreviewItem *
PreviewWidget::_find(const QString & f)
{
	for (QListIterator<PreviewItem> it(heldItemList_); it.current(); ++it)
		if (it.current()->track()->filename() == f)
			return it.current();

	for (QListViewItemIterator it(this); it.current(); ++it)
		if ((static_cast<PreviewItem *>(it.current())->track())->filename() == f)
			return static_cast<PreviewItem *>(it.current());

	return 0;
}

	PreviewItem *
PreviewWidget::_find(Track * t)
{
	for (QListIterator<PreviewItem> it(heldItemList_); it.current(); ++it)
		if (it.current()->track() == t)
			return it.current();

	for (QListViewItemIterator it(this); it.current(); ++it)
		if ((static_cast<PreviewItem *>(it.current())->track()) == t)
			return static_cast<PreviewItem *>(it.current());

	return 0;
}

#include "PreviewWidget.moc"

// vim:ts=2:sw=2:tw=78:noet
