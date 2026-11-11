# HINT2 (막혔을 때만)
```
static volatile sig_atomic_t g_count
on_signal(sig): g_count++
install: sa 채우고 sigaction(signo, &sa, NULL)
uninstall: sa.sa_handler = SIG_DFL; sigaction
count: return g_count; reset: g_count = 0
```
