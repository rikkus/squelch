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

#include <string.h> // For memset.

#include <vorbis/vorbisfile.h>

#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qlayout.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qprogressbar.h>

#include "Track.h"
#include "VorbisTagEditWidget.h"
#include "VorbisTagEditDialog.h"
#include "VorbisHelpers.h"
#include "VorbisTagEditor.h"
#include "AddArtistDialog.h"
#include "AddExtraDialog.h"
#include "HelpDialog.h"

VorbisTagEditDialog::VorbisTagEditDialog(QWidget * parent)
	:	QDialog						(parent),
		track_						(0),
		addArtistDialog_	(0),
		addExtraDialog_		(0),
		helpDialog_				(0),
		dirty_						(false)
{
	progressBar_ = new QProgressBar(100, 0);
	progressBar_->hide();

	QVBoxLayout * layout = new QVBoxLayout(this);

	widget_ = new VorbisTagEditWidget(this, "Vorbis tag edit dialog", false);

	layout->addWidget(widget_);

	connect(widget_->pb_save,					SIGNAL(clicked()),	SLOT(slotSave()));
	connect(widget_->pb_close,				SIGNAL(clicked()),	SLOT(slotClose()));
	connect(widget_->pb_help,					SIGNAL(clicked()),	SLOT(slotHelp()));
	connect(widget_->pb_addArtist,		SIGNAL(clicked()),	SLOT(slotAddArtist()));
	connect(widget_->pb_removeArtist,	SIGNAL(clicked()),	SLOT(slotRemoveArtist()));
	connect(widget_->pb_addExtra,			SIGNAL(clicked()),	SLOT(slotAddExtra()));
	connect(widget_->pb_removeExtra,	SIGNAL(clicked()),	SLOT(slotRemoveExtra()));

	connect
		(
		 widget_->lb_artists,
		 SIGNAL(currentChanged(QListBoxItem *)),
		 this,
		 SLOT(slotArtistSelectionChanged(QListBoxItem *))
		);

	connect
		(
		 widget_->lv_extra,
		 SIGNAL(currentChanged(QListViewItem *)),
		 this,
		 SLOT(slotExtraSelectionChanged(QListViewItem *))
		);

	connect
		(
		 widget_->le_album,
		 SIGNAL(textChanged(const QString &)),
		 this,
		 SLOT(slotAlbumTextChanged(const QString &))
		);

	connect
		(
		 widget_->le_title,
		 SIGNAL(textChanged(const QString &)),
		 this,
		 SLOT(slotTitleTextChanged(const QString &))
		);

	tagEditor_ = new VorbisTagEditor(this);

	connect
		(
		 tagEditor_,
		 SIGNAL(progress(int)),
		 this,
		 SLOT(slotWriteProgress(int))
		);


	connect
		(
		 tagEditor_,
		 SIGNAL(complete(bool)),
		 this,
		 SLOT(slotWriteComplete(bool))
		);

	enable(false);
}

	void
VorbisTagEditDialog::setTrack(Track * t)
{
	if (dirty_)
	{
		int ret =
			QMessageBox::warning
			(
			 widget_,
			 tr("Warning - Vorbis tag editor"),
			 tr("Save changes ?"),
			 tr("Save"),
			 tr("Discard")
			);

		if (0 == ret)
		{
			slotSave();
		}
	}

	track_ = t;

	load();
}

	void
VorbisTagEditDialog::slotSave()
{
	QFileInfo fi(track_->filename());

	fileSize_ = fi.size();
	progress_ = 0;

	progressBar_->show();

	tagEditor_->write(fi.absFilePath(), readTagsFromWidgets());
}

	void
VorbisTagEditDialog::slotClose()
{
	if (dirty_)
	{
		int ret =
			QMessageBox::warning
			(
			 widget_,
			 tr("Warning - Vorbis tag editor"),
			 tr("Save changes ?"),
			 tr("Save"),
			 tr("Discard")
			);

		if (0 == ret)
		{
			slotSave();
		}
	}

	close();
}

	void
