#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

struct tree_node {
    int value;
    struct tree_node *left;
    struct tree_node *right;
};

static struct tree_node *tree_node_new(
    int value,
    struct tree_node *left,
    struct tree_node *right)
{
    struct tree_node *this = malloc(sizeof(struct tree_node));
    *this = (struct tree_node) {
        .value = value,
        .left = left,
        .right = right,
    };
    return this;
}

static void tree_node_free(const struct tree_node *this)
{
    free((void *)this);
}

struct tree_node *tree_new()
{
    return NULL;
}

void tree_free(const struct tree_node *this)
{
    if(this == NULL) {
        return;
    }
    struct tree_node *left = this->left;
    struct tree_node *right = this->right;
    tree_node_free(this);
    tree_free(left);
    tree_free(right);
}

bool tree_add(struct tree_node **this, int value)
{
    if(*this == NULL) {
        *this = tree_node_new(value, NULL, NULL);
        return true;
    }
    if((*this)->value == value) {
        return false;
    }
    struct tree_node **next = (value < (*this)->value)
        ? &(*this)->left
        : &(*this)->right;
    return tree_add(next, value);
}

int tree_count(const struct tree_node *this)
{
    if(this == NULL) {
        return 0;
    }
    return 1 + tree_count(this->left) + tree_count(this->right);
}

int tree_height(const struct tree_node *this)
{
    if(this == NULL) {
        return 0;
    }
    int left = tree_height(this->left);
    int right = tree_height(this->right);
    int max = (left > right) ? left : right;
    return 1 + max;
}

int tree_min(const struct tree_node *this)
{
    assert(this != NULL);
    return (this->left == NULL) ? this->value : tree_min(this->left);
}

int tree_max(const struct tree_node *this)
{
    assert(this != NULL);
    return (this->right == NULL) ? this->value : tree_max(this->right);
}
