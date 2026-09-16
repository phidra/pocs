import backend.app as app_module
from backend.app import create_app


def get_log_history(app):
    request_log = app.extensions["request_log"]
    history, subscriber = request_log.subscribe()
    request_log.unsubscribe(subscriber)
    return history


def test_board_page_is_public_and_is_logged_as_public():
    app = create_app()
    response = app.test_client().get("/board")

    assert response.status_code == 200
    assert b'id="value"' in response.data
    assert b'id="toggle-polling"' in response.data
    assert [(entry.path, entry.category) for entry in get_log_history(app)] == [("/board", "public")]


def test_board_script_is_served_without_adding_a_log_entry():
    app = create_app()
    response = app.test_client().get("/board/board.js")

    assert response.status_code == 200
    assert b"setInterval(refreshValue, 3000)" in response.data
    assert b"stopPolling" in response.data
    assert get_log_history(app) == []


def test_current_value_rejects_a_missing_api_key():
    app = create_app()
    response = app.test_client().get("/api/current-value")

    assert response.status_code == 401
    assert [(entry.path, entry.category) for entry in get_log_history(app)] == [("/api/current-value", "missing-key")]


def test_current_value_rejects_an_unknown_api_key():
    app = create_app()
    client = app.test_client()
    client.set_cookie("apikey", "unknown")

    response = client.get("/api/current-value")

    assert response.status_code == 403
    assert [(entry.path, entry.category) for entry in get_log_history(app)] == [("/api/current-value", "unknown-key")]


def test_current_value_accepts_the_current_api_key(monkeypatch):
    monkeypatch.setattr(app_module.random, "randint", lambda minimum, maximum: 73)
    app = create_app()
    client = app.test_client()
    client.set_cookie("apikey", "initial-value")
    response = client.get("/api/current-value")

    assert response.status_code == 200
    assert response.get_json() == {"value": 73}
    assert [(entry.path, entry.category) for entry in get_log_history(app)] == [("/api/current-value", "current-key")]


def test_current_value_accepts_the_previous_key_during_transition(monkeypatch):
    monkeypatch.setattr(app_module.random, "randint", lambda minimum, maximum: 42)
    app = create_app("old")
    app.extensions["api_key_state"].rotate("new")
    client = app.test_client()
    client.set_cookie("apikey", "old")

    response = client.get("/api/current-value")

    assert response.status_code == 200
    assert response.get_json() == {"value": 42}
    assert "Deprecated API key" in response.headers["Warning"]
    assert "apikey=new" in response.headers["Set-Cookie"]
    assert "HttpOnly" not in response.headers["Set-Cookie"]

    next_response = client.get("/api/current-value")
    assert next_response.status_code == 200
    assert [(entry.path, entry.category) for entry in get_log_history(app)] == [
        ("/api/current-value", "previous-key-transition"),
        ("/api/current-value", "current-key"),
    ]
