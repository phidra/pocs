"""Flask application for administering in-memory API-key rotation state."""

from __future__ import annotations

import json
import random
import time
from functools import wraps
from pathlib import Path
from typing import Callable, TypeVar

from flask import Flask, Response, jsonify, request, send_from_directory

from backend.request_log import (
    MISSING_KEY,
    PREVIOUS_KEY,
    PUBLIC,
    UNKNOWN_KEY,
    RequestLog,
    classify_protected_request,
)
from backend.state import ApiKeyState

T = TypeVar("T")
ADMIN_DIRECTORY = Path(__file__).parent / "admin"
BOARD_DIRECTORY = Path(__file__).parent / "board"
LOGIN_DIRECTORY = Path(__file__).parent / "login"
LOGIN_PASSWORD = "pouet"
COOKIE_MAX_AGE = 60 * 60 * 24 * 365 * 50


def _sse_event(data: dict[str, object]) -> str:
    return f"event: request-log\ndata: {json.dumps(data)}\n\n"


def create_app(initial_key: str = "initial-value") -> Flask:
    app = Flask(__name__)
    key_state = ApiKeyState(initial_key)
    request_log = RequestLog()
    app.extensions["api_key_state"] = key_state
    app.extensions["request_log"] = request_log

    def record_public_access() -> None:
        """Record public routes; administration routes deliberately do not call it."""
        request_log.add(request.method, request.path, PUBLIC)

    def reset_everything():
        snapshot = key_state.reset_to_initial()
        request_log.clear()
        return snapshot

    def require_api_key(view: Callable[..., T]) -> Callable[..., T | Response]:
        """Authorize protected routes and silently refresh a transitioning API key."""

        @wraps(view)
        def wrapped(*args: object, **kwargs: object) -> T | Response:
            snapshot = key_state.snapshot()
            category = classify_protected_request(request.cookies.get("apikey"), snapshot)
            request_log.add(request.method, request.path, category)

            if category == MISSING_KEY:
                return jsonify(error="Missing apikey cookie."), 401
            if category == UNKNOWN_KEY:
                return jsonify(error="Invalid apikey cookie."), 403

            response = view(*args, **kwargs)
            if category == PREVIOUS_KEY:
                response = app.make_response(response)
                response.headers["Warning"] = '299 - "Deprecated API key in use"'
                response.set_cookie(
                    "apikey",
                    snapshot.current_key,
                    max_age=COOKIE_MAX_AGE,
                    samesite="Lax",
                )
            return response

        return wrapped

    # Expose reusable hooks for future public and protected routes.
    app.extensions["record_public_access"] = record_public_access
    app.extensions["require_api_key"] = require_api_key

    @app.get("/admin")
    def admin_page() -> Response:
        response = app.make_response(send_from_directory(ADMIN_DIRECTORY, "admin.html"))
        reset_everything()
        response.delete_cookie("apikey")
        return response

    @app.get("/board")
    def board_page() -> Response:
        record_public_access()
        return send_from_directory(BOARD_DIRECTORY, "board.html")

    @app.get("/board/board.js")
    def board_script() -> Response:
        return send_from_directory(BOARD_DIRECTORY, "board.js")

    @app.get("/api/current-value")
    @require_api_key
    def current_value() -> Response:
        return jsonify(value=random.randint(0, 100))

    @app.get("/login")
    def login_page() -> Response:
        record_public_access()
        return send_from_directory(LOGIN_DIRECTORY, "login.html")

    @app.get("/login/login.js")
    def login_script() -> Response:
        return send_from_directory(LOGIN_DIRECTORY, "login.js")

    @app.get("/api/session")
    @require_api_key
    def session() -> Response:
        time.sleep(2)  # simuler le fait que la validation de session n'est pas immédiate
        return jsonify(ok=True)

    @app.post("/api/login")
    def login() -> Response:
        record_public_access()
        payload = request.get_json(silent=True)
        password = payload.get("password") if isinstance(payload, dict) else None
        if password != LOGIN_PASSWORD:
            return jsonify(error="Invalid password."), 401

        response = jsonify(ok=True)
        response.set_cookie(
            "apikey",
            key_state.snapshot().current_key,
            max_age=COOKIE_MAX_AGE,
            samesite="Lax",
        )
        return response

    @app.get("/admin/<path:asset_name>")
    def admin_assets(asset_name: str) -> Response:
        if asset_name not in {"admin.css", "admin.js"}:
            return jsonify(error="Not found."), 404
        return send_from_directory(ADMIN_DIRECTORY, asset_name)

    @app.get("/admin/api/state")
    def get_state() -> Response:
        return jsonify(key_state.snapshot().as_dict())

    @app.post("/admin/api/rotate")
    def rotate_key() -> Response:
        payload = request.get_json(silent=True)
        new_key = payload.get("new_key") if isinstance(payload, dict) else None
        if not isinstance(new_key, str) or not new_key:
            return jsonify(error="Field 'new_key' must be a non-empty string."), 400
        return jsonify(key_state.rotate(new_key).as_dict())

    @app.post("/admin/api/end-transition")
    def end_transition() -> Response:
        return jsonify(key_state.end_transition().as_dict())

    @app.post("/admin/api/logs/clear")
    def clear_logs() -> Response:
        request_log.clear()
        return jsonify(cleared=True)

    @app.get("/admin/api/logs/stream")
    def logs_stream() -> Response:
        def generate() -> object:
            for entry in request_log.stream():
                if entry is None:
                    yield ": heartbeat\n\n"
                else:
                    yield _sse_event(entry.as_dict())

        return Response(
            generate(),
            content_type="text/event-stream",
            headers={"Cache-Control": "no-cache", "X-Accel-Buffering": "no"},
        )

    return app


app = create_app()


if __name__ == "__main__":
    app.run(host="127.0.0.1", port=5000, threaded=True, debug=True)
