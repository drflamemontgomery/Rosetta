#ifndef _SAFE_H
#define _SAFE_H

#define safecall(fn, ...)                                                      \
  if (fn != NULL) {                                                            \
    fn(__VA_ARGS__);                                                           \
  }

#endif //_SAFE_H
