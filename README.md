# PlainRadarWar
PlainRadarWar is clone of BattleShip game on Arduboy
---
## 概要
CPUと対戦するレーダー作戦ゲームです。Arduboyで動作します。
戦闘機とか機雷、水爆といったものはありません。５つの艦艇だけのバージョンです。

## ゲームの流れ
ゲームを開始すると、10x10のグリッドが２つ表示されます。右が自軍、左が敵軍（レーダー）です。
1. 艦艇を自軍エリアに配置する
  - 自軍エリアに表示される長方形が自軍の艦艇です。
  - 十字キーで移動させ、[A]ボタンで配置します。
  - [B]ボタンを押すと艦艇の向きの縦横が切り替わります。
  - 長方形が１マス分の大きさしかない場合は、そこには配置できないことを示しています。
 - ５艦配置し終われば対戦が開始されます。
2. 対戦する
  - 常にプレイヤーが先攻です。
  - 敵軍エリアに表示されたカーソルを十字キーで移動します。
  - [A]ボタンを押すとカーソル位置を攻撃します。
  - そこに敵軍の艦艇が隠れていれば×マークになります。いなければ・マークです。
  - １艦の全マスがヒットすれば●マークに変わります（撃沈）。
  - １マス攻撃すると、自動的に敵軍（CPU）が攻撃してきます。
  - 自軍の艦艇が撃沈されると■マークに変わります。
  - 先に相手の艦艇をすべて撃沈した方が勝者です。
3. 結果表示
  - 対戦が終わると勝敗とターン数が表示されます。
  - 負けた場合は隠れていた敵軍の艦艇の位置も表示されます。
  - [A]ボタンを押している間は結果表示枠が消えます。

## 連絡先
指摘、要望、ご相談等あれば、
iserlown@pop06.odn.ne.jp
までお願いします。
[HP](http://www.fpdock.net)は今は休止中ですが、そのうち再開しているかもしれません。
