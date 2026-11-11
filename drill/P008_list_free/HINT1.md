# HINT1
`free(p)` 하기 **전에** `next = p->next` 를 지역 변수에 저장한다.
`list_remove` 는 "앞 노드" 대신 `node_t **pp` (앞 노드의 next, 또는 head 자신의 주소)를 따라가면 머리 노드 특수 처리가 사라진다.
