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

#ifndef OUTPUT_H
#define OUTPUT_H

extern "C"
{
#include "ao/ao.h"
}

#include <qthread.h>
#include <qstringlist.h>

#include "Defines.h"

class BufferItem;

class Output : public QThread
{
	public:

		enum Command { ReOpen, Pause, Continue, Flush, Suspend };

		Output(QObject * parent);
		virtual ~Output();

		long time() const;
		void flush();
		void pause(bool);
		void reopen();

		QStringList driverList() const;

		uint latency() const;

	protected:

		virtual void run();
		virtual void closeDevice();
		virtual bool openDevice(int sampleRate, int channels);

		void setStatus(OutputStatus s)	{ status_	= s;	}
		void setPause(bool b)						{ pause_	= b;	}
		void setFlush(bool b)						{ flush_	= b;	}

		void playEmptyBlock();

	private:

		QObject			* parent_;

		bool					pause_;
		bool					reopen_;
		bool					flush_;
		OutputStatus	status_;
		long					time_;
		QString				trackID_;
		long					trackLength_;

		ao_device		* device_;

		QStringList		driverList_;
};

#endif

// vim:ts=2:sw=2:tw=78:noet
