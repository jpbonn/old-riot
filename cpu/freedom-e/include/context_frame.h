

/**
 * @defgroup    cpu_freedom-e Freedom E
 * @ingroup     cpu
 * @brief       Freedom E cpu
 * @{
 *
 * @file
 * @brief       Thread context frame stored on stack.
 *
 * @author      JP Bonn
 */

#ifndef CONTEXT_FRAME_H
#define CONTEXT_FRAME_H

#if !defined(__ASSEMBLER__)
#include <stdint.h>
#include <assert.h>
#endif //__ASSEMBLER__


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   context_switch_frame stores the registers and PC for a context switch.
 *
 * This also defines context_switch_frame offsets for assembly language.  The
 * structure is sized to maintain 16 byte stack alignment per the ABI.
 *
 */
#if !defined(__ASSEMBLER__)

// N.B.: update the definitions below if this changes
struct context_switch_frame {
    uint32_t  filler;  // filler to maintain 16 byte alignment
    uint32_t  pc;
    /* Callee saved registers */
    uint32_t  s0;
    uint32_t  s1;
    uint32_t  s2;
    uint32_t  s3;
    uint32_t  s4;
    uint32_t  s5;
    uint32_t  s6;
    uint32_t  s7;
    uint32_t  s8;
    uint32_t  s9;
    uint32_t  s10;
    uint32_t  s11;
    /* Caller saved register */
    uint32_t  ra;
    uint32_t  gp;
    uint32_t  tp;
    uint32_t  t0;
    uint32_t  t1;
    uint32_t  t2;
    uint32_t  t3;
    uint32_t  t4;
    uint32_t  t5;
    uint32_t  t6;
    uint32_t  a0;
    uint32_t  a1;
    uint32_t  a2;
    uint32_t  a3;
    uint32_t  a4;
    uint32_t  a5;
    uint32_t  a6;
    uint32_t  a7;
};
#endif //__ASSEMBLER__


// These values are checked for correctness in context_frame.c
#define filler_OFFSET 0
#define pc_OFFSET     4
#define s0_OFFSET     8
#define s1_OFFSET     12
#define s2_OFFSET     16
#define s3_OFFSET     20
#define s4_OFFSET     24
#define s5_OFFSET     28
#define s6_OFFSET     32
#define s7_OFFSET     36
#define s8_OFFSET     40
#define s9_OFFSET     44
#define s10_OFFSET    48
#define s11_OFFSET    52
#define ra_OFFSET     56
#define gp_OFFSET     60
#define tp_OFFSET     64
#define t0_OFFSET     68
#define t1_OFFSET     72
#define t2_OFFSET     76
#define t3_OFFSET     80
#define t4_OFFSET     84
#define t5_OFFSET     88
#define t6_OFFSET     92
#define a0_OFFSET     96
#define a1_OFFSET     100
#define a2_OFFSET     104
#define a3_OFFSET     108
#define a4_OFFSET     112
#define a5_OFFSET     116
#define a6_OFFSET     120
#define a7_OFFSET     124

#define CONTEXT_FRAME_SIZE (a7_OFFSET + 4)

#define SP_OFFSET_IN_THREAD 0


#ifdef __cplusplus
}
#endif

#endif /* CONTEXT_FRAME_H */
/** @} */
