# ROS 2 CI/CD 学習用サンプル

[![ROS 2 CI](../../actions/workflows/ci.yml/badge.svg)](../../actions/workflows/ci.yml)

GitHub Actions で ROS 2 (Jazzy) パッケージの **ビルド** と **単体テスト** を自動実行する CI のサンプルです。

## 構成

```
.
├── .github/workflows/ci.yml        # GitHub Actions のワークフロー定義
└── src/
    ├── sample_cpp_pkg/             # C++ (ament_cmake) + GTest
    │   ├── include/.../moving_average.hpp   # ロジック (ROS非依存)
    │   ├── src/moving_average.cpp
    │   ├── src/moving_average_node.cpp      # ROS ノード
    │   └── test/test_moving_average.cpp     # 単体テスト
    └── sample_py_pkg/              # Python (ament_python) + pytest
        ├── sample_py_pkg/clamp.py           # ロジック (ROS非依存)
        ├── sample_py_pkg/clamp_node.py      # ROS ノード
        └── test/test_clamp.py               # 単体テスト
```

| パッケージ | ノード | 動作 |
|---|---|---|
| `sample_cpp_pkg` | `moving_average_node` | `/input` (Float64) の移動平均を `/average` に配信 |
| `sample_py_pkg` | `clamp_node` | `/input` (Float64) を `[lower, upper]` に制限して `/clamped` に配信 |

**ポイント:** 計算ロジックを ROS から切り離したクラス・関数にしているので、ノードを起動しなくても単体テストできます。

## CI の流れ (`.github/workflows/ci.yml`)

`main` への push / Pull Request のたびに、次の順で実行されます。

1. **コンテナ起動** — `ros:jazzy` 公式 Docker イメージ (ROS 2 インストール済み) 上で動かす
2. **Checkout** — リポジトリを `ros_ws/src/repo` に取得
3. **rosdep install** — 各 `package.xml` の `<depend>` / `<test_depend>` を自動インストール
4. **colcon build** — ビルド
5. **colcon test** — GTest / pytest を実行
6. **colcon test-result** — 結果を集計。失敗があれば終了コード 1 でジョブを失敗させる
   (`colcon test` はテストが失敗しても成功扱いで終わるため、このステップが必要)
7. **Upload artifact** — テスト結果の XML を保存 (Actions の実行画面からダウンロード可能)

## ローカルで CI と同じことを実行する

```bash
source /opt/ros/jazzy/setup.bash
rosdep install --from-paths src --ignore-src -y
colcon build
source install/setup.bash
colcon test
colcon test-result --all --verbose
```

ノードを動かしてみる:

```bash
ros2 run sample_cpp_pkg moving_average_node
ros2 topic pub /input std_msgs/msg/Float64 "{data: 1.0}"   # 別ターミナル
ros2 topic echo /average                                     # 別ターミナル
```

## 試してみること

- テストをわざと失敗させて push し、CI が赤 (失敗) になるのを確認する
- Pull Request を作り、PR 画面に CI の結果が表示されるのを確認する
- GitHub の Settings → Branches → Branch protection rule で
  「Require status checks to pass」を有効にし、CI が通らないとマージできないようにする

## 発展

- **Lint の追加:** `ament_lint_auto` / `ament_lint_common` を `test_depend` に追加すると、
  `colcon test` でコードスタイルチェック (cpplint, flake8 など) も行われます
- **公式 Action の利用:** [`ros-tooling/setup-ros`](https://github.com/ros-tooling/setup-ros) と
  [`ros-tooling/action-ros-ci`](https://github.com/ros-tooling/action-ros-ci) を使うと、
  本サンプルの手順 3〜6 を数行で書けます
- **複数ディストリビューション:** `strategy.matrix` で `humble` / `jazzy` などを並列にテストできます
- **CD:** タグ push 時に Docker イメージをビルドして GHCR へ push する、などが典型的な次のステップです
