/* This program is licensed under the GNU Library General Public License, version 2,
 * a copy of which is included with this program (with filename LICENSE.LGPL).
 *
 * (c) 2000-2001 Michael Smith <msmith@labyrinth.net.au>
 *
 * VCEdit header.
 *
 * last modified: $ID:$
 */

#ifndef __VCEDIT_H
#define __VCEDIT_H

#ifdef __cplusplus
extern "C" {
#endif 

#include <stdio.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>

typedef void (*vcedit_progress_callback_t)(int, void *);

typedef size_t (*vcedit_read_func)(void *, size_t, size_t, void *);
typedef size_t (*vcedit_write_func)(const void *, size_t, size_t, void *);

typedef struct {
	ogg_sync_state		*oy;
	ogg_stream_state	*os;

	vorbis_comment		*vc;
	vorbis_info          vi;

	vcedit_read_func read;
	vcedit_write_func write;

	void		*in;
	long		serial;
	unsigned char	*mainbuf;
	unsigned char	*bookbuf;
	int		mainlen;
	int		booklen;
	char 	    *lasterror;
	int prevW;

  vcedit_progress_callback_t progress_callback;
  void * callback_metadata;

} vcedit_state;

extern vcedit_state *	vcedit_new_state(void);
extern void		vcedit_clear(vcedit_state *state);
extern vorbis_comment *	vcedit_comments(vcedit_state *state);
extern int		vcedit_open(vcedit_state *state, FILE *in);
extern int		vcedit_open_callbacks(vcedit_state *state, void *in,
		vcedit_read_func read_func, vcedit_write_func write_func);
extern int		vcedit_write(vcedit_state *state, void *out);
extern char *   vcedit_error(vcedit_state *state);

extern void vcedit_set_progress_callback(vcedit_state *,
  vcedit_progress_callback_t, void * metadata);

#ifdef __cplusplus
}
#endif 

#endif /* __VCEDIT_H */

