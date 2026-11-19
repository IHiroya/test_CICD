"""ROSに依存しないロジック部分(単体テストしやすくするため分離)."""


def clamp(value: float, lower: float, upper: float) -> float:
    """value を [lower, upper] の範囲に収める."""
    if lower > upper:
        raise ValueError(f'lower ({lower}) must be <= upper ({upper})')
    return max(lower, min(value, upper))


def deadband(value: float, width: float) -> float:
    """絶対値が width 未満の値を 0.0 にする(ジョイスティックの遊びなどの除去用)."""
    if width < 0.0:
        raise ValueError(f'width ({width}) must be >= 0')
    return 0.0 if abs(value) < width else value
