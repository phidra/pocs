"""In-memory request classification and fan-out for the administration SSE feed."""

from __future__ import annotations

from dataclasses import asdict, dataclass
from datetime import timezone, datetime
from queue import Empty, Queue
from threading import Lock
from typing import Iterator

from backend.state import ApiKeySnapshot

PUBLIC = "public"
CURRENT_KEY = "current-key"
PREVIOUS_KEY = "previous-key-transition"
UNKNOWN_KEY = "unknown-key"
MISSING_KEY = "missing-key"


def classify_protected_request(cookie_value: str | None, state: ApiKeySnapshot) -> str:
    """Classify a protected request according to the supplied API key."""
    if cookie_value is None:
        return MISSING_KEY
    if cookie_value == state.current_key:
        return CURRENT_KEY
    if state.previous_key is not None and cookie_value == state.previous_key:
        return PREVIOUS_KEY
    return UNKNOWN_KEY


@dataclass(frozen=True)
class RequestLogEntry:
    id: int
    timestamp: str
    method: str
    path: str
    category: str

    def as_dict(self) -> dict[str, str | int]:
        return asdict(self)


class RequestLog:
    """Stores all entries in process memory and broadcasts new ones to SSE clients."""

    def __init__(self) -> None:
        self._entries: list[RequestLogEntry] = []
        self._subscribers: set[Queue[RequestLogEntry]] = set()
        self._next_id = 1
        self._lock = Lock()

    def add(self, method: str, path: str, category: str) -> RequestLogEntry:
        with self._lock:
            entry = RequestLogEntry(
                id=self._next_id,
                timestamp=datetime.now(timezone.utc).isoformat(),
                method=method,
                path=path,
                category=category,
            )
            self._next_id += 1
            self._entries.append(entry)
            for subscriber in self._subscribers:
                subscriber.put(entry)
        return entry

    def clear(self) -> None:
        """Remove retained history without disrupting connected SSE subscribers."""
        with self._lock:
            self._entries.clear()

    def subscribe(self) -> tuple[list[RequestLogEntry], Queue[RequestLogEntry]]:
        """Atomically capture history and register a subscriber for future entries."""
        subscriber: Queue[RequestLogEntry] = Queue()
        with self._lock:
            history = list(self._entries)
            self._subscribers.add(subscriber)
        return history, subscriber

    def unsubscribe(self, subscriber: Queue[RequestLogEntry]) -> None:
        with self._lock:
            self._subscribers.discard(subscriber)

    def stream(self) -> Iterator[RequestLogEntry | None]:
        history, subscriber = self.subscribe()
        try:
            yield from history
            while True:
                try:
                    yield subscriber.get(timeout=15)
                except Empty:
                    # A heartbeat keeps intermediary proxies from closing an idle SSE connection.
                    yield None
        finally:
            self.unsubscribe(subscriber)
