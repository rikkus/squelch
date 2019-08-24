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
#include <qstring.h>
#include <qdir.h>
#include <qfile.h>
#include <qdatastream.h>
#include <qtl.h>
#include <qtimer.h>

#include "DirWatcher.h"
#include "PlayList.h"
#include "IOController.h"
#include "InfoReader.h"
#include "InfoEvent.h"
#include "PlayListReadEvent.h"
#include "PlayListWrittenEvent.h"
#include "TrackDisappearedEvent.h"

const Q_UINT32 playListVersion = 2;

PlayList * PlayList::instance_ = 0L;

PlayList::PlayList()
	: QObject(),
		currentTrack_(0),
		dirWatcher_(0),
		initialReadComplete_(false)
{
	InfoReader::instance()->registerListener(this);

	dirtyTimer_ = new QTimer(this);

	connect
		(
		dirtyTimer_, SIGNAL(timeout()),
		this, SLOT(slotWrite())
		);

	updateTimer_ = new QTimer(this);

	connect
		(
		updateTimer_, SIGNAL(timeout()),
		this, SLOT(slotUpdate())
		);

	mutex_ = new QMutex;

	readThread_		= new ReadThread	(&trackList_, mutex_, this);
	writeThread_	= new WriteThread (&trackList_, mutex_, this);

	dirWatcher_ = new DirWatcher(mutex_);

	connect
		(
		 dirWatcher_, SIGNAL(trackDisappeared(TrackList)),
		 this,				SLOT(slotTrackDisappeared(TrackList))
		);

	readThread_->start();
}

PlayList::~PlayList()
{
	InfoReader::instance()->unregisterListener(this);
	trackList_.setAutoDelete(true);
	trackList_.clear();

	delete readThread_;
	readThread_ = 0;

	delete writeThread_;
	writeThread_ = 0;

	delete dirWatcher_;
	dirWatcher_ = 0;

	delete mutex_;
	mutex_ = 0;
}

	void
PlayList::_update(uint delay)
{
	updateTimer_->stop();
	updateTimer_->start(delay, true);
}

	void
PlayList::slotUpdate()
{
	filteredTrackList_.clear();

	for (TrackListIterator it(trackList_); it.current(); ++it)
		if (it.current()->score() > 0)
			filteredTrackList_.append(it.current());

	filteredTrackList_.sort();

	emit(changed());
}

	long
PlayList::count() const
{
	return filteredTrackList_.count();
}

	long
PlayList::lastTrack() const
{
	return filteredTrackList_.count() - 1;
}

	Track *
PlayList::track(const QString & id)
{
	TrackListIterator it(filteredTrackList_);

	for (; it.current(); ++it)
	{
		if (it.current()->filename() == id)
			return it.current();
	}

	return 0;
}

	TrackList &
PlayList::filtered()
{
	return filteredTrackList_;
}

	TrackList &
PlayList::all()
{
	return trackList_;
}

	void
PlayList::increaseScore(Track * t)
{
	t->setScore(t->score() + 1);
	emit(scoreChanged(t));
	_update(100);
	_dirty();
}

	void
PlayList::decreaseScore(Track * t)
{
	if (t->score() != 0)
	{
		t->setScore(t->score() - 1);
		emit(scoreChanged(t));
		_update(100);
		_dirty();
	}
}

	bool
PlayList::initialReadComplete() const
{
	return initialReadComplete_;
}

	bool
PlayList::event(QEvent * ev)
{
	bool accepted = false;

	switch (ev->type())
	{
		case InfoEventT:
			{
				Track * t(static_cast<InfoEvent *>(ev)->track());

				Track * existing = trackList_.find(t->filename());

				if (0 == existing)
				{
					trackList_.append(t);
					_dirty();
					emit(trackAdded(t));
				}
				else if (*t != *existing)
				{
					*t = *existing;
					_dirty();
					emit(trackChanged(t));
				}
			}
			accepted = true;
			break;

		case PlayListReadEventT:
			emit(trackListAdded(trackList_));
			initialReadComplete_ = true;
			_update();
			dirWatcher_->start();
			accepted = true;
			break;

		case PlayListWrittenEventT:
			accepted = true;
			break;

		default:
			break;
	}

	return accepted;
}

	void
