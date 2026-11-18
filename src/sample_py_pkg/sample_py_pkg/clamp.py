"""ROSに依存しないロジック部分(単体テストしやすくするため分離)."""


def clamp(value: float, lower: float, upper: float) -> float:
    """value を [lower, upper] の範囲に収める."""
    if lower > upper:
        raise ValueError(f'lower ({lower}) must be <= upper ({upper})')
    return max(lower, min(value, upper))
