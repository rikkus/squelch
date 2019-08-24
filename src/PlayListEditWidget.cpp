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

#include <qapplication.h>
#include <qdir.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qheader.h>
#include <qstringlist.h>
#include <qpainter.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qsplitter.h>
#include <qdragobject.h>
#include <qfiledialog.h>
#include <qdatetime.h>

#include "PlayListEditWidget.h"
#include "TopItem.h"
#include "ArtistItem.h"
#include "AlbumItem.h"
#include "TitleItem.h"
#include "Track.h"
#include "PreviewWidget.h"
#include "PlayList.h"
#include "VorbisTagEditDialog.h"
#include "Defines.h"

PlayListEditWidget::PlayListEditWidget(QWidget * parent, const char * name)
	: QWidget(parent, name, WType_TopLevel),
		tagEditor_(0)
{
	setCaption("Playlist - squelch");

	QVBoxLayout * layout = new QVBoxLayout(this);

	QSplitter * split = new QSplitter(Vertical, this, "PlayListEdit split");

	layout->addWidget(split);

	list_ = new QListView(split);
	preview_ = new PreviewWidget(split);

	list_->addColumn(tr("Track"));
	list_->addColumn(tr("Time"));
	list_->addColumn(tr("Score"));
	list_->addColumn("+");
	list_->addColumn("-");
	list_->addColumn("X");
	list_->setRootIsDecorated(true);
//	list_->header()->hide();

	list_->header()->resizeSection(3, 16);
	list_->header()->resizeSection(4, 16);
	list_->header()->resizeSection(5, 16);
	list_->header()->setResizeEnabled(false, 3);
	list_->header()->setResizeEnabled(false, 4);
	list_->header()->setResizeEnabled(false, 5);

	connect
		(
		 list_,
		 SIGNAL(mouseButtonClicked(int, QListViewItem *, const QPoint &, int)),
		 this,
		 SLOT(slotMouseClicked(int, QListViewItem *, const QPoint &, int))
		);

	connect
		(
		 PlayList::instance(),	SIGNAL(trackAdded(Track *)),
		 this,									SLOT(slotTrackAdded(Track *))
		);

	connect
		(
		 PlayList::instance(),	SIGNAL(trackListAdded(TrackList &)),
		 this,									SLOT(slotTrackListAdded(TrackList &))
		);

	connect
		(
		 PlayList::instance(),	SIGNAL(trackListDisappeared(TrackList &)),
		 this,									SLOT(slotTrackListDisappeared(TrackList &))
		);

	top_ = new TopItem(list_);
	top_->setOpen(true);

	if (PlayList::instance()->initialReadComplete())
	{
		slotTrackListAdded(PlayList::instance()->all());
	}
}

PlayListEditWidget::~PlayListEditWidget()
{
}

	ArtistItem *
PlayListEditWidget::_artistItem(QString name)
{
	return top_->artistItem(name);
}

	void
PlayListEditWidget::slotMouseClicked(
		int button,
		QListViewItem * item,
		const QPoint &,
		int c
)
{
	if (button != LeftButton || 0 == item)
		return;

	SquelchItem * i = static_cast<SquelchItem *>(item);

	switch (c)
	{
		case 3:
			i->increaseScore();
			break;

		case 4:
			i->decreaseScore();
			break;

		case 5:
			if (i->type() == TitleItemT)
			{
				if (0 == tagEditor_)
					tagEditor_ = new VorbisTagEditDialog;

				tagEditor_->setTrack(static_cast<TitleItem *>(i)->track());
				tagEditor_->show();
			}
			break;

		default:
			break;
	}
}

	void
PlayListEditWidget::closeEvent(QCloseEvent * e)
{
	emit(closed());
	QWidget::closeEvent(e);
}

	void
PlayListEditWidget::slotTrackAdded(Track * t)
{
	if (t->album().isEmpty())
		_artistItem(t->artist())->titleItem(t);
	else
		_artistItem(t->artist())->albumItem(t->album())->titleItem(t);
}

	void
PlayListEditWidget::slotTrackChanged(Track * t)
{
	if (t->album().isEmpty())
		_artistItem(t->artist())->titleItem(t)->update();
	else
		_artistItem(t->artist())->albumItem(t->album())->titleItem(t)->update();
}

	void
PlayListEditWidget::slotTrackListAdded(TrackList & l) 
{
	list_->setUpdatesEnabled(false);

	// XXX Should use a QTimer here.
	QTime time;
	time.start();

	for (TrackListIterator it(l); it.current(); ++it)
	{
		Track * t(it.current());
		if (t->album().isEmpty())
			_artistItem(t->artist())->titleItem(t);
		else
			_artistItem(t->artist())->albumItem(t->album())->titleItem(t);

		if (time.elapsed() > 10)
		{
			time.restart();
			qApp->processEvents();
		}
	}

	list_->setUpdatesEnabled(true);
	list_->triggerUpdate();
}

	TitleItem *
PlayListEditWidget::_find(Track * t)
{
	for (QListViewItemIterator it(list_); it.current(); ++it)
		if (static_cast<SquelchItem *>(it.current())->type() == TitleItemT)
			if (*(static_cast<TitleItem *>(it.current())->track()) == *t)
				return static_cast<TitleItem *>(it.current());

	return 0;
}

	void
PlayListEditWidget::slotTrackListDisappeared(TrackList & l)
{
	list_->setUpdatesEnabled(false);

	for (TrackListIterator it(l); it.current(); ++it)
		delete _find(it.current());

	list_->setUpdatesEnabled(true);
	list_->triggerUpdate();
}

#include "PlayListEditWidget.moc"

// vim:ts=2:sw=2:tw=78:noet
