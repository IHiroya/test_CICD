import pytest

from sample_py_pkg.clamp import clamp


def test_value_within_range_is_unchanged():
    assert clamp(0.5, -1.0, 1.0) == 0.5


def test_value_below_range_is_clamped_to_lower():
    assert clamp(-5.0, -1.0, 1.0) == -1.0


def test_value_above_range_is_clamped_to_upper():
    assert clamp(5.0, -1.0, 1.0) == 1.0


def test_boundary_values():
    assert clamp(-1.0, -1.0, 1.0) == -1.0
    assert clamp(1.0, -1.0, 1.0) == 1.0


def test_invalid_range_raises():
    with pytest.raises(ValueError):
        clamp(0.0, 1.0, -1.0)
