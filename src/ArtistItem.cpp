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
#include "SquelchItem.h"
#include "ArtistItem.h"
#include "AlbumItem.h"
#include "TitleItem.h"
#include "Track.h"

ArtistItem::ArtistItem
(
 QListViewItem * parent,
 QString artist
)
	: SquelchItem(ArtistItemT, parent, artist),
		artist_(artist)
{
	setExpandable(true);
	setPixmap(0, Static::instance()->pixmap("artist"));
}

ArtistItem::~ArtistItem()
{
}

	AlbumItem *
ArtistItem::albumItem(QString name)
{
	SquelchItem * i = static_cast<SquelchItem *>(firstChild());

	while (i)
	{
		if (i->type() == AlbumItemT && i->name().lower() == name.lower())
			return static_cast<AlbumItem *>(i);

		i = static_cast<SquelchItem *>(i->nextSibling());
	}

	return new AlbumItem(this, artist_, name);
}

	TitleItem *
ArtistItem::titleItem(QString name)
{
	SquelchItem * i = static_cast<SquelchItem *>(firstChild());

	while (i)
	{
		if (i->type() == TitleItemT && i->name().lower() == name.lower())
			return static_cast<TitleItem *>(i);

		i = static_cast<SquelchItem *>(i->nextSibling());
	}

	return 0;
}

	TitleItem *
ArtistItem::titleItem(Track * t)
{
	SquelchItem * i = static_cast<SquelchItem *>(firstChild());

	while (i)
	{
		if (i->type() == TitleItemT && i->name().lower() == t->title().lower())
			return static_cast<TitleItem *>(i);

		i = static_cast<SquelchItem *>(i->nextSibling());
	}

	return new TitleItem(this, t);
}

	void
ArtistItem::increaseScore()
{
	SquelchItem * i = static_cast<SquelchItem *>(firstChild());

	while (0 != i)
	{
		i->increaseScore();
		i = static_cast<SquelchItem *>(i->nextSibling());
	}
}

	void
ArtistItem::decreaseScore()
{
	SquelchItem * i = static_cast<SquelchItem *>(firstChild());

	while (0 != i)
	{
		i->decreaseScore();
		i = static_cast<SquelchItem *>(i->nextSibling());
	}
}

// vim:ts=2:sw=2:tw=78:noet
