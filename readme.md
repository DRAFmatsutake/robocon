# ロボコン用プログラム
- [更新履歴](#更新履歴)
- [リファレンス](#リファレンス)
- [ファイル詳細](#ファイル詳細)

# 更新履歴
<details><summary>2021-11-09  ボール検知クラスの事前準備</summary><div>

- ballFinder
    - がわの実装
- リファレンス
    - 関数の参照の追加

</div></details>
<details><summary>2021-11-08  プログラムのアップデート</summary><div>

- camera
    - カメラの追加
- com
    - define の消去

</div></details>
<details><summary>2021-11-08  プログラムのアップデート</summary><div>

- robot
    - 処理実装部を別ファイルに切り分け
- serial
    - シリアルUSBを抜いた状態でフリーズするバグ回避

</div></details>
<details><summary>2021-11-05  プログラムのアップデート</summary><div>

- 全体
    - log吐き出しを#ifdefに変更
- moter
    - 時限関数の実装
    - 打ち出しの戻り値の変更
    - 停止の追加
- com
    - タイマーの実装

</div></details>
<details><summary>2021-10-27  readme作成</summary><div>

- moter
    - モーターの打ち出し部関数作成
- com
    - マニュアル用の定数の定義
- robot
    - マニュアル時の打ち出し実装
</div></details>

# 動作処理用リファレンス
- 基本的には main_proc.cpp 内の MainProc関数(動作処理) , State関数(状態処理) 内で処理の記述を行う

<details><summary>main_proc.cpp</summary><div>

```c++
int Robot::MainProc(void){
    
        switch (state=State())
        {
            case EXIT:    
                return 1;
                break;
            case BALL_SEARCH:   
                break;
            case BALL_FOCUS:    
                break;
            case BALL_MOVE:     
                break;
            case POLE_SEARCH:   
                break;
            case SHOT_PREPARE:  
                break;
            case SHOT_AFTER:    
                break;      
            default:
                return -1;
                break;
        }
        return 0;
}


int Robot::State(void){
    return EXIT;
}
```

</div></details>

## MainProc関数
- main文から呼ばれる関数、ロボットの制御を行う
- State関数にて状態遷移を決定しそれをもとに switch にて処理を実行する
- 基本的にはswitch文内を書き換えればよい
- 戻り値として　ループ：0、終了：1、エラー：‐1
## State関数
- 現在の状態を確定する、確定した状態を元にRun関数にて処理を行う
- 確定した状態をenumで定義した定数としてreturnする
## 基本
<table border="1">
    <tr>
        <td>enum STATE</td>
        <td>状態のリスト<br>ここを書き換え状態を管理する</td>
    </tr>
    <tr>
        <td>int Setup(void)</td>
        <td><details><summary>状態遷移後の初回セットアップ処理</summary>
            <b>引数　:</b>無し<br>
            <b>戻り値:</b>初回:1 2回目以降:0<br><br>
            <b>解説：</b>状態が変わった時に1度目のループでのみ1が変える<br>
        </details></td>
    </tr>
    <tr>
        <td>void Manual(void)</td>
        <td><details><summary>マニュアル処理を行う</summary>
            <b>引数　:</b>無し<br>
            <b>戻り値:</b>無し<br><br>
            <b>解説：</b>マニュアル制御用の処理<br>
            robot.cpp内でMODE_MANUALをdefineすると実行される<br>
        </details></td>
    </tr>
</table>

## タイヤ
<table border="1">
    <tr>
        <td>void moter->Wheel(char left,char right)</td>
        <td><details><summary>両モーターの動作設定</summary>
            <b>引数　:</b>left,right : それぞれのモーターの回転量<br>
            <b>戻り値:</b>無し<br><br>
            <b>解説</b>：両方のモーターを同時に設定する<br>
                -127～127の範囲の値を設定することができ、-128は停止となる<br>
                停止はWheelStop()を用意してあるがこの場合両方停止となる<br>
        </details></td>
    </tr>
    <tr>
        <td>void moter->TimerWheel(char left,char right,int time)</td>
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
        <td>void moter->WheelStop(void)</td>
        <td><details><summary>モーターを停止させる</summary>
            <b>引数　:</b>無し<br>
            <b>戻り値:</b>無し<br><br>
            <b>解説：</b>Wheel(-128,-128) が実行される<br>
        </details></td>
    </tr>
    <tr>
        <td>void moter->TimerStop(void)</td>
        <td><details><summary>TimerWheel() を中断し上書きを許可する</summary>
            <b>引数　:</b>無し<br>
            <b>戻り値:</b>無し<br><br>
            <b>解説：</b>TimerWheelに割り込み中止させる<br>
            TimeWheel() 中はWheel()　等の処理が実行されないため一度中断させる<br>
        </details></td>
    </tr>
    <tr>
        <td>void moter->WheelRight(char value)</td>
        <td><details><summary>右モーターにのみ指令を送る</summary>
            <b>引数　:</b>value :　回転量<br>
            <b>戻り値:</b>無し<br><br>
            <b>解説：</b>右モーターのみに指令を送る<br>
            -127～127の範囲の値を設定することができ、-128は停止となる<br>
            右のみのため左モーターは以前の指令値のまま動作する。<br>
        </details></td>
    </tr>
    <tr>
        <td>void moter->WheelLeft (char value)</td>
        <td><details><summary>左モーターにのみ指令を送る</summary>
            <b>引数　:</b>value :　回転量<br>
            <b>戻り値:</b>無し<br><br>
            <b>解説：</b>WHeelRight() の左バージョン<br>
        </details></td>
    </tr>
</table>

## アーム
<table border="1">
    <tr>
        <td>int moter->SetArm(char value)</td>
        <td><details><summary>アームを持ち上げる</summary>
            <b>引数　:</b>value : digree<br>
            <b>戻り値:</b>成功:1 失敗:0<br><br>
            <b>解説：</b>アームを指定した角度まで持ち上げる<br>
            関数の有効範囲は 0～120°　機体の設計上 90° 上限を推奨<br>
            打ち出し後処理等により持ち上げ不能な場合は失敗(0)が返される<br>
        </details></td>
    </tr>
    <tr>
        <td>int moter->ArmShot(void)</td>
        <td><details><summary>打ち出しを行う</summary>
            <b>引数　:</b>無し<br>
            <b>戻り値:</b>成功:1 失敗:0<br><br>
            <b>解説：</b>打ち出しアクションを実行する<br>
            持ち上がったアームを解放しボールを打ち出す<br>
            アームが持ち上がり切っていない場合等に失敗(0)が返される<br>
        </details></td>
    </tr>
</table>
## カメラ
<table border="1">
    <tr>
        <td>enum CAMERA_ID</td>
        <td><details><summary>カメラの番号</summary>
            <b>解説：</b>プログラムで割り振られたカメラ番号<br>
            2台使用するためCAM_1かCAM_2となる<br>
        </details></td>
    </tr>
    <tr>
        <td>int NowCam(void)</td>
        <td><details><summary>現在使用しているカメラの取得</summary>
            <b>引数　:</b>無し<br>
            <b>戻り値:</b>enum CAMERA_ID : カメラ番号<br><br>
            <b>解説：</b>現在使用しているカメラの取得<br>
            現在使用してるカメラを取得できる<br>
            戻り値はrobotクラス内で宣言されたenum CAMERA_IDとなる<br>
        </details></td>
    </tr>
    <tr>
        <td>void ChangeCam(int cam_id)</td>
        <td><details><summary>使用するカメラの切り替え</summary>
            <b>引数　:</b>enum CAMERA_ID : カメラ番号<br>
            <b>戻り値:</b>なし<br><br>
            <b>解説：</b>使用するカメラを切り替える<br>
            指定されたカメラに切り替える、同じ場合は切り替わらない<br>
            例外的な数値だった場合は無視される<br>
        </details></td>
    </tr>
</table>

## 検知
<table border="1">
    <tr>
        <td><s>int bf->GetDistance(int* value)<s></td>
        <td><details><summary>ボール距離の取得</summary>
            <b>引数　:</b>value : 変数のポインタ[mm]<br>
            <b>戻り値:</b>成功:1 失敗:0<br><br>
            <b>解説：</b>ボールの直線距離の取得<br>
            カメラに映る範囲で得られた値[mm]の単位で引数ポインタに代入する<br>
            ロボットのローカルでの正面を正とする<br>
            ボールが見つからないあった場合等は0を返す<br>
        </details></td>
    </tr>
    <tr>
        <td><s>int bf->GetDegree(int* value)<s></td>
        <td><details><summary>ボール方向の取得</summary>
            <b>引数　:</b>value : 変数のポインタ[degree]<br>
            <b>戻り値:</b>成功:1 失敗:0<br><br>
            <b>解説：</b>ボールの方向の取得<br>
            カメラに映る範囲で得られた値を -180° ~ 180° で引数ポインタに代入する<br>
            ロボットのローカルでの正面を0°とし時計回りを正とする<br>
            ボールが見つからないあった場合等は0を返す<br>
        </details></td>
    </tr>
    <tr>
        <td><s>int pf->GetDistance(int* value)<s></td>
        <td><details><summary>ポール距離の取得</summary>
            <b>引数　:</b>value : 変数のポインタ[mm]<br>
            <b>戻り値:</b>成功:1 失敗:0<br><br>
            <b>解説：</b>ポールの直線距離の取得<br>
            カメラに映る範囲で得られた値[mm]の単位で引数ポインタに代入する<br>
            ロボットのローカルでの正面を正とする<br>
            ボールが見つからないあった場合等は0を返す<br>
        </details></td>
    </tr>
    <tr>
        <td><s>int pf->GetDegree(int* value)<s></td>
        <td><details><summary>ポール方向の取得</summary>
            <b>引数　:</b>value : 変数のポインタ[degree]<br>
            <b>戻り値:</b>成功:1 失敗:0<br><br>
            <b>解説：</b>ボールの方向の取得<br>
            カメラに映る範囲で得られた値を -180° ~ 180° で引数ポインタに代入する<br>
            ロボットのローカルでの正面を0°とし時計回りを正とする<br>
            ボールが見つからないあった場合等は0を返す<br>
        </details></td>
    </tr>
</table>

# ファイル詳細
- main.cpp
    - メイン処理
- main_proc.cpp
    - メイン処理の実装
- robot
    - ロボット上でのメイン　ここに処理を書く
- com (commonの意)
    - 便利そうな関数やdefineを記載
- moter
    - モーター制御をクラス化したもの
- serial
    - arduinoとのシリアル通信用
- serial
    - カメラの制御をクラス化したもの


- _*.out
    - コンパイルされた実行ファイル
- makefile
    - コンパイル設定ファイル
- test_*.cpp
    - 各クラスの動作確認用に制作したもの
    - ご自由にお使いください