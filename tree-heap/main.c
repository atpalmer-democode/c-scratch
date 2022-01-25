#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *left;
    struct node *right;
};

struct node *tree_rand(int levels) {
    struct node *new = malloc(sizeof *new);
    new->value = rand() % 100;
    new->left = levels ? tree_rand(levels - 1) : NULL;
    new->right = levels ? tree_rand(levels - 1) : NULL;
    return new;
}

struct node *tree_new(void) {
    return tree_rand(3);
}

void tree_free(struct node *this) {
    if(!this)
        return;
    tree_free(this->left);
    tree_free(this->right);
    free(this);
}

void tree_heapify(struct node *this);

void _branch_heapify(struct node *this, struct node *branch) {
    if(!branch)
        return;

    tree_heapify(branch);
    if(branch->value < this->value) {
        int tmp = branch->value;
        branch->value = this->value;
        this->value = tmp;
        tree_heapify(branch);
    }
}

void tree_heapify(struct node *this) {
    _branch_heapify(this, this->left);
    _branch_heapify(this, this->right);
}

void _tree_print(struct node *this) {
    if(!this) {
        printf("-");
        return;
    }
    printf("%d (", this->value);
    _tree_print(this->left);
    printf(") (");
    _tree_print(this->right);
    printf(")");
}

void tree_print(struct node *this) {
    _tree_print(this);
    printf("\n");
}

int main(void) {
    struct node *tree = tree_new();
    tree_print(tree);
    tree_heapify(tree);
    tree_print(tree);
    tree_free(tree);
}
