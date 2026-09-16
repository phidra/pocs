from backend.request_log import (
    CURRENT_KEY,
    MISSING_KEY,
    PREVIOUS_KEY,
    UNKNOWN_KEY,
    RequestLog,
    classify_protected_request,
)
from backend.state import ApiKeySnapshot


def test_classifies_missing_key():
    assert classify_protected_request(None, ApiKeySnapshot("current", None)) == MISSING_KEY


def test_classifies_current_key():
    assert classify_protected_request("current", ApiKeySnapshot("current", None)) == CURRENT_KEY


def test_classifies_previous_key_only_during_transition():
    assert classify_protected_request("old", ApiKeySnapshot("current", "old")) == PREVIOUS_KEY
    assert classify_protected_request("old", ApiKeySnapshot("current", None)) == UNKNOWN_KEY


def test_classifies_unknown_key():
    assert classify_protected_request("other", ApiKeySnapshot("current", "old")) == UNKNOWN_KEY


def test_subscriber_receives_history_then_new_entries():
    log = RequestLog()
    old = log.add("GET", "/old", "public")
    history, subscriber = log.subscribe()
    new = log.add("GET", "/new", "current-key")

    assert history == [old]
    assert subscriber.get_nowait() == new


def test_clear_removes_retained_history_but_keeps_entry_ids_unique():
    log = RequestLog()
    log.add("GET", "/first", "public")

    log.clear()
    next_entry = log.add("GET", "/second", "public")
    history, subscriber = log.subscribe()
    log.unsubscribe(subscriber)

    assert next_entry.id == 2
    assert history == [next_entry]
