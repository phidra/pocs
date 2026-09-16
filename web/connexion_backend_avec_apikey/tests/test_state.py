from backend.state import ApiKeyState


def test_state_starts_in_normal_mode_with_default_key():
    state = ApiKeyState()

    assert state.snapshot().as_dict() == {
        "mode": "normal",
        "current_key": "initial-value",
        "previous_key": None,
    }


def test_rotate_enters_transition_and_retains_old_key():
    state = ApiKeyState("old")

    snapshot = state.rotate("new")

    assert snapshot.as_dict() == {
        "mode": "transition",
        "current_key": "new",
        "previous_key": "old",
    }


def test_end_transition_removes_previous_key():
    state = ApiKeyState("old")
    state.rotate("new")

    snapshot = state.end_transition()

    assert snapshot.as_dict() == {
        "mode": "normal",
        "current_key": "new",
        "previous_key": None,
    }


def test_rotate_rejects_an_empty_key():
    state = ApiKeyState()

    try:
        state.rotate("")
    except ValueError as error:
        assert str(error) == "The new API key must not be empty."
    else:
        raise AssertionError("An empty key must be rejected.")


def test_reset_to_initial_restores_the_starting_key_and_normal_mode():
    state = ApiKeyState("initial")
    state.rotate("first-rotation")
    state.rotate("second-rotation")

    snapshot = state.reset_to_initial()

    assert snapshot.as_dict() == {
        "mode": "normal",
        "current_key": "initial",
        "previous_key": None,
    }
