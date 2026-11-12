# HINT2
```
feed(a, v):
  a == NULL → NONE
  if !on:  above = v >= set ? above+1 : 0 ; if above >= set_n: on=1, above=below=0, n_occ++ → OCCURRED
  else:    below = v < clear ? below+1 : 0 ; if below >= clear_n: on=0, above=below=0, n_clr++ → CLEARED
  NONE
```