PlayList::_dirty()
{
	dirtyTimer_->stop();
	dirtyTimer_->start(1000, true);
}

	void
PlayList::slotWrite()
{
	writeThread_->start();
}

PlayList::WriteThread::WriteThread(TrackList * t, QMutex * m, QObject * parent)
	: trackList_(t),
		mutex_(m),
		parent_(parent)
{
}
				
	void
PlayList::WriteThread::run()
{
	mutex_->lock();

	QFile f(QDir::homeDirPath() + "/.squelch/mastertracklist");

	if (!f.open(IO_WriteOnly))
	{
		qWarning("Can't open master track list");
		postEvent(parent_, new PlayListWrittenEvent);
		mutex_->unlock();
		return;
	}
	
	QDataStream str(&f);

	TrackListIterator it(*trackList_);

	for (; it.current(); ++it)
	{
		Track * t(it.current());
		str << *t;
	}

	f.close();

	postEvent(parent_, new PlayListWrittenEvent);

	mutex_->unlock();
}

PlayList::ReadThread::ReadThread(TrackList * t, QMutex * m, QObject * parent)
	: trackList_(t),
		mutex_(m),
		parent_(parent)
{
}
				
	void
PlayList::ReadThread::run()
{
	mutex_->lock();

	QFile f(QDir::homeDirPath() + "/.squelch/mastertracklist");

	if (!f.open(IO_ReadOnly))
	{
		postEvent(parent_, new PlayListReadEvent);
		mutex_->unlock();
		return;
	}
	
	QDataStream str(&f);

	while (!f.atEnd())
	{
		Track * t = new Track;
		str >> *t;
		trackList_->append(t);
	}

	f.close();

	postEvent(parent_, new PlayListReadEvent);

	mutex_->unlock();
}

	Track *
PlayList::find(const QString & filename)
{
	for (TrackListIterator it(trackList_); it.current(); ++it)
		if (it.current()->filename() == filename)
			return it.current();

	return 0;
}

	QString
PlayList::firstTrack() const
{
	if (filteredTrackList_.isEmpty())
		return QString::null;
	else
		return filteredTrackList_.getFirst()->filename();
}

	QString
PlayList::nextTrack(const QString & current) const
{
	if (filteredTrackList_.isEmpty())
	{
		return QString::null;
	}

	TrackListIterator it(filteredTrackList_);

	for (; it.current(); ++it)
	{
		if (it.current()->filename() == current)
		{
			if (it.current() == filteredTrackList_.getLast())
			{
				QString fn = filteredTrackList_.getFirst()->filename();
				return fn;
			}
			else
			{
				++it;
				return it.current()->filename();
			}
		}
	}

	return QString::null;
}

	QString
PlayList::previousTrack(const QString & current) const
{
	if (filteredTrackList_.isEmpty())
	{
		return QString::null;
	}

	TrackListIterator it(filteredTrackList_);

	for (; it.current(); ++it)
	{
		if (it.current()->filename() == current)
		{
			if (it.current() == filteredTrackList_.getFirst())
			{
				QString fn = filteredTrackList_.getLast()->filename();
				return fn;
			}
			else
			{
				--it;
				return it.current()->filename();
			}
		}
	}

	return QString::null;
}

	void
PlayList::slotTrackDisappeared(TrackList l)
{
	emit(trackListDisappeared(l));

	for (TrackListIterator it(l); it.current(); ++it)
		trackList_.removeRef(it.current());

	_update(100);
}

	void
PlayList::audioDirChanged()
{
	dirWatcher_->restart();
}

#include "PlayList.moc"

// vim:ts=2:sw=2:tw=78:noet
