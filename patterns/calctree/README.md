# Calc Tree

Tree representation of a calculator.

The formula `2 + 2` may be represented with two "value" nodes and one "operation" node.

```
  +
 / \
2   2

(2 + 2) = 4
```

Value nodes may not have child nodes. Operation nodes must have two children which may be either value nodes or other operation nodes.

```
    *
   / \
  -   3
 / \
5   2

((5 - 2) * 3) = 9
```

