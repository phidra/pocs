from backend.app import COOKIE_MAX_AGE, create_app


def get_log_history(app):
    request_log = app.extensions["request_log"]
    history, subscriber = request_log.subscribe()
    request_log.unsubscribe(subscriber)
    return history


def test_login_page_is_public_and_logged():
    app = create_app()
    response = app.test_client().get("/login")

    assert response.status_code == 200
    assert b'id="login-form"' in response.data
    assert b'id="session-check"' in response.data
    assert [(entry.path, entry.category) for entry in get_log_history(app)] == [("/login", "public")]


def test_login_script_is_not_logged():
    app = create_app()
    response = app.test_client().get("/login/login.js")

    assert response.status_code == 200
    assert b"getRedirectTarget" in response.data
    assert get_log_history(app) == []


def test_login_rejects_an_invalid_password_and_logs_the_attempt():
    app = create_app()
    response = app.test_client().post("/api/login", json={"password": "wrong"})

    assert response.status_code == 401
    assert "apikey=" not in response.headers.get("Set-Cookie", "")
    assert [(entry.path, entry.category) for entry in get_log_history(app)] == [("/api/login", "public")]


def test_login_sets_a_long_lived_readable_current_key_cookie():
    app = create_app("current-key")
    response = app.test_client().post("/api/login", json={"password": "pouet"})
    cookie = response.headers["Set-Cookie"]

    assert response.status_code == 200
    assert response.get_json() == {"ok": True}
    assert "apikey=current-key" in cookie
    assert f"Max-Age={COOKIE_MAX_AGE}" in cookie
    assert "HttpOnly" not in cookie
    assert "SameSite=Lax" in cookie


def test_session_uses_the_same_key_authorization_rules():
    app = create_app("current")
    client = app.test_client()

    assert client.get("/api/session").status_code == 401
    client.set_cookie("apikey", "unknown")
    assert client.get("/api/session").status_code == 403
    client.set_cookie("apikey", "current")
    assert client.get("/api/session").get_json() == {"ok": True}

    app.extensions["api_key_state"].rotate("new")
    client.set_cookie("apikey", "current")
    response = client.get("/api/session")
    assert response.get_json() == {"ok": True}
    assert "Deprecated API key" in response.headers["Warning"]
