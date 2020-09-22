#include "load.h"

#include <string.h>
#include "environment.h"
#include "error.h"
#include "eval.h"
#include "inport.h"
#include "outport.h"
#include "output.h"
#include "parser.h"
#include "primproc.h"

#define AREA "LOAD"

static char *name = NULL;

obj load_u(struct inport *in, struct outport *out, obj *unev, bool verbose)
{
	char *prevname = name;
	name = in->name;
	obj exp = (unev == NULL) ? readp(in) : *unev;

	for (; !is_err(exp) && !is_eof(exp); exp = readp(in)) {
		obj dat = eval(exp, tge());
		if (is_err(dat)) {
			name = prevname;
			return dat;
		}
		if (verbose && !is_void(dat)) {
			writep(out, dat);
			newlinep(out);
		}
	}
	name = prevname;
	return _void;
}

static struct strbldr *addbase(struct strbldr *sb, char *path)
{
	char *last, *s;

	if (path == NULL)
		return sb;
	last = path + strlen(path);
	while (--last >= path && *last != '/' && *last != ':' && *last != '\\')
		;
	s = path;
	while (s <= last)
		sb_addc(sb, *s++);
	return sb;
}

static obj load(obj args, bool verbose)
{
	obj chk;

	if (is_err(chk = chkarity("load", 1, args)))
		return chk;

	obj filename = car(args);
	if (!is_string(filename))
		return error_argument_type(AREA, "load got a non-string: %s",
					   errstr(filename));

	struct strbldr *sb = new_strbldr();
	if (sb == NULL)
		return error_memory(AREA, "making strbldr");
	if (addbase(sb, name) == NULL)
		return error_io(AREA, "failed to get base path: %s", name);
	sb_adds(sb, to_string(filename));
	sb_adds(sb, ".sicp");
	char *path = sb_string(sb);

	struct inport *in = openin_file(path);
	if (in == NULL)
		return error_io(AREA, "failed to load file: %s", path);
	if (verbose) {
		displaydat(of_string("Loading: "));
		displaydat(of_string(path));
		newline(emptylst);
	}
	obj rslt = load_u(in, default_out(), NULL, verbose);
	if (verbose) {
		displaydat(of_string("Loaded: "));
		displaydat(of_string(path));
		newline(emptylst);
	}
	in_close(in);
	sb_free(&sb);
	return rslt;
}

obj loadq(obj args)
{
	return load(args, false);
}

obj loadv(obj args)
{
	return load(args, true);
}
