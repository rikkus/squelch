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

#ifndef ALBUM_ITEM
#define ALBUM_ITEM

#include <qlistview.h>
#include <qstring.h>

#include "SquelchItem.h"

class Track;
class ArtistItem;
class TitleItem;
class PlayListEditWidget;
class QPainter;
class QColorGroup;

class AlbumItem : public SquelchItem
{
	public:

		AlbumItem(ArtistItem *, QString artist, QString album);
		virtual ~AlbumItem();

		void increaseScore();
		void decreaseScore();

		TitleItem * titleItem(QString);
		TitleItem * titleItem(Track *);

		QString name() const	{ return album_; }

	private:

		PlayListEditWidget	* lv_;
		QString								artist_;
		QString								album_;
};

#endif
// vim:ts=2:sw=2:tw=78:noet
