#include "fosc.h"
#include "macro.h"
#include "port.h"
#include "config.h"

// ����������IO 
#define     BEEP_DIR    TRISC3
#define     BEEP_IO     LATC3

// ����Ƶ�ʣ���������������������������������
unsigned int const note_ll[7] = {65, 73, 82, 87, 98, 110, 123};
unsigned int const note_l[7] = {131, 147, 165, 174, 196, 220, 247};
unsigned int const note_m[7] = {262, 294, 330, 349, 392, 440, 494};
unsigned int const note_h[7] = {523, 587, 659, 698, 784, 880, 988};
unsigned int const note_hh[7] = {1046, 1175, 1318, 1397, 1568, 1760, 1967};

//����
unsigned int const speed = 120;        // �˹��ܻ�δʵ�֣���Ϊ�޷���̬��ֵ��ʱ����

// ���ָ�ʽ��3���ֽ�һ�飬�ֽ�1��ʾ���ף��ֽ�2��ʾ���ߣ��ֽ�3��ʾ��������
/*
 * �ֽ�1:1 - 7 ���� Do��Re��Mi��Fa��So��La��Si��0 ��������ͣ�� 
 * �ֽ�2�� 1��������
 *         2������
 *         3������
 *         4������
 *         5��������
 * �ֽ�3����������������1/4 ��������д4��1/8 ��������д8,1/16 ��������д 16
 */

// �ѹ¶���������
//unsigned char const bgddzwc[]={\
0,3,4, 0,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 2,3,8, 3,3,8, 5,3,8, 3,3,8, \
3,3,4, 3,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 1,3,8, 2,3,8, 7,3,8, 5,3,8, \
5,3,4, 5,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
\
6,3,4, 6,3,4, 2,3,4, 2,3,8, 3,3,8, \
5,3,4, 7,3,8, 1,4,8, 1,4,4, 3,3,8, 2,3,8, \
6,2,4, 3,3,4, 2,3,8, 3,3,8, 5,2,8, 6,2,8, \
6,2,4, 6,2,4, 6,2,4, 6,2,4, \
0,3,4, 0,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
\
1,3,4, 6,2,8, 6,2,8, 2,3,8, 3,3,8, 5,3,8, 3,3,8, \
3,3,4, 3,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 1,3,8, 2,3,8, 7,3,8, 5,3,8, \
5,3,4, 5,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
6,3,4, 6,3,4, 2,3,4, 2,3,8, 2,3,8, \
\
5,3,4, 7,3,8, 1,4,8, 1,4,4, 3,3,8, 2,3,8, \
6,2,4, 3,3,4, 2,3,8, 3,3,8, 5,2,8, 6,2,8, \
6,2,4, 6,2,4, 6,2,4, 6,2,4, \
0,3,4, 0,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 2,3,8, 3,3,8, 5,3,8, 3,3,8, \
\
3,3,4, 3,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 1,3,8, 2,3,8, 7,3,8, 5,3,8, \
5,3,4, 5,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,4, 2,3,4, 1,3,8, 6,2,8, \
6,2,4, 3,3,4, 3,3,4, 2,3,8, 1,3,8, \
\
2,3,4, 3,3,4, 2,3,8, 3,3,8, 5,2,8, 6,2,8, \
6,2,4, 6,2,4, 6,2,4, 6,2,4, \
0,3,4, 0,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 2,3,8, 3,3,8, 5,3,8, 3,3,8, \
3,3,4, 3,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
\
1,3,4, 6,2,8, 6,2,8, 1,3,8, 2,3,8, 7,3,8, 5,3,8, \
5,3,4, 5,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
6,3,4, 6,3,4, 2,3,4, 2,3,8, 2,3,8, \
5,3,4, 7,3,8, 1,4,8, 1,4,4, 3,3,8, 2,3,8, \
6,2,4, 3,3,4, 2,3,8, 3,3,8, 5,2,8, 6,2,8, \
\
6,2,4, 0,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 2,3,8, 3,3,8, 5,3,8, 3,3,8, \
3,3,4, 3,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 1,3,8, 2,3,8, 7,3,8, 5,3,8, \
\
5,3,4, 5,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,4, 2,3,4, 1,3,8, 6,2,8, \
6,2,4, 3,3,4, 3,3,4, 2,3,8, 1,3,8, \
2,3,4, 3,3,4, 2,3,8, 3,3,8, 5,2,8, 6,2,8, \
6,2,4, 0,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
\
1,3,4, 6,2,8, 6,2,8, 2,3,8, 3,3,8, 5,3,8, 3,3,8, \
3,3,4, 3,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
1,3,4, 6,2,8, 6,2,8, 1,3,8, 2,3,8, 7,3,8, 5,3,8, \
5,3,4, 5,3,8, 3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,8, \
6,3,4, 6,3,4, 2,3,4, 2,3,8, 3,3,8, \
5,3,4, 7,3,8, 1,4,8, 1,4,4, 3,3,8, 2,3,8, \
\
0,0,0 };

