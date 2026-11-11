# HINT2 (막혔을 때만)
```
list_push(head, v):
    if head == NULL: return -1
    n = malloc(sizeof(*n)); if !n: return -1
    n->val = v; n->next = *head; *head = n; return 0

list_remove(head, v):
    for (pp = head; *pp; pp = &(*pp)->next)
        if (*pp)->val == v: victim = *pp; *pp = victim->next; free(victim); return 1
    return 0

list_free(head):
    count = 0
    for (cur = *head; cur; cur = next): next = cur->next; free(cur); count++
    *head = NULL; return count
```
