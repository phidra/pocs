from backend.app import create_app


def test_admin_state_and_rotation_endpoints_are_not_logged():
    app = create_app("first")
    client = app.test_client()

    assert client.get("/admin/api/state").get_json() == {
        "mode": "normal",
        "current_key": "first",
        "previous_key": None,
    }
    assert client.post("/admin/api/rotate", json={"new_key": "second"}).get_json() == {
        "mode": "transition",
        "current_key": "second",
        "previous_key": "first",
    }
    assert client.post("/admin/api/end-transition").get_json() == {
        "mode": "normal",
        "current_key": "second",
        "previous_key": None,
    }

    request_log = app.extensions["request_log"]
    history, subscriber = request_log.subscribe()
    request_log.unsubscribe(subscriber)
    assert history == []


def test_protected_route_decorator_authorizes_current_and_previous_keys():
    app = create_app("current")
    require_api_key = app.extensions["require_api_key"]

    @app.get("/protected")
    @require_api_key
    def protected():
        return {"ok": True}

    client = app.test_client()
    assert client.get("/protected").status_code == 401
    client.set_cookie("apikey", "wrong")
    assert client.get("/protected").status_code == 403
    client.set_cookie("apikey", "current")
    assert client.get("/protected").status_code == 200

    app.extensions["api_key_state"].rotate("new")
    client.set_cookie("apikey", "current")
    response = client.get("/protected")
    assert response.status_code == 200
    assert "Deprecated API key" in response.headers["Warning"]


def test_sse_endpoint_replays_request_log_entries():
    app = create_app()
    app.extensions["request_log"].add("GET", "/future-public-page", "public")

    response = app.test_client().get("/admin/api/logs/stream", buffered=False)
    first_chunk = next(response.response).decode()
    response.close()

    assert response.content_type == "text/event-stream"
    assert "event: request-log" in first_chunk
    assert '"path": "/future-public-page"' in first_chunk


def test_admin_page_resets_keys_logs_and_the_callers_cookie():
    app = create_app("initial")
    app.extensions["api_key_state"].rotate("rotated")
    app.extensions["request_log"].add("GET", "/board", "public")
    client = app.test_client()
    client.set_cookie("apikey", "rotated")

    response = client.get("/admin")
    history, subscriber = app.extensions["request_log"].subscribe()
    app.extensions["request_log"].unsubscribe(subscriber)

    assert response.status_code == 200
    assert history == []
    assert app.extensions["api_key_state"].snapshot().as_dict() == {
        "mode": "normal",
        "current_key": "initial",
        "previous_key": None,
    }
    assert "apikey=;" in response.headers["Set-Cookie"]
    assert "Max-Age=0" in response.headers["Set-Cookie"]


def test_clear_log_endpoint_clears_history_without_logging_itself():
    app = create_app()
    app.extensions["request_log"].add("GET", "/board", "public")

    response = app.test_client().post("/admin/api/logs/clear")
    history, subscriber = app.extensions["request_log"].subscribe()
    app.extensions["request_log"].unsubscribe(subscriber)

    assert response.get_json() == {"cleared": True}
    assert history == []
