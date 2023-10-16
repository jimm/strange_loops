#include <string>
#include "formatter.h"

// Translate floating-point value (to a precision of 0.001) to a string,
// removing trailing zeroes and decimal point if possible.
void format_float(float val, char *buf) {
  snprintf(buf, 16, "%0.2f", val);
  char *p = buf + strlen(buf) - 1;
  while (*p == '0') --p;
  if (*p != '.') ++p;
  *p = 0;
}
