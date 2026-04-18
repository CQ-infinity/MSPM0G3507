#include "key.h"

extern int g_LinePortal_flag;

// ���尴����� - Define key handle
Key_t key1 = {
    .GPIOx = KEY_PORT,
    .GPIO_Pin = KEY_K1_PIN,
    .state = KEY_STATE_RELEASED,
    .pressTime = 0,
    .debounceTime = 0
};

Key_t key4 = {
    .GPIOx = KEY_PORT,
    .GPIO_Pin = KEY_K4_PIN,
    .state = KEY_STATE_RELEASED,
    .pressTime = 0,
    .debounceTime = 0
};


/* ����ʱ�䣨��λ��ms�� - Debounce time (ms) */
#define DEBOUNCE_DELAY 20  // ����ֵ��10-50ms / Typical value: 10-50ms

/**
 * @brief ����ɨ�躯����������ʽ�� - Key scan function (non-blocking)
 * @param key         �������ָ�� / Pointer to KeyHandle
 * @param currentTime ��ǰϵͳʱ�䣨��λ��ms�� / Current system time (ms)
 * @param longPressThreshold ����ʱ����ֵ����λ��ms�� / Long press threshold (ms)
 * @return KeyEvent   ���ذ����¼� / Returns key event
 */
KeyEvent Key_Scan(Key_t* key, uint32_t currentTime, uint32_t longPressThreshold) {
    // ��ȡ������ƽ��0��ʾ���£�1��ʾ�ͷ� / Read pin level: 0=pressed, 1=released
    uint8_t isPressed = 0;
    
    if(DL_GPIO_readPins(key->GPIOx, key->GPIO_Pin) == 0)
    {
        isPressed = 1;
    }
    
    switch (key->state) {
        /* ״̬1�������ͷ� - State 1: Key released */
        case KEY_STATE_RELEASED:
            if (isPressed) {
                key->state = KEY_STATE_DEBOUNCE;       // ��������״̬ / Enter debounce state
                key->debounceTime = currentTime;      // ��¼������ʼʱ�� / Record debounce start time
            }
            break;

        /* ״̬2��������� - State 2: Debounce checking */
        case KEY_STATE_DEBOUNCE:
            // ����ʱ�䵽������ȶ�״̬ / Check stable state after debounce delay
            if (currentTime - key->debounceTime >= DEBOUNCE_DELAY) {
                if (isPressed) {
                    key->state = KEY_STATE_PRESSED;    // ȷ�ϰ��� / Confirm press
                    key->pressTime = currentTime;      // ��¼����ʱ�� / Record press time
                } else {
                    key->state = KEY_STATE_RELEASED;   // �����󴥷� / False trigger due to bounce
                }
            }
            break;

        /* ״̬3���Ѱ��� - State 3: Pressed */
        case KEY_STATE_PRESSED:
            if (!isPressed) {
                key->state = KEY_STATE_RELEASED;       // �ͷŴ����̰� / Release triggers short press
                return KEY_EVENT_SHORT;                // ���ض̰��¼� / Return short press event
            } else if (currentTime - key->pressTime >= longPressThreshold) {
                key->state = KEY_STATE_LONG;           // �������� / Trigger long press
                return KEY_EVENT_LONG;                 // ���س����¼� / Return long press event
            }
            break;

        /* ״̬4�������Ѵ��� - State 4: Long press triggered */
        case KEY_STATE_LONG:
            if (!isPressed) {
                key->state = KEY_STATE_RELEASED;       // �ͷź�����״̬ / Reset state after release
            }
            break;
    }

    return KEY_EVENT_NONE;  // Ĭ�����¼� / Default: no event
}

void Key_Handle(void)
{
    int16_t LongPressThreshold = 700;// ����ɨ��ʱ��������ֵ��500ms    Threshold for long press during key scanning: 500ms
    static uint32_t lastTick = 0;  // ��ʼʱ��  initial time
    uint32_t currentTick = Get_Time();
    static int task_flag = 1;

    // ÿ10ms���һ�ΰ��� - Check key every 10ms
    if (currentTick - lastTick >= 10) {
        lastTick = currentTick;

        KeyEvent event = Key_Scan(&key1, currentTick, LongPressThreshold);
        KeyEvent N_event = Key_Scan(&key4, currentTick, LongPressThreshold);

        switch (event) {
            case KEY_EVENT_SHORT:
                // �����̰� Handle short press
//              printf("short press\r\n");
                g_LinePortal_flag = 1;
                bee_time = 500;
                break;
            case KEY_EVENT_LONG:
                Contrl_Pwm(0,0,0,0);
                g_LinePortal_flag = 0;
                // �������� Handle long press
                switch(task_flag)
                {
                    case 1:
                    {   
//                      printf("Task1");
                        State_Machine.Main_State = QUESTION_1;
                        task_flag +=3;
                        Beep_Times(1);
//                      printf("Hanshu:State_Machine.Main_State = %d\r\n",State_Machine.Main_State);
                        break;
                    }
                    case 2:
                    {   
//                      printf("Task2");
                        State_Machine.Main_State = QUESTION_2;
                        task_flag +=1;
                        Beep_Times(2);
                        break;
                    }
                    case 3:
                    {   
//                      printf("Task3");
                        State_Machine.Main_State = QUESTION_3;
                        task_flag +=1;
                        Beep_Times(3);
                        break;
                    }
                    case 4:
                    {   
//                      printf("Task4");
                        State_Machine.Main_State = QUESTION_4;
                        task_flag +=1;
                        Beep_Times(4);
                        break;
                    }
                    case 5:
                    {   
//                      printf("Task4");
                        State_Machine.Main_State = QUESTION_5;
                        task_flag +=1;
                        Beep_Times(5);
                        break;
                    }
                    case 6:
                    {   
//                      printf("Task4");
                        State_Machine.Main_State = QUESTION_6;
                        task_flag =1;
                        Beep_Times(6);
                        break;
                    }
                }
                break;

            default:
                break;
        }

        switch (N_event) 
        {
            case KEY_EVENT_SHORT:
            // �����̰� Handle short press
                State_Machine.Q1_State = RUN_STATE;
                bee_time = 500;
                break;

            case KEY_EVENT_LONG:
            // �������� Handle long press
                q1_circle_N++;
                if(q1_circle_N > 5) q1_circle_N = 1;
                Beep_Times(q1_circle_N);
                break;

            default:
                break;
        }
    }
}

