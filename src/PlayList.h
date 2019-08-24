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

#ifndef PLAY_LIST_H
#define PLAY_LIST_H

#include <qthread.h>
#include <qobject.h>
#include <qstringlist.h>

#include "Track.h"
#include "TrackList.h"

class QTimer;
class DirWatcher;

class PlayList : public QObject
{
	Q_OBJECT

	public:

		static PlayList * instance()
		{
			if (0 == instance_)
				instance_ = new PlayList;

			return instance_;
		}

		~PlayList();

		Track * track(const QString & id);
		Track * find(const QString & filename);
		TrackList & filtered();
		TrackList & all();

		long count() const;
		long lastTrack() const;
		long startTrack() const;

		QString firstTrack() const;
		QString nextTrack(const QString & currentTrackID) const;
		QString previousTrack(const QString & currentTrackID) const;

		void increaseScore(Track *);
		void decreaseScore(Track *);

		void audioDirChanged();

		bool initialReadComplete() const;

	protected:

		bool event(QEvent *);

	protected slots:

		void slotWrite();
		void slotUpdate();
		void slotTrackDisappeared(TrackList);

	signals:

		void trackDisappeared(Track *);
		void trackListDisappeared(TrackList &);
		void trackListAdded(TrackList &);
		void trackAdded(Track *);
		void trackChanged(Track *);
		void scoreChanged(Track *);
		void changed();
		void itemRead();

	private:

		class WriteThread : public QThread
		{
			public:

				WriteThread(TrackList *, QMutex *, QObject * parent);
				
			protected:

				void run();

			private:

				TrackList * trackList_;
				QMutex * mutex_;
				QObject * parent_;
		};

		class ReadThread : public QThread
		{
			public:

				ReadThread(TrackList *, QMutex *, QObject * parent);
				
			protected:

				void run();

			private:

				TrackList * trackList_;
				QMutex * mutex_;
				QObject * parent_;
		};

		static PlayList * instance_;

		PlayList();
		void _update(uint delay = 0);
		void _dirty();

		TrackList		filteredTrackList_;
		TrackList		trackList_;
		long					currentTrack_;
		long					startTrack_;
		QTimer			* dirtyTimer_;
		QTimer			* updateTimer_;
		ReadThread	* readThread_;
		WriteThread * writeThread_;
		DirWatcher	* dirWatcher_;
		QMutex			* mutex_;

		QList<Track>	scoreChangedList_;

		bool initialReadComplete_;
};

#endif
// vim:ts=2:sw=2:tw=78:noet
