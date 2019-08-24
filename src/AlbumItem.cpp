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

#include <qpixmap.h>

#include "Static.h"
#include "AlbumItem.h"
#include "ArtistItem.h"
#include "TitleItem.h"
#include "Track.h"

AlbumItem::AlbumItem
(
 ArtistItem * parent,
 QString artist,
 QString album
)
	: SquelchItem(AlbumItemT, parent, album),
		artist_(artist),
		album_(album)
{
	setExpandable(true);
	setPixmap(0, Static::instance()->pixmap("album"));

	if (album_.isEmpty())
		setText(0, "[none]");
}

AlbumItem::~AlbumItem()
{
}

	void
AlbumItem::increaseScore()
{
	TitleItem * i = static_cast<TitleItem *>(firstChild());

	while (0 != i)
	{
		i->increaseScore();
		i = static_cast<TitleItem *>(i->nextSibling());
	}
}

	void
AlbumItem::decreaseScore()
{
	TitleItem * i = static_cast<TitleItem *>(firstChild());

	while (0 != i)
	{
		i->decreaseScore();
		i = static_cast<TitleItem *>(i->nextSibling());
	}
}

	TitleItem *
AlbumItem::titleItem(QString title)
{
	TitleItem * i = static_cast<TitleItem *>(firstChild());

	while (i)
	{
		if (i->track()->title().lower() == title.lower())
			return i;

		i = static_cast<TitleItem *>(i->nextSibling());
	}

	return 0;
}

	TitleItem *
AlbumItem::titleItem(Track * t)
{
	TitleItem * i = static_cast<TitleItem *>(firstChild());

	while (i)
	{
		if (i->track()->title().lower() == t->title().lower())
			return i;

		i = static_cast<TitleItem *>(i->nextSibling());
	}

	return (new TitleItem(this, t));
}

// vim:ts=2:sw=2:tw=78:noet
