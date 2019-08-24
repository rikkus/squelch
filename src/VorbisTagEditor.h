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

#ifndef VORBIS_TAG_EDITOR_H
#define VORBIS_TAG_EDITOR_H

#include <qobject.h>
#include <qthread.h>
#include <qcstring.h>
#include <qstring.h>
#include <qmap.h>

class VorbisTagEditor : public QObject
{
	Q_OBJECT
	
	public:

		VorbisTagEditor(QObject * parent);

		~VorbisTagEditor();

		void write
			(
			 const QString & inputFile,
			 const QMap<QCString, QString> & tags
			);

		class Thread : public QThread
		{
			public:

				Thread(QObject * parent);

				void setInputFile(const QString & inputFile);
				void setTags(const QMap<QCString, QString> & tags);

				~Thread();

				void setProgress(int);

			protected:

				void run();

			private:

				QObject * parent_;
				QString inputFile_;
				QMap<QCString, QString> tags_;
		};

		bool event(QEvent *);

	signals:

		void progress(int);
		void complete(bool);

	private:

		Thread * thread_;
};

#endif
// vim:ts=2:sw=2:tw=78:noet