// Ե��һ����
unsigned char const yfydq[]={\
1,4,4,  6,3,8, 5,3,8, 6,3,4, 6,3,4, \
5,3,4, 5,3,8, 3,3,8, 5,3,4, 5,3,4, \
3,3,4, 5,3,4, 6,3,4, 1,4,4, \
2,4,8, 1,4,8, 6,3,8, 5,3,8, 6,3,4, 6,3,4, \
6,3,4, 1,4,4, 2,4,4, 3,4,4, \
\
5,4,8, 3,4,8, 5,4,4, 6,4,4, 6,4,4, \
2,4,16, 2,4,8, 2,4,16, 2,4,4, 6,3,8, 1,4,8, 3,3,8, 2,3,8, \
3,3,4, 5,3,8, 6,3,8, 6,3,4, 6,3,4, \
3,3,8, 2,3,8, 3,3,4, 2,3,8, 1,3,4, 1,3,8, \
5,2,4, 5,2,8, 7,2,8, 6,2,4, 6,2,4, \
\
3,3,8, 2,3,8, 3,3,8, 3,3,4, 3,3,8, 3,3,4, \
6,3,4, 3,3,16, 2,3,16, 3,3,8, 3,3,4, 3,3,4, \
3,3,4, 5,3,8, 6,3,8, 6,3,4, 3,3,4, \
7,3,4, 7,3,8, 3,3,16, 2,3,16, 3,3,4, 3,3,4, \
1,4,16, 1,4,8, 1,4,16, 1,4,4, 3,3,8, 5,3,8, 2,3,8, 1,3,8, \
\
2,3,8, 3,3,8, 1,3,4, 6,2,4, 5,2,8, 6,2,8, \
1,3,4, 6,2,8, 1,3,8, 2,3,4, 1,3,4, \
3,3,8, 5,3,8, 6,3,8, 5,3,8, 3,3,4, 6,3,8, 5,3,8, \
3,3,4, 5,3,8, 3,3,8, 2,3,8, 1,3,8, 1,3,8, 3,3,8, \
2,3,4, 2,3,4, 2,3,4, 5,2,8, 6,2,8, \
\
1,3,4, 6,2,8, 1,3,8, 2,3,4, 1,3,4, \
3,3,8, 5,3,8, 6,3,8, 7,3,8, 7,3,4, 6,3,8, 5,3,8, \
3,3,4, 5,3,8, 3,3,8, 2,3,8, 1,3,8, 1,3,8, 2,3,8, \
3,3,8, 3,3,8, 2,3,8, 2,3,8, 1,3,4, 5,2,8, 6,2,8, \
1,3,4, 6,2,8, 1,3,8, 2,3,4, 1,3,4, \
\
3,3,8, 5,3,8, 6,3,8, 5,3,8, 3,3,4, 6,3,8, 5,3,8, \
3,3,4, 5,3,8, 3,3,8, 2,3,8, 1,3,8, 1,3,8, 3,3,8, \
2,3,4, 2,3,4, 2,3,4, 5,2,8, 6,2,8, \
1,3,4, 6,2,8, 1,3,8, 2,3,4, 1,3,4, \
3,3,8, 5,3,8, 6,3,8, 7,3,8, 7,3,4, 6,3,8, 5,3,8, \
\
3,3,4, 5,3,8, 3,3,8, 2,3,8, 1,3,8, 1,3,8, 2,3,8, \
3,3,4, 3,3,4, 2,3,4, 2,3,4, \
6,2,4, 6,2,4, 6,2,4, 6,2,4, \
\
0,0,0 };

