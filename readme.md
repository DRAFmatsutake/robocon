# ���{�R���p�v���O����
- [�X�V����](#�X�V����)
- [���t�@�����X](#���t�@�����X)
- [�t�@�C���ڍ�](#�t�@�C���ڍ�)

# �X�V����
<details><summary>2021-11-05  �v���O�����̃A�b�v�f�[�g</summary><div>

- �S��
    - log�f���o����#ifdef�ɕύX
- moter
    - �����֐��̎���
    - �ł��o���̖߂�l�̕ύX
    - ��~�̒ǉ�
- com
    - �^�C�}�[�̎���

</div></details>
<details><summary>2021-10-27  readme�쐬</summary><div>

- moter
    - ���[�^�[�̑ł��o�����֐��쐬
- com
    - �}�j���A���p�̒萔�̒�`
- robot
    - �}�j���A�����̑ł��o������
</div></details>

# ���쏈���p���t�@�����X
- ��{�I�ɂ� robot.cpp ���� Run�֐�(���쏈��) , State�֐�(��ԏ���) ���ŏ����̋L�q���s��

<details><summary>main�֐�</summary><div>

```c++
int main(void){
    Robot robot;
    robot.Init();
    while(robot.Run()==0);
    robot.Deinit();
    sleep(3);
    return 0;
}
```

</div></details>

<details><summary>Run�֐�</summary><div>

```c++
int Robot::Run(void){

   moter.Update();

    if(R_MANUAL){
        return Manual();
    }
    else{
        switch (state=State())
        {
            case EXIT:    //�I��
                return 1;
                break;
            case BALL_SEARCH:   //�{�[����T��
                break;
            case BALL_FOCUS:    //�{�[���̕����֌���
                break;
            case BALL_MOVE:     //�{�[���֌�����
                break;
            case POLE_SEARCH:   //�|�[����T��
                break;
            case SHOT_PREPARE:  //�|�[���̕���������
                break;
            case SHOT_AFTER:    //�ł��o�����㏈��
                break;      
            default:
                return -1;
                break;
        }
        state_pre=state;
    }
   return 0;
}
```

</div></details>

## Run�֐�
    - main������Ă΂��֐��A���{�b�g�̐�����s��
    - State�֐��ɂď�ԑJ�ڂ����肵��������Ƃ� switch �ɂď��������s����
    - ��{�I�ɂ�switch����������������΂悢
## State�֐�
    - ���݂̏�Ԃ��m�肷��A�m�肵����Ԃ�����Run�֐��ɂď������s��
    - �m�肵����Ԃ�enum�Œ�`�����萔�Ƃ���return����
## ��{
<table border="1">
    <tr>
        <td>enum STATE</td>
        <td>��Ԃ̃��X�g<br>����������������Ԃ��Ǘ�����</td>
    </tr>
    <tr>
        <td>void Manual(void)</td>
        <td><details><summary>�}�j���A���������s��</summary>
            <b>�����@:</b>����<br>
            <b>�߂�l:</b>����<br><br>
            <b>����F</b>�}�j���A������p�̏���<br>
            com.h����define���ꂽmanual�ݒ��true�ɂ���Ǝ��s�����<br>
        </details></td>
    </tr>
    <tr>
        <td>int Setup(void)</td>
        <td><details><summary>��ԑJ�ڌ�̏���Z�b�g�A�b�v����</summary>
            <b>�����@:</b>����<br>
            <b>�߂�l:</b>����:1 2��ڈȍ~:0<br><br>
            <b>����F</b>��Ԃ��ς��������1�x�ڂ̃��[�v�ł̂�1���ς���<br>
        </details></td>
    </tr>
</table>

## �^�C��
<table border="1">
    <tr>
        <td>void Wheel(char left,char right)</td>
        <td><details><summary>�����[�^�[�̓���ݒ�</summary>
            <b>�����@:</b>left,right : ���ꂼ��̃��[�^�[�̉�]��<br>
            <b>�߂�l:</b>����<br><br>
            <b>���</b>�F�����̃��[�^�[�𓯎��ɐݒ肷��<br>
                -127�`127�͈̔͂̒l��ݒ肷�邱�Ƃ��ł��A-128�͒�~�ƂȂ�<br>
                ��~��WheelStop()��p�ӂ��Ă��邪���̏ꍇ������~�ƂȂ�<br>
        </details></td>
    </tr>
    <tr>
        <td>void TimerWheel(char left,char right,int time)</td>
        <td><details><summary>���[�^�[����莞�ԓ��삳����</summary>
            <b>�����@:</b>left,right : ���ꂼ��̃��[�^�[�̉�]��<br>
                        time : ���쎞��[ms]<br>
            <b>�߂�l:</b>����<br><br>
            <b>����F</b>���Ԑ����t���Ń��[�^�[�𓮍삳����<br>
            ��{����� Wheel() �Ɠ����Ŏ��Ԍo�ߌ�� WheelStop() �����s�����<br>
            ���̏��������쒆��Wheel[Right][Left]�A�͖��������<br>
        </details></td>
    </tr>
    <tr>
        <td>void WheelStop(void)</td>
        <td><details><summary>���[�^�[���~������</summary>
            <b>�����@:</b>����<br>
            <b>�߂�l:</b>����<br><br>
            <b>����F</b>Wheel(-128,-128) �����s�����<br>
        </details></td>
    </tr>
    <tr>
        <td>void TimerStop(void)</td>
        <td><details><summary>TimerWheel() �𒆒f���㏑����������</summary>
            <b>�����@:</b>����<br>
            <b>�߂�l:</b>����<br><br>
            <b>����F</b>TimerWheel�Ɋ��荞�ݒ��~������<br>
            TimeWheel() ����Wheel()�@���̏��������s����Ȃ����߈�x���f������<br>
        </details></td>
    </tr>
    <tr>
        <td>void WheelRight(char value)</td>
        <td><details><summary>�E���[�^�[�ɂ̂ݎw�߂𑗂�</summary>
            <b>�����@:</b>value :�@��]��<br>
            <b>�߂�l:</b>����<br><br>
            <b>����F</b>�E���[�^�[�݂̂Ɏw�߂𑗂�<br>
            -127�`127�͈̔͂̒l��ݒ肷�邱�Ƃ��ł��A-128�͒�~�ƂȂ�<br>
            �E�݂̂̂��ߍ����[�^�[�͈ȑO�̎w�ߒl�̂܂ܓ��삷��B<br>
        </details></td>
    </tr>
    <tr>
        <td>void WheelLeft (char value)</td>
        <td><details><summary>�����[�^�[�ɂ̂ݎw�߂𑗂�</summary>
            <b>�����@:</b>value :�@��]��<br>
            <b>�߂�l:</b>����<br><br>
            <b>����F</b>WHeelRight() �̍��o�[�W����<br>
        </details></td>
    </tr>
</table>

## �A�[��
<table border="1">
    <tr>
        <td>int SetArm(char value)</td>
        <td><details><summary>�A�[���������グ��</summary>
            <b>�����@:</b>value : digree<br>
            <b>�߂�l:</b>����:1 ���s:0<br><br>
            <b>����F</b>�A�[�����w�肵���p�x�܂Ŏ����グ��<br>
            �֐��̗L���͈͂� 0�`120���@�@�̂̐݌v�� 90�� ����𐄏�<br>
            �ł��o���㏈�����ɂ�莝���グ�s�\�ȏꍇ�͎��s(0)���Ԃ����<br>
        </details></td>
    </tr>
    <tr>
        <td>int ArmShot(void)</td>
        <td><details><summary>�ł��o�����s��</summary>
            <b>�����@:</b>����<br>
            <b>�߂�l:</b>����:1 ���s:0<br><br>
            <b>����F</b>�ł��o���A�N�V���������s����<br>
            �����オ�����A�[����������{�[����ł��o��<br>
            �A�[���������オ��؂��Ă��Ȃ��ꍇ���Ɏ��s(0)���Ԃ����<br>
        </details></td>
    </tr>
</table>

# �t�@�C���ڍ�
- main.cpp
    - ���C������
- robot
    - ���{�b�g��ł̃��C���@�����ɏ���������
- com (common�̈�)
    - �֗������Ȋ֐���define���L��
- moter
    - ���[�^�[������N���X����������
- serial
    - arduino�Ƃ̃V���A���ʐM�p


- _*.out
    - �R���p�C�����ꂽ���s�t�@�C��
- makefile
    - �R���p�C���ݒ�t�@�C��
- test_*.cpp
    - �e�N���X�̓���m�F�p�ɐ��삵������
    - �����R�ɂ��g����������