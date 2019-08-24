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

#include <qbrush.h>
#include <qpainter.h>

#include "SquelchItem.h"
#include "Static.h"

SquelchItem::SquelchItem(ItemType t, QListView * parent, QString name)
	: QListViewItem(parent),
		type_(t),
		name_(name)
{
	setText(0, name_.isEmpty() ? QString("[not set]") : name_);
}

SquelchItem::SquelchItem(ItemType t, QListViewItem * parent, QString name)
	: QListViewItem(parent),
		type_(t),
		name_(name)
{
	setText(0, name_.isEmpty() ? QString("[not set]") : name_);
}

SquelchItem::~SquelchItem()
{
}

	void
SquelchItem::paintCell(
		QPainter * p,
		const QColorGroup & g,
		int c,
		int w,
		int a
)
{
	if (c < 3)
		return QListViewItem::paintCell(p, g, c, w, a);

	int top = height() / 2 - 8;

	if (c < 5 || TitleItemT == type_)
	{
		p->fillRect(0, 0, 16, height(), g.base());
		p->setPen(g.dark());
		p->drawRect(0, top, 16, 16);
	}

	switch (c)
	{
		case 3:
			p->drawPixmap(2, top + 2, Static::instance()->pixmap("plus"));
			break;

		case 4:
			p->drawPixmap(2, top + 2, Static::instance()->pixmap("minus"));
			break;

		case 5:
			if (TitleItemT == type_)
				p->drawPixmap(2, top + 2, Static::instance()->pixmap("x"));
			else
				QListViewItem::paintCell(p, g, c, w, a);
			break;
	}
}

// vim:ts=2:sw=2:tw=78:noet
