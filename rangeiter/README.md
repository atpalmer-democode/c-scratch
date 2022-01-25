# Range Iterators

Two designs to iterate over a range of integers. (See Python's [range](https://docs.python.org/3/library/functions.html#func-range) function.)


The following snippet outputs the integers 1 through 9.

```
    struct iter_state *iter = iter_range(1, 10, 1);
    while(iter_has_next(iter))
        printf("value: %d\n", iter_next(iter));
    iter_free(iter);
```