VorbisTagEditDialog::slotHelp()
{
	if (0 == helpDialog_)
		helpDialog_ =
			new HelpDialog(widget_, "Vorbis tag editor help dialog", false);

	helpDialog_->show();
}

	void
VorbisTagEditDialog::slotAddArtist()
{
	if (0 == addArtistDialog_)
	{
		addArtistDialog_ = new AddArtistDialog(widget_, "Add artist", true);

		connect
			(
			 addArtistDialog_->le_artistName,
			 SIGNAL(textChanged(const QString &)),
			 this,
			 SLOT(slotAddArtistDialogTextChanged(const QString &))
			);

		connect
			(
			 addArtistDialog_->buttonOk,
			 SIGNAL(clicked()),
			 this,
			 SLOT(slotAddArtistDialogAccepted())
			);
	}

	addArtistDialog_->le_artistName->clear();
	addArtistDialog_->le_artistName->setFocus();
	addArtistDialog_->show();
}

	void
VorbisTagEditDialog::slotRemoveArtist()
{
	widget_->lb_artists->removeItem(widget_->lb_artists->currentItem());
}

	void
VorbisTagEditDialog::slotAddExtra()
{
	if (0 == addExtraDialog_)
	{
		addExtraDialog_ = new AddExtraDialog(widget_, "Add extra", true);

		connect
			(
			 addExtraDialog_->le_name,
			 SIGNAL(textChanged(const QString &)),
			 this,
			 SLOT(slotAddExtraDialogTextChanged(const QString &))
			);

		connect
			(
			 addExtraDialog_->le_value,
			 SIGNAL(textChanged(const QString &)),
			 this,
			 SLOT(slotAddExtraDialogTextChanged(const QString &))
			);

		connect
			(
			 addExtraDialog_->buttonOk,
			 SIGNAL(clicked()),
			 this,
			 SLOT(slotAddExtraDialogAccepted())
			);
	}

	addExtraDialog_->le_name->clear();
	addExtraDialog_->le_value->clear();
	addExtraDialog_->le_name->setFocus();
	addExtraDialog_->exec();
}

	void
VorbisTagEditDialog::slotRemoveExtra()
{
	delete(widget_->lv_extra->currentItem());
}

	bool
VorbisTagEditDialog::load()
{
	QFileInfo fi(track_->filename());

	QFile f(fi.absFilePath());

	if (!f.open(IO_ReadOnly))
	{
		QMessageBox::warning
			(
			 widget_,
			 tr("Warning - Vorbis tag editor"),
			 tr("Could not open file %1").arg(fi.fileName())
			);

		return false;
	}

	OggVorbis_File vf;

	memset(&vf, 0, sizeof(vf));

	ov_callbacks callbacks;

	callbacks.read_func   = fread_func;
	callbacks.seek_func   = fseek_func;
	callbacks.close_func  = fclose_func;
	callbacks.tell_func   = ftell_func;

	if (ov_open_callbacks(&f, &vf, NULL, 0, callbacks) < 0)
	{
		QMessageBox::warning
			(
			 widget_,
			 tr("Warning - Vorbis tag editor"),
			 tr("Could not open file %1").arg(fi.fileName())
			);

		return false;
	}

	QMap<QCString, QString> map;

	char **ptr = ov_comment(&vf, -1)->user_comments;

	while (0 != *ptr)
	{
		QCString line(*ptr++);

		int equalsPos = line.find('=');

		if (-1 != equalsPos)
			map[line.left(equalsPos)] = QString::fromUtf8(line.mid(equalsPos + 1));
	}

	writeTagsToWidgets(map);

	ov_clear(&vf);

	enable(true);
	setDirty(false);
	widget_->l_filename->setText(fi.baseName());

	return true;
}

	void
VorbisTagEditDialog::clear()
{
	widget_->lb_artists->clear();
	widget_->le_album->clear();
	widget_->le_title->clear();
	widget_->lv_extra->clear();
}

	void
VorbisTagEditDialog::slotAddArtistDialogTextChanged(const QString & s)
{
	addArtistDialog_->buttonOk->setEnabled(!s.isEmpty());
}

	void
VorbisTagEditDialog::slotAddArtistDialogAccepted()
{
	setDirty(true);
	widget_->lb_artists->insertItem(addArtistDialog_->le_artistName->text());
}

	void
