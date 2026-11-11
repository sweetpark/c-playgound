# HINT1
`pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK)` 후 `pthread_mutex_init(&c->mu, &attr)`. `counter_get` 은 `lock → v = c->v → unlock → return v`.
