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

#ifndef PLAYLIST_EDIT_WIDGET_H
#define PLAYLIST_EDIT_WIDGET_H

#include <qwidget.h>
#include <qstringlist.h>

#include "Track.h"

class QListView;
class QListViewItem;
class TopItem;
class ArtistItem;
class TitleItem;
class TrackList;
class PreviewWidget;
class QPopupMenu;
class VorbisTagEditDialog;

class PlayListEditWidget : public QWidget
{
	Q_OBJECT

	public:

		PlayListEditWidget(QWidget * parent = 0, const char * name = 0);
		virtual ~PlayListEditWidget();

	protected:

		void _readArtists(QString);
		void _readAlbums(QString, QString, QListViewItem *);
		void _readTracks(QString, QString, QListViewItem *);
		TitleItem * _find(Track *);

		void closeEvent(QCloseEvent *);

	signals:

		void signalChangeStatusbar(const QString& text);
		void signalChangeCaption(const QString& text);
		void closed();

	protected slots:

		void slotMouseClicked(int, QListViewItem *, const QPoint &, int);
		void slotTrackAdded(Track *);
		void slotTrackChanged(Track *);
		void slotTrackListAdded(TrackList &);
		void slotTrackListDisappeared(TrackList &);

	private:

		ArtistItem * _artistItem(QString);

		QListView * list_;
		TopItem * top_;
		PreviewWidget * preview_;

		VorbisTagEditDialog * tagEditor_;
};

#endif // PLAYLIST_EDIT_WIDGET_H
// vim:ts=2:sw=2:tw=78:noet
