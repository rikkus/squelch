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

#ifndef BUFFER_ITEM_H
#define BUFFER_ITEM_H

#include <qlist.h>
#include <qthread.h>

class BufferItem
{
	public:

		BufferItem();
		~BufferItem();

		static uint maxLength()				{ return maxLength_;		}

		char *	buffer()							{ return buffer_;				}
		uint		length()				const { return length_;				}
		int			time()					const { return time_;					}
		int			trackLength()		const { return trackLength_;	}
		QString	trackID()				const { return trackID_;			}
		int			bitRate()				const { return bitRate_;			}
		int			sampleRate()		const	{ return sampleRate_;		}
		int			channels()			const	{ return channels_;			}
		int			id()						const { return id_;						}

		void setLength(uint l)							{ length_				= l;	}
		void setTime(int t)									{ time_					= t;	}
		void setTrackLength(int l)					{ trackLength_	= l;	}
		void setTrackID(const QString & s)	{ trackID_			= s;	}
		void setBitRate(int r)							{ bitRate_			= r;	}
		void setSampleRate(int r)						{ sampleRate_		= r;	}
		void setChannels(int c)							{ channels_			= c;	}

	private:

		static uint maxLength_;

		char	* buffer_;
		uint		length_;
		int			time_;
		int			trackLength_;
		int			bitRate_;
		int			sampleRate_;
		int			channels_;

		QString	trackID_;

		int id_;
		static int ID_;
};

#endif
// vim:ts=2:sw=2:tw=78:noet
