# ���{�R���p�v���O����
- [�X�V����](#�X�V����)
- [���t�@�����X](#���t�@�����X)
- [�t�@�C���ڍ�](#�t�@�C���ڍ�)

# �X�V����
<details><summary>2021-11-09  �{�[�����m�N���X�̎��O����</summary><div>

- ballFinder
    - ����̎���
- ���t�@�����X
    - �֐��̎Q�Ƃ̒ǉ�

</div></details>
<details><summary>2021-11-08  �v���O�����̃A�b�v�f�[�g</summary><div>

- camera
    - �J�����̒ǉ�
- com
    - define �̏���

</div></details>
<details><summary>2021-11-08  �v���O�����̃A�b�v�f�[�g</summary><div>

- robot
    - ������������ʃt�@�C���ɐ؂蕪��
- serial
    - �V���A��USB�𔲂�����ԂŃt���[�Y����o�O���

</div></details>
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
- ��{�I�ɂ� main_proc.cpp ���� MainProc�֐�(���쏈��) , State�֐�(��ԏ���) ���ŏ����̋L�q���s��

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

## MainProc�֐�
- main������Ă΂��֐��A���{�b�g�̐�����s��
- State�֐��ɂď�ԑJ�ڂ����肵��������Ƃ� switch �ɂď��������s����
- ��{�I�ɂ�switch����������������΂悢
- �߂�l�Ƃ��ā@���[�v�F0�A�I���F1�A�G���[�F�]1
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
        <td>int Setup(void)</td>
        <td><details><summary>��ԑJ�ڌ�̏���Z�b�g�A�b�v����</summary>
            <b>�����@:</b>����<br>
            <b>�߂�l:</b>����:1 2��ڈȍ~:0<br><br>
            <b>����F</b>��Ԃ��ς��������1�x�ڂ̃��[�v�ł̂�1���ς���<br>
        </details></td>
    </tr>
    <tr>
        <td>void Manual(void)</td>
        <td><details><summary>�}�j���A���������s��</summary>
            <b>�����@:</b>����<br>
            <b>�߂�l:</b>����<br><br>
            <b>����F</b>�}�j���A������p�̏���<br>
            robot.cpp����MODE_MANUAL��define����Ǝ��s�����<br>
        </details></td>
    </tr>
</table>

## �^�C��
<table border="1">
    <tr>
        <td>void moter->Wheel(char left,char right)</td>
        <td><details><summary>�����[�^�[�̓���ݒ�</summary>
            <b>�����@:</b>left,right : ���ꂼ��̃��[�^�[�̉�]��<br>
            <b>�߂�l:</b>����<br><br>
            <b>���</b>�F�����̃��[�^�[�𓯎��ɐݒ肷��<br>
                -127�`127�͈̔͂̒l��ݒ肷�邱�Ƃ��ł��A-128�͒�~�ƂȂ�<br>
                ��~��WheelStop()��p�ӂ��Ă��邪���̏ꍇ������~�ƂȂ�<br>
        </details></td>
    </tr>
    <tr>
        <td>void moter->TimerWheel(char left,char right,int time)</td>
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
        <td>void moter->WheelStop(void)</td>
        <td><details><summary>���[�^�[���~������</summary>
            <b>�����@:</b>����<br>
            <b>�߂�l:</b>����<br><br>
            <b>����F</b>Wheel(-128,-128) �����s�����<br>
        </details></td>
    </tr>
    <tr>
        <td>void moter->TimerStop(void)</td>
        <td><details><summary>TimerWheel() �𒆒f���㏑����������</summary>
            <b>�����@:</b>����<br>
            <b>�߂�l:</b>����<br><br>
            <b>����F</b>TimerWheel�Ɋ��荞�ݒ��~������<br>
            TimeWheel() ����Wheel()�@���̏��������s����Ȃ����߈�x���f������<br>
        </details></td>
    </tr>
    <tr>
        <td>void moter->WheelRight(char value)</td>
        <td><details><summary>�E���[�^�[�ɂ̂ݎw�߂𑗂�</summary>
            <b>�����@:</b>value :�@��]��<br>
            <b>�߂�l:</b>����<br><br>
            <b>����F</b>�E���[�^�[�݂̂Ɏw�߂𑗂�<br>
            -127�`127�͈̔͂̒l��ݒ肷�邱�Ƃ��ł��A-128�͒�~�ƂȂ�<br>
            �E�݂̂̂��ߍ����[�^�[�͈ȑO�̎w�ߒl�̂܂ܓ��삷��B<br>
        </details></td>
    </tr>
    <tr>
        <td>void moter->WheelLeft (char value)</td>
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
        <td>int moter->SetArm(char value)</td>
        <td><details><summary>�A�[���������グ��</summary>
            <b>�����@:</b>value : digree<br>
            <b>�߂�l:</b>����:1 ���s:0<br><br>
            <b>����F</b>�A�[�����w�肵���p�x�܂Ŏ����グ��<br>
            �֐��̗L���͈͂� 0�`120���@�@�̂̐݌v�� 90�� ����𐄏�<br>
            �ł��o���㏈�����ɂ�莝���グ�s�\�ȏꍇ�͎��s(0)���Ԃ����<br>
        </details></td>
    </tr>
    <tr>
        <td>int moter->ArmShot(void)</td>
        <td><details><summary>�ł��o�����s��</summary>
            <b>�����@:</b>����<br>
            <b>�߂�l:</b>����:1 ���s:0<br><br>
            <b>����F</b>�ł��o���A�N�V���������s����<br>
            �����オ�����A�[����������{�[����ł��o��<br>
            �A�[���������オ��؂��Ă��Ȃ��ꍇ���Ɏ��s(0)���Ԃ����<br>
        </details></td>
    </tr>
</table>
## �J����
<table border="1">
    <tr>
        <td>enum CAMERA_ID</td>
        <td><details><summary>�J�����̔ԍ�</summary>
            <b>����F</b>�v���O�����Ŋ���U��ꂽ�J�����ԍ�<br>
            2��g�p���邽��CAM_1��CAM_2�ƂȂ�<br>
        </details></td>
    </tr>
    <tr>
        <td>int NowCam(void)</td>
        <td><details><summary>���ݎg�p���Ă���J�����̎擾</summary>
            <b>�����@:</b>����<br>
            <b>�߂�l:</b>enum CAMERA_ID : �J�����ԍ�<br><br>
            <b>����F</b>���ݎg�p���Ă���J�����̎擾<br>
            ���ݎg�p���Ă�J�������擾�ł���<br>
            �߂�l��robot�N���X���Ő錾���ꂽenum CAMERA_ID�ƂȂ�<br>
        </details></td>
    </tr>
    <tr>
        <td>void ChangeCam(int cam_id)</td>
        <td><details><summary>�g�p����J�����̐؂�ւ�</summary>
            <b>�����@:</b>enum CAMERA_ID : �J�����ԍ�<br>
            <b>�߂�l:</b>�Ȃ�<br><br>
            <b>����F</b>�g�p����J������؂�ւ���<br>
            �w�肳�ꂽ�J�����ɐ؂�ւ���A�����ꍇ�͐؂�ւ��Ȃ�<br>
            ��O�I�Ȑ��l�������ꍇ�͖��������<br>
        </details></td>
    </tr>
</table>

## ���m
<table border="1">
    <tr>
        <td><s>int bf->GetDistance(int* value)<s></td>
        <td><details><summary>�{�[�������̎擾</summary>
            <b>�����@:</b>value : �ϐ��̃|�C���^[mm]<br>
            <b>�߂�l:</b>����:1 ���s:0<br><br>
            <b>����F</b>�{�[���̒��������̎擾<br>
            �J�����ɉf��͈͂œ���ꂽ�l[mm]�̒P�ʂň����|�C���^�ɑ������<br>
            ���{�b�g�̃��[�J���ł̐��ʂ𐳂Ƃ���<br>
            �{�[����������Ȃ��������ꍇ����0��Ԃ�<br>
        </details></td>
    </tr>
    <tr>
        <td><s>int bf->GetDegree(int* value)<s></td>
        <td><details><summary>�{�[�������̎擾</summary>
            <b>�����@:</b>value : �ϐ��̃|�C���^[degree]<br>
            <b>�߂�l:</b>����:1 ���s:0<br><br>
            <b>����F</b>�{�[���̕����̎擾<br>
            �J�����ɉf��͈͂œ���ꂽ�l�� -180�� ~ 180�� �ň����|�C���^�ɑ������<br>
            ���{�b�g�̃��[�J���ł̐��ʂ�0���Ƃ����v���𐳂Ƃ���<br>
            �{�[����������Ȃ��������ꍇ����0��Ԃ�<br>
        </details></td>
    </tr>
    <tr>
        <td><s>int pf->GetDistance(int* value)<s></td>
        <td><details><summary>�|�[�������̎擾</summary>
            <b>�����@:</b>value : �ϐ��̃|�C���^[mm]<br>
            <b>�߂�l:</b>����:1 ���s:0<br><br>
            <b>����F</b>�|�[���̒��������̎擾<br>
            �J�����ɉf��͈͂œ���ꂽ�l[mm]�̒P�ʂň����|�C���^�ɑ������<br>
            ���{�b�g�̃��[�J���ł̐��ʂ𐳂Ƃ���<br>
            �{�[����������Ȃ��������ꍇ����0��Ԃ�<br>
        </details></td>
    </tr>
    <tr>
        <td><s>int pf->GetDegree(int* value)<s></td>
        <td><details><summary>�|�[�������̎擾</summary>
            <b>�����@:</b>value : �ϐ��̃|�C���^[degree]<br>
            <b>�߂�l:</b>����:1 ���s:0<br><br>
            <b>����F</b>�{�[���̕����̎擾<br>
            �J�����ɉf��͈͂œ���ꂽ�l�� -180�� ~ 180�� �ň����|�C���^�ɑ������<br>
            ���{�b�g�̃��[�J���ł̐��ʂ�0���Ƃ����v���𐳂Ƃ���<br>
            �{�[����������Ȃ��������ꍇ����0��Ԃ�<br>
        </details></td>
    </tr>
</table>

# �t�@�C���ڍ�
- main.cpp
    - ���C������
- main_proc.cpp
    - ���C�������̎���
- robot
    - ���{�b�g��ł̃��C���@�����ɏ���������
- com (common�̈�)
    - �֗������Ȋ֐���define���L��
- moter
    - ���[�^�[������N���X����������
- serial
    - arduino�Ƃ̃V���A���ʐM�p
- serial
    - �J�����̐�����N���X����������


- _*.out
    - �R���p�C�����ꂽ���s�t�@�C��
- makefile
    - �R���p�C���ݒ�t�@�C��
- test_*.cpp
    - �e�N���X�̓���m�F�p�ɐ��삵������
    - �����R�ɂ��g����������