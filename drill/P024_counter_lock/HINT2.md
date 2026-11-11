# HINT2 (막혔을 때만)
```
init: attr 만들어 ERRORCHECK 지정 -> mutex_init(&c->mu, &attr) -> attr destroy; c->v = 0
add:  lock; c->v += d; unlock
get:  lock; v = c->v; unlock; return v
destroy: return pthread_mutex_destroy(&c->mu)
```
