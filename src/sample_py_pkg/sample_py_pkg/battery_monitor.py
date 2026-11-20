"""バッテリー残量を監視し、残量が少ないときに通知するクラス."""


class BatteryMonitor:
    """
    バッテリー残量を監視する.

    reader と notifier は外から受け取るので、テストではモックに差し替えられる.
      reader.read_percentage() -> float : 残量 [%] を返す (読み取り失敗時は例外)
      notifier.warn(message: str)       : 警告を通知する (実機ではログ出力やトピック配信)
    """

    def __init__(self, reader, notifier, threshold: float = 20.0):
        self.reader = reader
        self.notifier = notifier
        self.threshold = threshold
        self._warned = False

    def check(self) -> bool:
        """残量を確認し、残量が少なければ True を返す."""
        try:
            percentage = self.reader.read_percentage()
        except OSError as e:
            self.notifier.warn(f'バッテリー残量を読み取れません: {e}')
            return False

        low = percentage < self.threshold
        # 同じ警告を何度も出さないよう、閾値を下回った最初の1回だけ通知する
        if low and not self._warned:
            self.notifier.warn(f'バッテリー残量が少なくなっています: {percentage:.0f}%')
        self._warned = low
        return low
