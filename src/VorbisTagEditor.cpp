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

#include <vorbis/vorbisfile.h>

#include "vcedit.h"

#include <qfile.h>
#include <qfileinfo.h>

#include "VorbisTagEditor.h"
#include "TagEditCompleteEvent.h"
#include "TagEditProgressEvent.h"

extern "C"
{
	static void vcedit_progress_callback(int progress, void * metadata)
	{
		VorbisTagEditor::Thread * object =
			reinterpret_cast<VorbisTagEditor::Thread *>(metadata);

		object->setProgress(progress);
	}
}

VorbisTagEditor::VorbisTagEditor(QObject * parent)
	:	QObject(parent)
{
	thread_ = new Thread(this);
}

VorbisTagEditor::~VorbisTagEditor()
{
	delete thread_;
}

	void
VorbisTagEditor::write
(
 const QString & inputFile,
 const QMap<QCString, QString> & tags
)
{
	if (thread_->running())
	{
		qDebug("Fuckin thread is running already");
		return;
	}
	thread_->setInputFile(inputFile);
	thread_->setTags(tags);
	thread_->start();
}

	bool
VorbisTagEditor::event(QEvent * ev)
{
	switch (ev->type())
	{
		case TagEditCompleteEventT:
			emit(complete(static_cast<TagEditCompleteEvent *>(ev)->success()));
			return true;
			break;

		case TagEditProgressEventT:
			emit(progress(static_cast<TagEditProgressEvent *>(ev)->progress()));
			return true;
			break;

		default:
			return QObject::event(ev);
			break;
	}
}

VorbisTagEditor::Thread::Thread(QObject * parent)
	:	QThread			(),
		parent_			(parent)
{
}

VorbisTagEditor::Thread::~Thread()
{
}

	void
VorbisTagEditor::Thread::setProgress(int i)
{
	postEvent(parent_, new TagEditProgressEvent(i));
}

	void
VorbisTagEditor::Thread::setInputFile(const QString & inputFile)
{
	inputFile_ = inputFile;
}

	void
VorbisTagEditor::Thread::setTags(const QMap<QCString, QString> & tags)
{
	tags_ = tags;
}

	void
VorbisTagEditor::Thread::run()
{
	vcedit_state * state = vcedit_new_state();

	if (0 == state)
	{
		postEvent(parent_, new TagEditCompleteEvent(false));

		return;
	}

	vcedit_set_progress_callback
		(
		 state,
		 vcedit_progress_callback,
		 reinterpret_cast<void *>(this)
		);

	qDebug("initialised state ok");

	QFileInfo fi(inputFile_);

	QString s
		(
		 fi.dirPath(true)
		 +
		 "/."
		 +
		 fi.fileName()
		 +
		 ".temporary_copy_while_tags_edited_by_squelch"
		);

	QCString inputFilename	(QFile::encodeName(fi.absFilePath()));
	QCString outputFilename	(QFile::encodeName(s));

	FILE * inputFile = ::fopen(inputFilename, "rb");

	if (0 != vcedit_open(state, inputFile))
	{
		vcedit_clear(state);
		::fclose(inputFile);

		postEvent(parent_, new TagEditCompleteEvent(false));

		return;
	}

	qDebug("opened input file ok");

	vorbis_comment * vc = vcedit_comments(state);
	vorbis_comment_clear(vc);
	vorbis_comment_init(vc);

	QMap<QCString, QString>::ConstIterator it(tags_.begin());

	for (; it != tags_.end(); ++it)
	{
		vorbis_comment_add_tag
			(
			 vc,
			 const_cast<char *>(it.key().data()),
			 const_cast<char *>(it.data().utf8().data())
			);
	}

	qDebug("tag setup done");

	FILE * outputFile = ::fopen(outputFilename, "wb");

	if (0 == outputFile)
	{
		vcedit_clear(state);
		::fclose(inputFile);

		postEvent(parent_, new TagEditCompleteEvent(false));

		return;
	}

	qDebug("opened output file ok");

	if (0 != vcedit_write(state, outputFile))
	{
		vcedit_clear(state);
		::fclose(inputFile);

		postEvent(parent_, new TagEditCompleteEvent(false));
	}
	else
	{
		qDebug("written output ok");

		int ok = ::rename(outputFilename, inputFilename);

		vcedit_clear(state);
		::fclose(inputFile);
		::fclose(outputFile);

		postEvent(parent_, new TagEditCompleteEvent(0 == ok));
	}
}


#include "VorbisTagEditor.moc"
// vim:ts=2:sw=2:tw=78:noet
