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

#ifndef IO_CONTROLLER_H
#define IO_CONTROLLER_H

#include <qobject.h>
#include <qlist.h>
#include <qqueue.h>
#include <qmap.h>
#include <qstringlist.h>

#include "Track.h"
#include "Defines.h"

class Input;
class Output;
class BufferItem;

class IOController : public QObject
{
	Q_OBJECT

	public:

		IOController();

		~IOController();

		static IOController * instance()
		{
			if (0 == instance_)
				instance_ = new IOController;

			return instance_;
		}

		bool event(QEvent *);

		void outputDriverChanged();

		QStringList outputDriverList() const;

		uint outputLatency() const;

	public slots:

		void slotSeek(int);
		void slotPlay();
		void slotStop();
		void slotNext();
		void slotPrevious();
		void slotRestart();
		void slotSkipToTrack(const QString &);
		void slotPlayListChanged();

	signals:

		void statusChange(OutputStatus);
		void timeChange(long);
		void trackLengthChange(long);
		void trackChange(const QString &);
		void bitRateChange(long);
		void newTrack(Track *);
		void deviceOpenInProgress();
		void deviceOpenComplete(bool);

	private:

		static IOController * instance_;

		// Order dependency.
		OutputStatus		outputStatus_;
		Input					* input_;
		Output				* output_;
		// End order dependency.
};

#endif
// vim:ts=2:sw=2:tw=78:noet
