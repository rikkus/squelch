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
#include "TopItem.h"
#include "ArtistItem.h"

TopItem::TopItem(QListView * parent)
	: SquelchItem(RootItemT, parent, "All files")
{
	setExpandable(true);
}

TopItem::~TopItem()
{
}

	ArtistItem *
TopItem::artistItem(QString name)
{
	ArtistItem * i = static_cast<ArtistItem *>(firstChild());

	while (i)
	{
		if (i->name().lower() == name.lower())
			return i;

		i = static_cast<ArtistItem *>(i->nextSibling());
	}

	i = new ArtistItem(this, name);
	return i;
}

	void
TopItem::increaseScore()
{
	ArtistItem * i = static_cast<ArtistItem *>(firstChild());

	while (0 != i)
	{
		i->increaseScore();
		i = static_cast<ArtistItem *>(i->nextSibling());
	}
}

	void
TopItem::decreaseScore()
{
	ArtistItem * i = static_cast<ArtistItem *>(firstChild());

	while (0 != i)
	{
		i->decreaseScore();
		i = static_cast<ArtistItem *>(i->nextSibling());
	}
}

// vim:ts=2:sw=2:tw=78:noet
