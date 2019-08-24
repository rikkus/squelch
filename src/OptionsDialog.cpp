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

#include <qcombobox.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qfile.h>
#include <qdir.h>
#include <qlineedit.h>
#include <qtextstream.h>

#include "IOController.h"
#include "OptionsDialog.h"
#include "PlayList.h"

OptionsDialog::OptionsDialog(QWidget * parent, const char * name)
	: OptionsDialogBase(parent, name)
{
	QStringList outputDriverList = IOController::instance()->outputDriverList();

	QStringList::ConstIterator it(outputDriverList.begin());

	for (; it != outputDriverList.end(); ++it)
		cmb_outputDriver->insertItem(*it);

	connect(pb_browse, SIGNAL(clicked()), this, SLOT(slotBrowse()));

	load();
}

OptionsDialog::~OptionsDialog()
{
}

	void
OptionsDialog::slotBrowse()
{
	QString s = QFileDialog::getExistingDirectory(QDir::homeDirPath(), this);

	if (!!s)
		le_audioDir->setText(s);
}

	void
OptionsDialog::accept()
{
	save();
	emit(closed());
	QDialog::accept();
}

	void
OptionsDialog::reject()
{
	load();
	emit(closed());
	QDialog::reject();
}

	void
OptionsDialog::closeEvent(QCloseEvent * e)
{
	emit(closed());
	QWidget::closeEvent(e);
}

	void
OptionsDialog::load()
{
	{
		QFile f(QDir::homeDirPath() + "/.squelch/audiodir");

		if (f.open(IO_ReadOnly))
		{
			QTextStream t(&f);
			QString s = t.readLine();
			le_audioDir->setText(s);
			savedAudioDir_ = s;
		}
	}

	{
		QFile f(QDir::homeDirPath() + "/.squelch/outputdriver");

		if (f.open(IO_ReadOnly))
		{
			QTextStream t(&f);
			QString s = t.readLine();

			for (int i = 0; i < cmb_outputDriver->count(); i++)
			{
				if (cmb_outputDriver->text(i).lower() == s.lower())
				{
					cmb_outputDriver->setCurrentItem(i);
					break;
				}
			}

			savedOutputDriver_ = s;
		}
	}

	{
		QFile f(QDir::homeDirPath() + "/.squelch/outputdriverflags");

		if (f.open(IO_ReadOnly))
		{
			QTextStream t(&f);
			QString s = t.readLine();
			le_outputFlags->setText(s);
			savedOutputFlags_ = s;
		}
	}
}

	void
OptionsDialog::save()
{
	bool audioDirChanged = le_audioDir->text() != savedAudioDir_;

	bool outputDriverChanged =
		cmb_outputDriver->currentText().lower() != savedOutputDriver_.lower();

	bool outputFlagsChanged = le_outputFlags->text() != savedOutputFlags_;

	if (audioDirChanged)
	{
		QFile f(QDir::homeDirPath() + "/.squelch/audiodir");

		if (f.open(IO_WriteOnly))
		{
			QTextStream t(&f);
			t << le_audioDir->text() << endl;
			savedAudioDir_ = le_audioDir->text();
		}
	}

	if (outputDriverChanged)
	{
		QFile f(QDir::homeDirPath() + "/.squelch/outputdriver");

		if (f.open(IO_WriteOnly))
		{
			QTextStream t(&f);
			t << cmb_outputDriver->currentText().lower() << endl;
			savedOutputDriver_ = cmb_outputDriver->currentText().lower();
		}
	}

	if (outputFlagsChanged)
	{
		QFile f(QDir::homeDirPath() + "/.squelch/outputdriverflags");

		if (f.open(IO_WriteOnly))
		{
			QTextStream t(&f);
			t << le_outputFlags->text() << endl;
			savedOutputFlags_ = le_outputFlags->text();
		}
	}

	if (audioDirChanged)
		PlayList::instance()->audioDirChanged();

	if (outputDriverChanged || outputFlagsChanged)
		IOController::instance()->outputDriverChanged();
}

#include "OptionsDialog.moc"

// vim:ts=2:sw=2:tw=78:noet
