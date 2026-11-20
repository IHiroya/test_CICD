// GTest テストコードのテンプレート
//
// 使い方:
//   1. パッケージの test/ 配下に test_<対象>.cpp という名前でコピーする
//   2. <...> の部分をテスト対象に合わせて書き換える
//   3. 不要なテスト(モック・フィクスチャなど)は削除する
//   4. CMakeLists.txt の if(BUILD_TESTING) 内に登録する
//      (モックを使う場合は ament_add_gmock、使わない場合は ament_add_gtest)

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <stdexcept>

#include "<パッケージ名>/<ヘッダ名>.hpp"

using ::testing::Return;

// 本物の依存の代わりに、返す値や呼び出しを確認できるモック
class Mock<依存インターフェース> : public <依存インターフェース>
{
public:
  MOCK_METHOD(<戻り値の型>, <メソッド名>, (<引数>), (override));
};

// 各テストで共通に使うモックとテスト対象を用意する
class <テスト対象>Test : public ::testing::Test
{
protected:
  Mock<依存インターフェース> dependency_;
  <テスト対象> target_{dependency_};
};

// <正常系: どういう条件で、どうなること>
TEST_F(<テスト対象>Test, <条件>_<期待結果>)
{
  // Arrange
  EXPECT_CALL(dependency_, <メソッド名>()).WillOnce(Return(<値>));

  // Act
  auto result = target_.<メソッド>(<引数>);

  // Assert
  EXPECT_EQ(result, <期待値>);
}

// <境界値: どういう条件で、どうなること>
TEST_F(<テスト対象>Test, <条件>_AtBoundary)
{
  EXPECT_CALL(dependency_, <メソッド名>()).WillOnce(Return(<境界値>));

  EXPECT_NEAR(target_.<メソッド>(<引数>), <期待値>, 1e-9);
}

// <異常系: 不正な入力で例外が出ること>
TEST_F(<テスト対象>Test, InvalidInput_Throws)
{
  EXPECT_THROW(target_.<メソッド>(<不正な値>), std::invalid_argument);
}
