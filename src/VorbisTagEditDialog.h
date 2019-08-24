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

#ifndef VORBIS_TAG_EDIT_DIALOG_H
#define VORBIS_TAG_EDIT_DIALOG_H

#include <qstring.h>
#include <qmap.h>
#include <qdialog.h>

class VorbisTagEditWidget;
class AddArtistDialog;
class AddExtraDialog;
class HelpDialog;
class VorbisTagEditor;

class QListBoxItem;
class QListViewItem;
class QProgressBar;

class Track;

class VorbisTagEditDialog : public QDialog
{
	Q_OBJECT

	public:

		VorbisTagEditDialog(QWidget * parent = 0);

		void setTrack(Track *);

	protected slots:
	
		void slotSave();
		void slotClose();
		void slotHelp();
		void slotAddArtist();
		void slotRemoveArtist();
		void slotAddExtra();
		void slotRemoveExtra();

		void slotAddArtistDialogTextChanged(const QString & s);
		void slotAddArtistDialogAccepted();
		void slotAddExtraDialogTextChanged(const QString &);
		void slotAddExtraDialogAccepted();

		void slotArtistSelectionChanged(QListBoxItem *);
		void slotExtraSelectionChanged(QListViewItem *);

		void slotAlbumTextChanged(const QString &);
		void slotTitleTextChanged(const QString &);

		void slotWriteProgress(int);
		void slotWriteComplete(bool);

	protected:

		bool load();
		void clear();
		void setDirty(bool);
		void enable(bool);

	private:

		QMap<QCString, QString> readTagsFromWidgets() const;
		void writeTagsToWidgets(const QMap<QCString, QString> &);
		QProgressBar * progressBar_;

		Track * track_;

		VorbisTagEditWidget * widget_;

		AddArtistDialog * addArtistDialog_;
		AddExtraDialog * addExtraDialog_;
		HelpDialog * helpDialog_;

		bool dirty_;

		VorbisTagEditor * tagEditor_;

		int fileSize_;
		int progress_;
};

#endif
// vim:ts=2:sw=2:tw=78:noet
