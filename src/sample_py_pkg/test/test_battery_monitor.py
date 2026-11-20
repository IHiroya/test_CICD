from unittest.mock import Mock

from sample_py_pkg.battery_monitor import BatteryMonitor


def make_monitor(*percentages):
    """read_percentage() が percentages を順番に返すモックを使ってモニターを作る."""
    reader = Mock()
    reader.read_percentage.side_effect = list(percentages)
    notifier = Mock()
    return BatteryMonitor(reader, notifier, threshold=20.0), reader, notifier


# 残量が閾値以上のとき、残量を1回読むだけで警告しないこと
def test_no_warning_when_battery_is_enough():
    monitor, reader, notifier = make_monitor(80.0)

    assert monitor.check() is False
    reader.read_percentage.assert_called_once()
    notifier.warn.assert_not_called()


# 残量が閾値を下回ったとき、残量を含むメッセージで警告すること
def test_warns_when_battery_is_low():
    monitor, _, notifier = make_monitor(15.0)

    assert monitor.check() is True
    notifier.warn.assert_called_once_with('バッテリー残量が少なくなっています: 15%')


# 残量が少ない状態が続いても、警告は最初の1回だけであること
def test_warns_only_once_while_battery_stays_low():
    monitor, _, notifier = make_monitor(15.0, 14.0, 13.0)

    for _ in range(3):
        monitor.check()

    assert notifier.warn.call_count == 1


# 充電で残量が回復した後、再び少なくなったら改めて警告すること
def test_warns_again_after_recharge():
    monitor, _, notifier = make_monitor(15.0, 90.0, 10.0)

    for _ in range(3):
        monitor.check()

    assert notifier.warn.call_count == 2


# 残量の読み取りに失敗したとき、エラー内容を含むメッセージで警告すること
def test_warns_when_reader_fails():
    reader = Mock()
    reader.read_percentage.side_effect = OSError('device not found')  # 読み取り失敗を再現する
    notifier = Mock()
    monitor = BatteryMonitor(reader, notifier)

    assert monitor.check() is False
    notifier.warn.assert_called_once()
    assert 'device not found' in notifier.warn.call_args.args[0]
