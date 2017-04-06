#ifndef _isr_control_zq_H
#define _isr_control_zq_H

extern void PORTA_IRQHandler(void);
extern void DMA0_IRQHandler(void);
extern void PIT0_IRQHandler(void);
extern void PORTE_IRQHandler(void);
extern void PIT2_IRQHandler(void);        //主要中断循环

#endif  //_GEMINI_ISR_H