VorbisTagEditDialog::slotAddExtraDialogTextChanged(const QString &)
{
	bool problem =
		addExtraDialog_->le_name->text().isEmpty() &&
		addExtraDialog_->le_value->text().isEmpty();

	addExtraDialog_->buttonOk->setEnabled(!problem);
}

	void
VorbisTagEditDialog::slotAddExtraDialogAccepted()
{
	setDirty(true);
	QListViewItem * i = new QListViewItem(widget_->lv_extra);
	i->setText(0, addExtraDialog_->le_name->text());
	i->setText(1, addExtraDialog_->le_value->text());
}

	void
VorbisTagEditDialog::slotArtistSelectionChanged(QListBoxItem * i)
{
	widget_->pb_removeArtist->setEnabled(0 != i);
}

	void
VorbisTagEditDialog::slotExtraSelectionChanged(QListViewItem * i)
{
	widget_->pb_removeExtra->setEnabled(0 != i);
}

	void
VorbisTagEditDialog::setDirty(bool b)
{
	dirty_ = b;
	widget_->pb_save->setEnabled(b);
}

	void
VorbisTagEditDialog::enable(bool b)
{
	widget_->lb_artists->setEnabled(b);
	widget_->le_album->setEnabled(b);
	widget_->le_title->setEnabled(b);
	widget_->lv_extra->setEnabled(b);
	widget_->pb_addArtist->setEnabled(b);
	widget_->pb_removeArtist->setEnabled(b);
	widget_->pb_addExtra->setEnabled(b);
	widget_->pb_removeExtra->setEnabled(b);
}

	void
VorbisTagEditDialog::slotAlbumTextChanged(const QString &)
{
	setDirty(true);
}

	void
VorbisTagEditDialog::slotTitleTextChanged(const QString &)
{
	setDirty(true);
}

	QMap<QCString, QString>
VorbisTagEditDialog::readTagsFromWidgets() const
{
	QMap<QCString, QString> tagMap;

	for (int i = 0; i < widget_->lb_artists->count(); i++)
		if (!widget_->lb_artists->text(i).isEmpty())
			tagMap["artist"] = widget_->lb_artists->text(i);

	if (!widget_->le_album->text().isEmpty())
		tagMap["album"] = widget_->le_album->text();

	if (!widget_->le_title->text().isEmpty())
		tagMap["title"] = widget_->le_title->text();

	for (QListViewItemIterator it(widget_->lv_extra); it.current(); ++it)
	{
		QCString	tagName		= it.current()->text(0).latin1();
		QString		tagValue	= it.current()->text(1);

		if (!tagName.isEmpty() && !tagValue.isEmpty())
			tagMap[tagName] = tagValue;
	}

	return tagMap;
}

	void
VorbisTagEditDialog::writeTagsToWidgets(const QMap<QCString, QString> & m)
{
	clear();

	for (QMap<QCString, QString>::ConstIterator it(m.begin()); it != m.end(); ++it)
	{
		QCString	key		(it.key());
		QString		value	(it.data());

		if (0 == qstrnicmp(key, "artist", 6))
			widget_->lb_artists->insertItem(value);
		else
			if (0 == qstrnicmp(key, "album", 5) && widget_->le_album->text().isEmpty())
			widget_->le_album->setText(value);
		else
			if (0 == qstrnicmp(key, "title", 5) && widget_->le_title->text().isEmpty())
			widget_->le_title->setText(value);
		else
			new QListViewItem(widget_->lv_extra, QString::fromLatin1(key), value);
	}
}

	void
VorbisTagEditDialog::slotWriteProgress(int i)
{
	progress_ += int((progress_ / float(fileSize_)) * 100);
	progressBar_->setProgress(progress_);
}

	void
VorbisTagEditDialog::slotWriteComplete(bool b)
{
	fileSize_ = progress_ = 0;
	progressBar_->hide();
	progressBar_->reset();
	setDirty(!b);
}

#include "VorbisTagEditDialog.moc"
// vim:ts=2:sw=2:tw=78:noet