// �������õ�ħ��
//unsigned char const lzttdmg_speed = 72;
//unsigned char const lzttdmg[]={\
6,2,8, 6,2,8, 6,2,16, 1,3,16, 6,2,16, 1,3,16, 5,3,8, 1,3,8, 1,3,8, 1,3,16, 1,3,16, \
3,3,8, 3,3,8, 3,3,16, 4,3,16, 3,3,16, 4,3,16, 3,3,8, 2,3,8, 2,3,8, 1,3,16, 7,2,16, \
6,2,8, 6,2,8, 6,2,16, 1,3,16, 6,2,16, 1,3,16, 5,3,8, 1,3,8, 1,3,8, 1,3,16, 2,3,16, \
\
3,3,16, 3,3,16, 3,3,8, 3,3,8, 2,3,16, 1,3,16, 7,2,4, 1,3,8, 2,3,8, \
3,3,8, 3,3,8, 3,3,8, 1,4,8, 4,3,8, 4,3,8, 4,3,8, 1,4,8, \
5,3,8, 5,3,8, 5,3,16, 4,3,16, 3,3,16, 4,3,16, 3,3,8, 2,3,8, 2,3,8, 1,3,16, 7,2,16, \
6,2,8, 6,2,8, 6,2,16, 1,3,16, 6,2,16, 1,3,16, 5,3,8, 1,3,8, 1,3,8, 1,3,16, 1,3,16, \
\
3,3,16, 3,3,16, 3,3,8, 3,3,8, 2,3,16, 1,3,16, 7,2,4, 0,3,4,\
0,3,4, 0,3,4, 0,3,8, 0,3,16, 5,3,16, 1,4,16, 7,3,16, 6,3,16, 7,3,16, \
1,4,4, 1,4,8, 6,3,16, 7,3,16, 7,3,4, 7,3,8, 5,3,16, 5,3,16, \
5,3,8, 5,3,16, 5,3,16, 5,3,4, 0,3,8, 0,3,16, 7,3,16, 7,3,16, 7,3,16, 6,3,16, 7,3,16, \
\
1,4,4, 1,4,8, 7,3,16, 7,3,16, 7,3,4, 7,3,8, 5,3,16, 5,3,16, \
0,3,8, 5,3,16, 5,3,16, 0,3,4, 0,3,8, 0,3,16, 1,4,16, 1,4,16, 7,3,16, 6,3,16, 7,3,16, \
1,4,4, 1,4,8, 1,4,16, 2,4,16, 2,4,4, 2,4,8, 1,4,16, 3,4,16, \
3,4,4, 3,4,8, 2,4,16, 7,3,16, 0,3,8, 7,3,16, 7,3,16, 1,4,16, 7,3,16, 6,3,16, 7,3,16, \
\
1,4,8, 1,4,16, 1,4,16, 0,3,8, 7,3,16, 1,4,16, 0,3,8, 7,3,16, 1,4,16, 0,3,8, 5,3,16, 6,3,16, \
0,3,8, 5,3,16, 6,3,16, 0,3,8, 5,3,16, 6,3,16, 0,3,8, 5,3,16, 5,3,16, 5,3,16, 4,3,16, 3,3,16, 2,3,16, \
1,3,4, 1,4,4, 7,3,8, 7,3,16, 5,3,16, 5,3,16, 4,3,16, 3,3,16, 2,3,16, \
\
0,0,0 };

// ��ʱ����ʼֵ�洢����
unsigned char tmr1_con, tmr1_h, tmr1_l;

void Beep_Init() {
    BEEP_DIR = 0;
    BEEP_IO = 0;
}

void TMR1_Init() {
    TMR1H = tmr1_h;                     // ��ʱ��
    TMR1L = tmr1_l; 
    
    TMR1ON = 1;
    TMR1IE = 1 ;
    TMR1IF = 0 ;
    PEIE = 1;
    GIE = 1;
}

void __interrupt() isr(void) {

     if(TMR1IF == 1) { 
        BEEP_IO = !BEEP_IO;
        
        TMR1H = tmr1_h;                     // ��ʱ��
        TMR1L = tmr1_l; 
        
        TMR1IF = 0;
     }
}

void delay(unsigned char t) {
    if(t == 1) {
        __delay_ms(208);
    }else if(t == 2){
        __delay_ms(416);
    }else if(t == 3){
        __delay_ms(600);
    }else if(t == 4){
        __delay_ms(833);
    }else if(t == 5){
        __delay_ms(1000);
    }else if(t == 6){
        __delay_ms(1200);
    } 
}

void main() {

    FOSC_Init();
  
    PORT_Init();
    
    Beep_Init();
    
    unsigned int i = 0;
    unsigned int t = 0;

    while(1){
        unsigned int audio, tone, duration, tune;       // ���ף�����������������
        unsigned int T, M;
        
        audio = tone = duration = tune = T = M = 0;
        unsigned char const *p = yfydq;               // ��ֵ��������
        
        audio = p[i];
        tone = p[i + 1];
        duration = 16 / p[i + 2];                       // ������ʱ��������
                
        if(audio == 0 && tone == 0 && duration == 0) {      // �����ѽ���
            i = 0;
        }else {
            i = i + 3;
            
            if(audio == 0) {
                TMR1IE = 0;                 // ����Ϊ0ʱ ����ͣ�� ֹͣ�ж� 
                GIE = 0;
                t = 0;
                BEEP_IO = 0;                // ������Դ�������Ǳ����
            }else {
                audio = audio - 1;

                if(tone == 1){
                    audio = note_ll[audio];
                }else if(tone == 2) {
                    audio = note_l[audio];
                }else if(tone == 3) {
                    audio = note_m[audio];
                }else if(tone == 4) {
                    audio = note_h[audio];
                }else if(tone == 5) {
                    audio = note_hh[audio];
                }    

                T = T + _XTAL_FREQ_1M / audio / 2;
                M = 0xFFFF - T * (_XTAL_FREQ / (_XTAL_FREQ_1M * 4));   // ��ʱ��ʼֵ���㹫ʽ T*��F/N ��    
                
                tmr1_h = M / 256;
                tmr1_l = M % 256;  
                
                if(!t) {                                                                 
                    TMR1_Init();    // �˺���ֻ����һ��
                    t++;
                }
            }
            
            delay(duration);
        }

    };
}