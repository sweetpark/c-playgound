# NOTE · P008 list_free (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[C] 동적 메모리 실습 — malloc·free 오류 패턴과 유연 배열 구조체 연결리스트」 §1 "free 후 참조 — 9-08 (치명적)", §6 연결리스트 delete / free_list
- 소스: [`c_basic/lecture/mem/9-08.c`](../../c_basic/lecture/mem/9-08.c), [`c_basic/mem/linkedlist.c`](../../c_basic/mem/linkedlist.c)

**함정 요약**
- `for (p = head; p; p = p->next) free(p)` — 해제된 노드의 `next` 를 읽는다. 테스트에서는 이전 값이 우연히 남아 통과하기 쉽고, 운영에서 프로세스가 죽는다(원본 글의 장애 사례).
- 삭제 시 머리/꼬리 갱신 누락. 이중 포인터(`pp`)로 순회하면 "앞 노드가 없는 경우" 분기가 사라진다.
- 해제 후 `*head = NULL` 로 되돌려 놓지 않으면 호출자의 `head` 가 댕글링.

**한 줄 설명 연습**: "해제하기 전에 필요한 값(next)을 먼저 꺼내 두고, 해제한 뒤에는 그 포인터를 건드리지 않는다."
