#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

struct tree_node *tree_new();
void tree_free(const struct tree_node *this);
bool tree_add(struct tree_node **this, int value);
int tree_count(const struct tree_node *this);
int tree_height(const struct tree_node *this);
int tree_min(const struct tree_node *this);
int tree_max(const struct tree_node *this);

#endif
