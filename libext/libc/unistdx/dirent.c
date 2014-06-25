/* $Id: dirent.c,v 1.1 2000/12/16 14:01:46 amai Exp $ dirent.c 
   taken from (emx+gcc) */

#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <emx/io.h>
#include <emx/syscalls.h>

struct _dircontents
{
  char *                _d_entry;
  long                  _d_size;
  unsigned short        _d_attr;
  unsigned short        _d_time;
  unsigned short        _d_date;
  struct _dircontents * _d_next;
};

struct _dirdesc
{
  int                   dd_id;
  long                  dd_loc;
  struct _dircontents * dd_contents;
  struct _dircontents * dd_cp;
  struct dirent         dd_dirent;
};


static int _readdir_ino = 31415926;

static void free_dircontents (struct _dircontents *dp);
static char *getdirent (const char *dir, struct _find *pfind,
    const char *base);

int _closedir (DIR *dirp)
{
  free_dircontents (dirp->dd_contents);
  free (dirp);
  return 0;
}


struct dirent *_readdir (DIR *dirp)
{
  if (dirp->dd_cp == NULL)
    return NULL;
  strcpy (dirp->dd_dirent.d_name, dirp->dd_cp->_d_entry);
  dirp->dd_dirent.d_namlen = strlen (dirp->dd_dirent.d_name);
  dirp->dd_dirent.d_reclen = dirp->dd_dirent.d_namlen;
  dirp->dd_dirent.d_ino = _readdir_ino++;
  if (_readdir_ino == 0)
    _readdir_ino = 1;
  dirp->dd_dirent.d_size = dirp->dd_cp->_d_size;
  dirp->dd_dirent.d_attr = dirp->dd_cp->_d_attr;
  dirp->dd_dirent.d_time = dirp->dd_cp->_d_time;
  dirp->dd_dirent.d_date = dirp->dd_cp->_d_date;
  dirp->dd_cp = dirp->dd_cp->_d_next;
  ++dirp->dd_loc;
  return &dirp->dd_dirent;
}


void _seekdir (DIR *dirp, long off)
{
  long i;
  struct _dircontents *dp;
  
  if (off >= 0)
    {
      i = 0;
      for (dp = dirp->dd_contents; i < off && dp != NULL; dp = dp->_d_next)
        ++i;
      dirp->dd_loc = i;
      dirp->dd_cp = dp;
    }
  else
    errno = EINVAL;
}


void _rewinddir (DIR *dirp)
{
  _seekdir (dirp, 0L);
}


long _telldir (DIR *dirp)
{
  return dirp->dd_loc;
}


DIR *_opendir (const char *name)
{
  struct stat statb;
  struct _find find;
  DIR *dirp;
  char *s;
  struct _dircontents *dp;
  char nbuf[MAXPATHLEN+1];
  int len;
  
  len = strlen (name);
  memcpy (nbuf, name, len + 1);
  s = nbuf + len;
  if (_trslash (nbuf, len, 0))
    {
      nbuf[len++] = '.';        /* s now points to '.' */
      nbuf[len] = 0;
    }
  if (_stat (nbuf, &statb) < 0)
    return NULL;
  if ((statb.st_mode & S_IFMT) != S_IFDIR)
    {
      errno = ENOTDIR;
      return NULL;
    }
  dirp = malloc (sizeof (DIR));
  if (dirp == NULL)
    {
      errno = ENOMEM;
      return NULL;
    }
  if (*s == 0)
    *s++ = '\\';
  strcpy (s, "*.*");
  dirp->dd_loc = 0;
  dirp->dd_contents = NULL;
  dirp->dd_cp = NULL;
  s = getdirent (nbuf, &find, name);
  if (s == NULL)
    {
      errno = ENOMEM;
      return dirp;
    }
  do
    {
      dp = malloc (sizeof (struct _dircontents));
      if (dp == NULL)
        {
          free_dircontents (dirp->dd_contents);
          errno = ENOMEM;
          return NULL;
        }
      dp->_d_entry = malloc (strlen (s) + 1);
      if (dp->_d_entry == NULL)
        {
          free (dp);
          free_dircontents (dirp->dd_contents);
          errno = ENOMEM;
          return NULL;
        }
      if (dirp->dd_contents != NULL)
        dirp->dd_cp->_d_next = dp;
      else
        dirp->dd_contents = dp;
      dirp->dd_cp = dp;
      strcpy (dp->_d_entry, s);
      dp->_d_next = NULL;
      dp->_d_size = ((unsigned long)find.size_hi << 16) + find.size_lo;
      dp->_d_attr = find.attr;
      dp->_d_time = find.time;
      dp->_d_date = find.date;
      s = getdirent (NULL, &find, name);
    } while (s != NULL);
  dirp->dd_cp = dirp->dd_contents;
  return dirp;
}

extern DIR *opendir (const char *name)
{return _opendir(name);}

static void free_dircontents (struct _dircontents *dp)
{
  struct _dircontents *odp;

  while (dp != NULL)
    {
      if (dp->_d_entry != NULL)
        free(dp->_d_entry);
      odp = dp;
      dp = dp->_d_next;
      free (odp);
    }
}


static char *getdirent (const char *dir, struct _find *pfind, const char *base)
{
  int rc;

  if (dir != NULL)
    rc = __findfirst (dir, A_DIR|A_HIDDEN|A_SYSTEM, pfind);
  else
    rc = __findnext (pfind);
  if (rc == 0)
    {
      _fnlwr2 (pfind->name, base);
      return pfind->name;
    }
  else
    return NULL;
}
