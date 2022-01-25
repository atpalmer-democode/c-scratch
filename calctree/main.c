#include <stdio.h>
#include <stdlib.h>

enum node_type {
    VALUE_NODE,
    OP_NODE,
};

enum op {
    ADD,
    SUB,
    MULT,
    DIV,
};

struct node {
    enum node_type type;
    union {
        double value;
        int op;
    } data;
    struct node *left;
    struct node *right;
};

struct node *node_new_value(double value) {
    struct node *new = malloc(sizeof *new);
    new->type = VALUE_NODE;
    new->data.value = value;
    new->left = NULL;
    new->right = NULL;
    return new;
}

struct node *node_new_op(enum op op, struct node *l, struct node *r) {
    struct node *new = malloc(sizeof *new);
    new->type = OP_NODE;
    new->data.op = op;
    new->left = l;
    new->right = r;
    return new;
}

void node_free(struct node *this) {
    if(!this)
        return;
    node_free(this->left);
    node_free(this->right);
    free(this);
}

double node_value(struct node *this);

double _value_node_value(struct node *this) {
    return this->data.value;
}

double _op_node_value(struct node *this) {
    switch(this->data.op) {
        case ADD:
            return node_value(this->left) + node_value(this->right);
        case SUB:
            return node_value(this->left) - node_value(this->right);
        case MULT:
            return node_value(this->left) * node_value(this->right);
        case DIV:
            return node_value(this->left) / node_value(this->right);
        default:
            exit(-1); /* unreachable */
    }
}

double node_value(struct node *this) {
    switch(this->type) {
        case VALUE_NODE:
            return _value_node_value(this);
        case OP_NODE:
            return _op_node_value(this);
        default:
            exit(-1); /* unreachable */
    }
}

int main(void) {
    /* ((5 * 2) - 10) + (-25 / 5) = -5 */
    struct node *calc = node_new_op(ADD,
        node_new_op(SUB,
            node_new_op(MULT, node_new_value(5), node_new_value(2)),
            node_new_value(10)),
        node_new_op(DIV,
            node_new_value(-25),
            node_new_value(5))
        );

    printf("Value: %f\n", node_value(calc));

    node_free(calc);
}
