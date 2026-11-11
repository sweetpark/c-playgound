# P008 · list_free — 해제한 노드를 다시 읽지 마라

## 요구사항
단일 연결리스트(`int` 값)의 세 함수를 만든다.
1. `list_push` — 값을 가진 새 노드를 **맨 앞**에 붙인다.
2. `list_remove` — 값이 같은 **첫 노드**를 떼어 내 해제한다(머리·가운데·꼬리 모두).
3. `list_free` — 모든 노드를 해제하고 `*head = NULL`, **해제한 개수**를 돌려준다.

## 시그니처
```c
typedef struct node { int val; struct node *next; } node_t;   /* solution.c 에 이미 있음 */

int    list_push(node_t **head, int v);     /* 0 성공 / -1 실패(head == NULL, 할당 실패) */
int    list_remove(node_t **head, int v);   /* 1 지움 / 0 없음·빈 리스트·head == NULL */
size_t list_free(node_t **head);            /* 해제한 개수. 빈 리스트·head == NULL 이면 0 */
```

## 입출력 예시
| 동작 | 리스트(앞→뒤) | 반환 |
|---|---|---|
| push 1, 2, 3 | `3 2 1` | 0, 0, 0 |
| remove 2 | `3 1` | 1 |
| remove 3 (머리) | `1` | 1 |
| remove 99 | `1` | 0 |
| remove 1 (마지막 하나) | (빈 리스트, `head == NULL`) | 1 |
| 5 개 push 후 free | `head == NULL` | 5 |

## 제약
- `for (p = head; p; p = p->next) free(p);` 는 금지 — 해제한 노드의 `next` 를 읽는다(use-after-free).
- 누수 0 (LeakSanitizer), 해제한 메모리 접근 0 (ASan).
- 정렬·중복 제거 등은 하지 않는다.

## 통과 기준
`make P008` → `PASS n/n`, 경고 0, ASan/UBSan/LSan 0. 힌트: `HINT1.md` → `HINT2.md`.
