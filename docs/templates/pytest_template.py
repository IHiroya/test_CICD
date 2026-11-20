# pytest テストコードのテンプレート
#
# 使い方:
#   1. パッケージの test/ 配下に test_<対象>.py という名前でコピーする
#   2. <...> の部分をテスト対象に合わせて書き換える
#   3. 不要なテスト(モック・パラメータ化など)は削除する

from unittest.mock import Mock

import pytest

from <パッケージ名>.<モジュール名> import <テスト対象クラス>


@pytest.fixture
def target():
    """各テストで使うテスト対象を作る(依存はモックで注入する)."""
    dependency = Mock()
    return <テスト対象クラス>(dependency), dependency


# <正常系: どういう条件で、どうなること>
def test_<対象>_<条件>_<期待結果>(target):
    # Arrange
    obj, dependency = target
    dependency.<メソッド>.return_value = <値>

    # Act
    result = obj.<メソッド>(<引数>)

    # Assert
    assert result == <期待値>
    dependency.<メソッド>.assert_called_once_with(<引数>)


# <境界値: 複数の入力で期待結果を確認する>
@pytest.mark.parametrize('value, expected', [
    (<最小値>, <期待値>),
    (<最大値>, <期待値>),
])
def test_<対象>_boundary(value, expected):
    assert <関数>(value) == expected


# <異常系: 不正な入力で例外が出ること>
def test_<対象>_invalid_input_raises_error():
    with pytest.raises(ValueError, match='<メッセージの一部>'):
        <関数>(<不正な値>)
