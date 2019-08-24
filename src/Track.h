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

#ifndef TRACK_H
#define TRACK_H

#include <qobject.h>
#include <qstring.h>
#include <qdatastream.h>
#include <qlist.h>
#include <qdatetime.h>

class Track : public QObject
{
	Q_OBJECT

	public:

		Track();

		Track(
				const QString	& artist,
				const QString	& album,
				const QString	& title,
				const QString	& filename,
				const QDateTime & mTime,
				long length
		);

		Track(const Track & t);

		Track & operator = (const Track & t);

		bool operator < (const Track & t) const;

		bool operator <= (const Track & t) const
		{ return (operator < (t) || operator == (t)); }

		bool operator > (const Track & t) const
		{ return ! operator < (t); }

		bool operator == (const Track &) const;

		bool operator != (const Track &) const;

		QString		artist()					const	{ return artist_;					}
		QString		album()						const	{ return album_;					}
		QString		title()						const	{ return title_;					}
		QString		filename()				const	{ return filename_;				}
		QDateTime	mTime()						const	{ return mTime_;					}
		long			length()					const	{ return length_;					}
		int				score()						const	{ return score_;					}
		int				fileScore()				const	{ return fileScore_;			}
		QString		key()							const;

		QString		lengthAsString()	const;

		void			setArtist		(const QString & s)		{ artist_		= s;	}
		void			setAlbum		(const QString & s)		{ album_		= s;	}
		void			setName			(const QString & s)		{ title_		= s;	}
		void			setFilename	(const QString & s)		{ filename_ = s;	}
		void			setMTime		(const QDateTime & t)	{ mTime_		= t;	}
		void			setLength		(long l)							{ length_		= l;	}
		void			setScore		(int i);

		friend QDataStream & operator << (QDataStream &, const Track &);
		friend QDataStream & operator >> (QDataStream & str, Track &);

	signals:

		void scoreChanged(int);

	private:

		const char * name() const { return ""; }

		QString		artist_;
		QString		album_;
		QString		title_;
		QString		filename_;
		QDateTime	mTime_;
		long			length_;
		int				score_;
		int				fileScore_;
};

#endif
// vim:ts=2:sw=2:tw=78:noet
