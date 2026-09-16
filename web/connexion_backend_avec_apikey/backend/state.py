"""Thread-safe in-memory state for the active and previous API keys."""

from dataclasses import dataclass
from threading import Lock


@dataclass(frozen=True)
class ApiKeySnapshot:
    """Immutable view of the API-key state."""

    current_key: str
    previous_key: str | None

    @property
    def mode(self) -> str:
        return "transition" if self.previous_key is not None else "normal"

    def as_dict(self) -> dict[str, str | None]:
        return {
            "mode": self.mode,
            "current_key": self.current_key,
            "previous_key": self.previous_key,
        }


class ApiKeyState:
    """Holds API keys in memory for the lifetime of the Flask process."""

    def __init__(self, initial_key: str = "initial-value") -> None:
        self._initial_key = initial_key
        self._current_key = initial_key
        self._previous_key: str | None = None
        self._lock = Lock()

    def snapshot(self) -> ApiKeySnapshot:
        with self._lock:
            return ApiKeySnapshot(self._current_key, self._previous_key)

    def rotate(self, new_key: str) -> ApiKeySnapshot:
        if not new_key:
            raise ValueError("The new API key must not be empty.")

        with self._lock:
            self._previous_key = self._current_key
            self._current_key = new_key
            return ApiKeySnapshot(self._current_key, self._previous_key)

    def end_transition(self) -> ApiKeySnapshot:
        with self._lock:
            self._previous_key = None
            return ApiKeySnapshot(self._current_key, self._previous_key)

    def reset_to_initial(self) -> ApiKeySnapshot:
        with self._lock:
            self._current_key = self._initial_key
            self._previous_key = None
            return ApiKeySnapshot(self._current_key, self._previous_key)
