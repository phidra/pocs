#!/usr/bin/env python3

import heapq
from typing import List


h: List[int] = []  # cette liste fait office de heap

heapq.heappush(h, 42)
heapq.heappush(h, 43)
heapq.heappush(h, 44)
heapq.heappush(h, 45)
heapq.heappush(h, 5)
heapq.heappush(h, 4)

print(heapq.heappop(h))  # 4
print(heapq.heappop(h))  # 5
print(heapq.heappop(h))  # 42
