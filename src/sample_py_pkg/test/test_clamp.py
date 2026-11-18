import pytest

from sample_py_pkg.clamp import clamp, deadband


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


def test_deadband_small_value_becomes_zero():
    assert deadband(0.05, 0.1) == 0.0
    assert deadband(-0.05, 0.1) == 0.0


def test_deadband_large_value_is_unchanged():
    assert deadband(0.5, 0.1) == 0.5
    assert deadband(-0.5, 0.1) == -0.5


def test_deadband_negative_width_raises():
    with pytest.raises(ValueError):
        deadband(0.0, -0.1)
