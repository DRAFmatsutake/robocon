# ロボコン用プログラム
- [更新履歴](#更新履歴)
- [リファレンス](#リファレンス)
- [ファイル詳細](#ファイル詳細)

## 更新履歴
- 2021-11-05  プログラムのアップデート
    - 全体
        - log吐き出しを#ifdefに変更
    - moter
        - 時限関数の実装
        - 打ち出しの戻り値の変更
        - 停止の追加
    - com
        - タイマーの実装

- 2021-10-27  readme作成
    - moter
        - モーターの打ち出し部関数作成
    - com
        - マニュアル用の定数の定義
    - robot
        - マニュアル時の打ち出し実装

## 動作処理用リファレンス
### タイヤ
<table border="1">
    <tr>
        <td>void Wheel(char left,char right)</td>
        <td><details><summary>両モーターの動作設定</summary>
            <b>引数　:</b>left,right : それぞれのモーターの回転量<br>
            <b>戻り値:</b>無し<br><br>
            <b>解説</b>：両方のモーターを同時に設定する<br>
                -127～127の範囲の値を設定することができ、-128は停止となる<br>
                停止はWheelStop()を用意してあるがこの場合両方停止となる<br>
        </details></td>
    </tr>
    <tr>
        <td>void TimerWheel(char left,char right,int time)</td>
        <td><details><summary>モーターを一定時間動作させる</summary>
            <b>引数　:</b>left,right : それぞれのモーターの回転量<br>
                        time : 動作時間[ms]<br>
            <b>戻り値:</b>無し<br><br>
            <b>解説：</b>時間制限付きでモーターを動作させる<br>
            基本動作は Wheel() と同じで時間経過後は WheelStop() が実行される<br>
            この処理が動作中はWheel[Right][Left]、は無視される<br>
        </details></td>
    </tr>
    <tr>
        <td>void WheelStop(void)</td>
        <td><details><summary>モーターを停止させる</summary>
            <b>引数　:</b>無し<br>
            <b>戻り値:</b>無し<br><br>
            <b>解説：</b>Wheel(-128,-128) が実行される<br>
        </details></td>
    </tr>
    <tr>
        <td>void TimerStop(void)</td>
        <td><details><summary>TimerWheel() を中断し上書きを許可する</summary>
            <b>引数　:</b>無し<br>
            <b>戻り値:</b>無し<br><br>
            <b>解説：</b>TimerWheelに割り込み中止させる<br>
            TimeWheel() 中はWheel()　等の処理が実行されないため一度中断させる<br>
        </details></td>
    </tr>
    <tr>
        <td>void WheelRight(char value)</td>
        <td><details><summary>右モーターにのみ指令を送る</summary>
            <b>引数　:</b>value :　回転量<br>
            <b>戻り値:</b>無し<br><br>
            <b>解説：</b>右モーターのみに指令を送る<br>
            -127～127の範囲の値を設定することができ、-128は停止となる<br>
            右のみのため左モーターは以前の指令値のまま動作する。<br>
        </details></td>
    </tr>
    <tr>
        <td>void WheelLeft (char value)</td>
        <td><details><summary>左モーターにのみ指令を送る</summary>
            <b>引数　:</b>value :　回転量<br>
            <b>戻り値:</b>無し<br><br>
            <b>解説：</b>WHeelRight() の左バージョン<br>
        </details></td>
    </tr>
</table>

### アーム
<table border="1">
    <tr>
        <td>int SetArm(char value)</td>
        <td><details><summary>アームを持ち上げる</summary>
            <b>引数　:</b>value : digree<br>
            <b>戻り値:</b>成功:1 失敗:0<br><br>
            <b>解説：</b>アームを指定した角度まで持ち上げる<br>
            関数の有効範囲は 0～120°　機体の設計上 90° 上限を推奨<br>
            打ち出し後処理等により持ち上げ不能な場合は失敗(0)が返される<br>
        </details></td>
    </tr>
    <tr>
        <td>int ArmShot(void)</td>
        <td><details><summary>打ち出しを行う</summary>
            <b>引数　:</b>無し<br>
            <b>戻り値:</b>成功:1 失敗:0<br><br>
            <b>解説：</b>打ち出しアクションを実行する<br>
            持ち上がったアームを解放しボールを打ち出す<br>
            アームが持ち上がり切っていない場合等に失敗(0)が返される<br>
        </details></td>
    </tr>
</table>

## ファイル詳細
    - main.cpp
        メイン処理
    - robot
        ロボット上でのメイン　ここに処理を書く
    - com (commonの意)
        便利そうな関数やdefineを記載
    - moter
        モーター制御をクラス化したもの
    - serial
        arduinoとのシリアル通信用


    - _*.out
        コンパイルされた実行ファイル
    - makefile
        コンパイル設定ファイル
    - test_*.cpp
        各クラスの動作確認用に制作したもの
        ご自由にお使いください