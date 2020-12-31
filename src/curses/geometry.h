#ifndef GEOMETRY_H
#define GEOMETRY_H

typedef struct rect {
  int row, col, height, width;
} rect;

rect geom_message_rect();
rect geom_info_rect();
rect geom_help_rect();
rect geom_prompt_rect();

#endif /* GEOMETRY_H */
