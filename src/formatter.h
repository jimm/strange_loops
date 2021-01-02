#ifndef FORMATTER_H
#define FORMATTER_H

// Translate floating-point value (to a precision of 0.001) to a string,
// removing trailing zeroes and decimal point if possible.
void format_float(float val, char *buf);

#endif /* FORMATTER_H */
