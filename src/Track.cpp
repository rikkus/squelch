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

#include <qfile.h>
#include "Track.h"

Track::Track()
	: QObject			(),
		length_			(0L),
		score_			(0),
		fileScore_	(0)
{
}

Track::Track(
		const QString		&	artist,
		const QString		&	album,
		const QString		&	title,
		const QString		&	filename,
		const QDateTime &	mTime,
		long							length
)
	: QObject				(),
		artist_				(artist),
		album_				(album),
		title_				(title),
		filename_			(filename),
		mTime_				(mTime),
		length_				(length),
		score_				(0),
		fileScore_		(0)
{
}

Track::Track(const Track & t)
	: QObject					(),
		artist_					(t.artist_),
		album_					(t.album_),
		title_					(t.title_),
		filename_				(t.filename_),
		mTime_					(t.mTime_),
		length_					(t.length_),
		score_					(t.score_),
		fileScore_			(t.fileScore_)
{
	if (title_.isEmpty())
		title_ = QString::fromUtf8("Unknown");
}

Track & Track::operator = (const Track & t)
{
	if (this == &t) return *this;

	artist_					= t.artist_;
	album_					= t.album_;
	title_					= t.title_;
	filename_				= t.filename_;
	mTime_					= t.mTime_;
	length_					= t.length_;
	score_					= t.score_;
	fileScore_			= t.fileScore_;

	return *this;
}

	bool
Track::operator == (const Track & t) const
{
	return
		(
		 artist_		== t.artist_		&&
		 album_			== t.album_			&&
		 title_			== t.title_			&&
		 filename_	== t.filename_	&&
		 length_		== t.length_
		);
}

	bool
Track::operator != (const Track & t) const
{
	return ! operator == (t);
}

QDataStream & operator << (QDataStream & str, const Track & t)
{
	str
		<< t.artist_
		<< t.album_
		<< t.title_
		<< t.filename_
		<< t.mTime_
		<< t.length_
		<< t.score_
		<< t.fileScore_;

	return str;
}

QDataStream & operator >> (QDataStream & str, Track & t)
{
	str
		>> t.artist_
		>> t.album_
		>> t.title_
		>> t.filename_
		>> t.mTime_
		>> t.length_
		>> t.score_
		>> t.fileScore_;

	return str;
}

	bool
Track::operator < (const Track & t) const
{
	return key() < t.key();
}

	QString
Track::key() const
{
	QString k;
	// Hopefully no-one is giving tracks a score of a million.
	k.sprintf("%07d", 1000000 - score_);
	k += filename_;
	return k;
}

	QString
Track::lengthAsString() const
{
	QString s;

	int min = length_ / 60;
	int sec = length_ - min * 60;

	s =
		QString::number(min) + ':';
	
	if (sec < 10)
	 s += '0';
	
	s += QString::number(sec);

	return s;
}

	void
Track::setScore(int i)
{
	score_ = i;
	emit(scoreChanged(score_));
}

#include "Track.moc"

// vim:ts=2:sw=2:tw=78:noet
